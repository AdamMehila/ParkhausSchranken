# Parksystem M450 (Adam Mehila)

Schulprojekt im Modul M450 (ELA). Zwei Raspberry Pi Pico W mit Arduino-Framework in PlatformIO. Pico 1 ist die Kasse, Pico 2 steuert die Ein- und die Ausfahrtsschranke (Servos SG90).

## Aktueller Auftrag

A01-04 Testdurchführung und Testprotokoll. Abgabe als PDF bis Samstag, 14.11.2026, 23:59.

- Auftrag und Bewertungsraster: `docs/A01-04_Auftrag_und_Bewertung.md` und `docs/A01-04_Auftrag.pdf`
- Testkonzept (Version 1.0 vom 25.09.2026, Testfälle T01 bis T24): `docs/Testkonzept_Parksystem_Adam.pdf`
- User Stories: `docs/UserStories_Parksystem.pdf`
- Bewertung des Testkonzepts mit allen Abzügen: `docs/Bewertung_Testkonzept.pdf`. Diese Fehler dürfen im Testprotokoll nicht wieder vorkommen.

## Befehle

- Build: `pio run`. Wenn `pio` nicht gefunden wird: `%USERPROFILE%\.platformio\penv\Scripts\pio.exe run`
- Serieller Monitor: `pio device monitor`
- Flashen macht Adam selbst: BOOTSEL gedrückt halten, Pico einstecken, `firmware.uf2` aus `.pio/build/<env>/` auf das Laufwerk RPI-RP2 ziehen.

## Regeln

- Die Tests an der Hardware führt nur Adam aus. Tatsächliche Ergebnisse, Messwerte, Datum, Uhrzeit und Status werden nie erfunden oder geschätzt. Was noch nicht beobachtet wurde, steht als `‹OFFEN›` im Dokument.
- Firmware nur ändern, um einen Fehler aus der Fehlertabelle zu beheben oder wenn Adam es ausdrücklich verlangt. Jede Änderung muss mit `pio run` fehlerfrei bauen. Ein Fehler pro Commit, die Commit-Nachricht beginnt mit der Fehler-ID, zum Beispiel `F-03: ...`.
- Gearbeitet wird auf dem Branch `testprotokoll`. Kein Push ohne Anweisung.

## Schreibstil für alle Dokumente in docs/

- Deutsch (Schweiz): "ss" statt "ß", korrekte Umlaute, korrekte Kommas.
- Sachlich und konkret, kurze Sätze. Keine Floskeln, keine Gedankenstriche, keine Strichpunkte.
- Erwartete und tatsächliche Ergebnisse müssen prüfbar sein: genauer Meldungstext, genauer Betrag, gemessene Zeit mit Toleranz, Ort der Beobachtung (Browser, Servo, serieller Monitor).
- Diese Wörter sind in Ergebnissen verboten: "etwa", "ca.", "klar", "entsprechend", "korrekt", "sauber", "ggf.", "entweder ... oder".
