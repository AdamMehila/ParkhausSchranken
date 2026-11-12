# Parksystem M450 (Adam Mehila)

Schulprojekt im Modul M450 (ELA). Zwei Raspberry Pi Pico W mit Arduino-Framework in PlatformIO. Pico 1 ist die Kasse, Pico 2 steuert die Ein- und die Ausfahrtsschranke (ein Servo SG90 an GP8 für beide).

## Aktueller Auftrag

A01-04 Testdurchführung und Testprotokoll. Abgabe als PDF bis Samstag, 14.11.2026, 23:59.

- Auftrag und Bewertungsraster: `docs/A01-04_Auftrag_und_Bewertung.md` und `docs/A01-04-testdurchfuehrung-parksystem.pdf`
- Testkonzept 1.0 vom 25.09.2026, Testfälle T01 bis T24: `docs/Testkonzept_Parksystem_Adam (1) 1 (1).pdf`
- Testkonzept 1.1, Nachtrag mit korrigierten Testfällen und T25 bis T30: `docs/Testkonzept_Nachtrag_v1.1.md`
- User Stories: `docs/UserStories_Parksystem.pdf`
- Bewertung des Testkonzepts mit allen Abzügen: `docs/Mehila_Adam (1).pdf`. Diese Fehler dürfen im Testprotokoll nicht wieder vorkommen.
- HERMES-Vorlage Testprotokoll (Gliederung des Protokolls): `docs/testprotokoll.dotx`
- Testprotokoll: `docs/Testprotokoll.md`. Notizen und Nachweise: `docs/nachweise/`. PDF-Erzeugung: `docs/pdf/`
- Plan mit allen Entscheiden: `C:\Users\amehila\.claude\plans\prompt-1-analyse-tender-dewdrop.md`

## Befehle

- Build: `pio run`. Wenn `pio` nicht gefunden wird: `%USERPROFILE%\.platformio\penv\Scripts\pio.exe run`
- Build-Nachweis für alle Umgebungen: `pio run -e pico1_cashdesk -e pico2_barriers -e servo_test -e servo_test_all_pins -e pico1_cashdesk_prod -e pico2_barriers_prod`
- Serieller Monitor: `pio device monitor --port COMx --filter time --filter log2file`
- Flashen macht Adam selbst: BOOTSEL gedrückt halten, Pico einstecken, `firmware.uf2` aus `.pio/build/<env>/` auf das Laufwerk RPI-RP2 ziehen.

## Regeln

- Die Tests an der Hardware führt nur Adam aus. Tatsächliche Ergebnisse, Messwerte, Datum, Uhrzeit und Status werden nie erfunden oder geschätzt. Was noch nicht beobachtet wurde, steht als `‹OFFEN›` im Dokument.
- Firmware nur ändern, um einen Fehler aus der Fehlertabelle zu beheben oder wenn Adam es ausdrücklich verlangt. Jede Änderung muss mit dem Build-Nachweis für alle Umgebungen fehlerfrei bauen.
- Commits: Fehlerbehebung mit Fehler-ID (`F-03: ...`, ein Fehler pro Commit), Testhilfe mit `H-0x: ...`, Dokumente nach jedem abgeschlossenen Schritt mit `Doku: ...`.
- Das WLAN-Passwort steht nie in einem Dokument, Log, Nachweis, Screenshot oder Commit. `src/common/config.h` mit den echten Netzwerkwerten bleibt lokal: nur einzelne Dateien stagen, nie `git add -A` oder `git commit -a`, vor jedem Commit `git diff --cached --name-only` prüfen.
- Gearbeitet wird auf dem Branch `Testprotokoll`. Kein Push ohne Anweisung.

## Schreibstil für alle Dokumente in docs/

- Deutsch (Schweiz): "ss" statt "ß", korrekte Umlaute, korrekte Kommas.
- Sachlich und konkret, kurze Sätze. Keine Floskeln, keine Gedankenstriche, keine Strichpunkte.
- Erwartete und tatsächliche Ergebnisse müssen prüfbar sein: genauer Meldungstext, genauer Betrag, gemessene Zeit mit Toleranz, Ort der Beobachtung (Browser, Servo, serieller Monitor).
- Diese Wörter sind in Ergebnissen verboten: "etwa", "ca.", "klar", "entsprechend", "korrekt", "sauber", "ggf.", "entweder ... oder".
- Versionsnummern immer mit dem Objekt nennen: "Firmware 1.3", "Testkonzept 1.1", "Protokoll 1.0", nie nur "Version 1.1".
- Referenzierte Dokumente mit Titel, Version und Datum nennen, nicht mit Dateinamen.
- Markdown ohne markdownlint-Warnungen: Tabellentrenner `| --- |`, Leerzeilen um Listen, keine Nummerierung, die nicht bei 1 beginnt.
