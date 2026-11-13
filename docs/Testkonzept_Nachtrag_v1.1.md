# Testkonzept Parksystem mit Schranken: Nachtrag

| Feld | Wert |
| --- | --- |
| Dokument | Testkonzept Parksystem mit Schranken, Nachtrag |
| Version | Testkonzept 1.1 |
| Stand | 12.11.2026 |
| Status | Bereit für Zyklus 1 |
| Autor | Adam Mehila (Einzelarbeit, Gruppe Mehila) |
| Modul | M450 Applikationen testen (ELA), Klasse INA24b |
| Ergänzt | Testkonzept 1.0 vom 25.09.2026 |
| Gilt für | Firmware 1.0 und höher, Test-Build. T30 prüft den Produktivbuild |

## Änderungshistorie

| Version | Datum | Autor | Änderung |
| --- | --- | --- | --- |
| Testkonzept 1.0 | 25.09.2026 | Adam Mehila | Erstfassung mit den Testfällen T01 bis T24 |
| Testkonzept 1.1 | 12.11.2026 | Adam Mehila | Nachtrag: Testumgebung an die Umsetzung angepasst, Testwerkzeuge und Ablauf beschrieben, T01 bis T24 präzisiert, T25 bis T30 neu |

## 1 Zweck des Nachtrags

Testkonzept 1.1 besteht aus Testkonzept 1.0 und diesem Nachtrag. Wo dieser Nachtrag etwas regelt, gilt der Nachtrag. Alle anderen Kapitel von Testkonzept 1.0 gelten unverändert, insbesondere die Testziele Z1 bis Z5 (Kapitel 1.1), die Teststufe (Kapitel 4) und die Fehlerklassifizierung (Kapitel 6).

Der Nachtrag hat zwei Gründe:

1. Die Bewertung des Testkonzepts hat Stellen genannt, an denen die Testfälle nicht eindeutig oder nicht vollständig waren.
2. Der Abgleich mit der Umsetzung hat gezeigt, dass die Testumgebung in einigen Punkten anders aufgebaut ist als in Testkonzept 1.0 beschrieben.

### 1.1 Behebung der Bemerkungen aus der Bewertung

| Nr. | Bemerkung | Behebung in Testkonzept 1.1 |
| --- | --- | --- |
| 1 | Wie wird der Endpunkt für Zeitmanipulation aufgerufen? | Kapitel 4.4: Aufruf, Antwort und Monitorzeile. Testfälle T13, T14, T22, T29 nennen den genauen Aufruf |
| 2 | Wie wird der Datenreset durchgeführt? | Kapitel 4.4 und 7.4: Aufruf, Antwort, Kontrolle und Zustand je Testfall |
| 3 | Was ist, wenn ein Kennzeichen doppelt einfährt? | Neuer Testfall T25 |
| 4 | Wieso sind nur Zahlen gültige Eingaben, Beispiel LU 122? | Kapitel 5.2 beschreibt das Kennzeichenformat. Neuer Testfall T26 prüft die Grenzen, LU 122 ist gültig |
| 5 | Titel und Text zu vage, "etwa 1 Minute warten" | T12 mit Zeitfenster für die Abfrage und genauer Anzeige |
| 6 | T16: Was ist der angezeigte Betrag? | T16 nennt den Betrag CHF 2.00 mit Begründung aus dem Tarif |
| 7 | Variante "entweder ausgegraut oder klickt" | T17 erwartet nur ein Verhalten: Der Knopf ist deaktiviert |
| 8 | Unklar: "klare Fehlermeldung", "Hinweis" | Alle Testfälle zitieren den Wortlaut aus dem Meldungskatalog (Kapitel 5.4) |
| 9 | Wie kann der Ablauf "Pico 1 fragt Pico 2" beobachtet werden? | Die Richtung ist umgekehrt: Pico 2 fragt Pico 1. Beobachtung über Monitorzeilen beider Picos (Kapitel 4.6), Testfälle T19 bis T21, T23, T29 |
| 10 | Person nicht in einer eigenen Spalte | Rollentabelle mit Spalte Person (Kapitel 3) |

## 2 Referenzierte Dokumente

| Nr. | Titel | Version | Datum |
| --- | --- | --- | --- |
| [1] | Testkonzept Parksystem mit Schranken | Testkonzept 1.0 | 25.09.2026 |
| [2] | User Stories Parksystem mit Schranken | ohne Versionsangabe | 07.09.2026 |
| [3] | Bewertung Testkonzept M450 INA24b | ohne Versionsangabe | 29.10.2026 |
| [4] | Auftrag A01-03 Implementierung Parkhaus mit Schranke | ohne Versionsangabe | ohne Datumsangabe |
| [5] | Auftrag A01-04 Testdurchführung und Testprotokoll Parksystem | ohne Versionsangabe | Abgabe 14.11.2026 |
| [6] | Firmware Parksystem, Git-Repository ParkhausSchranken, Branch Testprotokoll | Firmware 1.0 (Commit 867aed7) | 12.11.2026 |
| [7] | HERMES-Ergebnisvorlage Testprotokoll | ohne Versionsangabe | 26.01.2026 |

## 3 Rollen und Zuständigkeiten (ergänzt Kapitel 5)

Die Rollen bleiben wie in Testkonzept 1.0. Neu ist die Spalte Person.

| Rolle | Aufgabe | Person |
| --- | --- | --- |
| Testleitung | Plant den Ablauf, prüft die Eingangskriterien, vergibt die Fehlerklassen, empfiehlt die Inbetriebnahme | Adam Mehila |
| Tester Einfahrt und Ausfahrt | Bedient die Weboberfläche von Pico 2, misst Zeiten, beobachtet Servo und Monitor von Pico 2 | Adam Mehila |
| Tester Kasse | Bedient die Weboberfläche von Pico 1, prüft Gebühren und Listen, beobachtet den Monitor von Pico 1 | Adam Mehila |
| Entwicklung | Behebt Fehler, liefert neue Firmware-Versionen, erstellt die Build-Nachweise | Adam Mehila |

Hilfsmittel: Die Testhilfen H-01 bis H-05 und die Entwürfe der Testdokumente sind mit Unterstützung von Claude Code (KI-Assistent) entstanden. Prüfung, Testdurchführung und Verantwortung liegen bei Adam Mehila.

## 4 Angepasste Testumgebung (ersetzt Teile von Kapitel 2.1, 3.1, 3.2 und 3.3)

### 4.1 Hardware

| Komponente | Testkonzept 1.0 | Testkonzept 1.1 |
| --- | --- | --- |
| Pico 1 (Kasse) | Raspberry Pi Pico W, Strom über USB | unverändert |
| Pico 2 (Schranken) | Raspberry Pi Pico W, Strom über USB | unverändert |
| Servos | zwei SG90, Einfahrt an GPIO 15, Ausfahrt an GPIO 16 | ein SG90 an GP8 (Pin 11), Strom an VBUS (Pin 40), Masse an GND (Pin 8). Einfahrt und Ausfahrt teilen sich denselben Arm |
| Stellung zu | 0 Grad | 90 Grad, Arm waagrecht |
| Stellung offen | 90 Grad, Arm senkrecht | 0 Grad, Arm senkrecht |
| Schrankenarm | Kartonstreifen | mitgelieferter Arm des SG90 |
| Stoppuhr | Handy oder Stoppuhr | Stoppuhr auf dem Handy |

Die Bedeutung der Stellungen bleibt gleich: Arm waagrecht heisst zu, Arm senkrecht heisst offen. Nur die Gradzahlen sind vertauscht, weil der Arm anders auf der Achse sitzt. Mit einem Arm lassen sich Einfahrt und Ausfahrt nur über die Meldungen auf der Weboberfläche unterscheiden, nicht über die Mechanik.

### 4.2 Netzwerk

| Angabe | Wert |
| --- | --- |
| WLAN-Name (2,4 GHz) | ‹OFFEN› |
| WLAN-Passwort | wird nicht dokumentiert |
| Pico 1 (Kasse), Adresse IP-KASSE | ‹OFFEN› |
| Pico 2 (Schranken), Adresse IP-SCHRANKE | ‹OFFEN› |
| Gateway, Subnetzmaske | ‹OFFEN› |
| Testlaptop | im selben WLAN, Adresse vom Router |

In diesem Dokument steht IP-KASSE für die Adresse von Pico 1 und IP-SCHRANKE für die Adresse von Pico 2. Die Kasse-Seite ist `http://IP-KASSE`, die Schranken-Seite ist `http://IP-SCHRANKE`.

### 4.3 Software

| Software | Wert |
| --- | --- |
| Firmware Test-Build | Firmware 1.0, Commit 867aed7, Umgebungen pico1_cashdesk und pico2_barriers, TEST_BUILD=1 |
| Firmware Produktivbuild | gleicher Commit wie der Endstand, Umgebungen pico1_cashdesk_prod und pico2_barriers_prod, TEST_BUILD=0, nur für T30 |
| Browser | Google Chrome, Version ‹OFFEN›. Alle Testfälle laufen in Chrome |
| Entwicklungsumgebung | Visual Studio Code mit PlatformIO Core 6.2.0 |
| Serieller Monitor | je Pico ein Terminal mit `pio device monitor --port COMx --filter time --filter log2file`. COM-Port Pico 1 ‹OFFEN›, COM-Port Pico 2 ‹OFFEN› |
| Betriebssystem Testlaptop | Windows 11 Pro 10.0.26300 |

Der Filter `time` setzt vor jede Monitorzeile die Uhrzeit des Laptops mit Millisekunden. Der Filter `log2file` schreibt alle Zeilen zusätzlich in eine Logdatei.

### 4.4 Testwerkzeuge

Die Testwerkzeuge gibt es nur im Test-Build. Im Produktivbuild fehlen sie (geprüft mit T30).

| Werkzeug | Aufruf | Antwort im Browser | Monitorzeile |
| --- | --- | --- | --- |
| Einfahrtszeit verschieben | `http://IP-KASSE/api/debug/time?plate=BE%2055511&minutes=60` oder auf der Kasse-Seite die Karte "Testwerkzeuge (nur im Test-Build)": Kennzeichen und Minuten eintragen, "Einfahrt zurücksetzen" klicken | `{"ok":true,"message":"Einfahrtszeit um 60 Minuten zurückgesetzt. Gebühr für genau 1 h 00 min: CHF 2.00.","exactDuration":"1 h 00 min","exactFee":"CHF 2.00"}` | Pico 1: `[DEBUG] Einfahrtszeit verschoben: BE 55511 -60 Minuten` |
| Alle Vorgänge löschen (Reset) | `http://IP-KASSE/api/debug/reset` oder auf der Kasse-Seite "Alle Vorgänge löschen" | `{"ok":true,"message":"Alle Parkvorgänge gelöscht."}` | Pico 1: `[DEBUG] Alle Parkvorgaenge geloescht.` |
| Kasse stumm schalten | `http://IP-KASSE/api/debug/offline?seconds=30` | `{"ok":true,"seconds":30,"message":"Die Kasse beantwortet 30 Sekunden lang keine Anfragen."}` | Pico 1: `[DEBUG] Kasse antwortet 30 s lang nicht.` und nach Ablauf `[DEBUG] Kasse antwortet wieder.` |
| Servo-Test | Schranken-Seite, Karte "Smoke-Test der Servos (nur im Test-Build)", "Servo testen", oder `http://IP-SCHRANKE/api/servotest` | `{"ok":true,"message":"Servo an GP8 öffnet die Schranke und schliesst sie nach 5 Sekunden wieder."}` | Pico 2: `[SCHRANKE] Schranke oeffnet (0 Grad).` und 5 s später `[SCHRANKE] Schranke schliesst (90 Grad).` |

Regeln für die Testwerkzeuge:

- Einfahrtszeit verschieben setzt die Einfahrt auf "jetzt minus N Minuten". Die Angabe ist nicht kumulativ. Ein zweiter Aufruf mit 61 Minuten ergibt 61 Minuten, nicht 121.
- Einfahrtszeit verschieben wirkt nur auf Fahrzeuge im Parkhaus. Sonst lautet die Antwort `Kennzeichen steht nicht im Parkhaus.` mit `"ok":false`.
- Einfahrtszeit verschieben setzt ein bezahltes Fahrzeug wieder auf "offen". Darum immer vor dem Bezahlen aufrufen.
- Nach dem Verschieben läuft die Uhr weiter. Die Gebühr für genau N Minuten nennt die Antwort (Teil "Gebühr für genau ..."). Die Suche an der Kasse zeigt die laufende Parkdauer.
- Der Reset löscht auch die Liste "Abgeschlossene Vorgänge". Die Uhr der Kasse bleibt gestellt. Die Schranken-Seite zeigt die alte Belegung bis zu 3 s weiter.
- Kasse stumm schalten: Standard 30 s, höchstens 120 s. Die Kasse bleibt im WLAN und behält alle Parkvorgänge, beantwortet aber keine Anfrage.

### 4.5 Ort der Beobachtung

| Ort | Was dort beobachtet wird |
| --- | --- |
| Schranken-Seite | Meldungsfeld unter dem Eingabefeld "Einfahrt" oder "Ausfahrt" (grün bei Erfolg, rot bei Fehler, orange während einer Anfrage), Zustand des Arms ("offen" oder "geschlossen"), Belegung, Punkt "Kasse erreichbar" oder "Kasse nicht erreichbar" |
| Kasse-Seite | Meldungsfeld unter der Suche (grün, orange, rot), Suchresultat (Kennzeichen, Einfahrt, Parkdauer, Gebühr, Status), Knopf "Bezahlen", Listen "Fahrzeuge im Parkhaus" und "Abgeschlossene Vorgänge", Belegung, Uhr |
| Servo | Arm waagrecht (zu) oder senkrecht (offen), Zeit mit der Stoppuhr |
| Serieller Monitor | Zeilen aus Kapitel 4.6 mit Zeitstempel |

### 4.6 Monitorzeilen

Alle Fundstellen beziehen sich auf Firmware 1.0 (Commit 867aed7). STATUS ist `paid`, `unpaid` oder `unknown`.

| Zeile | Pico | Fundstelle |
| --- | --- | --- |
| `=== Parksystem M450 - Pico 1 (Kasse) - Firmware 1.0 ===` | 1 | src/cashdesk/main.cpp:509 |
| `=== Parksystem M450 - Pico 2 (Schranken) - Firmware 1.0 ===` | 2 | src/barriers/main.cpp:423 |
| `[KASSE] Uhr gestellt: TT.MM.JJJJ hh:mm:ss` | 1 | src/cashdesk/main.cpp:358 |
| `[KASSE] Einfahrt: KENNZEICHEN  Belegung: n/3` | 1 | src/cashdesk/parking_garage.cpp:222 |
| `[KASSE] Bezahlt: KENNZEICHEN CHF x.xx` | 1 | src/cashdesk/parking_garage.cpp:292 |
| `[KASSE] Ausfahrt: KENNZEICHEN  Belegung: n/3` | 1 | src/cashdesk/parking_garage.cpp:315 |
| `[KASSE] Statusabfrage von Pico 2: KENNZEICHEN -> STATUS` (Testhilfe H-01) | 1 | src/cashdesk/main.cpp:211 |
| `[DEBUG] Einfahrtszeit verschoben: KENNZEICHEN -N Minuten` | 1 | src/cashdesk/parking_garage.cpp:350 |
| `[DEBUG] Alle Parkvorgaenge geloescht.` | 1 | src/cashdesk/parking_garage.cpp:362 |
| `[DEBUG] Kasse antwortet N s lang nicht.` (Testhilfe H-04) | 1 | src/cashdesk/main.cpp:463 |
| `[DEBUG] Kasse antwortet wieder.` (Testhilfe H-04) | 1 | src/cashdesk/main.cpp:477 |
| `[SCHRANKE] Schranke oeffnet (0 Grad).` | 2 | src/barriers/barrier.cpp:51 |
| `[SCHRANKE] Schranke schliesst (90 Grad).` | 2 | src/barriers/barrier.cpp:100 |
| `[EINGABE] Abgelehnt, ungueltiges Kennzeichen. Laenge: N` | 2 | src/barriers/main.cpp:137 |
| `[AUSFAHRT] Kasse meldet fuer KENNZEICHEN: STATUS` (Testhilfe H-01) | 2 | src/barriers/main.cpp:255 |
| `[AUSFAHRT] Unbekanntes Kennzeichen: KENNZEICHEN` | 2 | src/barriers/main.cpp:267 |
| `[AUSFAHRT] Noch nicht bezahlt: KENNZEICHEN` | 2 | src/barriers/main.cpp:284 |
| `[AUSFAHRT] Kasse nicht erreichbar, Schranke bleibt zu.` | 2 | src/barriers/main.cpp:245 |
| `[KASSE-CLIENT] Kasse nicht erreichbar, Fehler N` | 2 | src/barriers/cashdesk_client.cpp:110 |

Mit diesen Zeilen lässt sich die Statusabfrage von Pico 2 an Pico 1 auf beiden Seiten beobachten: Pico 1 zeigt den Eingang der Abfrage, Pico 2 zeigt die Antwort. Die Zeitstempel beider Monitore stammen von derselben Laptop-Uhr und lassen sich direkt vergleichen.

## 5 Testdaten (ergänzt Kapitel 3.4)

### 5.1 Kennzeichen

| Zweck | Kennzeichen |
| --- | --- |
| Gültige Kennzeichen aus Testkonzept 1.0 | LU 12345, ZH 98765, BE 55511, SG 11122, TG 44499 |
| Nie eingefahrene Kennzeichen | UR 99988 (T15), ZG 77766 (T18), BE 55512 (T21) |
| Grenzwerte des Formats | siehe Kapitel 5.2 und T26 |

### 5.2 Kennzeichenformat

Ein gültiges Kennzeichen besteht aus zwei Buchstaben für den Kanton, einem Leerzeichen und einer bis sechs Ziffern. Das entspricht dem Aufbau der Schweizer Kennzeichen: Nach dem Kantonskürzel folgt nur eine Nummer. Darum sind nach dem Leerzeichen nur Ziffern erlaubt. Die ganze Eingabe darf höchstens 10 Zeichen lang sein (src/common/config.h:121, src/common/license_plate.cpp:69-133).

Vor der Prüfung wird die Eingabe vereinheitlicht: Kleinbuchstaben werden zu Grossbuchstaben, Leerzeichen am Anfang und Ende fallen weg, mehrere Leerzeichen werden zu einem, und ein fehlendes Leerzeichen nach den zwei Buchstaben wird ergänzt.

| Eingabe | Zeichen | Ergebnis |
| --- | --- | --- |
| LU 122 | 6 | gültig, drei Ziffern |
| LU 1 | 4 | gültig, eine Ziffer |
| LU 123456 | 9 | gültig, sechs Ziffern |
| lu1 | 3 | gültig, wird zu "LU 1" |
| lu12345 | 7 | gültig, wird zu "LU 12345" |
| 12345 | 5 | ungültig, Meldung M03 |
| LU | 2 | ungültig, Meldung M03 |
| L 12345 | 7 | ungültig, Meldung M03 |
| LU 12A45 | 8 | ungültig, Meldung M03 |
| LU 1234567 | 10 | ungültig, sieben Ziffern, Meldung M03 |
| LU 12345678 | 11 | ungültig, länger als 10 Zeichen, Meldung M02 |
| leer | 0 | ungültig, Meldung M01 |

### 5.3 Tarif und Zeitfenster nach Bezahlung

Der Tarif bleibt wie in Testkonzept 1.0: CHF 2.00 pro angefangene Stunde, keine Gratiszeit, kein Tagesmaximum.

| Parkdauer | Gebühr |
| --- | --- |
| 1 Minute | CHF 2.00 |
| 59 Minuten | CHF 2.00 |
| genau 60 Minuten | CHF 2.00 |
| 61 Minuten | CHF 4.00 |
| 90 Minuten | CHF 4.00 |
| 121 Minuten | CHF 6.00 |
| 195 Minuten (3 h 15 min) | CHF 8.00 |

Neu gilt ein Zeitfenster nach der Bezahlung (User Story 7, Akzeptanzkriterium 3): Wer bezahlt hat, kann innerhalb von 15 Minuten ohne weitere Gebühr ausfahren. Nach Ablauf der 15 Minuten wird die Gebühr ab der Einfahrt neu berechnet. Verlangt wird nur die Differenz zum bezahlten Betrag. Ist die Differenz null, öffnet die Schranke.

### 5.4 Meldungskatalog

Alle Testfälle zitieren diese Texte. KENNZEICHEN und Beträge stehen für die Werte im jeweiligen Testfall. Die Fundstellen beziehen sich auf Firmware 1.0 (Commit 867aed7).

| ID | Ort | Wortlaut | Fundstelle |
| --- | --- | --- | --- |
| M01 | Schranken-Seite, rot | Bitte ein Kennzeichen eingeben. | src/common/license_plate.cpp:139 |
| M02 | Schranken-Seite, rot | Das Kennzeichen ist zu lang. Erlaubt sind höchstens 10 Zeichen, zum Beispiel LU 12345. | src/common/license_plate.cpp:142 |
| M03 | Schranken-Seite, rot | Das ist kein gültiges Kennzeichen. Erwartet wird zum Beispiel LU 12345. | src/common/license_plate.cpp:145 |
| M04 | Schranken-Seite, grün | KENNZEICHEN ist registriert. Die Einfahrtsschranke öffnet und schliesst nach 5 Sekunden wieder. | src/barriers/main.cpp:221-222 |
| M05 | Schranken-Seite, rot | Dieses Fahrzeug steht bereits im Parkhaus. | src/cashdesk/parking_garage.cpp:198 |
| M06 | Schranken-Seite, rot | Das Parkhaus ist voll. Keine weitere Einfahrt möglich. | src/cashdesk/main.cpp:253 |
| M07 | Schranken-Seite, rot | Die Kasse ist zurzeit nicht erreichbar. Die Einfahrt kann nicht registriert werden. | src/barriers/cashdesk_client.cpp:195 |
| M08 | Schranken-Seite, rot | Die Kasse ist zurzeit nicht erreichbar. Der Bezahlstatus kann nicht geprüft werden, die Schranke bleibt geschlossen. | src/barriers/main.cpp:246 |
| M09 | Schranken-Seite, rot | Zu dem Kennzeichen KENNZEICHEN ist keine Einfahrt bekannt. Die Schranke bleibt geschlossen. | src/barriers/main.cpp:270-272 |
| M10 | Schranken-Seite, rot | Die Parkgebühr für KENNZEICHEN ist noch offen (CHF x.xx). Bitte zuerst an der Kasse bezahlen. Die Schranke bleibt geschlossen. | src/barriers/main.cpp:287-291 |
| M11 | Schranken-Seite, grün | Bezahlung bestätigt. Die Ausfahrtsschranke öffnet für KENNZEICHEN. | src/barriers/main.cpp:318-320 |
| M12 | Schranken-Seite, rot | Die Kasse konnte den Parkvorgang nicht abschliessen. Die Schranke bleibt geschlossen. | src/barriers/main.cpp:308 |
| M13 | Schranken-Seite, rot | Pico 2 antwortet nicht. | src/barriers/web_page_barriers.h:157 und 180 |
| M14 | Kasse-Seite, rot | Bitte ein Kennzeichen eingeben. | src/cashdesk/web_page_cashdesk.h:175 |
| M15 | Kasse-Seite, rot | Das Kennzeichen KENNZEICHEN wurde im System nicht gefunden. Es ist keine Einfahrt bekannt. | src/cashdesk/web_page_cashdesk.h:195-197 |
| M16 | Kasse-Seite, orange | Offener Betrag: CHF x.xx | src/cashdesk/web_page_cashdesk.h:215 |
| M17 | Kasse-Seite, grün | Für dieses Kennzeichen wurde bereits bezahlt. Eine weitere Zahlung ist nicht nötig. | src/cashdesk/web_page_cashdesk.h:211-212 |
| M18 | Kasse-Seite, grün | Bezahlung erfolgt. Betrag: CHF x.xx. | src/cashdesk/main.cpp:293 |
| M19 | Browser, Aufruf /api/pay | Für dieses Kennzeichen wurde bereits bezahlt (CHF x.xx). | src/cashdesk/main.cpp:291 |
| M20 | Browser, Aufruf /api/pay | Dieses Kennzeichen ist nicht im System. Eine Bezahlung ist nicht möglich. | src/cashdesk/main.cpp:289 |
| M21 | Kasse-Seite, Testwerkzeuge | Einfahrtszeit um N Minuten zurückgesetzt. Gebühr für genau h h mm min: CHF x.xx. Sonst: Kennzeichen steht nicht im Parkhaus. | src/cashdesk/main.cpp:397-400 |
| M22 | Kasse-Seite, Testwerkzeuge | Alle Parkvorgänge gelöscht. | src/cashdesk/main.cpp:420 |
| M23 | Schranken-Seite, grün | Servo an GP8 öffnet die Schranke und schliesst sie nach 5 Sekunden wieder. | src/barriers/main.cpp:390-392 |
| M24 | Schranken-Seite, orange | Anfrage läuft ... (Einfahrt) und Bezahlstatus wird bei der Kasse abgefragt ... (Ausfahrt) | src/barriers/web_page_barriers.h:141 und 169 |
| M25 | Browser, unbekannte Adresse | {"ok":false,"message":"Unbekannter Endpunkt."} | src/cashdesk/main.cpp:490, src/barriers/main.cpp:409 |

Neue Anzeigen, die Testkonzept 1.1 verlangt und Firmware 1.0 laut Analyse des Codes noch nicht hat:

| Anzeige | Ort | Testfall |
| --- | --- | --- |
| Feld "Bezahlt am" mit Datum und Uhrzeit im Suchresultat | Kasse-Seite | T28 |
| Liste "Fehlversuche" mit den Spalten Datum, Uhrzeit, Eingabe, Grund. Grund ist "leer", "zu lang" oder "falsches Format". Eine leere Eingabe erscheint als "(leer)". Die neueste Zeile steht oben | Schranken-Seite | T27 |
| Nachzahlung nach Ablauf des Zeitfensters über die bestehenden Meldungen M16 und M10 mit dem Differenzbetrag | Kasse-Seite und Schranken-Seite | T29 |

### 5.5 Namen der Nachweise

| Nachweis | Muster | Beispiel |
| --- | --- | --- |
| Screenshot | Testfall_Zyklus_Nummer.png | T07_Z1_1.png |
| Screenshot der Eingangskriterien | EK_Zyklus_Nummer.png | EK_Z1_1.png |
| Monitor-Log | Monitor_Pico1_Zyklus.log | Monitor_Pico1_Z1.log |
| Notizen | ZyklusN_Notizen.md | Zyklus1_Notizen.md |
| Build-Nachweis | ID_Kurzhash.txt | H-01_d73ec89.txt |

Kein Nachweis zeigt das WLAN-Passwort. Screenshots von src/common/config.h sind nicht erlaubt.

## 6 Abgrenzung (ergänzt Kapitel 2.4)

| Punkt | Begründung |
| --- | --- |
| User Story 2, Akzeptanzkriterium 3 (Erkennung dauert zu lange) | Nicht anwendbar. Es gibt keine Kennzeichenerkennung, das Kennzeichen wird von Hand eingegeben (Testkonzept 1.0, Kapitel 2.4) |
| Router-Ausfall und Wiederverbinden des WLANs | Nicht getestet. Ein ausgeschalteter Router trennt auch den Testlaptop, die Ausfahrt liesse sich nicht auslösen. T23 simuliert den Ausfall der Kasse mit der Testhilfe "Kasse stumm schalten" |
| Mechanische Trennung von Einfahrt und Ausfahrt | Nicht prüfbar. Beide teilen sich einen Arm (Kapitel 4.1) |
| Produktivbuild | Nur mit T30 geprüft. Alle anderen Testfälle laufen im Test-Build |

## 7 Ablauf (ergänzt Kapitel 3.5 und 3.6)

### 7.1 Testzyklen

| Zyklus | Firmware | Inhalt | Termin |
| --- | --- | --- | --- |
| Zyklus 1 | Firmware 1.0, Test-Build | T01 bis T29 nach der Checkliste in Kapitel 7.5 | ‹OFFEN› |
| Fehlerbehebung | Firmware 1.1 und höher | je ein Commit pro Fehler mit Fehler-ID, Build-Nachweis | ‹OFFEN› |
| Zyklus 2 | Endstand, Test-Build | vollständiger Durchlauf T01 bis T29 mit einer Retest-Zeile je Fehler-ID | ‹OFFEN› |
| Abnahme | Endstand, Produktivbuild | T30 | ‹OFFEN› |

Die Ergebnisse von Zyklus 1 werden nie überschrieben. Zyklus 2 steht als eigener Eintrag mit Datum, Uhrzeit, Commit und Firmware-Version im Testprotokoll. Testkonzept 1.0 sah zwei Zyklen im Abstand von einer Woche vor. Wegen der Abgabe am 14.11.2026 liegen beide Zyklen in derselben Woche.

### 7.2 Testarten (ergänzt Kapitel 4)

Zu Positivtest, Negativtest, Grenzwerttest und Integrationstest kommt der Abnahmetest. Er prüft den Produktivbuild, der ausgeliefert wird (T30).

### 7.3 Bewertung eines Testfalls

| Status | Bedeutung |
| --- | --- |
| bestanden | Alle Punkte des erwarteten Ergebnisses wurden so beobachtet |
| nicht bestanden | Mindestens ein Punkt weicht ab. Die Abweichung bekommt eine Fehler-ID und eine Klasse nach Testkonzept 1.0, Kapitel 6 |
| nicht durchgeführt | Der Testfall konnte nicht ausgeführt werden. Der Grund steht im Protokoll |

### 7.4 Ausgangszustand und Reset

Reset bedeutet: `http://IP-KASSE/api/debug/reset` aufrufen oder auf der Kasse-Seite "Alle Vorgänge löschen" klicken. Danach zeigen beide Seiten "Belegung 0 / 3". Auf der Schranken-Seite kann das bis zu 3 s dauern.

| Testfall | Ausgangszustand | So wird er hergestellt |
| --- | --- | --- |
| T01 | leer | Reset |
| T02 | wie nach T01 | direkt nach T01 |
| T03 | leer, LU 12345 hat einen Durchlauf abgeschlossen | nach T02 LU 12345 an der Kasse bezahlen und ausfahren |
| T04 | leer | Reset |
| T05 | Arm offen nach T04 | direkt nach T04 |
| T06, T07, T08, T15, T18, T27 | beliebig | keine Vorarbeit |
| T09 | LU 12345 und ZH 98765 im Parkhaus | Reset, beide einfahren |
| T10 | voll (3 / 3) | direkt nach T09 |
| T11 | voll, LU 12345 bezahlt | nach T10 LU 12345 an der Kasse bezahlen |
| T12, T13, T14, T22, T24, T26, T29 | leer | Reset |
| T16 | LU 12345 eingefahren, nicht bezahlt | Reset, LU 12345 einfahren |
| T28, T17, T19 | LU 12345 bezahlt | direkt nach T16, in dieser Reihenfolge |
| T20 | ZH 98765 eingefahren, nicht bezahlt | ZH 98765 einfahren |
| T21 | BE 55511 eingefahren und bezahlt | BE 55511 einfahren und bezahlen |
| T23 | TG 44499 eingefahren und bezahlt | Reset, TG 44499 einfahren und bezahlen |
| T25 | LU 12345 im Parkhaus | LU 12345 einfahren |
| T30 | Produktivbuild geflasht | beide Picos mit dem Produktivbuild flashen |

### 7.5 Checkliste Zyklus 1

Vorbereitung:

1. Beide Picos mit dem Test-Build von Firmware 1.0 (Commit 867aed7) geflasht. Der Monitor zeigt das Banner mit "Firmware 1.0".
2. Für jeden Pico einen Monitor mit Zeitstempel und Logdatei geöffnet. COM-Ports notiert.
3. Zuerst die Kasse-Seite öffnen. Der Monitor von Pico 1 zeigt `[KASSE] Uhr gestellt: ...`. Danach die Schranken-Seite öffnen. Sie stellt nach der Behebung von User Story 3, Akzeptanzkriterium 3 auch die Uhr von Pico 2. In Zyklus 1 hat dieser Schritt noch keine Wirkung, gehört aber zum Ablauf.
4. Eingangskriterien prüfen und notieren:
   - beide Seiten laden in Chrome innerhalb von 3 s
   - Servo-Test: Meldung M23, Arm geht hoch und nach 5 s wieder runter
   - Reset: Meldung M22
   - Belegung "0 / 3" auf beiden Seiten
   - Chrome-Version notiert

Ablauf:

1. T29 Schritte 1 bis 3 als ersten Testfall. Zahlungszeit Z und Parkdauer P notieren. Timer auf Z plus 14 Minuten und Z plus 16 Minuten stellen.
2. In der Wartezeit T06, T07, T08, T27, T15, T18 und T25 ausführen. Während der Wartezeit gilt: kein Reset, kein Neustart von Pico 1, die Kasse nicht stumm schalten. Das T29-Fahrzeug belegt einen der drei Plätze, T25 belegt einen weiteren.
3. T29 Schritte 4 bis 8 zu den Zeitpunkten Z plus 14 Minuten und Z plus 16 Minuten.
4. Reset, dann T01, T02 und T03.
5. Reset, dann T04 und T05.
6. Reset, dann T09, T10 und T11.
7. Je ein Reset vor T12, vor T13 und vor T14.
8. Reset, dann T16, T28, T17 und T19.
9. T20 und T21.
10. Je ein Reset vor T22, vor T24 und vor T26.
11. Reset, dann T23 als letzten Testfall.

Bei jedem Testfall notieren: Uhrzeit, Kennzeichen, Ausgangszustand und wie er hergestellt wurde, Beobachtung (Meldungstext, Betrag, Servo, Monitorzeile), Status und Dateiname des Nachweises. Dafür gibt es die Notizdatei des Zyklus.

### 7.6 Zyklus 2 und Abnahme

Zyklus 2 folgt derselben Checkliste auf dem Endstand mit dem Test-Build. Das Banner zeigt dann die Firmware-Version des Endstands. Vor dem ersten Testfall auch die Schranken-Seite öffnen, damit die Uhr von Pico 2 gestellt ist. Nach T23 flasht die Testleitung den Produktivbuild desselben Commits auf beide Picos und führt T30 aus.

### 7.7 Austrittskriterien (ergänzt Kapitel 3.6)

Die Austrittskriterien aus Testkonzept 1.0 gelten unverändert für alle Testfälle T01 bis T30:

- Alle Testfälle wurden mindestens einmal durchgeführt.
- Kein Fehler der Klasse 1 oder Klasse 2 ist mehr offen.
- Alle Testfälle mit Priorität Hoch sind bestanden.
- Die Ergebnisse aller Testfälle sind mit Datum notiert.

## 8 Testfälle in Testkonzept 1.1

Jeder Testfall gilt in dieser Fassung. Die Zeile "Änderung gegenüber Testkonzept 1.0" nennt, was neu oder präziser ist. Alle Eingaben auf der Schranken-Seite werden mit einem Klick auf den Knopf bestätigt, nicht mit der Eingabetaste. Die Eingabetaste sendet auch bei gesperrtem Knopf eine Anfrage.

### 8.1 Kennzeichenerfassung bei Einfahrt

#### T01 Gültiges Kennzeichen bei der Einfahrt eintippen

| Feld | Inhalt |
| --- | --- |
| Bezug | User Story 1 (Akzeptanzkriterien 1 und 2), User Story 2 (Akzeptanzkriterium 1) |
| Priorität | Hoch |
| Testart | Positivtest |
| Vorbedingung | Reset. Belegung "0 / 3" auf beiden Seiten. Kasse-Seite und Schranken-Seite in Chrome offen, Uhr der Kasse gestellt. Monitore beider Picos laufen |
| Testdaten | LU 12345 |

Schritte:

1. Auf der Schranken-Seite im Feld "Einfahrt" LU 12345 eingeben.
2. "Einfahrt bestätigen" klicken und die Uhrzeit des Laptops notieren.
3. Den Arm beobachten.
4. Die Monitore beider Picos lesen.
5. Auf der Kasse-Seite die Liste "Fahrzeuge im Parkhaus" und die Belegung prüfen.

Erwartetes Ergebnis:

- Schranken-Seite: grüne Meldung M04 "LU 12345 ist registriert. Die Einfahrtsschranke öffnet und schliesst nach 5 Sekunden wieder."
- Servo: Der Arm geht innerhalb von 3 s von waagrecht auf senkrecht und nach 5 s ± 1 s wieder auf waagrecht.
- Monitor Pico 2: `[SCHRANKE] Schranke oeffnet (0 Grad).`, danach `[SCHRANKE] Schranke schliesst (90 Grad).`
- Monitor Pico 1: `[KASSE] Einfahrt: LU 12345  Belegung: 1/3`
- Kasse-Seite: LU 12345 steht unter "Fahrzeuge im Parkhaus" mit dem heutigen Datum und einer Uhrzeit als Einfahrt, Status "offen". Belegung "1 / 3" auf beiden Seiten.

Änderung gegenüber Testkonzept 1.0: Servo an GP8 statt GPIO 15, Stellungen nach Kapitel 4.1, Meldung im Wortlaut, Monitorzeilen, Reset als Vorbedingung.

#### T02 Einfahrtszeit stimmt mit der Uhrzeit des Laptops überein

| Feld | Inhalt |
| --- | --- |
| Bezug | User Story 1 (Akzeptanzkriterium 2) |
| Priorität | Mittel |
| Testart | Positivtest |
| Vorbedingung | T01 ausgeführt, LU 12345 im Parkhaus. Die Uhr der Kasse wurde beim ersten Öffnen der Kasse-Seite mit der Uhr dieses Laptops gestellt |
| Testdaten | LU 12345, in T01 notierte Uhrzeit des Klicks |

Schritte:

1. Auf der Kasse-Seite in der Liste "Fahrzeuge im Parkhaus" die Einfahrt von LU 12345 ablesen.
2. Datum und Uhrzeit mit der in T01 notierten Uhrzeit vergleichen.

Erwartetes Ergebnis:

- Das Datum stimmt mit dem Datum des Laptops überein.
- Die Uhrzeit weicht höchstens 1 Minute von der notierten Uhrzeit ab.

Hinweis: Die Kasse hat keine eigene Uhr. Sie bekommt die Uhrzeit beim ersten Öffnen der Kasse-Seite vom Laptop. Der Testfall prüft deshalb, ob die Einfahrtszeit gespeichert und richtig umgerechnet wird, nicht eine unabhängige Uhr.

Änderung gegenüber Testkonzept 1.0: Referenzuhr und Vergleichswert festgelegt.

#### T03 Gleiches Kennzeichen fährt nach abgeschlossenem Vorgang erneut ein

| Feld | Inhalt |
| --- | --- |
| Bezug | User Story 1 (Akzeptanzkriterium 3) |
| Priorität | Mittel |
| Testart | Grenzwerttest |
| Vorbedingung | Nach T02: LU 12345 an der Kasse bezahlt (M18) und an der Schranke ausgefahren (M11). Belegung "0 / 3". "Abgeschlossene Vorgänge" enthält LU 12345 |
| Testdaten | LU 12345 |

Schritte:

1. Im Feld "Einfahrt" LU 12345 eingeben und "Einfahrt bestätigen" klicken.
2. Auf der Kasse-Seite beide Listen prüfen.

Erwartetes Ergebnis:

- Schranken-Seite: M04 mit LU 12345. Der Arm öffnet und schliesst.
- Monitor Pico 1: `[KASSE] Einfahrt: LU 12345  Belegung: 1/3`
- "Fahrzeuge im Parkhaus": neue Zeile LU 12345 mit der aktuellen Einfahrtszeit, Status "offen".
- "Abgeschlossene Vorgänge": die alte Zeile LU 12345 mit unveränderter Einfahrt, Ausfahrt und Betrag, Status "ausgefahren".
- Belegung "1 / 3".

Änderung gegenüber Testkonzept 1.0: Vorbedingung als Ablauf beschrieben, beide Listen genannt.

### 8.2 Steuerung der Einfahrtsschranke

#### T04 Einfahrtsschranke öffnet innerhalb von 3 Sekunden

| Feld | Inhalt |
| --- | --- |
| Bezug | User Story 2 (Akzeptanzkriterium 1), Testziel Z3 |
| Priorität | Hoch |
| Testart | Positivtest |
| Vorbedingung | Reset, Belegung "0 / 3" |
| Testdaten | ZH 98765, Stoppuhr |

Schritte:

1. Im Feld "Einfahrt" ZH 98765 eingeben.
2. "Einfahrt bestätigen" klicken und gleichzeitig die Stoppuhr starten.
3. Die Stoppuhr stoppen, sobald der Arm senkrecht steht.
4. Die gemessene Zeit notieren.

Erwartetes Ergebnis:

- Der Arm steht höchstens 3,0 s nach dem Klick senkrecht (Stoppuhr).
- Schranken-Seite: M04 mit ZH 98765.
- Monitor Pico 2: `[SCHRANKE] Schranke oeffnet (0 Grad).`

Änderung gegenüber Testkonzept 1.0: Stellung "senkrecht" statt "90 Grad", Reset als Vorbedingung.

#### T05 Einfahrtsschranke schliesst nach 5 Sekunden

| Feld | Inhalt |
| --- | --- |
| Bezug | User Story 2 (Akzeptanzkriterium 2) |
| Priorität | Hoch |
| Testart | Positivtest |
| Vorbedingung | Direkt nach T04, der Arm ist offen |
| Testdaten | Monitor von Pico 2 mit Zeitstempel, Stoppuhr |

Schritte:

1. Die Stoppuhr starten, sobald der Arm senkrecht steht.
2. Die Stoppuhr stoppen, sobald der Arm wieder waagrecht steht.
3. Im Monitor von Pico 2 die Zeitstempel der Zeilen `[SCHRANKE] Schranke oeffnet (0 Grad).` und `[SCHRANKE] Schranke schliesst (90 Grad).` ablesen und die Differenz berechnen.

Erwartetes Ergebnis:

- Die Differenz der Zeitstempel beträgt 5,0 s ± 1,0 s.
- Die Stoppuhr zeigt 5,0 s ± 1,0 s.
- Der Arm steht danach waagrecht. Die Schranken-Seite zeigt beim Schrankenarm "geschlossen".

Änderung gegenüber Testkonzept 1.0: Messung zusätzlich über die Zeitstempel im Monitor, Stellung "waagrecht" statt "0 Grad".

### 8.3 Grenzfälle bei der Einfahrt

#### T06 Leeres Kennzeichen wird abgelehnt

| Feld | Inhalt |
| --- | --- |
| Bezug | User Story 3 (Akzeptanzkriterien 1 und 2) |
| Priorität | Hoch |
| Testart | Negativtest |
| Vorbedingung | Schranken-Seite offen, Zustand beliebig |
| Testdaten | leeres Feld |

Schritte:

1. Das Feld "Einfahrt" leer lassen.
2. "Einfahrt bestätigen" klicken.
3. Seite, Arm, Monitore und Kasse-Seite beobachten.

Erwartetes Ergebnis:

- Schranken-Seite: rote Meldung M01 "Bitte ein Kennzeichen eingeben."
- Der Arm bewegt sich nicht.
- Monitor Pico 2: `[EINGABE] Abgelehnt, ungueltiges Kennzeichen. Laenge: 0`
- Monitor Pico 1: keine neue Zeile `[KASSE] Einfahrt: ...`
- Kasse-Seite: Liste und Belegung unverändert.

Änderung gegenüber Testkonzept 1.0: Wortlaut der Meldung statt "klare Fehlermeldung", Monitorzeilen.

#### T07 Kennzeichen mit Sonderzeichen wird abgelehnt

| Feld | Inhalt |
| --- | --- |
| Bezug | User Story 3 (Akzeptanzkriterien 1 und 2) |
| Priorität | Mittel |
| Testart | Negativtest |
| Vorbedingung | Schranken-Seite offen, Zustand beliebig |
| Testdaten | `!@#$%^&*()` (10 Zeichen) |

Schritte:

1. Im Feld "Einfahrt" `!@#$%^&*()` eingeben.
2. "Einfahrt bestätigen" klicken.
3. Seite, Arm, Monitore und Kasse-Seite beobachten.

Erwartetes Ergebnis:

- Schranken-Seite: rote Meldung M03 "Das ist kein gültiges Kennzeichen. Erwartet wird zum Beispiel LU 12345."
- Der Arm bewegt sich nicht.
- Monitor Pico 2: `[EINGABE] Abgelehnt, ungueltiges Kennzeichen. Laenge: 10`
- Kasse-Seite: Liste und Belegung unverändert.

Hinweis: Die Eingabe hat genau 10 Zeichen und ist damit nicht zu lang. Deshalb kommt die Formatmeldung M03 und nicht M02.

Änderung gegenüber Testkonzept 1.0: Wortlaut der Meldung, Länge der Eingabe, Monitorzeile.

#### T08 Überlange Eingabe wird abgelehnt und lässt das System nicht abstürzen

| Feld | Inhalt |
| --- | --- |
| Bezug | User Story 3 (Akzeptanzkriterien 1 und 2), Testziel Z5 |
| Priorität | Mittel |
| Testart | Negativtest |
| Vorbedingung | Schranken-Seite offen, Monitor von Pico 2 läuft |
| Testdaten | 30 mal der Buchstabe A |

Schritte:

1. Im Feld "Einfahrt" 30 mal A eingeben.
2. "Einfahrt bestätigen" klicken.
3. Seite, Arm und Monitor von Pico 2 beobachten.

Erwartetes Ergebnis:

- Schranken-Seite: rote Meldung M02 "Das Kennzeichen ist zu lang. Erlaubt sind höchstens 10 Zeichen, zum Beispiel LU 12345."
- Der Arm bewegt sich nicht.
- Monitor Pico 2: `[EINGABE] Abgelehnt, ungueltiges Kennzeichen. Laenge: 30`
- Kein Neustart: Das Banner `=== Parksystem M450 - Pico 2 (Schranken) - Firmware 1.0 ===` erscheint nicht erneut. Die Seite bleibt bedienbar.

Änderung gegenüber Testkonzept 1.0: Wortlaut der Meldung, Neustart an einer bestimmten Monitorzeile erkennbar.

### 8.4 Kapazität des Parkhauses

#### T09 Letzter freier Platz wird belegt

| Feld | Inhalt |
| --- | --- |
| Bezug | User Story 4 (Akzeptanzkriterium 1) |
| Priorität | Hoch |
| Testart | Grenzwerttest |
| Vorbedingung | Reset, dann LU 12345 und ZH 98765 eingefahren (je M04). Belegung "2 / 3" |
| Testdaten | SG 11122 |

Schritte:

1. Im Feld "Einfahrt" SG 11122 eingeben und "Einfahrt bestätigen" klicken.
2. Die Belegung auf beiden Seiten prüfen.

Erwartetes Ergebnis:

- Schranken-Seite: M04 mit SG 11122. Der Arm öffnet und schliesst.
- Monitor Pico 1: `[KASSE] Einfahrt: SG 11122  Belegung: 3/3`
- Belegung "3 / 3" auf beiden Seiten.

Änderung gegenüber Testkonzept 1.0: Vorbedingung mit Reset und Einfahrten, Wortlaut der Meldung.

#### T10 Einfahrt wird bei vollem Parkhaus verweigert

| Feld | Inhalt |
| --- | --- |
| Bezug | User Story 4 (Akzeptanzkriterien 1 und 3) |
| Priorität | Hoch |
| Testart | Grenzwerttest |
| Vorbedingung | Direkt nach T09, Belegung "3 / 3" |
| Testdaten | TG 44499 |

Schritte:

1. Im Feld "Einfahrt" TG 44499 eingeben und "Einfahrt bestätigen" klicken.
2. Seite, Arm, Monitor von Pico 1 und Kasse-Seite beobachten.

Erwartetes Ergebnis:

- Schranken-Seite: rote Meldung M06 "Das Parkhaus ist voll. Keine weitere Einfahrt möglich."
- Der Arm bewegt sich nicht.
- Monitor Pico 1: keine Zeile `[KASSE] Einfahrt: TG 44499 ...`
- Kasse-Seite: TG 44499 steht nicht in der Liste. Belegung "3 / 3".

Änderung gegenüber Testkonzept 1.0: Wortlaut der Meldung statt "klarer Hinweis".

#### T11 Platz wird nach einer Ausfahrt wieder frei

| Feld | Inhalt |
| --- | --- |
| Bezug | User Story 4 (Akzeptanzkriterium 2), User Story 7 (Akzeptanzkriterien 1 und 2) |
| Priorität | Hoch |
| Testart | Positivtest |
| Vorbedingung | Direkt nach T10, Belegung "3 / 3". LU 12345 an der Kasse gesucht und bezahlt (M18 mit dem angezeigten Betrag) |
| Testdaten | LU 12345 für die Ausfahrt, TG 44499 für die Einfahrt |

Schritte:

1. Im Feld "Ausfahrt" LU 12345 eingeben und "Ausfahrt bestätigen" klicken.
2. Die Belegung auf beiden Seiten prüfen.
3. Im Feld "Einfahrt" TG 44499 eingeben und "Einfahrt bestätigen" klicken.
4. Die Belegung erneut prüfen.

Erwartetes Ergebnis:

- Schritt 1: grüne Meldung M11 "Bezahlung bestätigt. Die Ausfahrtsschranke öffnet für LU 12345." Der Arm öffnet und schliesst.
- Schritt 1, Monitor Pico 2: `[AUSFAHRT] Kasse meldet fuer LU 12345: paid`
- Schritt 1, Monitor Pico 1: `[KASSE] Statusabfrage von Pico 2: LU 12345 -> paid` und `[KASSE] Ausfahrt: LU 12345  Belegung: 2/3`
- Schritt 2: Belegung "2 / 3" auf beiden Seiten.
- Schritt 3: M04 mit TG 44499, der Arm öffnet und schliesst.
- Schritt 4: Belegung "3 / 3" auf beiden Seiten.

Änderung gegenüber Testkonzept 1.0: Bezahlung als Vorarbeit beschrieben, Wortlaut der Meldungen, Monitorzeilen.

### 8.5 Gebührenberechnung

#### T12 Gebühr nach 1 Minute Parkdauer

| Feld | Inhalt |
| --- | --- |
| Bezug | User Story 5 (Akzeptanzkriterien 1 und 2) |
| Priorität | Hoch |
| Testart | Positivtest |
| Vorbedingung | Reset, Belegung "0 / 3" |
| Testdaten | ZH 98765, Stoppuhr |

Schritte:

1. ZH 98765 einfahren lassen. Sobald M04 erscheint, die Stoppuhr starten.
2. Wenn die Stoppuhr 60 s zeigt, auf der Kasse-Seite ZH 98765 suchen. Die Suche muss zwischen 60 s und 119 s erfolgen.
3. Parkdauer und Gebühr ablesen und die Stoppuhrzeit der Suche notieren.

Erwartetes Ergebnis:

- Parkdauer "0 h 01 min".
- Gebühr "CHF 2.00", orange Meldung M16 "Offener Betrag: CHF 2.00", Status "offen".

Änderung gegenüber Testkonzept 1.0: "etwa 1 Minute" ersetzt durch ein Zeitfenster für die Suche und die genaue Anzeige.

#### T13 Gebühr genau am Übergang von der ersten zur zweiten Stunde

| Feld | Inhalt |
| --- | --- |
| Bezug | User Story 5 (Akzeptanzkriterium 2), Testziel Z4 |
| Priorität | Hoch |
| Testart | Grenzwerttest |
| Vorbedingung | Reset, BE 55511 eingefahren (M04) |
| Testdaten | BE 55511, Einfahrtszeit um 59, 60 und 61 Minuten verschoben |

Schritte:

1. Auf der Kasse-Seite in der Karte "Testwerkzeuge" BE 55511 und 59 Minuten eintragen und "Einfahrt zurücksetzen" klicken. Meldung ablesen.
2. Innerhalb von 60 s BE 55511 suchen, Parkdauer und Gebühr ablesen.
3. In der Karte "Testwerkzeuge" 60 Minuten eintragen und "Einfahrt zurücksetzen" klicken. Meldung ablesen.
4. In der Karte "Testwerkzeuge" 61 Minuten eintragen und "Einfahrt zurücksetzen" klicken.
5. Innerhalb von 60 s BE 55511 erneut suchen, Parkdauer und Gebühr ablesen.

Erwartetes Ergebnis:

- Schritt 1: "Einfahrtszeit um 59 Minuten zurückgesetzt. Gebühr für genau 0 h 59 min: CHF 2.00."
- Schritt 2: Parkdauer "0 h 59 min", M16 "Offener Betrag: CHF 2.00".
- Schritt 3: "Einfahrtszeit um 60 Minuten zurückgesetzt. Gebühr für genau 1 h 00 min: CHF 2.00." Das ist die Gebühr bei genau 60 Minuten.
- Schritt 5: Parkdauer "1 h 01 min", M16 "Offener Betrag: CHF 4.00".
- Monitor Pico 1 bei jeder Verschiebung: `[DEBUG] Einfahrtszeit verschoben: BE 55511 -N Minuten`

Hinweis: Nach jeder Verschiebung läuft die Uhr weiter. Schon eine Sekunde später ist die Parkdauer länger als 60 Minuten. Die Grenze bei genau 60 Minuten lässt sich deshalb nur über die Antwort der Testwerkzeuge prüfen (Testhilfe H-03). 59 und 61 Minuten werden über die Suche geprüft, also über die Anzeige, die der Gast sieht.

Änderung gegenüber Testkonzept 1.0: genaue Grenze über Testhilfe H-03, 59 und 61 Minuten über die Suche, Aufruf der Testwerkzeuge beschrieben.

#### T14 Gebühr für eine längere Parkdauer

| Feld | Inhalt |
| --- | --- |
| Bezug | User Story 5 (Akzeptanzkriterien 1 und 2) |
| Priorität | Mittel |
| Testart | Positivtest |
| Vorbedingung | Reset, SG 11122 eingefahren (M04) |
| Testdaten | SG 11122, Einfahrtszeit um 195 Minuten (3 h 15 min) verschoben |

Schritte:

1. In der Karte "Testwerkzeuge" SG 11122 und 195 Minuten eintragen und "Einfahrt zurücksetzen" klicken. Meldung ablesen.
2. Innerhalb von 60 s SG 11122 suchen, Parkdauer und Gebühr ablesen.

Erwartetes Ergebnis:

- Schritt 1: "Einfahrtszeit um 195 Minuten zurückgesetzt. Gebühr für genau 3 h 15 min: CHF 8.00."
- Schritt 2: Parkdauer "3 h 15 min", M16 "Offener Betrag: CHF 8.00". Begründung: Die vierte Stunde hat begonnen, 4 mal CHF 2.00.

Änderung gegenüber Testkonzept 1.0: Aufruf der Testwerkzeuge und Zeitfenster für die Suche festgelegt.

#### T15 Unbekanntes Kennzeichen an der Kasse

| Feld | Inhalt |
| --- | --- |
| Bezug | User Story 5 (Akzeptanzkriterium 3) |
| Priorität | Mittel |
| Testart | Negativtest |
| Vorbedingung | Kasse-Seite offen. UR 99988 ist nicht im Parkhaus |
| Testdaten | UR 99988 |

Schritte:

1. Auf der Kasse-Seite UR 99988 eingeben und "Suchen" klicken.
2. Meldung, Suchresultat und Knopf "Bezahlen" prüfen.

Erwartetes Ergebnis:

- Rote Meldung M15 "Das Kennzeichen UR 99988 wurde im System nicht gefunden. Es ist keine Einfahrt bekannt."
- Es erscheinen keine Angaben zu Einfahrt, Parkdauer und Gebühr.
- Der Knopf "Bezahlen" ist ausgegraut und lässt sich nicht klicken.

Änderung gegenüber Testkonzept 1.0: Wortlaut der Meldung.

### 8.6 Bezahlung

#### T16 Bezahlung erfolgreich durchführen

| Feld | Inhalt |
| --- | --- |
| Bezug | User Story 6 (Akzeptanzkriterien 1 und 2) |
| Priorität | Hoch |
| Testart | Positivtest |
| Vorbedingung | Reset, LU 12345 eingefahren (M04), Uhrzeit der Einfahrt notiert. Die Parkdauer ist kürzer als 60 Minuten |
| Testdaten | LU 12345 |

Schritte:

1. Auf der Kasse-Seite LU 12345 suchen.
2. Parkdauer und Betrag ablesen.
3. "Bezahlen" klicken und die Uhrzeit des Klicks notieren (wird in T28 gebraucht).
4. Meldung, Status, Knopf und Liste prüfen.

Erwartetes Ergebnis:

- Schritt 2: Parkdauer kürzer als "1 h 00 min", M16 "Offener Betrag: CHF 2.00". Begründung: eine angefangene Stunde mal CHF 2.00.
- Schritt 4: grüne Meldung M18 "Bezahlung erfolgt. Betrag: CHF 2.00." Status "bezahlt". Der Knopf "Bezahlen" ist ausgegraut. In der Liste steht bei LU 12345 Status "bezahlt" und Gebühr CHF 2.00.
- Monitor Pico 1: `[KASSE] Bezahlt: LU 12345 CHF 2.00`

Änderung gegenüber Testkonzept 1.0: Betrag CHF 2.00 mit Begründung statt "stimmt mit dem Tarif überein".

#### T17 Doppelte Bezahlung wird verhindert

| Feld | Inhalt |
| --- | --- |
| Bezug | User Story 6, robustes Verhalten der Bezahlung |
| Priorität | Mittel |
| Testart | Negativtest |
| Vorbedingung | Direkt nach T16 und T28, LU 12345 bezahlt |
| Testdaten | LU 12345 |

Schritte:

1. Auf der Kasse-Seite LU 12345 erneut suchen.
2. Meldung und Knopf "Bezahlen" prüfen.
3. In einem neuen Tab `http://IP-KASSE/api/pay?plate=LU%2012345` aufrufen. Das löst eine Zahlung direkt aus, ohne Knopf.
4. Auf der Kasse-Seite LU 12345 erneut suchen.

Erwartetes Ergebnis:

- Schritt 2: grüne Meldung M17 "Für dieses Kennzeichen wurde bereits bezahlt. Eine weitere Zahlung ist nicht nötig." Status "bezahlt". Der Knopf "Bezahlen" ist ausgegraut und lässt sich nicht klicken.
- Schritt 3: Antwort `{"ok":true,"status":"paid","alreadyPaid":true,"amount":"CHF 2.00","message":"Für dieses Kennzeichen wurde bereits bezahlt (CHF 2.00)."}`. Im Monitor von Pico 1 erscheint keine neue Zeile `[KASSE] Bezahlt: ...`
- Schritt 4: Gebühr weiterhin CHF 2.00.

Änderung gegenüber Testkonzept 1.0: nur ein erwartetes Verhalten (Knopf ausgegraut) statt zwei Varianten. Zusätzlich wird der Schutz auf der Kasse über den direkten Aufruf geprüft.

#### T18 Bezahlung für unbekanntes Kennzeichen

| Feld | Inhalt |
| --- | --- |
| Bezug | User Story 5 (Akzeptanzkriterium 3), User Story 6 (Akzeptanzkriterium 3) |
| Priorität | Mittel |
| Testart | Negativtest |
| Vorbedingung | Kasse-Seite offen. ZG 77766 ist nicht im Parkhaus |
| Testdaten | ZG 77766 |

Schritte:

1. Auf der Kasse-Seite ZG 77766 suchen.
2. Den Knopf "Bezahlen" prüfen.
3. In einem neuen Tab `http://IP-KASSE/api/pay?plate=ZG%2077766` aufrufen.

Erwartetes Ergebnis:

- Schritt 1: rote Meldung M15 "Das Kennzeichen ZG 77766 wurde im System nicht gefunden. Es ist keine Einfahrt bekannt."
- Schritt 2: Der Knopf "Bezahlen" ist ausgegraut und lässt sich nicht klicken.
- Schritt 3: Antwort `{"ok":false,"status":"unknown","alreadyPaid":false,"amount":"CHF 0.00","message":"Dieses Kennzeichen ist nicht im System. Eine Bezahlung ist nicht möglich."}`. Im Monitor von Pico 1 erscheint keine Zeile `[KASSE] Bezahlt: ...`

Änderung gegenüber Testkonzept 1.0: direkter Aufruf ergänzt, damit sich T18 von T15 unterscheidet und der Schutz auf der Kasse geprüft wird.

### 8.7 Steuerung der Ausfahrtsschranke

#### T19 Ausfahrt nach erfolgreicher Bezahlung

| Feld | Inhalt |
| --- | --- |
| Bezug | User Story 7 (Akzeptanzkriterien 1 und 2), Testziel Z3 |
| Priorität | Hoch |
| Testart | Positivtest |
| Vorbedingung | Direkt nach T17, LU 12345 bezahlt und im Parkhaus |
| Testdaten | LU 12345, Stoppuhr |

Schritte:

1. Im Feld "Ausfahrt" LU 12345 eingeben.
2. "Ausfahrt bestätigen" klicken und gleichzeitig die Stoppuhr starten.
3. Die Stoppuhr stoppen, sobald der Arm senkrecht steht.
4. Die Monitore beider Picos lesen.
5. Auf der Kasse-Seite die Listen und die Belegung prüfen.

Erwartetes Ergebnis:

- Monitor Pico 1: `[KASSE] Statusabfrage von Pico 2: LU 12345 -> paid`, danach `[KASSE] Ausfahrt: LU 12345  Belegung: 0/3`
- Monitor Pico 2: `[AUSFAHRT] Kasse meldet fuer LU 12345: paid`, danach `[SCHRANKE] Schranke oeffnet (0 Grad).`
- Schranken-Seite: grüne Meldung M11 "Bezahlung bestätigt. Die Ausfahrtsschranke öffnet für LU 12345."
- Servo: Der Arm steht höchstens 3,0 s nach dem Klick senkrecht und nach 5 s ± 1 s wieder waagrecht.
- Kasse-Seite: LU 12345 steht nicht mehr unter "Fahrzeuge im Parkhaus", sondern unter "Abgeschlossene Vorgänge" mit Status "ausgefahren". Belegung "0 / 3".

Änderung gegenüber Testkonzept 1.0: Statusabfrage über die Monitorzeilen beobachtbar (Testhilfe H-01), Servo an GP8 statt GPIO 16, Stellungen nach Kapitel 4.1.

#### T20 Ausfahrt ohne Bezahlung wird verweigert

| Feld | Inhalt |
| --- | --- |
| Bezug | User Story 8 (Akzeptanzkriterien 1, 2 und 3), Testziel Z2 |
| Priorität | Hoch |
| Testart | Negativtest |
| Vorbedingung | ZH 98765 eingefahren (M04) und nicht bezahlt. Parkdauer kürzer als 60 Minuten |
| Testdaten | ZH 98765 |

Schritte:

1. Im Feld "Ausfahrt" ZH 98765 eingeben und "Ausfahrt bestätigen" klicken.
2. Seite, Arm und Monitore beobachten.
3. Auf der Kasse-Seite ZH 98765 suchen und bezahlen.
4. Im Feld "Ausfahrt" ZH 98765 erneut eingeben und "Ausfahrt bestätigen" klicken.

Erwartetes Ergebnis:

- Schritt 1: rote Meldung M10 "Die Parkgebühr für ZH 98765 ist noch offen (CHF 2.00). Bitte zuerst an der Kasse bezahlen. Die Schranke bleibt geschlossen."
- Schritt 2: Der Arm bewegt sich nicht. Monitor Pico 1: `[KASSE] Statusabfrage von Pico 2: ZH 98765 -> unpaid`. Monitor Pico 2: `[AUSFAHRT] Kasse meldet fuer ZH 98765: unpaid` und `[AUSFAHRT] Noch nicht bezahlt: ZH 98765`
- Schritt 3: M18 "Bezahlung erfolgt. Betrag: CHF 2.00."
- Schritt 4: M11 "Bezahlung bestätigt. Die Ausfahrtsschranke öffnet für ZH 98765." Der Arm öffnet und schliesst.

Änderung gegenüber Testkonzept 1.0: Wortlaut der Meldung statt "klarer Hinweis", Monitorzeilen, Schritte 3 und 4 für User Story 8, Akzeptanzkriterium 3.

#### T21 Grenzfall bezahlt, aber Kennzeichen bei der Ausfahrt anders

| Feld | Inhalt |
| --- | --- |
| Bezug | Zusätzlicher Grenzfall aus dem Feedback zu den User Stories, User Story 7 (Akzeptanzkriterium 1) |
| Priorität | Hoch |
| Testart | Grenzwerttest |
| Vorbedingung | BE 55511 eingefahren (M04) und an der Kasse bezahlt (M18) |
| Testdaten | BE 55511 bezahlt, bei der Ausfahrt zuerst BE 55512 |

Schritte:

1. Im Feld "Ausfahrt" BE 55512 eingeben und "Ausfahrt bestätigen" klicken.
2. Seite, Arm und Monitore beobachten.
3. Im Feld "Ausfahrt" BE 55511 eingeben und "Ausfahrt bestätigen" klicken.

Erwartetes Ergebnis:

- Schritt 1: rote Meldung M09 "Zu dem Kennzeichen BE 55512 ist keine Einfahrt bekannt. Die Schranke bleibt geschlossen."
- Schritt 2: Der Arm bewegt sich nicht. Monitor Pico 1: `[KASSE] Statusabfrage von Pico 2: BE 55512 -> unknown`. Monitor Pico 2: `[AUSFAHRT] Kasse meldet fuer BE 55512: unknown` und `[AUSFAHRT] Unbekanntes Kennzeichen: BE 55512`
- Schritt 3: M11 "Bezahlung bestätigt. Die Ausfahrtsschranke öffnet für BE 55511." Der Arm öffnet und schliesst. Monitor Pico 1: `[KASSE] Ausfahrt: BE 55511  Belegung: 0/3`

Änderung gegenüber Testkonzept 1.0: Wortlaut der Meldung statt "Hinweis", Monitorzeilen.

### 8.8 Zusammenspiel beider Picos

#### T22 Kompletter Durchlauf von der Einfahrt bis zur Ausfahrt

| Feld | Inhalt |
| --- | --- |
| Bezug | Alle User Stories, Integrationstest über das gesamte System |
| Priorität | Hoch |
| Testart | Integrationstest |
| Vorbedingung | Reset, Belegung "0 / 3" |
| Testdaten | SG 11122, Einfahrtszeit um 90 Minuten verschoben |

Schritte:

1. Im Feld "Einfahrt" SG 11122 eingeben und "Einfahrt bestätigen" klicken.
2. Warten, bis der Arm wieder waagrecht steht.
3. In der Karte "Testwerkzeuge" SG 11122 und 90 Minuten eintragen und "Einfahrt zurücksetzen" klicken. Dieser Schritt kommt vor dem Bezahlen, weil die Verschiebung eine Bezahlung aufhebt.
4. Innerhalb von 60 s SG 11122 suchen, Parkdauer und Gebühr ablesen.
5. "Bezahlen" klicken.
6. Im Feld "Ausfahrt" SG 11122 eingeben und "Ausfahrt bestätigen" klicken.
7. Warten, bis der Arm wieder waagrecht steht.
8. Auf der Kasse-Seite die Listen und die Belegung prüfen.

Erwartetes Ergebnis:

- Schritt 1: M04 mit SG 11122. Der Arm öffnet.
- Schritt 2: Der Arm schliesst nach 5 s ± 1 s.
- Schritt 3: "Einfahrtszeit um 90 Minuten zurückgesetzt. Gebühr für genau 1 h 30 min: CHF 4.00."
- Schritt 4: Parkdauer "1 h 30 min", M16 "Offener Betrag: CHF 4.00".
- Schritt 5: M18 "Bezahlung erfolgt. Betrag: CHF 4.00."
- Schritt 6: M11 "Bezahlung bestätigt. Die Ausfahrtsschranke öffnet für SG 11122." Monitorzeilen mit `paid` wie in T19.
- Schritt 7: Der Arm schliesst nach 5 s ± 1 s.
- Schritt 8: SG 11122 steht unter "Abgeschlossene Vorgänge" mit Bezahlt CHF 4.00 und Status "ausgefahren". Belegung "0 / 3" auf beiden Seiten.

Hinweis: Einfahrt und Ausfahrt nutzen denselben Arm. Welche Logik ihn öffnet, zeigen die Meldungen M04 und M11.

Änderung gegenüber Testkonzept 1.0: Reihenfolge von Verschiebung und Bezahlung begründet, Wortlaut der Meldungen und Beträge.

#### T23 Verbindung zur Kasse fällt kurz aus

| Feld | Inhalt |
| --- | --- |
| Bezug | Integrationstest, robustes Verhalten bei einem Ausfall, Testziele Z2 und Z5 |
| Priorität | Hoch |
| Testart | Integrationstest |
| Vorbedingung | Reset, TG 44499 eingefahren (M04) und bezahlt (M18). Kasse-Seite und Schranken-Seite offen. Monitore beider Picos laufen |
| Testdaten | TG 44499, `http://IP-KASSE/api/debug/offline?seconds=30`, Stoppuhr |

Schritte:

1. In einem neuen Tab `http://IP-KASSE/api/debug/offline?seconds=30` aufrufen.
2. Innerhalb von 20 s auf der Schranken-Seite im Feld "Ausfahrt" TG 44499 eingeben, "Ausfahrt bestätigen" klicken und gleichzeitig die Stoppuhr starten.
3. Die Stoppuhr stoppen, sobald die Meldung erscheint. Arm, Seite und Monitore beobachten.
4. Warten, bis der Monitor von Pico 1 `[DEBUG] Kasse antwortet wieder.` zeigt, dann noch 10 s warten.
5. Im Feld "Ausfahrt" TG 44499 erneut eingeben und "Ausfahrt bestätigen" klicken.

Erwartetes Ergebnis:

- Schritt 1: Antwort `{"ok":true,"seconds":30,"message":"Die Kasse beantwortet 30 Sekunden lang keine Anfragen."}`. Monitor Pico 1: `[DEBUG] Kasse antwortet 30 s lang nicht.`
- Schritt 3: rote Meldung M08 "Die Kasse ist zurzeit nicht erreichbar. Der Bezahlstatus kann nicht geprüft werden, die Schranke bleibt geschlossen." Sie erscheint höchstens 5 s nach dem Klick. Begründung: 2 s Wartezeit der Statusabfrage und bis zu 2 s für eine Abfrage der Belegung, die Pico 2 gerade im Hintergrund ausführt.
- Schritt 3: Der Arm bewegt sich nicht. Monitor Pico 2: eine Zeile, die mit `[KASSE-CLIENT] Kasse nicht erreichbar, Fehler` beginnt, und `[AUSFAHRT] Kasse nicht erreichbar, Schranke bleibt zu.` Die Schranken-Seite zeigt oben "Kasse nicht erreichbar".
- Während des ganzen Tests erscheint in keinem Monitor erneut ein Banner. Kein Pico startet neu.
- Schritt 5: M11 "Bezahlung bestätigt. Die Ausfahrtsschranke öffnet für TG 44499." Der Arm öffnet und schliesst. Monitor Pico 2: `[AUSFAHRT] Kasse meldet fuer TG 44499: paid`. Die Schranken-Seite zeigt wieder "Kasse erreichbar".

Änderung gegenüber Testkonzept 1.0: Der Ausfall wird mit der Testhilfe "Kasse stumm schalten" (H-04) simuliert statt mit dem Router. Ein ausgeschalteter Router würde auch den Laptop trennen, und die Ausfahrt liesse sich nicht auslösen. Wortlaut der Meldung, Zeitgrenze und Monitorzeilen ergänzt.

#### T24 Kennzeichen bei Pico 2 eingegeben taucht bei Pico 1 auf

| Feld | Inhalt |
| --- | --- |
| Bezug | Integrationstest, Datenaustausch zwischen den Picos, User Story 1 (Akzeptanzkriterien 1 und 2) |
| Priorität | Mittel |
| Testart | Integrationstest |
| Vorbedingung | Reset, Belegung "0 / 3". Kasse-Seite in einem zweiten Tab offen |
| Testdaten | ZH 98765 |

Schritte:

1. Auf der Schranken-Seite ZH 98765 einfahren lassen und die Uhrzeit des Klicks notieren.
2. Innerhalb von 3 s zum Tab der Kasse-Seite wechseln.
3. Die Liste "Fahrzeuge im Parkhaus" beobachten, ohne die Seite neu zu laden.

Erwartetes Ergebnis:

- ZH 98765 erscheint unter "Fahrzeuge im Parkhaus" spätestens 3 s nach dem Wechsel. Die Seite fragt alle 2 s neu ab, dazu kommt die Antwortzeit.
- Die Einfahrt weicht höchstens 1 Minute von der notierten Uhrzeit ab. Status "offen".
- Monitor Pico 1: `[KASSE] Einfahrt: ZH 98765  Belegung: 1/3`

Änderung gegenüber Testkonzept 1.0: Aktualisierung ohne Neuladen beschrieben, Zeitgrenzen und Monitorzeile.

### 8.9 Neue Testfälle

#### T25 Kennzeichen fährt ein, obwohl es schon im Parkhaus steht

| Feld | Inhalt |
| --- | --- |
| Bezug | User Story 1 (Akzeptanzkriterium 3, Abgrenzung), User Story 4 (Kapazität). Grund: Bemerkung 3 aus der Bewertung |
| Priorität | Hoch |
| Testart | Negativtest |
| Vorbedingung | LU 12345 im Parkhaus (M04). In Zyklus 1 steht zusätzlich das Fahrzeug aus T29 im Parkhaus. Belegung notiert |
| Testdaten | LU 12345, lu12345 |

Schritte:

1. Im Feld "Einfahrt" LU 12345 eingeben und "Einfahrt bestätigen" klicken.
2. Seite, Arm, Belegung und Monitor von Pico 1 beobachten.
3. Im Feld "Einfahrt" lu12345 eingeben und "Einfahrt bestätigen" klicken.
4. Auf der Kasse-Seite die Liste "Fahrzeuge im Parkhaus" prüfen.

Erwartetes Ergebnis:

- Schritte 1 und 3: rote Meldung M05 "Dieses Fahrzeug steht bereits im Parkhaus."
- Der Arm bewegt sich nicht. Die Belegung bleibt wie notiert.
- Monitor Pico 1: keine neue Zeile `[KASSE] Einfahrt: LU 12345 ...`
- Kasse-Seite: genau eine Zeile LU 12345 unter "Fahrzeuge im Parkhaus" mit der ursprünglichen Einfahrtszeit.

#### T26 Kennzeichenformat an den Grenzen

| Feld | Inhalt |
| --- | --- |
| Bezug | User Story 3 (Akzeptanzkriterien 1 und 2). Grund: Bemerkung 4 aus der Bewertung |
| Priorität | Mittel |
| Testart | Grenzwerttest |
| Vorbedingung | Reset, Belegung "0 / 3" |
| Testdaten | Eingaben aus der Tabelle unten |

Schritte:

1. Jede Eingabe der Tabelle in dieser Reihenfolge im Feld "Einfahrt" eingeben und "Einfahrt bestätigen" klicken.
2. Nach jeder Eingabe Meldung, Arm und Monitor von Pico 2 beobachten.
3. Zum Schluss die Liste "Fahrzeuge im Parkhaus" und die Belegung prüfen.

| Eingabe | Zeichen | Erwartete Meldung | Arm |
| --- | --- | --- | --- |
| LU 122 | 6 | M04 "LU 122 ist registriert. ..." | öffnet |
| LU 123456 | 9 | M04 "LU 123456 ist registriert. ..." | öffnet |
| lu1 | 3 | M04 "LU 1 ist registriert. ..." | öffnet |
| 12345 | 5 | M03 | bleibt zu |
| LU | 2 | M03 | bleibt zu |
| L 12345 | 7 | M03 | bleibt zu |
| LU 12A45 | 8 | M03 | bleibt zu |
| LU 1234567 | 10 | M03 | bleibt zu |
| LU 12345678 | 11 | M02 | bleibt zu |

Erwartetes Ergebnis:

- Jede Eingabe ergibt die Meldung aus der Tabelle.
- Bei jeder ungültigen Eingabe zeigt der Monitor von Pico 2 `[EINGABE] Abgelehnt, ungueltiges Kennzeichen. Laenge: N` mit der Zeichenzahl aus der Tabelle.
- Kasse-Seite: LU 122, LU 123456 und LU 1 stehen unter "Fahrzeuge im Parkhaus". Belegung "3 / 3".

#### T27 Fehlversuch bei der Einfahrt wird mit Datum und Uhrzeit festgehalten

| Feld | Inhalt |
| --- | --- |
| Bezug | User Story 3 (Akzeptanzkriterium 3) |
| Priorität | Mittel |
| Testart | Negativtest |
| Vorbedingung | Die Schranken-Seite wurde seit dem letzten Start von Pico 2 geöffnet, damit die Uhr von Pico 2 gestellt ist. Zustand sonst beliebig |
| Testdaten | leeres Feld, 30 mal der Buchstabe A, `!@#$%^&*()` |

Schritte:

1. Das Feld "Einfahrt" leer lassen, "Einfahrt bestätigen" klicken, Uhrzeit notieren.
2. 30 mal A eingeben, "Einfahrt bestätigen" klicken, Uhrzeit notieren.
3. `!@#$%^&*()` eingeben, "Einfahrt bestätigen" klicken, Uhrzeit notieren.
4. Auf der Schranken-Seite den Bereich "Fehlversuche" ablesen.

Erwartetes Ergebnis:

- Schritte 1 bis 3: Meldungen M01, M02 und M03 wie in T06 bis T08.
- Schritt 4: Die Schranken-Seite zeigt unter der Überschrift "Fehlversuche" eine Liste mit den Spalten Datum, Uhrzeit, Eingabe und Grund. Die drei Versuche aus diesem Testfall stehen zuoberst, der neueste oben:

| Eingabe | Grund |
| --- | --- |
| !@#$%^&*() | falsches Format |
| AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA | zu lang |
| (leer) | leer |

- Datum jeder Zeile: heutiges Datum. Uhrzeit jeder Zeile: höchstens 1 Minute Abweichung zur notierten Uhrzeit.

Hinweis: Die Liste liegt im Arbeitsspeicher von Pico 2. Nach einem Neustart von Pico 2 ist sie leer, und die Uhr von Pico 2 muss über die Schranken-Seite neu gestellt werden. Das ist eine bekannte Einschränkung.

#### T28 Zahlungszeitpunkt wird gespeichert und angezeigt

| Feld | Inhalt |
| --- | --- |
| Bezug | User Story 6 (Akzeptanzkriterium 2) |
| Priorität | Mittel |
| Testart | Positivtest |
| Vorbedingung | Direkt nach T16. LU 12345 bezahlt, Uhrzeit des Klicks auf "Bezahlen" notiert |
| Testdaten | LU 12345 |

Schritte:

1. Auf der Kasse-Seite LU 12345 suchen.
2. Die Angaben im Suchresultat ablesen.

Erwartetes Ergebnis:

- Das Suchresultat zeigt zusätzlich zu Kennzeichen, Einfahrt, Parkdauer, Gebühr und Status das Feld "Bezahlt am" mit Datum (TT.MM.JJJJ) und Uhrzeit (hh:mm:ss).
- Das Datum stimmt mit dem Datum des Laptops überein. Die Uhrzeit weicht höchstens 1 Minute von der in T16 notierten Uhrzeit ab.

#### T29 Gebühr wird nach Ablauf des Zeitfensters nachberechnet

| Feld | Inhalt |
| --- | --- |
| Bezug | User Story 7 (Akzeptanzkriterium 3) |
| Priorität | Hoch |
| Testart | Grenzwerttest |
| Vorbedingung | Reset, Kasse-Seite und Schranken-Seite offen. Während des ganzen Testfalls: kein Reset, kein Neustart von Pico 1, die Kasse nicht stumm schalten |
| Testdaten | SG 11122, Einfahrtszeit um 50 Minuten verschoben, Zeitfenster 15 Minuten, Stoppuhr oder Uhr des Laptops |

Die Parkdauer zeigt ganze Minuten. Für Parkdauern gilt deshalb eine Toleranz von 1 Minute. Z ist die Uhrzeit der Zahlung, P ist die Parkdauer, die beim Bezahlen angezeigt wird.

Schritte:

1. SG 11122 einfahren lassen.
2. In der Karte "Testwerkzeuge" SG 11122 und 50 Minuten eintragen und "Einfahrt zurücksetzen" klicken. Dieser Schritt kommt vor dem Bezahlen, weil die Verschiebung eine Bezahlung aufhebt.
3. Innerhalb von 9 Minuten nach Schritt 2 auf der Kasse-Seite SG 11122 suchen und "Bezahlen" klicken. Z und P notieren.
4. Bei Z plus 14 Minuten SG 11122 suchen.
5. Bei Z plus 16 Minuten SG 11122 erneut suchen.
6. Im Feld "Ausfahrt" SG 11122 eingeben und "Ausfahrt bestätigen" klicken.
7. Auf der Kasse-Seite SG 11122 suchen und "Bezahlen" klicken.
8. Im Feld "Ausfahrt" SG 11122 erneut eingeben und "Ausfahrt bestätigen" klicken.

Erwartetes Ergebnis:

- Schritt 1: M04 mit SG 11122.
- Schritt 2: "Einfahrtszeit um 50 Minuten zurückgesetzt. Gebühr für genau 0 h 50 min: CHF 2.00."
- Schritt 3 vor dem Bezahlen: Parkdauer zwischen "0 h 50 min" und "0 h 59 min", M16 "Offener Betrag: CHF 2.00". Nach dem Bezahlen: M18 "Bezahlung erfolgt. Betrag: CHF 2.00."
- Schritt 4: Parkdauer P plus 14 Minuten, also mehr als 60 Minuten, die zweite Stunde hat begonnen. Status "bezahlt", grüne Meldung M17. Innerhalb des Zeitfensters wird nichts nachberechnet.
- Schritt 5: Parkdauer P plus 16 Minuten. Status "offen", M16 "Offener Betrag: CHF 2.00". Begründung: Gesamtgebühr CHF 4.00 für die angefangene zweite Stunde minus bezahlte CHF 2.00.
- Schritt 6: rote Meldung M10 "Die Parkgebühr für SG 11122 ist noch offen (CHF 2.00). Bitte zuerst an der Kasse bezahlen. Die Schranke bleibt geschlossen." Der Arm bewegt sich nicht. Monitor Pico 2: `[AUSFAHRT] Kasse meldet fuer SG 11122: unpaid`
- Schritt 7: M18 "Bezahlung erfolgt. Betrag: CHF 2.00."
- Schritt 8: M11 "Bezahlung bestätigt. Die Ausfahrtsschranke öffnet für SG 11122." Der Arm öffnet und schliesst.

Hinweis: Die Verschiebung um 50 statt 59 Minuten lässt 10 Minuten Puffer. Sonst könnte die Gebühr schon vor dem Bezahlen auf CHF 4.00 springen, und der Testfall würde aus dem falschen Grund scheitern.

#### T30 Abnahmetest des Produktivbuilds

| Feld | Inhalt |
| --- | --- |
| Bezug | Abnahme, alle User Stories |
| Priorität | Hoch |
| Testart | Abnahmetest |
| Vorbedingung | Zyklus 2 abgeschlossen. Beide Picos mit dem Produktivbuild des Endstand-Commits geflasht (Umgebungen pico1_cashdesk_prod und pico2_barriers_prod). Zuerst die Kasse-Seite öffnen, dann die Schranken-Seite |
| Testdaten | LU 12345 |

Schritte:

1. Im Monitor beider Picos das Banner ablesen.
2. Im Browser nacheinander aufrufen und die Antwort notieren: `http://IP-KASSE/api/debug/time?plate=LU%2012345&minutes=60`, `http://IP-KASSE/api/debug/reset`, `http://IP-KASSE/api/debug/offline?seconds=30`, `http://IP-SCHRANKE/api/servotest`
3. Kasse-Seite und Schranken-Seite nach den Karten der Testwerkzeuge absuchen.
4. LU 12345 einfahren lassen, an der Kasse bezahlen und ausfahren lassen.

Erwartetes Ergebnis:

- Schritt 1: `=== Parksystem M450 - Pico 1 (Kasse) - Firmware X ===` und `=== Parksystem M450 - Pico 2 (Schranken) - Firmware X ===`. X ist die Firmware-Version des Endstands.
- Schritt 2: Jeder Aufruf ergibt `{"ok":false,"message":"Unbekannter Endpunkt."}` (M25).
- Schritt 3: Die Kasse-Seite zeigt keine Karte "Testwerkzeuge (nur im Test-Build)". Die Schranken-Seite zeigt keine Karte "Smoke-Test der Servos (nur im Test-Build)".
- Schritt 4: Einfahrt M04, der Arm öffnet und schliesst. Bezahlen M18 "Bezahlung erfolgt. Betrag: CHF 2.00." Ausfahrt M11, der Arm öffnet und schliesst. Danach Belegung "0 / 3".

Hinweis: Das HTML der beiden Karten bleibt auch im Produktivbuild in der Seite und wird nur ausgeblendet. Geprüft werden deshalb die Sichtbarkeit und die Endpunkte, nicht der Quelltext der Seite.

## 9 Zuordnung der Akzeptanzkriterien zu den Testfällen

| User Story, Akzeptanzkriterium | Inhalt kurz | Testfälle | Bemerkung |
| --- | --- | --- | --- |
| 1.1 | Kennzeichen wird bei der Einfahrt erfasst | T01, T24 | Eingabe ersetzt die Kamera (Testkonzept 1.0, Kapitel 2.4) |
| 1.2 | Gespeichert mit Datum und Uhrzeit | T01, T02, T24 | |
| 1.3 | Erneute Einfahrt ist ein neuer Eintrag | T03 | T25 prüft die Einfahrt, solange das Fahrzeug noch drin ist |
| 2.1 | Schranke öffnet bei gültigem Kennzeichen | T01, T04, T09 | |
| 2.2 | Schranke schliesst von selbst | T05 | zeitgesteuert, 5 s |
| 2.3 | Erkennung dauert zu lange | keiner | nicht anwendbar (Kapitel 6) |
| 3.1 | Ungültiges Kennzeichen: Schranke bleibt zu | T06, T07, T08, T26 | |
| 3.2 | Information zum weiteren Vorgehen | T06, T07, T08, T26 | |
| 3.3 | Fehlversuch mit Zeitstempel protokolliert | T27 | |
| 4.1 | Volles Parkhaus: Schranke bleibt zu | T09, T10 | |
| 4.2 | Platz nach Ausfahrt wieder frei | T11 | |
| 4.3 | Information "voll" | T10 | |
| 5.1 | Parkdauer ab Einfahrt | T12, T13, T14 | |
| 5.2 | Gebühr nach Tarif | T12, T13, T14, T22 | |
| 5.3 | Unbekanntes Kennzeichen: Fehlermeldung statt Gebühr | T15, T18 | |
| 6.1 | Offener Betrag wird angezeigt | T16 | |
| 6.2 | Als bezahlt markiert, mit Zeitpunkt | T16, T28 | |
| 6.3 | Zahlung scheitert: bleibt unbezahlt, Information | T18 | Zahlung ist simuliert, geprüft wird der Fall "unbekanntes Kennzeichen" |
| 7.1 | Bezahlt: Ausfahrtsschranke öffnet | T11, T19, T21, T22 | |
| 7.2 | Vorgang abgeschlossen, Platz frei | T19, T22 | Abschluss geschieht vor dem Öffnen der Schranke |
| 7.3 | Nachberechnung nach Ablauf einer Frist | T29 | Zeitfenster 15 Minuten |
| 8.1 | Ohne Zahlung bleibt die Schranke zu | T20 | |
| 8.2 | Meldung mit Ort der Bezahlung | T20 | |
| 8.3 | Nach dem Bezahlen direkt weiterfahren | T20 | Schritte 3 und 4 |

Testfälle ohne eigenes Akzeptanzkriterium: T17 (doppelte Bezahlung), T21 (Grenzfall aus dem Feedback), T23 (Ausfall der Kasse), T25 (doppelte Einfahrt), T30 (Abnahme des Produktivbuilds).
