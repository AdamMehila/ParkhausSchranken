# Notizen Zyklus 1

Arbeitsnotizen für die Testdurchführung. Adam füllt sie beim Testen aus, danach werden sie ins Testprotokoll übertragen. Nur eintragen, was beobachtet wurde. Das WLAN-Passwort nie eintragen.

Reihenfolge nach der Checkliste in Testkonzept 1.1, Kapitel 7.5. Screenshots nach dem Muster T07_Z1_1.png, Eingangskriterien als EK_Z1_1.png.

## Kopf

- Datum:
- Ort:
- Firmware-Version laut Banner Pico 1:
- Firmware-Version laut Banner Pico 2:
- Commit: 867aed7
- Chrome-Version:
- COM-Port Pico 1:
- COM-Port Pico 2:
- WLAN-Name (ohne Passwort):
- Adresse Pico 1:
- Adresse Pico 2:
- Gateway, Subnetzmaske:
- Uhrzeit Beginn:
- Uhrzeit Ende:

## Eingangskriterien

- Uhrzeit:
- Banner beider Picos mit "Firmware 1.0" gesehen:
- Monitore mit Zeitstempel und Logdatei offen:
- Kasse-Seite zuerst geöffnet, Monitorzeile `[KASSE] Uhr gestellt: ...` (Zeile abschreiben):
- Schranken-Seite danach geöffnet:
- Ladezeit Kasse-Seite (höchstens 3 s):
- Ladezeit Schranken-Seite (höchstens 3 s):
- Servo-Test: Meldung (Soll M23) und Armbewegung:
- Reset: Meldung (Soll M22):
- Belegung auf beiden Seiten (Soll 0 / 3):
- Nachweis:
- Bemerkung:

## T29 Start (Schritte 1 bis 3)

Soll: M04. Meldung der Testwerkzeuge "Einfahrtszeit um 50 Minuten zurückgesetzt. Gebühr für genau 0 h 50 min: CHF 2.00." Vor dem Bezahlen Parkdauer 0 h 50 min bis 0 h 59 min und M16 CHF 2.00, danach M18 CHF 2.00. Bezahlen innerhalb von 9 Minuten nach der Verschiebung.

- Uhrzeit Einfahrt:
- Kennzeichen: SG 11122
- Ausgangszustand (Reset gemacht):
- Meldung nach der Einfahrt:
- Uhrzeit der Verschiebung:
- Meldung der Testwerkzeuge:
- Parkdauer vor dem Bezahlen (P):
- Offener Betrag vor dem Bezahlen:
- Uhrzeit der Zahlung (Z):
- Meldung nach dem Bezahlen:
- Timer gestellt auf Z plus 14 Minuten:
- Timer gestellt auf Z plus 16 Minuten:
- Monitorzeilen:
- Nachweis:
- Bemerkung:

## T06 Leeres Kennzeichen

Soll: M01 "Bitte ein Kennzeichen eingeben.", Arm bleibt zu, Monitor Pico 2 `[EINGABE] Abgelehnt, ungueltiges Kennzeichen. Laenge: 0`, Kasse unverändert.

- Uhrzeit:
- Kennzeichen: leeres Feld
- Ausgangszustand:
- Meldungstext:
- Servo:
- Monitorzeile Pico 2:
- Monitor Pico 1 (neue Zeile ja oder nein):
- Kasse-Seite:
- Status:
- Nachweis:
- Bemerkung:

## T07 Sonderzeichen

Soll: M03 "Das ist kein gültiges Kennzeichen. Erwartet wird zum Beispiel LU 12345.", Arm bleibt zu, Monitor Pico 2 `... Laenge: 10`, Kasse unverändert.

- Uhrzeit:
- Kennzeichen: `!@#$%^&*()`
- Ausgangszustand:
- Meldungstext:
- Servo:
- Monitorzeile Pico 2:
- Kasse-Seite:
- Status:
- Nachweis:
- Bemerkung:

## T08 Überlange Eingabe

Soll: M02 "Das Kennzeichen ist zu lang. Erlaubt sind höchstens 10 Zeichen, zum Beispiel LU 12345.", Arm bleibt zu, Monitor Pico 2 `... Laenge: 30`, kein neues Banner.

- Uhrzeit:
- Kennzeichen: 30 mal A
- Ausgangszustand:
- Meldungstext:
- Servo:
- Monitorzeile Pico 2:
- Neues Banner im Monitor (ja oder nein):
- Seite danach bedienbar:
- Status:
- Nachweis:
- Bemerkung:

## T27 Fehlversuch mit Datum und Uhrzeit

Soll: M01, M02, M03. Liste "Fehlversuche" auf der Schranken-Seite mit Datum, Uhrzeit, Eingabe und Grund, drei Zeilen, neueste oben, Uhrzeit höchstens 1 Minute Abweichung.

- Uhrzeit Klick 1 (leer):
- Uhrzeit Klick 2 (30 mal A):
- Uhrzeit Klick 3 (`!@#$%^&*()`):
- Ausgangszustand (Schranken-Seite seit Start von Pico 2 geöffnet):
- Meldungstexte:
- Liste "Fehlversuche" vorhanden (ja oder nein):
- Inhalt der Liste (abschreiben):
- Status:
- Nachweis:
- Bemerkung:

## T15 Unbekanntes Kennzeichen an der Kasse

Soll: M15 "Das Kennzeichen UR 99988 wurde im System nicht gefunden. Es ist keine Einfahrt bekannt.", keine Angaben zu Einfahrt, Parkdauer und Gebühr, Bezahlen ausgegraut.

- Uhrzeit:
- Kennzeichen: UR 99988
- Ausgangszustand:
- Meldungstext:
- Angaben im Suchresultat:
- Knopf "Bezahlen":
- Status:
- Nachweis:
- Bemerkung:

## T18 Bezahlung für unbekanntes Kennzeichen

Soll: M15 mit ZG 77766, Bezahlen ausgegraut. Aufruf `/api/pay?plate=ZG%2077766` ergibt M20 mit `"ok":false`. Keine Zeile `[KASSE] Bezahlt`.

- Uhrzeit:
- Kennzeichen: ZG 77766
- Ausgangszustand:
- Meldungstext Suche:
- Knopf "Bezahlen":
- Antwort des Aufrufs (abschreiben):
- Monitor Pico 1:
- Status:
- Nachweis:
- Bemerkung:

## T25 Doppelte Einfahrt

Soll: zweimal M05 "Dieses Fahrzeug steht bereits im Parkhaus.", Arm bleibt zu, Belegung unverändert, eine Zeile LU 12345, keine neue Zeile `[KASSE] Einfahrt: LU 12345`. Knopf klicken, nicht Enter.

- Uhrzeit Einfahrt LU 12345 (Vorarbeit):
- Belegung vor dem Test:
- Uhrzeit Versuch "LU 12345":
- Meldungstext:
- Uhrzeit Versuch "lu12345":
- Meldungstext:
- Servo:
- Belegung nach dem Test:
- Zeilen LU 12345 auf der Kasse-Seite:
- Monitor Pico 1:
- Status:
- Nachweis:
- Bemerkung:

## T29 Fortsetzung (Schritte 4 bis 8)

Soll: Bei Z plus 14 Minuten Parkdauer P plus 14 Minuten, Status "bezahlt", M17. Bei Z plus 16 Minuten Status "offen", M16 "Offener Betrag: CHF 2.00". Ausfahrt M10 mit CHF 2.00, Arm bleibt zu. Nachzahlung M18 CHF 2.00. Ausfahrt M11, Arm öffnet.

- Uhrzeit Suche bei Z plus 14 Minuten:
- Parkdauer, Status, Meldung:
- Uhrzeit Suche bei Z plus 16 Minuten:
- Parkdauer, Status, Meldung, Betrag:
- Uhrzeit Ausfahrt:
- Meldungstext Ausfahrt:
- Servo:
- Monitorzeilen Pico 1 und Pico 2:
- Nachzahlung: Meldung und Betrag:
- Zweite Ausfahrt: Meldung und Servo:
- Status:
- Nachweis:
- Bemerkung:

## T01 Gültiges Kennzeichen bei der Einfahrt

Soll: M04 "LU 12345 ist registriert. Die Einfahrtsschranke öffnet und schliesst nach 5 Sekunden wieder.", Arm offen in höchstens 3 s, zu nach 5 s ± 1 s, `[KASSE] Einfahrt: LU 12345  Belegung: 1/3`, Eintrag mit Datum und Uhrzeit, Belegung 1 / 3.

- Uhrzeit des Klicks:
- Kennzeichen: LU 12345
- Ausgangszustand (Reset gemacht):
- Meldungstext:
- Servo:
- Monitorzeilen Pico 2:
- Monitorzeile Pico 1:
- Eintrag Kasse-Seite (Einfahrt, Status):
- Belegung beide Seiten:
- Status:
- Nachweis:
- Bemerkung:

## T02 Einfahrtszeit gegen Laptop-Uhr

Soll: gleiches Datum, Uhrzeit höchstens 1 Minute Abweichung zur Uhrzeit des Klicks aus T01.

- Uhrzeit:
- Angezeigte Einfahrt LU 12345:
- Uhrzeit des Klicks aus T01:
- Abweichung:
- Status:
- Nachweis:
- Bemerkung:

## T03 Erneute Einfahrt nach Abschluss

Vorarbeit: LU 12345 bezahlen (M18) und ausfahren (M11). Soll: M04, neue Zeile unter "Fahrzeuge im Parkhaus", alte Zeile unverändert unter "Abgeschlossene Vorgänge", Belegung 1 / 3.

- Uhrzeit Vorarbeit:
- Meldungen der Vorarbeit:
- Uhrzeit der erneuten Einfahrt:
- Meldungstext:
- Servo:
- "Fahrzeuge im Parkhaus":
- "Abgeschlossene Vorgänge":
- Belegung:
- Status:
- Nachweis:
- Bemerkung:

## T04 Einfahrtsschranke öffnet in 3 Sekunden

Soll: Arm senkrecht höchstens 3,0 s nach dem Klick, M04, `[SCHRANKE] Schranke oeffnet (0 Grad).`

- Uhrzeit:
- Kennzeichen: ZH 98765
- Ausgangszustand (Reset gemacht):
- Gemessene Zeit (Stoppuhr):
- Meldungstext:
- Monitorzeile Pico 2:
- Status:
- Nachweis:
- Bemerkung:

## T05 Einfahrtsschranke schliesst nach 5 Sekunden

Soll: Differenz der Zeitstempel und Stoppuhr je 5,0 s ± 1,0 s, Arm danach waagrecht, Anzeige "geschlossen".

- Zeitstempel `oeffnet` im Monitor:
- Zeitstempel `schliesst` im Monitor:
- Differenz:
- Stoppuhr:
- Anzeige auf der Schranken-Seite danach:
- Status:
- Nachweis:
- Bemerkung:

## T09 Letzter freier Platz

Vorarbeit: Reset, LU 12345 und ZH 98765 einfahren. Soll: M04, `[KASSE] Einfahrt: SG 11122  Belegung: 3/3`, Belegung 3 / 3 auf beiden Seiten.

- Uhrzeit:
- Kennzeichen: SG 11122
- Belegung vor dem Test:
- Meldungstext:
- Servo:
- Monitorzeile Pico 1:
- Belegung beide Seiten danach:
- Status:
- Nachweis:
- Bemerkung:

## T10 Parkhaus voll

Soll: M06 "Das Parkhaus ist voll. Keine weitere Einfahrt möglich.", Arm bleibt zu, keine Zeile `[KASSE] Einfahrt: TG 44499`, TG 44499 nicht in der Liste.

- Uhrzeit:
- Kennzeichen: TG 44499
- Meldungstext:
- Servo:
- Monitor Pico 1:
- Liste und Belegung:
- Status:
- Nachweis:
- Bemerkung:

## T11 Platz nach Ausfahrt frei

Vorarbeit: LU 12345 an der Kasse bezahlen. Soll: M11, Monitorzeilen mit `paid`, Belegung 2 / 3, danach M04 für TG 44499 und Belegung 3 / 3.

- Uhrzeit Bezahlung (Vorarbeit) und Betrag:
- Uhrzeit Ausfahrt LU 12345:
- Meldungstext Ausfahrt:
- Servo:
- Monitorzeilen Pico 1 und Pico 2:
- Belegung nach der Ausfahrt:
- Meldungstext Einfahrt TG 44499:
- Belegung danach:
- Status:
- Nachweis:
- Bemerkung:

## T12 Gebühr nach 1 Minute

Soll: Suche zwischen 60 s und 119 s nach M04: Parkdauer "0 h 01 min", M16 "Offener Betrag: CHF 2.00", Status "offen".

- Uhrzeit Einfahrt:
- Kennzeichen: ZH 98765
- Ausgangszustand (Reset gemacht):
- Stoppuhrzeit der Suche:
- Parkdauer:
- Meldung und Betrag:
- Status:
- Nachweis:
- Bemerkung:

## T13 Gebühr an der Stundengrenze

Soll: 59 min: Meldung mit "Gebühr für genau 0 h 59 min: CHF 2.00", Suche "0 h 59 min", CHF 2.00. 60 min: Meldung mit "Gebühr für genau 1 h 00 min: CHF 2.00". 61 min: Suche "1 h 01 min", CHF 4.00. Suche jeweils innerhalb von 60 s.

- Uhrzeit:
- Kennzeichen: BE 55511
- Ausgangszustand (Reset und Einfahrt):
- Meldung bei 59 Minuten:
- Suche bei 59 Minuten (Parkdauer, Betrag):
- Meldung bei 60 Minuten:
- Meldung bei 61 Minuten:
- Suche bei 61 Minuten (Parkdauer, Betrag):
- Monitorzeilen Pico 1:
- Status:
- Nachweis:
- Bemerkung:

## T14 Gebühr nach 3 h 15 min

Soll: Meldung mit "Gebühr für genau 3 h 15 min: CHF 8.00", Suche "3 h 15 min", M16 CHF 8.00.

- Uhrzeit:
- Kennzeichen: SG 11122
- Ausgangszustand (Reset und Einfahrt):
- Meldung der Testwerkzeuge:
- Suche (Parkdauer, Betrag):
- Status:
- Nachweis:
- Bemerkung:

## T16 Bezahlung erfolgreich

Vorarbeit: Reset, LU 12345 einfahren. Soll: M16 CHF 2.00, nach dem Klick M18 "Bezahlung erfolgt. Betrag: CHF 2.00.", Status "bezahlt", Bezahlen ausgegraut, `[KASSE] Bezahlt: LU 12345 CHF 2.00`.

- Uhrzeit Einfahrt:
- Parkdauer und Betrag vor dem Bezahlen:
- Uhrzeit des Klicks auf "Bezahlen" (für T28):
- Meldungstext:
- Status und Knopf:
- Monitorzeile Pico 1:
- Status:
- Nachweis:
- Bemerkung:

## T28 Zahlungszeitpunkt

Soll: Suchresultat mit Feld "Bezahlt am", Datum und Uhrzeit, höchstens 1 Minute Abweichung zur Uhrzeit des Klicks in T16.

- Uhrzeit:
- Feld "Bezahlt am" vorhanden (ja oder nein):
- Angezeigter Zeitpunkt:
- Abweichung:
- Status:
- Nachweis:
- Bemerkung:

## T17 Doppelte Bezahlung

Soll: M17, Bezahlen ausgegraut. Aufruf `/api/pay?plate=LU%2012345` ergibt M19 mit `"alreadyPaid":true`. Keine neue Zeile `[KASSE] Bezahlt`. Gebühr weiterhin CHF 2.00.

- Uhrzeit:
- Meldungstext Suche:
- Knopf "Bezahlen":
- Antwort des Aufrufs (abschreiben):
- Monitor Pico 1:
- Gebühr bei erneuter Suche:
- Status:
- Nachweis:
- Bemerkung:

## T19 Ausfahrt nach Bezahlung

Soll: `[KASSE] Statusabfrage von Pico 2: LU 12345 -> paid`, `[AUSFAHRT] Kasse meldet fuer LU 12345: paid`, M11, Arm offen in höchstens 3,0 s, zu nach 5 s ± 1 s, Eintrag unter "Abgeschlossene Vorgänge", Belegung 0 / 3.

- Uhrzeit:
- Kennzeichen: LU 12345
- Gemessene Zeit bis zum Öffnen:
- Meldungstext:
- Monitorzeilen Pico 1:
- Monitorzeilen Pico 2:
- Listen und Belegung:
- Status:
- Nachweis:
- Bemerkung:

## T20 Ausfahrt ohne Bezahlung

Vorarbeit: ZH 98765 einfahren. Soll: M10 mit CHF 2.00, Arm bleibt zu, Monitorzeilen mit `unpaid`, nach Bezahlung (M18) M11 und Arm öffnet.

- Uhrzeit Einfahrt:
- Uhrzeit erste Ausfahrt:
- Meldungstext:
- Servo:
- Monitorzeilen Pico 1 und Pico 2:
- Bezahlung: Meldung und Betrag:
- Zweite Ausfahrt: Meldung und Servo:
- Status:
- Nachweis:
- Bemerkung:

## T21 Bezahlt, Kennzeichen anders

Vorarbeit: BE 55511 einfahren und bezahlen. Soll: M09 für BE 55512, Arm bleibt zu, Monitorzeilen mit `unknown`, danach M11 für BE 55511 und Arm öffnet.

- Uhrzeit:
- Meldungstext BE 55512:
- Servo:
- Monitorzeilen Pico 1 und Pico 2:
- Meldungstext BE 55511:
- Servo:
- Status:
- Nachweis:
- Bemerkung:

## T22 Kompletter Durchlauf

Soll: M04, Meldung mit "Gebühr für genau 1 h 30 min: CHF 4.00", Suche "1 h 30 min" und M16 CHF 4.00, M18 CHF 4.00, M11, Eintrag mit Bezahlt CHF 4.00, Belegung 0 / 3.

- Uhrzeit:
- Kennzeichen: SG 11122
- Ausgangszustand (Reset gemacht):
- Einfahrt: Meldung und Servo:
- Meldung der Testwerkzeuge:
- Suche (Parkdauer, Betrag):
- Bezahlung: Meldung:
- Ausfahrt: Meldung und Servo:
- Listen und Belegung:
- Status:
- Nachweis:
- Bemerkung:

## T24 Kennzeichen erscheint an der Kasse

Soll: Eintrag unter "Fahrzeuge im Parkhaus" höchstens 3 s nach dem Wechsel, Einfahrt höchstens 1 Minute Abweichung, Status "offen", `[KASSE] Einfahrt: ZH 98765  Belegung: 1/3`.

- Uhrzeit des Klicks:
- Kennzeichen: ZH 98765
- Ausgangszustand (Reset gemacht):
- Zeit bis zur Anzeige:
- Angezeigte Einfahrt und Status:
- Monitorzeile Pico 1:
- Status:
- Nachweis:
- Bemerkung:

## T26 Kennzeichenformat

Soll: M04 für LU 122, LU 123456 und lu1 (wird LU 1). M03 für 12345, LU, L 12345, LU 12A45 und LU 1234567. M02 für LU 12345678. Belegung danach 3 / 3.

- Uhrzeit:
- Ausgangszustand (Reset gemacht):
- LU 122:
- LU 123456:
- lu1:
- 12345:
- LU:
- L 12345:
- LU 12A45:
- LU 1234567:
- LU 12345678:
- Monitorzeilen Pico 2 (Laenge):
- Liste und Belegung danach:
- Status:
- Nachweis:
- Bemerkung:

## T23 Kasse fällt kurz aus

Vorarbeit: Reset, TG 44499 einfahren und bezahlen. Soll: Antwort der Testhilfe, M08 höchstens 5 s nach dem Klick, Arm bleibt zu, `[AUSFAHRT] Kasse nicht erreichbar, Schranke bleibt zu.`, kein neues Banner, nach Ablauf M11 und Arm öffnet.

- Uhrzeit Aufruf `/api/debug/offline?seconds=30`:
- Antwort des Aufrufs:
- Monitorzeile Pico 1:
- Uhrzeit Klick Ausfahrt:
- Zeit bis zur Meldung (Stoppuhr):
- Meldungstext:
- Servo:
- Monitorzeilen Pico 2:
- Anzeige "Kasse erreichbar" oder "Kasse nicht erreichbar":
- Neues Banner in einem Monitor (ja oder nein):
- Uhrzeit `[DEBUG] Kasse antwortet wieder.`:
- Zweite Ausfahrt: Meldung und Servo:
- Status:
- Nachweis:
- Bemerkung:
