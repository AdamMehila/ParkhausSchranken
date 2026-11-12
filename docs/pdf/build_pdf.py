"""Erzeugt das PDF des Testprotokolls mit Testkonzept 1.1 als Anhang A.

Aufruf aus dem Projektordner:

    python -I docs/pdf/build_pdf.py
    python -I docs/pdf/build_pdf.py --out C:/Pfad/Datei.pdf --keep

Voraussetzungen: Python-Modul markdown, Microsoft Edge, pdftotext (xpdf oder
poppler). Die Umgebungsvariablen EDGE und PDFTOTEXT überschreiben die Pfade.

Ablauf:

1. Beide Markdown-Dateien in HTML umwandeln (Erweiterungen tables, sane_lists, toc).
2. Messlauf: In jede Überschrift des Inhaltsverzeichnisses kommt eine
   unsichtbare Marke. Edge erzeugt ein PDF, pdftotext liefert den Text pro
   Seite, daraus folgt die Seite jeder Überschrift. Edge kennt kein
   target-counter, deshalb dieser Umweg.
3. Endlauf: Inhaltsverzeichnis mit den gemessenen Seitenzahlen, ohne Marken.
   Die Seitenzahlen haben eine feste Breite, der Umbruch bleibt gleich.
4. Prüfung des fertigen PDF: Fusszeile "Seite n von N" auf jeder Seite, jede
   Überschrift auf der Seite aus dem Inhaltsverzeichnis, Anhang A vorhanden,
   keine Marken, kein WLAN-Passwort aus der lokalen config.h.

Markierungen im Markdown (HTML-Kommentare, im Markdown unsichtbar):

    <!-- INHALTSVERZEICHNIS -->   Ort des Inhaltsverzeichnisses
    <!-- SEITENUMBRUCH -->        Seitenumbruch
    <!-- QUERFORMAT-ANFANG -->    Beginn eines Abschnitts im Querformat
    <!-- QUERFORMAT-ENDE -->      Ende des Abschnitts im Querformat
"""

import argparse
import html
import os
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

import markdown

REPO = Path(__file__).resolve().parents[2]
DOCS = REPO / "docs"
PROTOKOLL = DOCS / "Testprotokoll.md"
NACHTRAG = DOCS / "Testkonzept_Nachtrag_v1.1.md"
CSS_FILE = DOCS / "pdf" / "style.css"
CONFIG_H = REPO / "src" / "common" / "config.h"
DEFAULT_OUT = DOCS / "Testprotokoll_Parksystem_Mehila_Adam.pdf"
EDGE_DEFAULT = r"C:\Program Files (x86)\Microsoft\Edge\Application\msedge.exe"
MARK = "ZZMARK{:03d}ZZ"


def slug_factory(prefix):
    """IDs der Überschriften, mit Präfix je Dokument, damit sie eindeutig sind."""
    def slugify(value, separator):
        value = value.lower()
        for src, dst in (("ä", "ae"), ("ö", "oe"), ("ü", "ue"), ("é", "e"), ("è", "e")):
            value = value.replace(src, dst)
        value = re.sub(r"[^a-z0-9]+", separator, value).strip(separator)
        return prefix + value
    return slugify


def flatten(tokens):
    out = []
    for token in tokens:
        out.append(token)
        out.extend(flatten(token.get("children", [])))
    return out


def convert(md_text, prefix):
    md = markdown.Markdown(
        extensions=["tables", "sane_lists", "toc"],
        extension_configs={"toc": {"slugify": slug_factory(prefix), "toc_depth": "1-4"}},
    )
    body = md.convert(md_text)
    return body, flatten(md.toc_tokens)


def protocol_version(md_text):
    match = re.search(r"^\|\s*Version\s*\|\s*([^|]+?)\s*\|", md_text, re.MULTILINE)
    return match.group(1) if match else "Protokoll"


def toc_entries(protokoll_tokens, nachtrag_tokens):
    """Protokoll: Ebenen 2 und 3. Anhang: Ebenen 1 und 2."""
    entries = []
    for token in protokoll_tokens:
        if token["level"] in (2, 3):
            entries.append((token["level"] - 2, token["id"], html.unescape(token["name"])))
    for token in nachtrag_tokens:
        if token["level"] in (1, 2):
            entries.append((token["level"] - 1, token["id"], html.unescape(token["name"])))
    return entries


def toc_html(entries, pages):
    rows = ['<nav class="inhaltsverzeichnis"><p class="toc-titel">Inhaltsverzeichnis</p>']
    for (stufe, hid, name), page in zip(entries, pages):
        rows.append(
            f'<div class="toc-zeile stufe-{stufe}"><a href="#{hid}">'
            f'<span class="toc-text">{html.escape(name)}</span>'
            f'<span class="toc-punkte"></span>'
            f'<span class="toc-seite">{page}</span></a></div>'
        )
    rows.append("</nav>")
    return "\n".join(rows)


def add_marks(body, entries):
    for index, (_, hid, _) in enumerate(entries):
        anchor = f'id="{hid}">'
        if anchor not in body:
            continue
        body = body.replace(anchor, f'{anchor}<span class="mess">{MARK.format(index)}</span>', 1)
    return body


def apply_markers(body):
    body = body.replace("<!-- SEITENUMBRUCH -->", '<div class="seitenumbruch"></div>')
    body = body.replace("<!-- QUERFORMAT-ANFANG -->", '<div class="quer">')
    body = body.replace("<!-- QUERFORMAT-ENDE -->", "</div>")
    # Bilder relativ zu docs/ als absolute Datei-URL, damit Edge sie findet.
    def fix_src(match):
        src = match.group(1)
        if re.match(r"^[a-z]+:", src):
            return match.group(0)
        return f'src="{(DOCS / src).resolve().as_uri()}"'
    return re.sub(r'src="([^"]+)"', fix_src, body)


def build_html(css, protokoll_body, nachtrag_body, entries, pages, with_marks):
    p_body = protokoll_body
    n_body = nachtrag_body
    if with_marks:
        p_body = add_marks(p_body, entries)
        n_body = add_marks(n_body, entries)
    p_body = p_body.replace("<!-- INHALTSVERZEICHNIS -->", toc_html(entries, pages))
    return (
        '<!doctype html>\n<html lang="de-CH">\n<head>\n<meta charset="utf-8">\n'
        "<title>Testprotokoll Parksystem mit Schranken</title>\n"
        f"<style>\n{css}\n</style>\n</head>\n<body>\n"
        f'<div class="protokoll">\n{apply_markers(p_body)}\n</div>\n'
        f'<div class="anhang">\n{apply_markers(n_body)}\n</div>\n'
        "</body>\n</html>\n"
    )


def render(edge, html_path, pdf_path, profile_dir):
    if pdf_path.exists():
        pdf_path.unlink()
    cmd = [
        edge,
        "--headless",
        "--disable-gpu",
        "--no-first-run",
        "--no-default-browser-check",
        f"--user-data-dir={profile_dir}",
        "--no-pdf-header-footer",
        f"--print-to-pdf={pdf_path}",
        html_path.as_uri(),
    ]
    subprocess.run(cmd, check=True, timeout=240, capture_output=True)
    if not pdf_path.exists() or pdf_path.stat().st_size == 0:
        raise RuntimeError(f"Edge hat kein PDF erzeugt: {pdf_path}")


def pdf_pages(pdftotext, pdf_path):
    result = subprocess.run(
        [pdftotext, "-layout", "-enc", "UTF-8", str(pdf_path), "-"],
        check=True,
        capture_output=True,
    )
    pages = result.stdout.decode("utf-8", "replace").split("\f")
    if pages and pages[-1].strip() == "":
        pages = pages[:-1]
    return pages


def norm(text):
    return re.sub(r"\s+", " ", text).strip()


def read_password():
    try:
        text = CONFIG_H.read_text(encoding="utf-8")
    except OSError:
        return None
    match = re.search(r'#define\s+WIFI_PASSWORD\s+"([^"]*)"', text)
    if not match or len(match.group(1)) < 4:
        return None
    return match.group(1)


def page_formats(pdf_path):
    data = pdf_path.read_bytes()
    boxes = re.findall(rb"/MediaBox\s*\[\s*([\d.]+)\s+([\d.]+)\s+([\d.]+)\s+([\d.]+)\s*\]", data)
    hoch = sum(1 for b in boxes if float(b[3]) > float(b[2]))
    quer = sum(1 for b in boxes if float(b[2]) > float(b[3]))
    return hoch, quer


def verify(pages, entries, page_numbers):
    problems = []
    total = len(pages)
    for number, text in enumerate(pages, start=1):
        if f"Seite {number} von {total}" not in norm(text):
            problems.append(f"Seite {number}: Fusszeile 'Seite {number} von {total}' fehlt")
    for (_, _, name), page in zip(entries, page_numbers):
        key = norm(name)[:40]
        if key not in norm(pages[page - 1]):
            problems.append(f"'{name}' steht nicht auf Seite {page}")
    full = norm("\n".join(pages))
    for must in ("Testkonzept Parksystem mit Schranken: Nachtrag", "T30 Abnahmetest des Produktivbuilds"):
        if must not in full:
            problems.append(f"Anhang A unvollständig, fehlt: '{must}'")
    if "ZZMARK" in full:
        problems.append("Messmarken im fertigen PDF")
    password = read_password()
    if password and password in "\n".join(pages):
        problems.append("Das WLAN-Passwort aus config.h steht im PDF")
    return problems, password is not None


def main():
    # Windows gibt sonst in der Codepage der Konsole aus, die Umlaute gehen verloren.
    if hasattr(sys.stdout, "reconfigure"):
        sys.stdout.reconfigure(encoding="utf-8")

    parser = argparse.ArgumentParser(description="PDF des Testprotokolls erzeugen")
    parser.add_argument("--out", type=Path, default=DEFAULT_OUT, help="Zieldatei des PDF")
    parser.add_argument("--keep", action="store_true", help="Zwischendateien behalten")
    args = parser.parse_args()

    edge = os.environ.get("EDGE", EDGE_DEFAULT)
    pdftotext = os.environ.get("PDFTOTEXT") or shutil.which("pdftotext")
    if not Path(edge).exists():
        sys.exit(f"Edge nicht gefunden: {edge}")
    if not pdftotext:
        sys.exit("pdftotext nicht gefunden")

    protokoll_md = PROTOKOLL.read_text(encoding="utf-8")
    nachtrag_md = NACHTRAG.read_text(encoding="utf-8")
    css = CSS_FILE.read_text(encoding="utf-8").replace("{{VERSION}}", protocol_version(protokoll_md))

    protokoll_body, protokoll_tokens = convert(protokoll_md, "p-")
    nachtrag_body, nachtrag_tokens = convert(nachtrag_md, "a-")
    entries = toc_entries(protokoll_tokens, nachtrag_tokens)

    work = Path(tempfile.mkdtemp(prefix="testprotokoll_pdf_"))
    profile = work / "edge-profil"
    try:
        # Messlauf
        measure_html = work / "messlauf.html"
        measure_pdf = work / "messlauf.pdf"
        measure_html.write_text(
            build_html(css, protokoll_body, nachtrag_body, entries, ["000"] * len(entries), True),
            encoding="utf-8",
        )
        render(edge, measure_html, measure_pdf, profile)
        measure_pages = pdf_pages(pdftotext, measure_pdf)

        page_numbers = []
        missing = []
        for index, entry in enumerate(entries):
            mark = MARK.format(index)
            found = next((n for n, text in enumerate(measure_pages, start=1) if mark in text), None)
            if found is None:
                missing.append(entry[2])
            page_numbers.append(found or 0)
        if missing:
            sys.exit("Messmarken nicht gefunden für: " + ", ".join(missing))

        # Endlauf
        final_html = work / "endlauf.html"
        final_html.write_text(
            build_html(css, protokoll_body, nachtrag_body, entries, [str(n) for n in page_numbers], False),
            encoding="utf-8",
        )
        args.out.parent.mkdir(parents=True, exist_ok=True)
        render(edge, final_html, args.out, profile)
        final_pages = pdf_pages(pdftotext, args.out)

        if len(final_pages) != len(measure_pages):
            print(f"Achtung: Messlauf {len(measure_pages)} Seiten, Endlauf {len(final_pages)} Seiten")

        problems, password_checked = verify(final_pages, entries, page_numbers)
        hoch, quer = page_formats(args.out)

        print(f"PDF: {args.out}")
        print(f"Seiten: {len(final_pages)} (Hochformat {hoch}, Querformat {quer})")
        print(f"Einträge im Inhaltsverzeichnis: {len(entries)}")
        for (stufe, _, name), page in zip(entries, page_numbers):
            print(f"  {'  ' * stufe}{name} ... {page}")
        print("Passwortprüfung: " + ("durchgeführt" if password_checked else "kein Passwort in config.h gefunden, übersprungen"))
        if problems:
            print("PRÜFUNG FEHLGESCHLAGEN:")
            for problem in problems:
                print(f"  {problem}")
            sys.exit(1)
        print("Prüfung bestanden: Fusszeilen, Seitenzahlen im Inhaltsverzeichnis, Anhang A, keine Marken, kein Passwort.")
    finally:
        if args.keep:
            print(f"Zwischendateien: {work}")
        else:
            shutil.rmtree(work, ignore_errors=True)


if __name__ == "__main__":
    main()
