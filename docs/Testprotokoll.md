# Testprotokoll Parksystem mit Schranken

| Feld | Wert |
| --- | --- |
| Projekt | Parksystem mit Schranken, Modul M450, Auftrag A01-04 |
| Dokument | Testprotokoll |
| Version | Protokoll 0.1 |
| Bearbeitungsdatum | 12.11.2026 |
| Dokumentstatus | in Arbeit |
| Klassifizierung | nicht klassifiziert |
| Autor | Adam Mehila |
| Gruppe | Gruppe Mehila, Einzelarbeit. Mitgewirkt hat nur Adam Mehila |
| Modul | M450 Applikationen testen (ELA), Klasse INA24b |
| Auftraggeber | Parkhausbesitzer (fiktiv), vertreten durch die Lehrperson M450 (ELA) |

<!-- SEITENUMBRUCH -->

<!-- INHALTSVERZEICHNIS -->

## 0 Einleitung

### 0.1 Ziel und Zweck des Dokuments

Dieses Testprotokoll hält fest, wie das Parksystem getestet wurde und was dabei herauskam. Die Ergebnisse sind nach den Fehlerklassen des Testkonzepts bewertet.

Das Protokoll beantwortet für den Auftraggeber eine Frage: Kann das Parksystem in Betrieb genommen werden? Die Antwort steht in Kapitel 1 und ausführlich in Kapitel 9.

Grundlage ist Testkonzept 1.1. Es besteht aus Testkonzept 1.0 und einem Nachtrag mit korrigierten und neuen Testfällen. Der Nachtrag ist vollständig in Anhang A enthalten.

### 0.2 Referenzierte Dokumente

| Nr. | Titel | Version | Datum |
| --- | --- | --- | --- |
| [1] | Testkonzept Parksystem mit Schranken | Testkonzept 1.0 | 25.09.2026 |
| [2] | Testkonzept Parksystem mit Schranken, Nachtrag (Anhang A) | Testkonzept 1.1 | 12.11.2026 |
| [3] | User Stories Parksystem mit Schranken | ohne Versionsangabe | 07.09.2026 |
| [4] | Bewertung Testkonzept M450 INA24b | ohne Versionsangabe | 29.10.2026 |
| [5] | Auftrag A01-03 Implementierung Parkhaus mit Schranke | ohne Versionsangabe | ohne Datumsangabe |
| [6] | Auftrag A01-04 Testdurchführung und Testprotokoll Parksystem | ohne Versionsangabe | Abgabe 14.11.2026 |
| [7] | Firmware Parksystem, Git-Repository ParkhausSchranken, Branch Testprotokoll | Firmware 1.0 (Commit 867aed7) | 12.11.2026 |
| [8] | HERMES-Ergebnisvorlage Testprotokoll | ohne Versionsangabe | 26.01.2026 |

### 0.3 Mitgeltende Unterlagen

| Titel | Version | Stand |
| --- | --- | --- |
| Parkhaus mit Schranke (README des Projekts) | ohne Versionsangabe | Repository, Branch Testprotokoll |
| Projekt einrichten und starten (Anleitung) | ohne Versionsangabe | Repository, Branch Testprotokoll |
| Das Parkhaus-Projekt verstehen (Erklärung des Codes) | ohne Versionsangabe | Repository, Branch Testprotokoll |

### 0.4 Definitionen und Abkürzungen

| Begriff | Bedeutung |
| --- | --- |
| AK | Akzeptanzkriterium einer User Story. AK 3.3 heisst User Story 3, Akzeptanzkriterium 3 |
| Fehler-ID | Kennung eines gefundenen Fehlers, F-01 bis F-nn |
| H-01 bis H-07 | Testhilfen, siehe Kapitel 7.1. H-01 bis H-05 sind in der Firmware, H-06 und H-07 sind Werkzeuge auf dem Testlaptop |
| Kasse-Seite | Weboberfläche von Pico 1 im Browser |
| M01 bis M25 | Meldungen aus dem Meldungskatalog, Testkonzept 1.1, Kapitel 5.4 |
| MK | Mängelklasse, entspricht der Fehlerklasse 1 bis 4 aus Testkonzept 1.0, Kapitel 6 |
| Pico 1 | Raspberry Pi Pico W mit der Kasse |
| Pico 2 | Raspberry Pi Pico W mit den Schranken und dem Servo |
| Produktivbuild | Firmware ohne Testwerkzeuge (TEST_BUILD=0) |
| Schranken-Seite | Weboberfläche von Pico 2 im Browser |
| Test-Build | Firmware mit Testwerkzeugen (TEST_BUILD=1) |
| Zyklus | ein vollständiger Durchgang durch die Testfälle |
| Z1 bis Z5 | Testziele aus Testkonzept 1.0, Kapitel 1.1 |
| ‹OFFEN› | Wert noch nicht beobachtet oder noch nicht bekannt |

### 0.5 Vorgaben, Methoden und Werkzeuge

| Titel | Vorgabe, Methode oder Werkzeug | Version |
| --- | --- | --- |
| Vorlage | HERMES-Ergebnisvorlage Testprotokoll | ohne Versionsangabe, 26.01.2026 |
| Testgrundlage | Testkonzept Parksystem mit Schranken | Testkonzept 1.1 |
| Entwicklungsumgebung | Visual Studio Code mit PlatformIO | PlatformIO Core 6.2.0 |
| Serieller Monitor | tools/serial_monitor.py (H-06), Python und pyserial aus PlatformIO | Python 3.11.7, pyserial 3.5 |
| Steuerung und Ablesung von Chrome | tools/browser_test.mjs (H-07) über das Chrome-DevTools-Protokoll | Node.js 26.13.0 |
| Browser für alle Tests | Google Chrome | ‹OFFEN› |
| Versionsverwaltung | Git | Branch Testprotokoll |
| Zeitmessung | Stoppuhr auf dem Handy, Zeitstempel im seriellen Monitor | |
| Erzeugung des PDF | Python mit Modul markdown, Microsoft Edge im Hintergrundmodus | Edge 157.0.4341.46 |

## 1 Zusammenfassung und Empfehlung

Empfehlung der Testleitung: ‹OFFEN›

| Kennzahl | Zyklus 1 | Zyklus 2 |
| --- | --- | --- |
| Ausgeführte Testfälle | ‹OFFEN› | ‹OFFEN› |
| bestanden | ‹OFFEN› | ‹OFFEN› |
| nicht bestanden | ‹OFFEN› | ‹OFFEN› |
| nicht durchgeführt | ‹OFFEN› | ‹OFFEN› |
| Offene Fehler Klasse 1 und 2 | ‹OFFEN› | ‹OFFEN› |
| Offene Fehler Klasse 3 und 4 | ‹OFFEN› | ‹OFFEN› |

Wichtigste Restrisiken für den Betrieb: siehe Kapitel 9.2. ‹OFFEN›

## 2 Identifikation

### 2.1 Testobjekt

Getestet wird das Parksystem mit Schranken aus Auftrag A01-03. Es besteht aus zwei Raspberry Pi Pico W:

- Pico 1 ist die Kasse. Er speichert alle Parkvorgänge, berechnet die Gebühr und nimmt die Bezahlung entgegen.
- Pico 2 steuert die Schranken. Er nimmt Einfahrt und Ausfahrt entgegen, fragt vor jeder Ausfahrt bei Pico 1 den Bezahlstatus ab und bewegt den Servo.

Beide Picos haben eine Weboberfläche, die im Browser des Testlaptops geöffnet wird. Das Eintippen des Kennzeichens ersetzt die Kamera.

### 2.2 Getestete Stände

| Einsatz | Firmware | Commit | Build | Umgebungen | Geflasht am |
| --- | --- | --- | --- | --- | --- |
| Zyklus 1 | Firmware 1.0 | 867aed7 | Test-Build (TEST_BUILD=1) | pico1_cashdesk, pico2_barriers | ‹OFFEN› |
| Zyklus 2 | ‹OFFEN› | ‹OFFEN› | Test-Build (TEST_BUILD=1) | pico1_cashdesk, pico2_barriers | ‹OFFEN› |
| T30 | ‹OFFEN› | ‹OFFEN› | Produktivbuild (TEST_BUILD=0) | pico1_cashdesk_prod, pico2_barriers_prod | ‹OFFEN› |

Die Netzwerkwerte (WLAN-Name, Adressen) stehen nur lokal in der Konfigurationsdatei und sind nicht Teil der Commits. Das WLAN-Passwort wird nirgends dokumentiert.

### 2.3 Bezug zum Testkonzept

Getestet wird gegen Testkonzept 1.1 (Anhang A). Es übernimmt Testziele, Teststufe, Rollen und Fehlerklassifizierung aus Testkonzept 1.0. Es passt die Testumgebung an die Umsetzung an, beschreibt Testwerkzeuge und Ablauf und enthält die Testfälle T01 bis T30. T01 bis T24 sind präzisiert, T25 bis T30 sind neu. Kapitel 7.2 nennt die Gründe.

## 3 Testumgebung und Tester

### 3.1 Ort

| Zyklus | Ort |
| --- | --- |
| Zyklus 1 | ‹OFFEN› |
| Zyklus 2 | ‹OFFEN› |

### 3.2 Hardware

| Komponente | Ist |
| --- | --- |
| Pico 1 (Kasse) | Raspberry Pi Pico W, Strom über USB vom Testlaptop |
| Pico 2 (Schranken) | Raspberry Pi Pico W, Strom über USB vom Testlaptop |
| Servo | ein SG90 an GP8 (Pin 11), VBUS (Pin 40), GND (Pin 8). Einfahrt und Ausfahrt teilen sich den Arm |
| Stellungen | zu: 90 Grad, Arm waagrecht. Offen: 0 Grad, Arm senkrecht |
| Testlaptop | Windows 11 Pro 10.0.26300 |
| Stoppuhr | Handy |

### 3.3 Netzwerk

| Angabe | Ist |
| --- | --- |
| WLAN-Name (2,4 GHz) | ‹OFFEN› |
| Pico 1 (Kasse) | ‹OFFEN› |
| Pico 2 (Schranken) | ‹OFFEN› |
| Gateway, Subnetzmaske | ‹OFFEN› |
| Testlaptop | im selben WLAN |

### 3.4 Software

| Software | Ist |
| --- | --- |
| Browser | Google Chrome ‹OFFEN› |
| Entwicklungsumgebung | Visual Studio Code mit PlatformIO Core 6.2.0 |
| Serieller Monitor | bis 13.11.2026 17:33 `pio device monitor --port COMx --filter time --filter log2file`, ab 17:34 tools/serial_monitor.py (H-06), Kapitel 4, A15 |
| Steuerung von Chrome | tools/browser_test.mjs (H-07), Kapitel 4, A16 |
| COM-Port Pico 1 | ‹OFFEN› |
| COM-Port Pico 2 | ‹OFFEN› |

### 3.5 Rollen und Personen

| Rolle | Aufgabe | Person |
| --- | --- | --- |
| Testleitung | Plant den Ablauf, prüft die Eingangskriterien, vergibt die Fehlerklassen, empfiehlt die Inbetriebnahme | Adam Mehila |
| Tester Einfahrt und Ausfahrt | Bedient die Schranken-Seite, misst Zeiten, beobachtet Servo und Monitor von Pico 2 | Adam Mehila |
| Tester Kasse | Bedient die Kasse-Seite, prüft Gebühren und Listen, beobachtet den Monitor von Pico 1 | Adam Mehila |
| Entwicklung | Behebt Fehler, liefert neue Firmware-Versionen, erstellt die Build-Nachweise | Adam Mehila |
| Auftraggeber | Entscheidet über die Inbetriebnahme | Parkhausbesitzer (fiktiv), vertreten durch die Lehrperson M450 (ELA) |

Hilfsmittel: Die Testhilfen H-01 bis H-07 und die Entwürfe der Testdokumente sind mit Unterstützung von Claude Code (KI-Assistent) entstanden. Prüfung und Verantwortung liegen bei Adam Mehila.

Durchführung in Zyklus 1: Bis 13.11.2026 18:15 hat Adam Mehila alle Schritte in Chrome ausgeführt und beobachtet. Danach hat auf Anweisung von Adam Mehila Claude Code die Schritte ausgeführt und Meldungen, Zustände und Monitor-Logs abgelesen: in seiner Abwesenheit per HTTP-Abfrage, nach seiner Rückkehr mit H-07 in Chrome. Adam Mehila hat dabei den Servo-Arm beobachtet. Jedes Einzelprotokoll nennt, welche Beobachtung von wem stammt (Kapitel 4, A16).

### 3.6 Zeitpunkte der Durchführung

| Zyklus | Datum | Beginn | Ende |
| --- | --- | --- | --- |
| Zyklus 1 | ‹OFFEN› | ‹OFFEN› | ‹OFFEN› |
| Zyklus 2 | ‹OFFEN› | ‹OFFEN› | ‹OFFEN› |
| T30 (Abnahme) | ‹OFFEN› | ‹OFFEN› | ‹OFFEN› |

### 3.7 Eingangskriterien

| Kriterium (Testkonzept 1.1, Kapitel 7.5) | Zyklus 1 | Zyklus 2 |
| --- | --- | --- |
| Beide Picos mit dem getesteten Stand geflasht, Banner zeigt die Firmware-Version | ‹OFFEN› | ‹OFFEN› |
| Monitor für jeden Pico mit Zeitstempel und Logdatei offen | ‹OFFEN› | ‹OFFEN› |
| Kasse-Seite zuerst geöffnet, Monitor zeigt `[KASSE] Uhr gestellt: ...`, danach Schranken-Seite geöffnet | ‹OFFEN› | ‹OFFEN› |
| Beide Seiten laden in Chrome innerhalb von 3 s | ‹OFFEN› | ‹OFFEN› |
| Servo-Test: Meldung M23, Arm geht hoch und nach 5 s wieder runter | ‹OFFEN› | ‹OFFEN› |
| Reset: Meldung M22, Belegung "0 / 3" auf beiden Seiten | ‹OFFEN› | ‹OFFEN› |

### 3.8 Testwerkzeuge und Beobachtung

Aufruf, Antwort und Regeln der Testwerkzeuge stehen in Testkonzept 1.1, Kapitel 4.4. Die Orte der Beobachtung und die Monitorzeilen stehen in Kapitel 4.5 und 4.6. Die Statusabfrage von Pico 2 an Pico 1 ist auf beiden Seiten sichtbar: Pico 1 zeigt `[KASSE] Statusabfrage von Pico 2: KENNZEICHEN -> STATUS`, Pico 2 zeigt `[AUSFAHRT] Kasse meldet fuer KENNZEICHEN: STATUS`.

## 4 Abweichungen der Umsetzung vom Testkonzept

Grundsätzlich gelten Testumgebung, Rollen und Testfälle aus dem Testkonzept. Diese Tabelle nennt jede Abweichung.

| Nr. | Bereich | Testkonzept 1.0 | Umsetzung oder Durchführung | Begründung | Behandlung in Testkonzept 1.1 |
| --- | --- | --- | --- | --- | --- |
| A01 | Hardware | zwei Servos, Einfahrt an GPIO 15, Ausfahrt an GPIO 16 | ein Servo an GP8 für Einfahrt und Ausfahrt | Umsetzung mit einem Servo nach dem Verkabelungsbeispiel aus Auftrag A01-03. Einfahrt und Ausfahrt sind nur über die Meldungen unterscheidbar | Kapitel 4.1, Soll-Werte der Testfälle angepasst |
| A02 | Hardware | zu bei 0 Grad, offen bei 90 Grad | zu bei 90 Grad, offen bei 0 Grad | Der Arm sitzt anders auf der Achse. Waagrecht bleibt zu, senkrecht bleibt offen | Kapitel 4.1 |
| A03 | Hardware | Kartonstreifen als Schrankenarm | mitgelieferter Arm des SG90 | Arm war vorhanden | Kapitel 4.1 |
| A04 | Netzwerk | WLAN PARK-TEST-NET, Pico 1 auf 192.168.4.11, Pico 2 auf 192.168.4.12 | ‹OFFEN› | ‹OFFEN› | Kapitel 4.2 |
| A05 | Software | "Version 1.0 des Test-Builds", ohne Kennung im Gerät | Firmware 1.0 mit den Testhilfen H-01 bis H-05, Commit 867aed7, Version im Banner | Der getestete Stand muss eindeutig erkennbar sein | Kapitel 4.3 |
| A06 | Testwerkzeuge | nur der Endpunkt zur Zeitmanipulation, ohne Aufruf | zusätzlich Reset, Kasse stumm schalten, Servo-Test, Gebühr für exakte Dauer, jeweils mit Aufruf und Antwort | Ausgangszustand, T13 und T23 brauchen sie | Kapitel 4.4 |
| A07 | T23 | WLAN-Router für 30 s ausschalten | Pico 1 beantwortet 30 s lang keine Anfragen (Testhilfe H-04) | Ein ausgeschalteter Router trennt auch den Laptop, die Ausfahrt liesse sich nicht auslösen | T23, Kapitel 6 |
| A08 | Ablauf | alle Testfälle in der Reihenfolge der ID | T29 zuerst, die Wartezeit von 16 Minuten wird für andere Testfälle genutzt. Reset vor jedem Block | T29 braucht echte Wartezeit. Die Vorbedingungen der Testfälle passen nicht zur Reihenfolge der ID | Kapitel 7.4 und 7.5 |
| A09 | Zeitplan | zwei Zyklen im Abstand einer Woche, jeweils Donnerstag 14:05 bis 15:50 | beide Zyklen bis zur Abgabe am 14.11.2026 | Abgabetermin | Kapitel 7.1 |
| A10 | Zyklus 2 | Retest der fehlerhaften Testfälle und Regression über alle Testfälle mit Priorität Hoch | vollständiger Durchlauf T01 bis T29, danach T30 | Die Regression für die geplanten Behebungen umfasst ohnehin fast alle Testfälle (Kapitel 7.4) | Kapitel 7.1 |
| A11 | Messmethode T05 | Stoppuhr | Stoppuhr und Zeitstempel im seriellen Monitor | genauer als die Reaktionszeit beim Stoppen | T05 |
| A12 | Umfang | Akzeptanzkriterium 2.3 (Erkennung dauert zu lange) ohne Testfall | nicht anwendbar | Es gibt keine Kennzeichenerkennung | Kapitel 6 |
| A13 | Testfälle | T01 bis T24 | T01 bis T24 präzisiert, T25 bis T30 neu | Bemerkungen aus der Bewertung, fehlende Akzeptanzkriterien, Abnahme des Produktivbuilds | Kapitel 8 |
| A14 | Dokumentation | Das README sagt, im Produktivbuild seien die Testwerkzeuge "gar nicht mehr in der Firmware enthalten" | Die Endpunkte fehlen, die Karten der Testwerkzeuge sind nur ausgeblendet | Laut Code bleibt das HTML der Karten in der Seite | T30 prüft Sichtbarkeit und Endpunkte |
| A15 | Serieller Monitor | Testkonzept 1.1, Kapitel 4.3: je Pico ein Terminal mit `pio device monitor --port COMx --filter time --filter log2file` | ab 13.11.2026 17:34 tools/serial_monitor.py (H-06) im Hintergrund, gleiches Zeilenformat, eine Logdatei je Pico und Zyklus | `pio device monitor` verlangt ein interaktives Terminal und hat sich nach dem Neustart der Picos nicht wieder verbunden. Banner und `[KASSE] Uhr gestellt` fehlten deshalb im ersten Durchgang der Eingangskriterien | keine, Abweichung nur in diesem Protokoll |
| A16 | Durchführung | Tester Einfahrt und Ausfahrt und Tester Kasse bedienen die Seiten in Chrome und beobachten selbst | ab 13.11.2026 18:15 führt Claude Code die Schritte aus und liest Meldungen, Zustände und Monitor-Logs ab: per HTTP-Abfrage bis zur Rückkehr von Adam Mehila, danach mit H-07 in Chrome. Adam Mehila beobachtet den Servo-Arm | Entscheid der Testleitung während Zyklus 1. H-07 benutzt dieselbe Seite, dieselben Knöpfe und dieselben Anfragen an die Picos wie die Bedienung von Hand. Jede Aktion steht mit Uhrzeit, Eingabe und abgelesenem Ergebnis im Log | keine, Abweichung nur in diesem Protokoll |

## 5 Testdurchführung Zyklus 1

Stand: Firmware 1.0, Test-Build, Commit 867aed7. Reihenfolge nach der Checkliste in Testkonzept 1.1, Kapitel 7.5. Durchgeführt am 13.11.2026 von 17:18 bis 20:54 und am 14.11.2026 von 21:13 bis 21:53, dazwischen waren beide Picos ausgesteckt. Wer welchen Schritt ausgeführt und beobachtet hat, steht in jedem Einzelprotokoll in der Zeile Tester (Kapitel 3.5 und Kapitel 4, A16).

<!-- QUERFORMAT-ANFANG -->

### 5.1 Übersicht Zyklus 1

| ID | Bezeichnung | Datum, Uhrzeit | Erwartet (kurz) | Tatsächlich (kurz) | Status | MK |
| --- | --- | --- | --- | --- | --- | --- |
| T01 | Gültiges Kennzeichen bei der Einfahrt | 13.11.2026 20:31 | M04, Arm offen in höchstens 3 s, zu nach 5 s ± 1 s, Belegung 1 / 3 | M04, Arm bewegt: ja, Öffnen 0,134 s und Schliessen 5,012 s laut Monitor, Belegung 1 / 3 | bestanden | – |
| T02 | Einfahrtszeit stimmt mit der Laptop-Uhr | 13.11.2026 20:31 | Abweichung höchstens 1 Minute | Einfahrt 20:30:59, Klick 20:31:00, Abweichung 1 s | bestanden | – |
| T03 | Erneute Einfahrt nach Abschluss | 13.11.2026 20:32 | neuer Eintrag, alter Eintrag bleibt erhalten | neuer Eintrag 20:32:49 offen, alter Eintrag unverändert ausgefahren | bestanden | – |
| T04 | Einfahrtsschranke öffnet in 3 s | 14.11.2026 21:24 | Arm senkrecht in höchstens 3,0 s | M04, Öffnen 0,109 s laut Monitor, Beobachtung des Arms nicht verwertbar, keine Stoppuhr | nicht durchgeführt | – |
| T05 | Einfahrtsschranke schliesst nach 5 s | 14.11.2026 21:24 | 5,0 s ± 1,0 s | 5,034 s laut Monitor, keine Stoppuhr, keine verwertbare Beobachtung | nicht durchgeführt | – |
| T06 | Leeres Kennzeichen | 13.11.2026 18:04 | M01, Arm bleibt zu | M01, Arm bewegt: nein, Laenge 0 | bestanden | – |
| T07 | Sonderzeichen | 13.11.2026 18:18 | M03, Arm bleibt zu | M03 per HTTP, Laenge 10, Schranke zu laut Zustand | bestanden | – |
| T08 | Überlange Eingabe | 13.11.2026 20:19 | M02, kein Neustart | M02, Arm bewegt: nein, Laenge 30, kein Neustart | bestanden | – |
| T09 | Letzter freier Platz | 14.11.2026 21:37 | M04, Belegung 3 / 3 | M04, Arm bewegt: ja, Belegung 3 / 3 | bestanden | – |
| T10 | Parkhaus voll | 14.11.2026 21:26 und 21:37 | M06, Arm bleibt zu | erster Durchgang ohne Meldung und Belegung 0 / 0, zweiter Durchgang M06, Arm bewegt: nein | nicht bestanden | 2 (F-04) |
| T11 | Platz nach Ausfahrt frei | 14.11.2026 21:38 | M11, Belegung 2 / 3, dann 3 / 3 | M11, Arm bewegt: ja, Belegung 2 / 3, dann M04 und 3 / 3 | bestanden | – |
| T12 | Gebühr nach 1 Minute | 14.11.2026 21:41 | "0 h 01 min", CHF 2.00 | nach 69 s "0 h 01 min", CHF 2.00, offen | bestanden | – |
| T13 | Gebühr an der Stundengrenze | 14.11.2026 21:41 | 59 min und genau 60 min CHF 2.00, 61 min CHF 4.00 | 59 min CHF 2.00, genau 60 min CHF 2.00, 61 min CHF 4.00 | bestanden | – |
| T14 | Gebühr nach 3 h 15 min | 14.11.2026 21:42 | CHF 8.00 | "3 h 15 min", CHF 8.00 | bestanden | – |
| T15 | Unbekanntes Kennzeichen an der Kasse | 13.11.2026 20:24 | M15, Bezahlen ausgegraut | M15, kein Suchresultat, Bezahlen gesperrt | bestanden | – |
| T16 | Bezahlung erfolgreich | 14.11.2026 21:43 | M16 CHF 2.00, M18 | M16 CHF 2.00, M18 CHF 2.00, Status bezahlt | bestanden | – |
| T17 | Doppelte Bezahlung verhindert | 14.11.2026 21:43 | M17, Bezahlen ausgegraut, M19 ohne zweite Buchung | M17, Bezahlen gesperrt, M19 mit `"alreadyPaid":true`, keine zweite Buchung | bestanden | – |
| T18 | Bezahlung unbekanntes Kennzeichen | 13.11.2026 20:24 | M15, Bezahlen ausgegraut, M20 | M15, Bezahlen gesperrt, M20 mit `"ok":false` | bestanden | – |
| T19 | Ausfahrt nach Bezahlung | 14.11.2026 21:44 | Statusabfrage paid, M11, Arm offen in höchstens 3 s | Statusabfrage paid, M11, Arm bewegt: ja, Öffnen 0,149 s laut Monitor | bestanden | – |
| T20 | Ausfahrt ohne Bezahlung | 14.11.2026 21:45 | M10, Arm bleibt zu, nach Bezahlung M11 | M10 CHF 2.00, Arm bewegt: nein, nach Bezahlung M11 und Arm bewegt: ja | bestanden | – |
| T21 | Bezahlt, Kennzeichen anders | 14.11.2026 21:46 | M09 für BE 55512, M11 für BE 55511 | M09 für BE 55512. BE 55511: Kasse verbucht die Ausfahrt, Schranke bleibt zu | nicht bestanden | 2 (F-04) |
| T22 | Kompletter Durchlauf | 14.11.2026 21:49 | CHF 4.00, M18, M11, Belegung 0 / 3 | CHF 4.00 und M18, Ausfahrt mit M08 statt M11, Arm bewegt: nein | nicht bestanden | 2 (F-04) |
| T23 | Kasse fällt kurz aus | 14.11.2026 21:52 | M08 in höchstens 5 s, kein Neustart, danach M11 | M08 nach 2,1 s, Arm bewegt: nein, kein Neustart, danach M11 und Arm bewegt: ja | bestanden | – |
| T24 | Kennzeichen erscheint an der Kasse | 14.11.2026 21:50 | Eintrag in höchstens 3 s, Status offen | Eintrag nach 0,3 s, Status offen. Nebenbefund F-04 | bestanden | – |
| T25 | Doppelte Einfahrt | 13.11.2026 20:26 | M05, Belegung unverändert | zweimal M05, Belegung unverändert, eine Zeile LU 12345 | bestanden | – |
| T26 | Kennzeichenformat | 14.11.2026 21:50 | 3 mal M04, 5 mal M03, 1 mal M02 | 3 mal M04, 5 mal M03, 1 mal M02, Belegung 3 / 3 | bestanden | – |
| T27 | Fehlversuch mit Datum und Uhrzeit | 13.11.2026 20:22 | Liste "Fehlversuche" mit 3 Zeilen | M01, M02, M03, keine Liste "Fehlversuche" | nicht bestanden | 3 (F-02) |
| T28 | Zahlungszeitpunkt | 14.11.2026 21:43 | "Bezahlt am", Abweichung höchstens 1 Minute | kein Feld "Bezahlt am" | nicht bestanden | 3 (F-01) |
| T29 | Nachberechnung nach Zeitfenster | 13.11.2026 18:01 bis 18:19 | nach 14 min bezahlt, nach 16 min offen CHF 2.00, M10, nach Nachzahlung M11 | bei Z plus 17 min Status "bezahlt", Schritt 4 nicht durchgeführt | nicht bestanden | 2 (F-03) |

<!-- QUERFORMAT-ENDE -->

Zusammenfassung Zyklus 1: 29 Testfälle, 21 bestanden, 6 nicht bestanden (T10, T21, T22, T27, T28, T29), 2 nicht durchgeführt (T04, T05). Gefunden: F-01 bis F-04.

### 5.2 Einzelprotokolle Zyklus 1

Jedes Einzelprotokoll nennt das erwartete Ergebnis aus Testkonzept 1.1 in Kurzform. Massgebend ist der vollständige Testfall in Anhang A.

#### T29 Gebühr wird nach Ablauf des Zeitfensters nachberechnet (Zyklus 1, Start)

| Feld | Eintrag |
| --- | --- |
| Datum, Uhrzeit | 13.11.2026, 17:58:25 bis 18:01:02 |
| Tester | Adam Mehila, Ausführung und Beobachtung in Chrome |
| Stand, Browser | Firmware 1.0, Test-Build, Commit 867aed7, Chrome 157.0.8103.79 |
| Ausgangszustand | Soll: leer nach Reset. Hergestellt: Reset um 17:45:54 bei den Eingangskriterien, danach kein Fahrzeug eingefahren |
| Eingaben | SG 11122, Einfahrtszeit um 50 Minuten verschoben |
| Erwartet | Schritte 1 bis 3: M04, Meldung der Testwerkzeuge mit CHF 2.00, Parkdauer 0 h 50 min bis 0 h 59 min, M16 CHF 2.00, M18 CHF 2.00 |
| Tatsächlich | Einfahrt: grüne Meldung M04 mit SG 11122, Arm ging hoch und nach 5 s wieder runter (Adam). Testwerkzeuge: "Einfahrtszeit um 50 Minuten zurückgesetzt. Gebühr für genau 0 h 50 min: CHF 2.00." Vor dem Bezahlen Parkdauer "0 h 51 min" und orange Meldung "Offener Betrag: CHF 2.00". Nach dem Bezahlen "Bezahlung erfolgt. Betrag: CHF 2.00.", Status "bezahlt", Belegung "1 / 3". Monitor Pico 1: `[KASSE] Einfahrt: SG 11122  Belegung: 1/3`, `[DEBUG] Einfahrtszeit verschoben: SG 11122 -50 Minuten`, `[KASSE] Bezahlt: SG 11122 CHF 2.00` |
| Messwerte | Zahlungszeit Z 18:01:01.778 (Monitor Pico 1), Parkdauer P "0 h 51 min" |
| Nachweise | T29_Z1_1.png, Monitor_Pico1_Z1.log, Monitor_Pico2_Z1.log |

Die Bewertung von T29 steht beim zweiten Teil weiter unten.

#### T06 Leeres Kennzeichen wird abgelehnt (Zyklus 1)

| Feld | Eintrag |
| --- | --- |
| Datum, Uhrzeit | 13.11.2026, 18:04:52 |
| Tester | Adam Mehila, Ausführung und Beobachtung in Chrome |
| Stand, Browser | Firmware 1.0, Test-Build, Commit 867aed7, Chrome 157.0.8103.79 |
| Ausgangszustand | Soll: beliebig. Ist: SG 11122 aus T29 im Parkhaus und bezahlt, Belegung "1 / 3" |
| Eingaben | leeres Feld |
| Erwartet | M01, Arm bleibt zu, Monitor Pico 2 `[EINGABE] Abgelehnt, ungueltiges Kennzeichen. Laenge: 0`, Kasse unverändert |
| Tatsächlich | Rote Meldung "Bitte ein Kennzeichen eingeben." (Adam). Arm bewegte sich nicht (Adam). Monitor Pico 2: `18:04:52.745 > [EINGABE] Abgelehnt, ungueltiges Kennzeichen. Laenge: 0`. Monitor Pico 1 ohne neue Zeile. Kasse-Seite unverändert mit SG 11122 und Belegung "1 / 3" |
| Messwerte | keine Messung |
| Status | bestanden |
| Fehler-ID, Klasse | keine |
| Nachweise | T06_Z1_2.png, Monitor_Pico2_Z1.log |

#### T07 Kennzeichen mit Sonderzeichen wird abgelehnt (Zyklus 1)

| Feld | Eintrag |
| --- | --- |
| Datum, Uhrzeit | 13.11.2026, 18:18:47 |
| Tester | Erster Versuch Adam Mehila in Chrome, die Eingabe erreichte Pico 2 nicht (keine Logzeile, angezeigt blieb die Meldung aus T06). Zweiter Versuch: Ausführung und Ablesung per HTTP-Abfrage (Kapitel 4, A16). Servo-Arm nicht beobachtet |
| Stand, Browser | Firmware 1.0, Test-Build, Commit 867aed7, Chrome 157.0.8103.79 |
| Ausgangszustand | Soll: beliebig. Ist: SG 11122 aus T29 im Parkhaus und bezahlt, `/api/vehicles` mit `"occupied":1` |
| Eingaben | `!@#$%^&*()` |
| Erwartet | M03, Arm bleibt zu, Monitor Pico 2 `[EINGABE] Abgelehnt, ungueltiges Kennzeichen. Laenge: 10`, Kasse unverändert |
| Tatsächlich | Antwort von `/api/entry` (gleiche Kodierung wie die Seite): `{"ok":false,"message":"Das ist kein gültiges Kennzeichen. Erwartet wird zum Beispiel LU 12345."}`. Monitor Pico 2: `18:18:47.988 > [EINGABE] Abgelehnt, ungueltiges Kennzeichen. Laenge: 10`. `/api/state` mit `"entryBarrierOpen":false`, keine Zeile `Schranke oeffnet`. Kasse unverändert, keine Zeile im Monitor Pico 1 |
| Messwerte | keine Messung |
| Status | bestanden |
| Fehler-ID, Klasse | keine |
| Nachweise | Monitor_Pico2_Z1.log, Notizen Zyklus 1 |

#### T08 Überlange Eingabe wird abgelehnt (Zyklus 1)

| Feld | Eintrag |
| --- | --- |
| Datum, Uhrzeit | 13.11.2026, 20:19:00 |
| Tester | Ausführung und Ablesung mit H-07 in Chrome (Kapitel 4, A16). Servo-Arm beobachtet von Adam Mehila |
| Stand, Browser | Firmware 1.0, Test-Build, Commit 867aed7, Chrome 157.0.8103.79 |
| Ausgangszustand | Soll: beliebig. Ist: Belegung "0 / 3", keine Fahrzeuge, kein Reset seit 17:45:54 |
| Eingaben | 30 mal A |
| Erwartet | M02, Arm bleibt zu, Monitor Pico 2 `[EINGABE] Abgelehnt, ungueltiges Kennzeichen. Laenge: 30`, kein neues Banner |
| Tatsächlich | Rote Meldung "Das Kennzeichen ist zu lang. Erlaubt sind höchstens 10 Zeichen, zum Beispiel LU 12345." Arm bewegt: nein (Adam), Anzeige Schrankenarm "geschlossen". Monitor Pico 2: `20:19:00.177 > [EINGABE] Abgelehnt, ungueltiges Kennzeichen. Laenge: 30`. Kein neues Banner, die Seite erhielt danach 21 Antworten auf `/api/state` in 20 s. Ein erster Klick um 20:17:05 kam nicht bei der Seite an (verdeckter Tab, Korrektur in H-07), ohne Anfrage an Pico 2 |
| Messwerte | keine Messung |
| Status | bestanden |
| Fehler-ID, Klasse | keine. Nebenbefund: 20:19:32 Belegung "0 / 0", siehe F-04 |
| Nachweise | T08_Z1_1.png, T08_Z1_2.png, Browser_Z1.log, Monitor_Pico2_Z1.log |

#### T27 Fehlversuch wird mit Datum und Uhrzeit festgehalten (Zyklus 1)

| Feld | Eintrag |
| --- | --- |
| Datum, Uhrzeit | 13.11.2026, 20:22:00 bis 20:23:00 |
| Tester | Ausführung und Ablesung mit H-07 in Chrome (Kapitel 4, A16). Servo-Arm beobachtet von Adam Mehila |
| Stand, Browser | Firmware 1.0, Test-Build, Commit 867aed7, Chrome 157.0.8103.79 |
| Ausgangszustand | Soll: Schranken-Seite seit dem Start von Pico 2 geöffnet. Ist: Pico 2 seit 17:53:58 gestartet, Schranken-Seite seither geöffnet |
| Eingaben | leeres Feld, 30 mal A, `!@#$%^&*()` |
| Erwartet | M01, M02, M03. Liste "Fehlversuche" mit Datum, Uhrzeit, Eingabe und Grund, drei Zeilen, neueste oben, Uhrzeit höchstens 1 Minute Abweichung |
| Tatsächlich | Meldungen: "Bitte ein Kennzeichen eingeben.", "Das Kennzeichen ist zu lang. Erlaubt sind höchstens 10 Zeichen, zum Beispiel LU 12345.", "Das ist kein gültiges Kennzeichen. Erwartet wird zum Beispiel LU 12345." Arm bewegt: dreimal nein (Adam). Monitor Pico 2 mit `Laenge: 0`, `Laenge: 30`, `Laenge: 10`. Die Schranken-Seite enthält keinen Bereich "Fehlversuche" und keine Liste mit Datum, Uhrzeit, Eingabe und Grund |
| Messwerte | Klicks um 20:22:00.111, 20:22:20.109, 20:22:40.122 |
| Status | nicht bestanden |
| Fehler-ID, Klasse | F-02, Klasse 3 |
| Nachweise | T27_Z1_1.png, Browser_Z1.log, Monitor_Pico2_Z1.log |

#### T15 Unbekanntes Kennzeichen an der Kasse (Zyklus 1)

| Feld | Eintrag |
| --- | --- |
| Datum, Uhrzeit | 13.11.2026, 20:24:08 |
| Tester | Ausführung und Ablesung mit H-07 in Chrome (Kapitel 4, A16). Servo-Arm nicht beteiligt |
| Stand, Browser | Firmware 1.0, Test-Build, Commit 867aed7, Chrome 157.0.8103.79 |
| Ausgangszustand | Soll: UR 99988 nicht im Parkhaus. Ist: Keine Fahrzeuge im Parkhaus (`/api/vehicles` um 20:24:07) |
| Eingaben | UR 99988 |
| Erwartet | M15 mit UR 99988, keine Angaben zu Einfahrt, Parkdauer und Gebühr, Bezahlen ausgegraut |
| Tatsächlich | Rote Meldung "Das Kennzeichen UR 99988 wurde im System nicht gefunden. Es ist keine Einfahrt bekannt." Suchresultat mit Einfahrt, Parkdauer und Gebühr ausgeblendet. Knopf "Bezahlen" gesperrt und grau. Ein Mausklick auf den gesperrten Knopf schickte keine Anfrage an `/api/pay` |
| Messwerte | keine Messung |
| Status | bestanden |
| Fehler-ID, Klasse | keine |
| Nachweise | T15_Z1_1.png, Browser_Z1.log |

#### T18 Bezahlung für unbekanntes Kennzeichen (Zyklus 1)

| Feld | Eintrag |
| --- | --- |
| Datum, Uhrzeit | 13.11.2026, 20:24:24 bis 20:24:30 |
| Tester | Ausführung und Ablesung mit H-07 in Chrome (Kapitel 4, A16). Servo-Arm nicht beteiligt |
| Stand, Browser | Firmware 1.0, Test-Build, Commit 867aed7, Chrome 157.0.8103.79 |
| Ausgangszustand | Soll: ZG 77766 nicht im Parkhaus. Ist: Keine Fahrzeuge im Parkhaus |
| Eingaben | ZG 77766, Aufruf /api/pay?plate=ZG%2077766 |
| Erwartet | M15 mit ZG 77766, Bezahlen ausgegraut, Antwort mit M20 und `"ok":false`, keine Zeile `[KASSE] Bezahlt` |
| Tatsächlich | Rote Meldung "Das Kennzeichen ZG 77766 wurde im System nicht gefunden. Es ist keine Einfahrt bekannt." "Bezahlen" gesperrt, ein Mausklick darauf schickte keine Anfrage. Aufruf `/api/pay?plate=ZG%2077766` in einem neuen Tab: `{"ok":false,"status":"unknown","alreadyPaid":false,"amount":"CHF 0.00","message":"Dieses Kennzeichen ist nicht im System. Eine Bezahlung ist nicht möglich."}`. Monitor Pico 1 ohne Zeile `[KASSE] Bezahlt` |
| Messwerte | keine Messung |
| Status | bestanden |
| Fehler-ID, Klasse | keine |
| Nachweise | T18_Z1_1.png, T18_Z1_2.png, Browser_Z1.log, Monitor_Pico1_Z1.log |

#### T25 Kennzeichen fährt ein, obwohl es schon im Parkhaus steht (Zyklus 1)

| Feld | Eintrag |
| --- | --- |
| Datum, Uhrzeit | 13.11.2026, 20:26:50 und 20:27:10, Wiederholung 20:28:30 |
| Tester | Ausführung und Ablesung mit H-07 in Chrome (Kapitel 4, A16). Servo-Arm beobachtet von Adam Mehila |
| Stand, Browser | Firmware 1.0, Test-Build, Commit 867aed7, Chrome 157.0.8103.79 |
| Ausgangszustand | Soll: LU 12345 im Parkhaus, zusätzlich SG 11122 aus T29. Ist: LU 12345 um 20:26:30 eingefahren (M04), Belegung "1 / 3". SG 11122 aus T29 war schon ausgefahren |
| Eingaben | LU 12345, lu12345 |
| Erwartet | zweimal M05, Arm bleibt zu, Belegung unverändert, eine Zeile LU 12345, keine neue Zeile `[KASSE] Einfahrt: LU 12345` |
| Tatsächlich | Beide Versuche: rote Meldung "Dieses Fahrzeug steht bereits im Parkhaus." Arm: Versuch "LU 12345" Adam "ja", Versuch "lu12345" Adam "nein". Monitor Pico 2 ohne Zeile `Schranke oeffnet` nach 20:26:35, Anzeige "geschlossen". Wiederholung "LU 12345" um 20:28:30: gleiche Meldung, Adam "nein", `/api/state` sechsmal `"entryBarrierOpen":false`. Belegung "1 / 3", genau eine Zeile LU 12345 mit Einfahrt 20:26:29, keine neue Zeile `[KASSE] Einfahrt: LU 12345` |
| Messwerte | keine Messung |
| Status | bestanden |
| Fehler-ID, Klasse | keine. Die Beobachtung "ja" beim ersten Versuch ist weder im Log noch im Zustand von Pico 2 zu sehen, die Wiederholung ergab "nein" |
| Nachweise | T25_Z1_1.png, T25_Z1_2.png, T25_Z1_3.png, Monitor_Pico1_Z1.log, Monitor_Pico2_Z1.log |

#### T29 Gebühr wird nach Ablauf des Zeitfensters nachberechnet (Zyklus 1, Fortsetzung)

| Feld | Eintrag |
| --- | --- |
| Datum, Uhrzeit | 13.11.2026, 18:18:04 bis 18:19:04 |
| Tester | Ausführung und Ablesung per HTTP-Abfrage (Kapitel 4, A16). Servo-Arm nicht beobachtet. Adam Mehila war abwesend |
| Stand, Browser | Firmware 1.0, Test-Build, Commit 867aed7, Chrome 157.0.8103.79 |
| Ausgangszustand | Soll: SG 11122 bezahlt um Z, kein Reset und kein Neustart von Pico 1 seither. Ist: SG 11122 bezahlt um Z 18:01:01, kein Reset und kein Neustart von Pico 1 seither |
| Eingaben | SG 11122 |
| Erwartet | Bei Z plus 14 Minuten: Parkdauer P plus 14 Minuten, Status "bezahlt", M17. Bei Z plus 16 Minuten: Status "offen", M16 CHF 2.00. Ausfahrt: M10 mit CHF 2.00. Nach Nachzahlung (M18 CHF 2.00): M11 |
| Tatsächlich | Schritt 4 (Z plus 14) nicht durchgeführt, der Zeitpunkt war verpasst. Schritt 5 um 18:18:04 (Z plus 17 min 02 s): `/api/status` mit `"status":"paid"`, `"duration":"1 h 08 min"`, `"fee":"CHF 2.00"`. Erwartet war Status offen mit CHF 2.00. Schritt 6 um 18:18:55: `{"ok":true,"message":"Bezahlung bestätigt. Die Ausfahrtsschranke öffnet für SG 11122."}`, Monitor Pico 2 `Schranke oeffnet` und nach 5,032 s `schliesst`. Schritt 7: Status "unknown", eine Nachzahlung war nicht möglich. Schritt 8: "Zu dem Kennzeichen SG 11122 ist keine Einfahrt bekannt. Die Schranke bleibt geschlossen." |
| Messwerte | Suche bei Z plus 17 min 02 s, Parkdauer "1 h 08 min" |
| Status | nicht bestanden |
| Fehler-ID, Klasse | F-03, Klasse 2 |
| Nachweise | Monitor_Pico1_Z1.log, Monitor_Pico2_Z1.log, Notizen Zyklus 1 |

#### T01 Gültiges Kennzeichen bei der Einfahrt eintippen (Zyklus 1)

| Feld | Eintrag |
| --- | --- |
| Datum, Uhrzeit | 13.11.2026, 20:31:00 |
| Tester | Ausführung und Ablesung mit H-07 in Chrome (Kapitel 4, A16). Servo-Arm beobachtet von Adam Mehila |
| Stand, Browser | Firmware 1.0, Test-Build, Commit 867aed7, Chrome 157.0.8103.79 |
| Ausgangszustand | Soll: leer nach Reset. Hergestellt: Reset um 20:29:40 (M22), Belegung "0 / 3" auf beiden Seiten |
| Eingaben | LU 12345 |
| Erwartet | M04, Arm offen in höchstens 3 s und zu nach 5 s ± 1 s, Monitorzeilen `[SCHRANKE] ...` und `[KASSE] Einfahrt: LU 12345  Belegung: 1/3`, Eintrag mit Datum und Uhrzeit, Belegung 1 / 3 |
| Tatsächlich | Grüne Meldung "LU 12345 ist registriert. Die Einfahrtsschranke öffnet und schliesst nach 5 Sekunden wieder." Arm bewegt: ja (Adam). Monitor Pico 2: `20:31:00.257 > [SCHRANKE] Schranke oeffnet (0 Grad).`, `20:31:05.269 > [SCHRANKE] Schranke schliesst (90 Grad).` Monitor Pico 1: `20:31:00.257 > [KASSE] Einfahrt: LU 12345  Belegung: 1/3`. Kasse-Seite: "LU 12345, 13.11.2026 20:30:59, 0 h 00 min, CHF 2.00, offen". Belegung "1 / 3" auf beiden Seiten |
| Messwerte | Befehl zum Öffnen 0,134 s nach dem Klick, Schliessen 5,012 s nach dem Öffnen (Zeitstempel im Monitor). Keine Stoppuhr |
| Status | bestanden |
| Fehler-ID, Klasse | keine |
| Nachweise | T01_Z1_1.png, T01_Z1_2.png, Browser_Z1.log, Monitor_Pico1_Z1.log, Monitor_Pico2_Z1.log |

#### T02 Einfahrtszeit stimmt mit der Uhrzeit des Laptops überein (Zyklus 1)

| Feld | Eintrag |
| --- | --- |
| Datum, Uhrzeit | 13.11.2026, 20:31:12 |
| Tester | Ausführung und Ablesung mit H-07 in Chrome (Kapitel 4, A16). Servo-Arm nicht beteiligt |
| Stand, Browser | Firmware 1.0, Test-Build, Commit 867aed7, Chrome 157.0.8103.79 |
| Ausgangszustand | Soll: nach T01. Ist: direkt nach T01 |
| Eingaben | LU 12345, Uhrzeit des Klicks aus T01 |
| Erwartet | gleiches Datum, Uhrzeit höchstens 1 Minute Abweichung |
| Tatsächlich | Angezeigte Einfahrt "13.11.2026 20:30:59" (Kasse-Seite und `/api/vehicles`). Klick in T01 um 13.11.2026 20:31:00.123 (Uhr des Laptops) |
| Messwerte | Abweichung 1 s, Datum gleich |
| Status | bestanden |
| Fehler-ID, Klasse | keine |
| Nachweise | T01_Z1_2.png, Browser_Z1.log |

#### T03 Gleiches Kennzeichen fährt nach abgeschlossenem Vorgang erneut ein (Zyklus 1)

| Feld | Eintrag |
| --- | --- |
| Datum, Uhrzeit | 13.11.2026, 20:32:50 |
| Tester | Ausführung und Ablesung mit H-07 in Chrome (Kapitel 4, A16). Servo-Arm beobachtet von Adam Mehila |
| Stand, Browser | Firmware 1.0, Test-Build, Commit 867aed7, Chrome 157.0.8103.79 |
| Ausgangszustand | Soll: LU 12345 bezahlt und ausgefahren, Belegung 0 / 3. Hergestellt: LU 12345 um 20:31:29 bezahlt (M18 CHF 2.00) und um 20:32:30 ausgefahren (M11), Belegung "0 / 3" |
| Eingaben | LU 12345 |
| Erwartet | M04, neue Zeile unter "Fahrzeuge im Parkhaus", alte Zeile unverändert unter "Abgeschlossene Vorgänge", Belegung 1 / 3 |
| Tatsächlich | Grüne Meldung M04 mit LU 12345. Arm bewegt: ja (Adam). Monitor Pico 1: `20:32:50.215 > [KASSE] Einfahrt: LU 12345  Belegung: 1/3`. "Fahrzeuge im Parkhaus": "LU 12345, 13.11.2026 20:32:49, 0 h 00 min, CHF 2.00, offen". "Abgeschlossene Vorgänge": "LU 12345, 13.11.2026 20:30:59, 13.11.2026 20:32:29, CHF 2.00, ausgefahren". Belegung "1 / 3" |
| Messwerte | keine Messung |
| Status | bestanden |
| Fehler-ID, Klasse | keine |
| Nachweise | T03_Z1_1.png, T03_Z1_2.png, Browser_Z1.log, Monitor_Pico1_Z1.log, Monitor_Pico2_Z1.log |

#### T04 Einfahrtsschranke öffnet innerhalb von 3 Sekunden (Zyklus 1)

| Feld | Eintrag |
| --- | --- |
| Datum, Uhrzeit | 14.11.2026, 21:24:47 |
| Tester | Ausführung und Ablesung mit H-07 in Chrome (Kapitel 4, A16). Servo-Arm beobachtet von Adam Mehila. Die Beobachtung war nicht verwertbar, siehe Tatsächlich |
| Stand, Browser | Firmware 1.0, Test-Build, Commit 867aed7, Chrome 157.0.8103.79 |
| Ausgangszustand | Soll: leer nach Reset. Hergestellt: Reset um 21:23:18 (M22), Belegung "0 / 3" |
| Eingaben | ZH 98765 |
| Erwartet | Arm senkrecht höchstens 3,0 s nach dem Klick, M04, `[SCHRANKE] Schranke oeffnet (0 Grad).` |
| Tatsächlich | Grüne Meldung "ZH 98765 ist registriert. Die Einfahrtsschranke öffnet und schliesst nach 5 Sekunden wieder." Monitor Pico 2: `21:24:47.660 > [SCHRANKE] Schranke oeffnet (0 Grad).` Der Piepton für den Beobachter kam 7,5 s vor dem Klick, die Antwort von Adam zum Arm ist deshalb nicht verwertbar. Keine Stoppuhrmessung |
| Messwerte | Befehl zum Öffnen 0,109 s nach dem Klick (21:24:47.551 bis 21:24:47.660) |
| Status | nicht durchgeführt |
| Fehler-ID, Klasse | keine. Grund: Beobachtung des Arms und Stoppuhr fehlen. Wiederholt in Zyklus 2 |
| Nachweise | T04_Z1_1.png, T04_Z1_2.png, Browser_Z1_Teil2.log, Monitor_Pico2_Z1_Teil2.log |

#### T05 Einfahrtsschranke schliesst nach 5 Sekunden (Zyklus 1)

| Feld | Eintrag |
| --- | --- |
| Datum, Uhrzeit | 14.11.2026, 21:24:47 bis 21:24:53 |
| Tester | Ausführung und Ablesung mit H-07 in Chrome (Kapitel 4, A16). Servo-Arm beobachtet von Adam Mehila. Die Beobachtung war nicht verwertbar |
| Stand, Browser | Firmware 1.0, Test-Build, Commit 867aed7, Chrome 157.0.8103.79 |
| Ausgangszustand | Soll: direkt nach T04, Arm offen. Ist: direkt nach T04 |
| Eingaben | keine |
| Erwartet | Differenz der Zeitstempel und Stoppuhr je 5,0 s ± 1,0 s, Arm danach waagrecht, Anzeige "geschlossen" |
| Tatsächlich | Monitor Pico 2: `21:24:47.660 > [SCHRANKE] Schranke oeffnet (0 Grad).`, `21:24:52.694 > [SCHRANKE] Schranke schliesst (90 Grad).` Keine Stoppuhr, keine verwertbare Beobachtung des Arms |
| Messwerte | Differenz der Zeitstempel 5,034 s |
| Status | nicht durchgeführt |
| Fehler-ID, Klasse | keine. Grund: Stoppuhr und Beobachtung des Arms fehlen. Wiederholt in Zyklus 2 |
| Nachweise | Monitor_Pico2_Z1_Teil2.log |

#### T09 Letzter freier Platz wird belegt (Zyklus 1)

| Feld | Eintrag |
| --- | --- |
| Datum, Uhrzeit | 14.11.2026, 21:37:20 |
| Tester | Ausführung und Ablesung mit H-07 in Chrome (Kapitel 4, A16). Servo-Arm beobachtet von Adam Mehila |
| Stand, Browser | Firmware 1.0, Test-Build, Commit 867aed7, Chrome 157.0.8103.79 |
| Ausgangszustand | Soll: LU 12345 und ZH 98765 im Parkhaus, Belegung 2 / 3. Hergestellt: Reset um 21:36:45, LU 12345 (21:36:51) und ZH 98765 (21:37:03) eingefahren, Belegung "2 / 3" |
| Eingaben | SG 11122 |
| Erwartet | M04, `[KASSE] Einfahrt: SG 11122  Belegung: 3/3`, Belegung 3 / 3 auf beiden Seiten |
| Tatsächlich | Grüne Meldung M04 mit SG 11122. Arm bewegt: ja (Adam). Monitor Pico 1: `21:37:20.426 > [KASSE] Einfahrt: SG 11122  Belegung: 3/3`. Belegung "3 / 3" auf beiden Seiten. Ein erster Durchgang um 21:25:52 ergab dieselben Meldungen, die Beobachtung des Arms war dort nicht verwertbar |
| Messwerte | keine Messung |
| Status | bestanden |
| Fehler-ID, Klasse | keine |
| Nachweise | T09_Z1_1.png, Browser_Z1_Teil2.log, Monitor_Pico1_Z1_Teil2.log |

#### T10 Einfahrt wird bei vollem Parkhaus verweigert (Zyklus 1)

| Feld | Eintrag |
| --- | --- |
| Datum, Uhrzeit | 14.11.2026, 21:26:12 und 21:37:40 |
| Tester | Ausführung und Ablesung mit H-07 in Chrome (Kapitel 4, A16). Servo-Arm beobachtet von Adam Mehila |
| Stand, Browser | Firmware 1.0, Test-Build, Commit 867aed7, Chrome 157.0.8103.79 |
| Ausgangszustand | Soll: direkt nach T09, Belegung 3 / 3. Ist: Belegung "3 / 3" mit LU 12345, ZH 98765 und SG 11122 |
| Eingaben | TG 44499 |
| Erwartet | M06, Arm bleibt zu, keine Zeile `[KASSE] Einfahrt: TG 44499`, TG 44499 nicht in der Liste |
| Tatsächlich | Erster Durchgang 21:26:12: Die Schranken-Seite zeigte keine Meldung und die Belegung "0 / 0". Pico 2 schrieb keine Zeile, Pico 1 trug TG 44499 nicht ein. Zweiter Durchgang 21:37:40: rote Meldung "Das Parkhaus ist voll. Keine weitere Einfahrt möglich.", Arm bewegt: nein (Adam), keine Zeile `[KASSE] Einfahrt: TG 44499`, TG 44499 nicht in der Liste, Belegung "3 / 3" |
| Messwerte | keine Messung |
| Status | nicht bestanden |
| Fehler-ID, Klasse | F-04, Klasse 2 |
| Nachweise | T10_Z1_1.png, T10_Z1_2.png, Browser_Z1_Teil2.log, Monitor-Logs Teil 2 |

#### T11 Platz wird nach einer Ausfahrt wieder frei (Zyklus 1)

| Feld | Eintrag |
| --- | --- |
| Datum, Uhrzeit | 14.11.2026, 21:37:53 bis 21:38:25 |
| Tester | Ausführung und Ablesung mit H-07 in Chrome (Kapitel 4, A16). Servo-Arm beobachtet von Adam Mehila |
| Stand, Browser | Firmware 1.0, Test-Build, Commit 867aed7, Chrome 157.0.8103.79 |
| Ausgangszustand | Soll: Belegung 3 / 3, LU 12345 bezahlt. Hergestellt: direkt nach T10, Belegung "3 / 3" |
| Eingaben | LU 12345 (Ausfahrt), TG 44499 (Einfahrt) |
| Erwartet | M11, Monitorzeilen mit `paid`, Belegung 2 / 3, danach M04 für TG 44499 und Belegung 3 / 3 |
| Tatsächlich | LU 12345 bezahlt um 21:37:53 (M18 CHF 2.00). Ausfahrt 21:38:05: "Bezahlung bestätigt. Die Ausfahrtsschranke öffnet für LU 12345.", Arm bewegt: ja (Adam). Monitor: `[KASSE] Statusabfrage von Pico 2: LU 12345 -> paid`, `[AUSFAHRT] Kasse meldet fuer LU 12345: paid`, `[KASSE] Ausfahrt: LU 12345  Belegung: 2/3`. Belegung "2 / 3". Einfahrt TG 44499 um 21:38:25: M04, Arm bewegt: ja (Adam), Belegung "3 / 3" |
| Messwerte | keine Messung |
| Status | bestanden |
| Fehler-ID, Klasse | keine |
| Nachweise | T11_Z1_1.png, T11_Z1_2.png, Browser_Z1_Teil2.log, Monitor-Logs Teil 2 |

#### T12 Gebühr nach 1 Minute Parkdauer (Zyklus 1)

| Feld | Eintrag |
| --- | --- |
| Datum, Uhrzeit | 14.11.2026, 21:39:56 bis 21:41:05 |
| Tester | Ausführung und Ablesung mit H-07 in Chrome (Kapitel 4, A16). Servo-Arm nicht beteiligt (Einfahrt als Vorbereitung) |
| Stand, Browser | Firmware 1.0, Test-Build, Commit 867aed7, Chrome 157.0.8103.79 |
| Ausgangszustand | Soll: leer nach Reset. Hergestellt: Reset um 21:39:50, ZH 98765 eingefahren um 21:39:56 |
| Eingaben | ZH 98765 |
| Erwartet | Suche zwischen 60 s und 119 s: Parkdauer "0 h 01 min", M16 "Offener Betrag: CHF 2.00" |
| Tatsächlich | Suche nach 69 s: Parkdauer "0 h 01 min", Gebühr "CHF 2.00", Status "offen", orange Meldung "Offener Betrag: CHF 2.00" |
| Messwerte | Suche 69 s nach der Einfahrt (Uhr des Laptops) |
| Status | bestanden |
| Fehler-ID, Klasse | keine |
| Nachweise | T12_Z1_1.png, Browser_Z1_Teil2.log |

#### T13 Gebühr genau am Übergang zur zweiten Stunde (Zyklus 1)

| Feld | Eintrag |
| --- | --- |
| Datum, Uhrzeit | 14.11.2026, 21:41:17 bis 21:41:48 |
| Tester | Ausführung und Ablesung mit H-07 in Chrome (Kapitel 4, A16). Servo-Arm nicht beteiligt |
| Stand, Browser | Firmware 1.0, Test-Build, Commit 867aed7, Chrome 157.0.8103.79 |
| Ausgangszustand | Soll: Reset, BE 55511 eingefahren. Hergestellt: Reset um 21:41:11, BE 55511 eingefahren um 21:41:17 |
| Eingaben | BE 55511, Verschiebung 59, 60 und 61 Minuten |
| Erwartet | 59 min: "0 h 59 min", CHF 2.00. Genau 60 min (Meldung der Testwerkzeuge): CHF 2.00. 61 min: "1 h 01 min", CHF 4.00 |
| Tatsächlich | 59 min: "Einfahrtszeit um 59 Minuten zurückgesetzt. Gebühr für genau 0 h 59 min: CHF 2.00.", Suche "0 h 59 min", "Offener Betrag: CHF 2.00". 60 min: "Gebühr für genau 1 h 00 min: CHF 2.00." 61 min: "Gebühr für genau 1 h 01 min: CHF 4.00.", Suche "1 h 01 min", "Offener Betrag: CHF 4.00". Monitor Pico 1: `Einfahrtszeit verschoben: BE 55511 -59`, `-60`, `-61 Minuten` |
| Messwerte | Suchen 6 s nach der jeweiligen Verschiebung |
| Status | bestanden |
| Fehler-ID, Klasse | keine |
| Nachweise | T13_Z1_1.png, T13_Z1_2.png, T13_Z1_3.png, Monitor_Pico1_Z1_Teil2.log |

#### T14 Gebühr für eine längere Parkdauer (Zyklus 1)

| Feld | Eintrag |
| --- | --- |
| Datum, Uhrzeit | 14.11.2026, 21:42:06 bis 21:42:12 |
| Tester | Ausführung und Ablesung mit H-07 in Chrome (Kapitel 4, A16). Servo-Arm nicht beteiligt |
| Stand, Browser | Firmware 1.0, Test-Build, Commit 867aed7, Chrome 157.0.8103.79 |
| Ausgangszustand | Soll: Reset, SG 11122 eingefahren. Hergestellt: Reset um 21:41:54, SG 11122 eingefahren um 21:42:00 |
| Eingaben | SG 11122, Verschiebung 195 Minuten |
| Erwartet | Meldung mit "Gebühr für genau 3 h 15 min: CHF 8.00", Suche: "3 h 15 min", M16 CHF 8.00 |
| Tatsächlich | "Einfahrtszeit um 195 Minuten zurückgesetzt. Gebühr für genau 3 h 15 min: CHF 8.00." Suche: Parkdauer "3 h 15 min", "Offener Betrag: CHF 8.00" |
| Messwerte | keine Messung |
| Status | bestanden |
| Fehler-ID, Klasse | keine |
| Nachweise | T14_Z1_1.png, Monitor_Pico1_Z1_Teil2.log |

#### T16 Bezahlung erfolgreich durchführen (Zyklus 1)

| Feld | Eintrag |
| --- | --- |
| Datum, Uhrzeit | 14.11.2026, 21:43:40 bis 21:43:41 |
| Tester | Ausführung und Ablesung mit H-07 in Chrome (Kapitel 4, A16). Servo-Arm nicht beteiligt |
| Stand, Browser | Firmware 1.0, Test-Build, Commit 867aed7, Chrome 157.0.8103.79 |
| Ausgangszustand | Soll: Reset, LU 12345 eingefahren, Parkdauer unter 60 Minuten. Hergestellt: Reset um 21:43:26, LU 12345 eingefahren um 21:43:32 |
| Eingaben | LU 12345 |
| Erwartet | M16 CHF 2.00, nach dem Klick M18 "Bezahlung erfolgt. Betrag: CHF 2.00.", Status "bezahlt", Bezahlen ausgegraut, `[KASSE] Bezahlt: LU 12345 CHF 2.00` |
| Tatsächlich | Vor dem Bezahlen: "0 h 00 min", "CHF 2.00", Status "offen", "Offener Betrag: CHF 2.00". Nach dem Klick: "Bezahlung erfolgt. Betrag: CHF 2.00.", Status "bezahlt", "Bezahlen" gesperrt. Monitor Pico 1: `21:43:41.062 > [KASSE] Bezahlt: LU 12345 CHF 2.00` |
| Messwerte | Klick auf "Bezahlen" um 21:43:41.008 |
| Status | bestanden |
| Fehler-ID, Klasse | keine |
| Nachweise | T16_Z1_1.png, Monitor_Pico1_Z1_Teil2.log |

#### T28 Zahlungszeitpunkt wird gespeichert und angezeigt (Zyklus 1)

| Feld | Eintrag |
| --- | --- |
| Datum, Uhrzeit | 14.11.2026, 21:43:41 |
| Tester | Ausführung und Ablesung mit H-07 in Chrome (Kapitel 4, A16). Servo-Arm nicht beteiligt |
| Stand, Browser | Firmware 1.0, Test-Build, Commit 867aed7, Chrome 157.0.8103.79 |
| Ausgangszustand | Soll: direkt nach T16. Ist: direkt nach T16 |
| Eingaben | LU 12345 |
| Erwartet | Feld "Bezahlt am" mit Datum und Uhrzeit, höchstens 1 Minute Abweichung zur Uhrzeit des Klicks in T16 |
| Tatsächlich | Das Suchresultat zeigt Kennzeichen, Einfahrt, Parkdauer, Gebühr und Status. Ein Feld "Bezahlt am" fehlt, `/api/status` enthält keinen Zahlungszeitpunkt |
| Messwerte | keine Messung |
| Status | nicht bestanden |
| Fehler-ID, Klasse | F-01, Klasse 3 |
| Nachweise | T16_Z1_1.png, Browser_Z1_Teil2.log |

#### T17 Doppelte Bezahlung wird verhindert (Zyklus 1)

| Feld | Eintrag |
| --- | --- |
| Datum, Uhrzeit | 14.11.2026, 21:43:41 bis 21:43:44 |
| Tester | Ausführung und Ablesung mit H-07 in Chrome (Kapitel 4, A16). Servo-Arm nicht beteiligt |
| Stand, Browser | Firmware 1.0, Test-Build, Commit 867aed7, Chrome 157.0.8103.79 |
| Ausgangszustand | Soll: LU 12345 bezahlt. Ist: direkt nach T28, LU 12345 bezahlt |
| Eingaben | LU 12345, Aufruf /api/pay?plate=LU%2012345 |
| Erwartet | M17, Bezahlen ausgegraut, Antwort mit M19 und `"alreadyPaid":true`, keine neue Zeile `[KASSE] Bezahlt`, Gebühr weiterhin CHF 2.00 |
| Tatsächlich | Grüne Meldung "Für dieses Kennzeichen wurde bereits bezahlt. Eine weitere Zahlung ist nicht nötig.", "Bezahlen" gesperrt, ein Mausklick darauf schickte keine Anfrage. Aufruf `/api/pay?plate=LU%2012345`: `{"ok":true,"status":"paid","alreadyPaid":true,"amount":"CHF 2.00","message":"Für dieses Kennzeichen wurde bereits bezahlt (CHF 2.00)."}`. Keine zweite Zeile `[KASSE] Bezahlt`. Gebühr weiterhin "CHF 2.00" |
| Messwerte | keine Messung |
| Status | bestanden |
| Fehler-ID, Klasse | keine |
| Nachweise | T17_Z1_1.png, Monitor_Pico1_Z1_Teil2.log |

#### T19 Ausfahrt nach erfolgreicher Bezahlung (Zyklus 1)

| Feld | Eintrag |
| --- | --- |
| Datum, Uhrzeit | 14.11.2026, 21:44:27 |
| Tester | Ausführung und Ablesung mit H-07 in Chrome (Kapitel 4, A16). Servo-Arm beobachtet von Adam Mehila |
| Stand, Browser | Firmware 1.0, Test-Build, Commit 867aed7, Chrome 157.0.8103.79 |
| Ausgangszustand | Soll: LU 12345 bezahlt und im Parkhaus. Ist: direkt nach T17, LU 12345 bezahlt |
| Eingaben | LU 12345 |
| Erwartet | `[KASSE] Statusabfrage von Pico 2: LU 12345 -> paid`, `[AUSFAHRT] Kasse meldet fuer LU 12345: paid`, M11, Arm offen in höchstens 3,0 s und zu nach 5 s ± 1 s, Eintrag unter "Abgeschlossene Vorgänge", Belegung 0 / 3 |
| Tatsächlich | "Bezahlung bestätigt. Die Ausfahrtsschranke öffnet für LU 12345." Arm bewegt: ja (Adam). Monitor Pico 1: `21:44:27.253 > [KASSE] Statusabfrage von Pico 2: LU 12345 -> paid`, `21:44:27.316 > [KASSE] Ausfahrt: LU 12345  Belegung: 0/3`. Monitor Pico 2: `21:44:27.346 > [AUSFAHRT] Kasse meldet fuer LU 12345: paid`, `Schranke oeffnet` 21:44:27.346, `schliesst` 21:44:32.352. LU 12345 unter "Abgeschlossene Vorgänge", Belegung "0 / 3" |
| Messwerte | Befehl zum Öffnen 0,149 s nach dem Klick, Schliessen nach 5,006 s (Zeitstempel). Keine Stoppuhr |
| Status | bestanden |
| Fehler-ID, Klasse | keine |
| Nachweise | T19_Z1_1.png, T19_Z1_2.png, Monitor-Logs Teil 2 |

#### T20 Ausfahrt ohne Bezahlung wird verweigert (Zyklus 1)

| Feld | Eintrag |
| --- | --- |
| Datum, Uhrzeit | 14.11.2026, 21:45:12 bis 21:45:42 |
| Tester | Ausführung und Ablesung mit H-07 in Chrome (Kapitel 4, A16). Servo-Arm beobachtet von Adam Mehila |
| Stand, Browser | Firmware 1.0, Test-Build, Commit 867aed7, Chrome 157.0.8103.79 |
| Ausgangszustand | Soll: ZH 98765 eingefahren, nicht bezahlt. Hergestellt: ZH 98765 eingefahren um 21:44:46, nicht bezahlt |
| Eingaben | ZH 98765 |
| Erwartet | M10 mit CHF 2.00, Arm bleibt zu, Monitorzeilen mit `unpaid`, nach Bezahlung (M18) M11 und Arm öffnet |
| Tatsächlich | Erste Ausfahrt: rote Meldung "Die Parkgebühr für ZH 98765 ist noch offen (CHF 2.00). Bitte zuerst an der Kasse bezahlen. Die Schranke bleibt geschlossen." Arm bewegt: nein (Adam). Monitor: `[KASSE] Statusabfrage von Pico 2: ZH 98765 -> unpaid`, `[AUSFAHRT] Kasse meldet fuer ZH 98765: unpaid`. Bezahlung: "Bezahlung erfolgt. Betrag: CHF 2.00." Zweite Ausfahrt: "Bezahlung bestätigt. Die Ausfahrtsschranke öffnet für ZH 98765.", Arm bewegt: ja (Adam) |
| Messwerte | keine Messung |
| Status | bestanden |
| Fehler-ID, Klasse | keine |
| Nachweise | T20_Z1_1.png, T20_Z1_2.png, Monitor-Logs Teil 2 |

#### T21 Grenzfall bezahlt, aber Kennzeichen bei der Ausfahrt anders (Zyklus 1)

| Feld | Eintrag |
| --- | --- |
| Datum, Uhrzeit | 14.11.2026, 21:46:30 und 21:46:50 |
| Tester | Ausführung und Ablesung mit H-07 in Chrome (Kapitel 4, A16). Servo-Arm beobachtet von Adam Mehila |
| Stand, Browser | Firmware 1.0, Test-Build, Commit 867aed7, Chrome 157.0.8103.79 |
| Ausgangszustand | Soll: BE 55511 eingefahren und bezahlt. Hergestellt: BE 55511 eingefahren um 21:46:01 und bezahlt um 21:46:12 |
| Eingaben | BE 55512, danach BE 55511 |
| Erwartet | M09 für BE 55512, Arm bleibt zu, Monitorzeilen mit `unknown`, danach M11 für BE 55511 und Arm öffnet |
| Tatsächlich | BE 55512: "Zu dem Kennzeichen BE 55512 ist keine Einfahrt bekannt. Die Schranke bleibt geschlossen.", Arm bewegt: nein (Adam), Monitor `BE 55512 -> unknown`. BE 55511: rote Meldung "Die Kasse konnte den Parkvorgang nicht abschliessen. Die Schranke bleibt geschlossen.", Arm bewegt: nein (Adam). Monitor Pico 1: `21:46:50.263 > [KASSE] Statusabfrage von Pico 2: BE 55511 -> paid`, `21:46:50.325 > [KASSE] Ausfahrt: BE 55511  Belegung: 0/3`. Die Kasse hat die Ausfahrt verbucht, die Schranke blieb zu |
| Messwerte | keine Messung |
| Status | nicht bestanden |
| Fehler-ID, Klasse | F-04, Klasse 2 |
| Nachweise | T21_Z1_1.png, T21_Z1_2.png, T21_Z1_3.png, Monitor-Logs Teil 2 |

#### T22 Kompletter Durchlauf von der Einfahrt bis zur Ausfahrt (Zyklus 1)

| Feld | Eintrag |
| --- | --- |
| Datum, Uhrzeit | 14.11.2026, 21:49:15 bis 21:49:52 |
| Tester | Ausführung und Ablesung mit H-07 in Chrome (Kapitel 4, A16). Servo-Arm beobachtet von Adam Mehila |
| Stand, Browser | Firmware 1.0, Test-Build, Commit 867aed7, Chrome 157.0.8103.79 |
| Ausgangszustand | Soll: leer nach Reset. Hergestellt: Reset um 21:49:04 |
| Eingaben | SG 11122, Verschiebung 90 Minuten |
| Erwartet | M04, Meldung mit "Gebühr für genau 1 h 30 min: CHF 4.00", Suche "1 h 30 min" und M16 CHF 4.00, M18 CHF 4.00, M11, Eintrag mit Bezahlt CHF 4.00, Belegung 0 / 3 |
| Tatsächlich | Einfahrt M04, Arm bewegt: ja (Adam). "Einfahrtszeit um 90 Minuten zurückgesetzt. Gebühr für genau 1 h 30 min: CHF 4.00." Suche "1 h 30 min", "Offener Betrag: CHF 4.00". "Bezahlung erfolgt. Betrag: CHF 4.00." Ausfahrt 21:49:50: "Die Kasse ist zurzeit nicht erreichbar. Der Bezahlstatus kann nicht geprüft werden, die Schranke bleibt geschlossen.", Arm bewegt: nein (Adam). Monitor Pico 1: `21:49:50.314 > [KASSE] Statusabfrage von Pico 2: SG 11122 -> paid`. Monitor Pico 2: `21:49:52.316 > [KASSE-CLIENT] Kasse nicht erreichbar, Fehler -11` |
| Messwerte | keine Messung |
| Status | nicht bestanden |
| Fehler-ID, Klasse | F-04, Klasse 2 |
| Nachweise | T22_Z1_1.png, T22_Z1_2.png, T22_Z1_3.png, Monitor-Logs Teil 2 |

#### T24 Kennzeichen bei Pico 2 eingegeben taucht bei Pico 1 auf (Zyklus 1)

| Feld | Eintrag |
| --- | --- |
| Datum, Uhrzeit | 14.11.2026, 21:50:05 |
| Tester | Ausführung per HTTP-Abfrage an `/api/entry` von Pico 2 (gleiche Anfrage wie die Schranken-Seite), Ablesung der Kasse-Seite mit H-07 alle 100 ms (Kapitel 4, A16) |
| Stand, Browser | Firmware 1.0, Test-Build, Commit 867aed7, Chrome 157.0.8103.79 |
| Ausgangszustand | Soll: leer nach Reset. Hergestellt: Reset um 21:49:59 |
| Eingaben | ZH 98765 |
| Erwartet | Eintrag unter "Fahrzeuge im Parkhaus" höchstens 3 s nach dem Wechsel, Einfahrt höchstens 1 Minute Abweichung, Status "offen", `[KASSE] Einfahrt: ZH 98765  Belegung: 1/3` |
| Tatsächlich | Die Kasse-Seite zeigte "ZH 98765, 14.11.2026 21:50:06, 0 h 00 min, CHF 2.00, offen" um 21:50:06.150. Monitor Pico 1: `21:50:05.962 > [KASSE] Einfahrt: ZH 98765  Belegung: 1/3`. Antwort von Pico 2: `{"ok":false,"message":""}`, kein `Schranke oeffnet` im Log |
| Messwerte | 0,3 s vom Aufruf bis zur Anzeige |
| Status | bestanden |
| Fehler-ID, Klasse | keine für die Anzeige. Nebenbefund: Pico 2 meldete eine leere Ablehnung und öffnete nicht, siehe F-04 |
| Nachweise | Browser_Z1_Teil2.log, Monitor_Pico1_Z1_Teil2.log |

#### T26 Kennzeichenformat an den Grenzen (Zyklus 1)

| Feld | Eintrag |
| --- | --- |
| Datum, Uhrzeit | 14.11.2026, 21:50:17 bis 21:51:04 |
| Tester | Ausführung und Ablesung mit H-07 in Chrome (Kapitel 4, A16). Servo-Arm nicht beteiligt |
| Stand, Browser | Firmware 1.0, Test-Build, Commit 867aed7, Chrome 157.0.8103.79 |
| Ausgangszustand | Soll: leer nach Reset. Hergestellt: Reset um 21:50:12 |
| Eingaben | LU 122, LU 123456, lu1, 12345, LU, L 12345, LU 12A45, LU 1234567, LU 12345678 |
| Erwartet | M04 für LU 122, LU 123456 und LU 1, M03 für die nächsten fünf Eingaben, M02 für LU 12345678, Belegung 3 / 3 |
| Tatsächlich | "LU 122", "LU 123456" und "lu1" (als "LU 1"): M04. "12345", "LU", "L 12345", "LU 12A45", "LU 1234567": "Das ist kein gültiges Kennzeichen. Erwartet wird zum Beispiel LU 12345." "LU 12345678": "Das Kennzeichen ist zu lang. Erlaubt sind höchstens 10 Zeichen, zum Beispiel LU 12345." Monitor Pico 2: Laenge 5, 2, 7, 8, 10, 11. Liste LU 122, LU 123456, LU 1, Belegung "3 / 3" |
| Messwerte | keine Messung |
| Status | bestanden |
| Fehler-ID, Klasse | keine. Nebenbefund: um 21:50:58 Belegung "0 / 0", siehe F-04 |
| Nachweise | T26_Z1_1.png, T26_Z1_2.png, Monitor-Logs Teil 2 |

#### T23 Verbindung zur Kasse fällt kurz aus (Zyklus 1)

| Feld | Eintrag |
| --- | --- |
| Datum, Uhrzeit | 14.11.2026, 21:52:19 bis 21:53:10 |
| Tester | Ausführung und Ablesung mit H-07 in Chrome (Kapitel 4, A16). Servo-Arm beobachtet von Adam Mehila |
| Stand, Browser | Firmware 1.0, Test-Build, Commit 867aed7, Chrome 157.0.8103.79 |
| Ausgangszustand | Soll: Reset, TG 44499 eingefahren und bezahlt. Hergestellt: Reset um 21:51:54, TG 44499 eingefahren um 21:52:00 und bezahlt um 21:52:11 |
| Eingaben | TG 44499, Aufruf /api/debug/offline?seconds=30 |
| Erwartet | Antwort der Testhilfe, M08 höchstens 5 s nach dem Klick, Arm bleibt zu, `[AUSFAHRT] Kasse nicht erreichbar, Schranke bleibt zu.`, kein neues Banner, nach Ablauf M11 und Arm öffnet |
| Tatsächlich | Antwort der Testhilfe: `{"ok":true,"seconds":30,"message":"Die Kasse beantwortet 30 Sekunden lang keine Anfragen."}`, Monitor Pico 1 `21:52:19.173 > [DEBUG] Kasse antwortet 30 s lang nicht.` Ausfahrt 21:52:28: "Die Kasse ist zurzeit nicht erreichbar. Der Bezahlstatus kann nicht geprüft werden, die Schranke bleibt geschlossen.", Arm bewegt: nein (Adam), Anzeige "Kasse nicht erreichbar", Monitor Pico 2 `21:52:30.304 > [AUSFAHRT] Kasse nicht erreichbar, Schranke bleibt zu.` Kein neues Banner. `21:52:49.220 > [DEBUG] Kasse antwortet wieder.` Zweite Ausfahrt 21:53:10: "Bezahlung bestätigt. Die Ausfahrtsschranke öffnet für TG 44499.", Arm bewegt: ja (Adam) |
| Messwerte | Meldung 2,1 s nach dem Klick (Zeitstempel) |
| Status | bestanden |
| Fehler-ID, Klasse | keine |
| Nachweise | T23_Z1_1.png, T23_Z1_2.png, T23_Z1_3.png, Monitor-Logs Teil 2 |

## 6 Gefundene Abweichungen und Fehlerklassifizierung

### 6.1 Fehlerklassen

Die Klassen stammen aus Testkonzept 1.0, Kapitel 6. Zu jedem Fehler wird der Ort festgehalten: Kasse (Pico 1), Schranken (Pico 2) oder Kommunikation (zwischen beiden).

| Klasse | Bezeichnung | Bedeutung | Behandlung und Auswirkung auf die Freigabe |
| --- | --- | --- | --- |
| 1 | Kritisch | Das System oder eine Kernfunktion ist nicht nutzbar | sofort, der Testdurchlauf wird gestoppt. Verhindert die Freigabe |
| 2 | Schwer | Eine wichtige Funktion arbeitet nicht wie verlangt, es gibt einen manuellen Umweg | vor der Abgabe beheben und den Testfall wiederholen. Verhindert die Freigabe, solange offen |
| 3 | Mittel | Eine Funktion verhält sich nicht wie verlangt, der Betrieb bleibt möglich | beheben, wenn Zeit bleibt. Freigabe möglich, Auflage im Fazit |
| 4 | Leicht | Kosmetisches Problem ohne Auswirkung auf die Funktion | bei Gelegenheit beheben. Freigabe möglich |

### 6.2 Fehlertabelle

Die Klassen sind ein Vorschlag nach Testkonzept 1.0, Kapitel 6, mit Begründung in Kapitel 6.3. Die Testleitung bestätigt sie mit der Freigabe des Protokolls.

| Fehler-ID | Testfall | Ort | Klasse | Kurzbeschreibung | Status | Behoben in |
| --- | --- | --- | --- | --- | --- | --- |
| F-01 | T28 | Kasse | 3 | Der Zahlungszeitpunkt wird nicht gespeichert und nicht angezeigt | behoben, Retest siehe Kapitel 7.5 | Firmware 1.1, Commit 6c3877d |
| F-02 | T27 | Schranken | 3 | Fehlversuche bei der Einfahrt werden nicht mit Datum und Uhrzeit festgehalten | behoben, Retest siehe Kapitel 7.5 | Firmware 1.2, Commit 5c01ce8 |
| F-03 | T29 | Kasse | 2 | Nach der Bezahlung wird nie nachberechnet, auch nicht nach Ablauf des Zeitfensters | behoben, Retest siehe Kapitel 7.5 | Firmware 1.3, Commit b812ed9 |
| F-04 | T10, T21, T22, T24 | Kommunikation | 2 | Pico 2 wertet fehlende oder leere Antworten der Kasse falsch aus. Die Kasse verbucht Einfahrt oder Ausfahrt, die Schranke bleibt zu | behoben, Retest siehe Kapitel 7.5 | Firmware 1.4, Commit bad02f7 |

### 6.3 Beschreibung der Fehler

Jeder Fehler wird so beschrieben, dass eine andere Person ihn nachvollziehen kann: Schritte, Eingaben mit Kennzeichen, Uhrzeit, Browser mit Version, erwartetes und tatsächliches Ergebnis, Begründung der Klasse. Stand bei allen Fehlern: Firmware 1.0, Test-Build, Commit 867aed7, Chrome 157.0.8103.79.

#### F-01 Zahlungszeitpunkt fehlt (T28)

- Schritte: Reset, LU 12345 einfahren, auf der Kasse-Seite LU 12345 suchen und "Bezahlen" klicken, danach LU 12345 erneut suchen. Durchgeführt am 14.11.2026, Klick auf "Bezahlen" um 21:43:41.
- Erwartet: Das Suchresultat zeigt das Feld "Bezahlt am" mit Datum und Uhrzeit der Zahlung (User Story 6, Akzeptanzkriterium 2).
- Tatsächlich: Das Suchresultat zeigt nur Kennzeichen, Einfahrt, Parkdauer, Gebühr und Status. `/api/status` liefert keinen Zahlungszeitpunkt. Die Kasse speichert ihn nicht.
- Klasse 3: Die Bezahlung selbst funktioniert, der Betrieb bleibt möglich. Es fehlt eine verlangte Angabe für die Nachvollziehbarkeit.

#### F-02 Fehlversuche werden nicht festgehalten (T27)

- Schritte: Auf der Schranken-Seite im Feld "Einfahrt" nacheinander nichts, 30 mal A und `!@#$%^&*()` eingeben und je "Einfahrt bestätigen" klicken. Durchgeführt am 13.11.2026 um 20:22:00, 20:22:20 und 20:22:40.
- Erwartet: Die Schranken-Seite zeigt eine Liste "Fehlversuche" mit Datum, Uhrzeit, Eingabe und Grund (User Story 3, Akzeptanzkriterium 3).
- Tatsächlich: Die Meldungen M01, M02 und M03 erscheinen, der Arm bleibt unten. Eine Liste gibt es nicht, und Pico 2 hat keine Uhr.
- Klasse 3: Die Ablehnung ungültiger Eingaben funktioniert. Es fehlt das verlangte Protokoll der Fehlversuche.

#### F-03 Keine Nachberechnung nach Ablauf des Zeitfensters (T29)

- Schritte: Reset, SG 11122 einfahren, Einfahrtszeit um 50 Minuten zurücksetzen, bezahlen (Z), bei Z plus 16 Minuten suchen, dann ausfahren. Durchgeführt am 13.11.2026, Z um 18:01:01, Suche um 18:18:04.
- Erwartet: Nach 15 Minuten ohne Ausfahrt rechnet die Kasse neu. Bei Z plus 16 Minuten Status "offen" und "Offener Betrag: CHF 2.00", Ausfahrt mit M10 (User Story 7, Akzeptanzkriterium 3, Testkonzept 1.1, Kapitel 5.3).
- Tatsächlich: Bei Z plus 17 min 02 s Status "paid", Parkdauer "1 h 08 min", Gebühr "CHF 2.00". Die Ausfahrt öffnete die Schranke ohne Nachzahlung.
- Klasse 2: Eine wichtige Funktion arbeitet nicht wie verlangt. Ein bezahltes Fahrzeug kann unbegrenzt stehen bleiben, die Gebühr für die weitere Zeit geht verloren. Umweg: Kontrolle durch Personal.

#### F-04 Antwort der Kasse fehlt oder ist leer (T10, T21, T22, T24)

- Schritte und Beobachtungen am 14.11.2026:
  - T10 um 21:26:12: Einfahrt TG 44499 bei Belegung 3 / 3. Keine Meldung, Belegung "0 / 0", keine Logzeile.
  - T21 um 21:46:50: Ausfahrt BE 55511 nach Bezahlung. Die Kasse meldet `paid` und verbucht die Ausfahrt (`[KASSE] Ausfahrt: BE 55511  Belegung: 0/3`). Pico 2 zeigt "Die Kasse konnte den Parkvorgang nicht abschliessen. Die Schranke bleibt geschlossen."
  - T22 um 21:49:50: Ausfahrt SG 11122 nach Bezahlung. Die Kasse meldet `paid` (21:49:50.314), Pico 2 erhält keine Antwort (`Fehler -11` um 21:49:52.316) und zeigt M08.
  - T24 um 21:50:05: Die Kasse verbucht die Einfahrt ZH 98765, Pico 2 antwortet `{"ok":false,"message":""}` und öffnet nicht.
  - Dazu am 13.11.2026 um 20:19:32 und am 14.11.2026 um 21:14:09 und 21:50:58 die Belegung "0 / 0" auf der Schranken-Seite.
- Erwartet: Pico 2 öffnet genau dann, wenn die Kasse den Vorgang verbucht hat. Die Belegung zeigt nie "0 / 0".
- Tatsächlich: Pico 2 wertet eine leere Antwort mit HTTP-Status 200 als gültig (Platzzahl 0, leere Meldung) und bricht nach 2 s ohne Abgleich ab, auch wenn die Kasse den Vorgang schon verbucht hat. Die Kasse und die Schranke sind danach nicht mehr im gleichen Zustand.
- Klasse 2: Kernfunktionen Einfahrt und Ausfahrt arbeiten zeitweise nicht wie verlangt, ein bezahltes Fahrzeug kommt nicht hinaus und gilt an der Kasse als ausgefahren. Umweg: Personal öffnet die Schranke. Klasse 1 trifft nicht zu, weil das System in den meisten Durchgängen arbeitet.

## 7 Erweiterungen und Anpassungen

### 7.1 Testhilfen

Die Testhilfen ändern keine Funktion des Parksystems. Sie machen Abläufe beobachtbar oder Testfälle durchführbar. H-01 bis H-05 sind in der Firmware und vor Zyklus 1 umgesetzt. H-06 und H-07 sind Werkzeuge auf dem Testlaptop im Ordner tools. Sie sind am 13.11.2026 während Zyklus 1 entstanden und ändern die Firmware nicht. Darum gibt es für sie keinen Build-Nachweis.

| ID | Änderung | Begründung | Commit | Build-Nachweis |
| --- | --- | --- | --- | --- |
| H-01 | Monitorzeilen zur Statusabfrage auf Pico 1 und Pico 2 | Die Abfrage von Pico 2 an Pico 1 war bei bezahlten Fahrzeugen nicht beobachtbar (Bemerkung 9 aus der Bewertung) | d73ec89 | alle Umgebungen SUCCESS, Anhang B |
| H-02 | Firmware-Version im Banner beider Picos, eigene Datei version.h | Der getestete Stand muss im Gerät erkennbar sein. Eigene Datei, damit die lokalen WLAN-Daten nie in einen Commit gelangen | 7334c5f | alle Umgebungen SUCCESS, Anhang B |
| H-03 | Testwerkzeug zur Zeitverschiebung nennt die Gebühr für genau N Minuten | Nach der Verschiebung läuft die Uhr weiter, die Grenze bei genau 60 Minuten wäre sonst nicht prüfbar (T13) | a9377ce | alle Umgebungen SUCCESS, Anhang B |
| H-04 | Kasse für N Sekunden stumm schalten, nur im Test-Build | Ausfall der Kasse simulieren, ohne den Laptop vom WLAN zu trennen und ohne Datenverlust (T23) | f708382 | alle Umgebungen SUCCESS, Anhang B |
| H-05 | Zwei Build-Umgebungen für den Produktivbuild (TEST_BUILD=0) | Abnahmetest T30 mit demselben Commit wie Zyklus 2 | 867aed7 | alle sechs Umgebungen SUCCESS, Anhang B |
| H-06 | Serieller Monitor tools/serial_monitor.py: Uhrzeit des Laptops mit Millisekunden vor jeder Zeile wie beim Filter `time`, eine Logdatei je Pico, verbindet nach einem Neustart des Picos selbst neu, vermerkt jede Verbindung und Trennung mit `---` | `pio device monitor` lässt sich nicht im Hintergrund starten und hat sich nach dem Neustart der Picos nicht wieder verbunden. Banner und `[KASSE] Uhr gestellt` fehlten deshalb im Log (Kapitel 4, A15) | 40cedb5, 3bba959 | keiner, Firmware unverändert |
| H-07 | Steuerung von Chrome tools/browser_test.mjs: Eingabe als Tastatureingabe, Klick als Mausereignis auf den Knopf, Ablesen von Meldung, Belegung, Listen und Suchresultat nach der Antwort des Picos, Screenshot der ganzen Seite, Log jeder Aktion mit Uhrzeit | Neue Arbeitsweise nach Entscheid der Testleitung: Claude Code führt die Schritte im Browser aus und liest ab, Adam Mehila beobachtet den Servo-Arm. Das Log macht nachvollziehbar, was wann eingegeben und abgelesen wurde (Kapitel 4, A16) | a99ec18, 84f99c7, 3228aa1 | keiner, Firmware unverändert |

### 7.2 Korrekturen und Ergänzungen am Testkonzept

Beim Abgleich des Testkonzepts mit der Umsetzung und mit der Bewertung zeigten sich unklare und fehlende Testfälle. Sie sind in Testkonzept 1.1 korrigiert und ergänzt (Anhang A):

- T01 bis T24: Meldungen im Wortlaut, Ort der Beobachtung, Vorbedingungen mit Verfahren, Soll-Werte an die Hardware angepasst.
- T25 Kennzeichen fährt ein, obwohl es schon im Parkhaus steht: fehlte, Bemerkung 3 aus der Bewertung.
- T26 Kennzeichenformat an den Grenzen: fehlte, Bemerkung 4 aus der Bewertung.
- T27, T28 und T29: Akzeptanzkriterien 3.3, 6.2 und 7.3 hatten keinen Testfall.
- T30 Abnahmetest des Produktivbuilds: Die Freigabe soll für den Stand gelten, der ausgeliefert wird.

### 7.3 Fehlerbehebungen

Alle vier Fehler sind direkt nach Zyklus 1 behoben, je ein Commit mit eigener Firmware-Version. Die Reihenfolge F-01 vor F-03 ist zwingend, weil die Nachberechnung den Zahlungszeitpunkt braucht.

| Fehler-ID | Änderung | Commit | Firmware | Betroffene Funktionen | Build-Nachweis |
| --- | --- | --- | --- | --- | --- |
| F-01 | Jeder Parkvorgang speichert den Zahlungszeitpunkt. `/api/status` und `/api/pay` liefern `paidAt`, die Kasse-Seite zeigt "Bezahlt am" | 6c3877d | Firmware 1.1 | Bezahlung, Statusabfrage, Suchresultat, Zeitverschiebung, Reset (T03, T11, T13 bis T22, T28) | 6 von 6 SUCCESS, Anhang B |
| F-02 | Pico 2 speichert die letzten zehn abgelehnten Eingaben bei der Einfahrt. Die Schranken-Seite stellt die Uhr von Pico 2 und zeigt die Liste "Fehlversuche" | 5c01ce8 | Firmware 1.2 | Kennzeichenprüfung bei Ein- und Ausfahrt, Zustand und Seite von Pico 2 (T01, T04 bis T10, T19 bis T21, T24 bis T27) | 6 von 6 SUCCESS, Anhang B |
| F-03 | Zeitfenster 15 Minuten nach der Zahlung. Danach Neuberechnung ab Einfahrt, offen ist die Differenz zum bezahlten Betrag | b812ed9 | Firmware 1.3 | Bezahlung, Statusabfrage, Ausfahrt, Fahrzeugliste (T11, T13 bis T22, T28, T29) | 6 von 6 SUCCESS, Anhang B |
| F-04 | Nur vollständige JSON-Antworten mit Status 200 gelten. Zeitlimit 3 s. WLAN-Energiesparmodus aus. Fehlt die Antwort auf Einfahrt oder Ausfahrt, gleicht Pico 2 mit `/api/status` ab. Abfrage der Belegung mit einer Wiederholung | bad02f7 | Firmware 1.4 | Alle Anfragen von Pico 2 an die Kasse: Einfahrt, Ausfahrt, Belegung, Statusabfrage, WLAN beider Picos (T01, T03, T09 bis T11, T19 bis T26, T29) | 6 von 6 SUCCESS, Anhang B |

### 7.4 Begründung von Zyklus 2

Zyklus 2 ist ein vollständiger Durchlauf von T01 bis T29 auf dem Endstand (Firmware 1.4, Commit bad02f7), danach folgt T30. Die betroffenen Funktionen je Fehler stehen in Kapitel 7.3. Zusammen decken sie fast alle Testfälle ab, F-04 betrifft jede Anfrage von Pico 2 an die Kasse. Dazu kommen die Fälle mit Priorität Hoch. Ein vollständiger Durchlauf testet deshalb genau den Stand, der ausgeliefert wird. Retest der Fehler: F-01 mit T28, F-02 mit T27, F-03 mit T29, F-04 mit T10, T21, T22 und T24.

### 7.5 Zyklus 2

Die Ergebnisse von Zyklus 1 bleiben unverändert stehen. Zyklus 2 wird hier als eigener Eintrag mit Datum, Uhrzeit, Commit und Firmware-Version erfasst. Die Einzelprotokolle von Zyklus 2 werden vor dem Zyklus angelegt.

| Fehler-ID | Testfall | Status Zyklus 1 | Status Zyklus 2 | Datum, Uhrzeit | Commit |
| --- | --- | --- | --- | --- | --- |
| ‹OFFEN› | ‹OFFEN› | ‹OFFEN› | ‹OFFEN› | ‹OFFEN› | ‹OFFEN› |

## 8 Abdeckung der Akzeptanzkriterien und Testziele

### 8.1 Akzeptanzkriterien

| AK | Inhalt kurz | Testfälle | Zyklus 1 | Zyklus 2 |
| --- | --- | --- | --- | --- |
| 1.1 | Kennzeichen wird bei der Einfahrt erfasst | T01, T24 | ‹OFFEN› | ‹OFFEN› |
| 1.2 | Gespeichert mit Datum und Uhrzeit | T01, T02, T24 | ‹OFFEN› | ‹OFFEN› |
| 1.3 | Erneute Einfahrt ist ein neuer Eintrag | T03 | ‹OFFEN› | ‹OFFEN› |
| 2.1 | Schranke öffnet bei gültigem Kennzeichen | T01, T04, T09 | ‹OFFEN› | ‹OFFEN› |
| 2.2 | Schranke schliesst von selbst | T05 | ‹OFFEN› | ‹OFFEN› |
| 2.3 | Erkennung dauert zu lange | keiner | nicht anwendbar | nicht anwendbar |
| 3.1 | Ungültiges Kennzeichen: Schranke bleibt zu | T06, T07, T08, T26 | ‹OFFEN› | ‹OFFEN› |
| 3.2 | Information zum weiteren Vorgehen | T06, T07, T08, T26 | ‹OFFEN› | ‹OFFEN› |
| 3.3 | Fehlversuch mit Zeitstempel protokolliert | T27 | ‹OFFEN› | ‹OFFEN› |
| 4.1 | Volles Parkhaus: Schranke bleibt zu | T09, T10 | ‹OFFEN› | ‹OFFEN› |
| 4.2 | Platz nach Ausfahrt wieder frei | T11 | ‹OFFEN› | ‹OFFEN› |
| 4.3 | Information "voll" | T10 | ‹OFFEN› | ‹OFFEN› |
| 5.1 | Parkdauer ab Einfahrt | T12, T13, T14 | ‹OFFEN› | ‹OFFEN› |
| 5.2 | Gebühr nach Tarif | T12, T13, T14, T22 | ‹OFFEN› | ‹OFFEN› |
| 5.3 | Unbekanntes Kennzeichen: Fehlermeldung statt Gebühr | T15, T18 | ‹OFFEN› | ‹OFFEN› |
| 6.1 | Offener Betrag wird angezeigt | T16 | ‹OFFEN› | ‹OFFEN› |
| 6.2 | Als bezahlt markiert, mit Zeitpunkt | T16, T28 | ‹OFFEN› | ‹OFFEN› |
| 6.3 | Zahlung scheitert: bleibt unbezahlt, Information | T18 | ‹OFFEN› | ‹OFFEN› |
| 7.1 | Bezahlt: Ausfahrtsschranke öffnet | T11, T19, T21, T22 | ‹OFFEN› | ‹OFFEN› |
| 7.2 | Vorgang abgeschlossen, Platz frei | T19, T22 | ‹OFFEN› | ‹OFFEN› |
| 7.3 | Nachberechnung nach Ablauf einer Frist | T29 | ‹OFFEN› | ‹OFFEN› |
| 8.1 | Ohne Zahlung bleibt die Schranke zu | T20 | ‹OFFEN› | ‹OFFEN› |
| 8.2 | Meldung mit Ort der Bezahlung | T20 | ‹OFFEN› | ‹OFFEN› |
| 8.3 | Nach dem Bezahlen direkt weiterfahren | T20 | ‹OFFEN› | ‹OFFEN› |

### 8.2 Testziele

| Testziel (Testkonzept 1.0, Kapitel 1.1) | Bewertung | Begründung |
| --- | --- | --- |
| Z1 Abdeckung der Anforderungen | ‹OFFEN› | Akzeptanzkriterium 2.3 hat keinen Testfall, weil es nicht anwendbar ist. Weitere Punkte ‹OFFEN› |
| Z2 Keine Gratisausfahrt | ‹OFFEN› | ‹OFFEN› |
| Z3 Kein blockierter Gast | ‹OFFEN› | ‹OFFEN› |
| Z4 Gebühr stimmt an allen Stundenübergängen mit dem Tarif überein | ‹OFFEN› | ‹OFFEN› |
| Z5 Robustes Verhalten bei Fehlern | ‹OFFEN› | ‹OFFEN› |

## 9 Fazit und Genehmigung

### 9.1 Austrittskriterien

| Austrittskriterium (Testkonzept 1.0, Kapitel 3.6) | Erfüllt | Begründung |
| --- | --- | --- |
| Alle Testfälle wurden mindestens einmal durchgeführt | ‹OFFEN› | ‹OFFEN› |
| Kein Fehler der Klasse 1 oder Klasse 2 ist mehr offen | ‹OFFEN› | ‹OFFEN› |
| Alle Testfälle mit Priorität Hoch sind bestanden | ‹OFFEN› | ‹OFFEN› |
| Die Ergebnisse aller Testfälle sind mit Datum notiert | ‹OFFEN› | ‹OFFEN› |

### 9.2 Restrisiken und bekannte Einschränkungen

Diese Einschränkungen sind aus Umsetzung und Dokumentation bekannt. Sie hängen nicht vom Testergebnis ab:

- Die Kasse speichert alle Parkvorgänge nur im Arbeitsspeicher. Nach einem Stromausfall oder Neustart von Pico 1 sind alle Fahrzeuge unbekannt und können nicht mehr regulär ausfahren.
- Einfahrt und Ausfahrt teilen sich einen Arm. Während einer Einfahrt ist der Arm auch für die Ausfahrt offen.
- Die Bezahlung ist simuliert. Es werden keine echten Zahlungen verarbeitet.
- Die Kommunikation ist unverschlüsselt (HTTP) und ohne Anmeldung. Das System ist für ein geschlossenes Netz gedacht.
- Die Kasse speichert 24 Parkvorgänge. Danach wird der älteste abgeschlossene Vorgang überschrieben.
- Antwortet die Kasse nicht, wartet Pico 2 bis zu 2 s und reagiert in dieser Zeit nicht.

Weitere Restrisiken aus offenen Fehlern: ‹OFFEN›

### 9.3 Empfehlung der Testleitung

‹OFFEN›

### 9.4 Genehmigung

Die Testleitung empfiehlt, der Auftraggeber entscheidet über die Inbetriebnahme.

| Rolle | Name | Entscheid | Datum | Unterschrift |
| --- | --- | --- | --- | --- |
| Testleitung | Adam Mehila | Empfehlung: ‹OFFEN› | ‹OFFEN› | |
| Auftraggeber | Parkhausbesitzer (fiktiv), vertreten durch die Lehrperson M450 (ELA) | | | |

## 10 Dokument-Protokoll

### 10.1 Änderungskontrolle

| Version | Name | Datum | Bemerkungen |
| --- | --- | --- | --- |
| Protokoll 0.1 | Adam Mehila | 12.11.2026 | Gerüst mit Testumgebung, Abweichungen, Testhilfen und leeren Einzelprotokollen |

### 10.2 Prüfung

| Version | Name | Datum | Bemerkungen |
| --- | --- | --- | --- |
| ‹OFFEN› | ‹OFFEN› | ‹OFFEN› | ‹OFFEN› |

### 10.3 Freigabe

| Version | Name | Datum | Bemerkungen |
| --- | --- | --- | --- |
| Protokoll 1.0 | ‹OFFEN› | ‹OFFEN› | ‹OFFEN› |

## Anhänge

### Anhang A Testkonzept 1.1

Testkonzept Parksystem mit Schranken, Nachtrag, Testkonzept 1.1 vom 12.11.2026. Der vollständige Text folgt am Ende dieses Dokuments.

### Anhang B Build-Nachweise

Jeder Build-Nachweis ist nach dem Commit entstanden. Befehl: `pio run` mit allen genannten Umgebungen, PlatformIO Core 6.2.0. Die Kompilierläufe zeigten Warnungen, alle aus der Datei WiFiClient.h:71 des Frameworks (`-Woverloaded-virtual`), keine aus dem Code des Projekts.

| ID | Commit | Zeitpunkt des Builds | Umgebungen | Ergebnis |
| --- | --- | --- | --- | --- |
| H-01 | d73ec89 | 12.11.2026 18:30:39 | pico1_cashdesk, pico2_barriers, servo_test, servo_test_all_pins | 4 von 4 SUCCESS |
| H-02 | 7334c5f | 12.11.2026 18:33:55 | pico1_cashdesk, pico2_barriers, servo_test, servo_test_all_pins | 4 von 4 SUCCESS |
| H-03 | a9377ce | 12.11.2026 18:35:48 | pico1_cashdesk, pico2_barriers, servo_test, servo_test_all_pins | 4 von 4 SUCCESS |
| H-04 | f708382 | 12.11.2026 18:38:08 | pico1_cashdesk, pico2_barriers, servo_test, servo_test_all_pins | 4 von 4 SUCCESS |
| H-05 | 867aed7 | 12.11.2026 18:42:24 | zusätzlich pico1_cashdesk_prod, pico2_barriers_prod | 6 von 6 SUCCESS |

### Anhang C Monitor-Auszüge

‹OFFEN›

### Anhang D Screenshots und Fotos

‹OFFEN›

### Anhang E Verkabelung

![Verkabelung von Pico 2 und Servo SG90: rot an VBUS (Pin 40), braun an GND (Pin 8), orange an GP8 (Pin 11)](verkabelung.png)
