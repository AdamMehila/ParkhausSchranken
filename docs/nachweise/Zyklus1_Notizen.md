# Notizen Zyklus 1

Arbeitsnotizen für die Testdurchführung. Adam füllt sie beim Testen aus, danach werden sie ins Testprotokoll übertragen. Nur eintragen, was beobachtet wurde. Das WLAN-Passwort nie eintragen.

Reihenfolge nach der Checkliste in Testkonzept 1.1, Kapitel 7.5. Screenshots nach dem Muster T07_Z1_1.png, Eingangskriterien als EK_Z1_1.png.

## Kopf

- Datum: 13.11.2026
- Ort: zuhause
- Firmware-Version laut Banner Pico 1: Firmware 1.0 (Monitor_Pico1_Z1.log, 17:36:45)
- Firmware-Version laut Banner Pico 2: Firmware 1.0 (Monitor_Pico2_Z1.log, 17:53:58)
- Commit: 867aed7
- Chrome-Version: 157.0.8103.79 (Dateiversion von chrome.exe)
- COM-Port Pico 1: COM6
- COM-Port Pico 2: COM3
- WLAN-Name (ohne Passwort): nbkauzAM02 7335, Windows-Hotspot des Testlaptops, 2,4 GHz, Kanal 1, WPA2
- Adresse Pico 1: 192.168.137.211
- Adresse Pico 2: 192.168.137.212
- Gateway, Subnetzmaske: 192.168.137.1, 255.255.255.0
- Monitore bis 17:33: `pio device monitor --port COMx --filter time --filter log2file` je in einem eigenen Konsolenfenster, gestartet 17:15:19 (Pico 1) und 17:15:26 (Pico 2). Logdateien logs/device-monitor-261113-171519.log (Pico 1) und logs/device-monitor-261113-171526.log (Pico 2)
- Monitore ab 17:34:02: tools/serial_monitor.py im Hintergrund, Logdateien Monitor_Pico1_Z1.log (COM6) und Monitor_Pico2_Z1.log (COM3). Gleiches Zeilenformat wie der Filter `time`. Verbindungen und Trennungen stehen als Zeilen mit `---` im Log. Grund siehe "Beobachtungen vor Zyklus 1"
- Uhrzeit Beginn: 17:18 (erster Durchgang der Eingangskriterien), 17:36 (zweiter Durchgang)
- Uhrzeit Ende:

## Durchführung: wer hat was ausgeführt

| Zeitraum | Ausführung | Beobachtung |
| --- | --- | --- |
| bis 18:15 (Eingangskriterien, T29 Schritte 1 bis 3, T06) | Adam in Chrome nach Anweisung von Claude | Adam: Meldungen, Anzeigen, Servo-Arm. Claude: Monitor-Logs, Screenshots aus dem Ordner des Snipping Tools übernommen und umbenannt |
| 18:15 bis zur Rückkehr von Adam (T29 Schritte 5 bis 8, T07 zweiter Versuch) | Claude per HTTP-Abfrage an die Endpunkte, die auch die Seiten benutzen | Claude: JSON-Antworten, Monitor-Logs. Servo-Arm von niemandem beobachtet |
| ab der Rückkehr von Adam | Claude per HTTP-Abfrage oder automatisiertem Chrome | Claude: Meldungen, Zustände, Logs. Adam nur Servo-Arm ("Arm bewegt: ja/nein") |

Bei jedem Testfall steht, welche Beobachtung von wem stammt.

## Beobachtungen vor Zyklus 1

Nur Beobachtungen, keine Bewertung.

Erster Durchgang der Eingangskriterien mit Neustart beider Picos (Adam, zwischen 17:18 und 17:30):

- Adam: Pico 1 und Pico 2 ab- und wieder eingesteckt, danach in Chrome zuerst die Kasse-Seite und dann die Schranken-Seite mit F5 neu geladen. "Beide Seiten haben geladen und funktionieren." Ladezeit nicht gemessen.
- Adam: Servo-Test auf der Schranken-Seite mit "Servo testen": grüne Meldung "Servo an GP8 öffnet die Schranke und schliesst sie nach 5 Sekunden wieder.", Arm ging hoch und nach 5 s wieder runter.
- Adam: Auf der Kasse-Seite gibt es keine Karte "Testwerkzeuge (nur im Test-Build)", auch nicht nach Strg+F5 und ganz nach unten scrollen. Der Reset wurde deshalb nicht ausgeführt.
- Log Pico 2 (logs/device-monitor-261113-171526.log), danach keine weiteren Zeilen:
  - `17:22:14.569 > [KASSE-CLIENT] Kasse nicht erreichbar, Fehler -11`
  - `17:22:42.566 > [KASSE-CLIENT] Kasse nicht erreichbar, Fehler -1`
  - `17:23:34.644 > [KASSE-CLIENT] Kasse nicht erreichbar, Fehler -11`
  - `17:25:20.565 > [KASSE-CLIENT] Kasse nicht erreichbar, Fehler -1`
  - `17:25:45.565 > [KASSE-CLIENT] Kasse nicht erreichbar, Fehler -1`
- Log Pico 1 (logs/device-monitor-261113-171519.log): leer, kein Banner, keine Zeile `[KASSE] Uhr gestellt`.

Prüfung durch Claude auf Adams Auftrag, 17:30 bis 17:34:

- platformio.ini, Umgebung pico1_cashdesk: `-DTEST_BUILD=1` (Zeile 95).
- Aufruf `http://192.168.137.211/api/debug/time` ohne Parameter um 17:30: HTTP 200, `{"ok":false,"message":"Kennzeichen steht nicht im Parkhaus.","exactDuration":"0 h 00 min","exactFee":"CHF 2.00"}`.
- Aufruf `http://192.168.137.211/api/vehicles` um 17:30: `"testBuild":true`, `"clockIsSet":true`, `"now":"13.11.2026 17:30:15"`.
- Banner Pico 1 vor dem Neustart, Mitschnitt beim Flashen: `17:06:53.216 > === Parksystem M450 - Pico 1 (Kasse) - Firmware 1.0 ===`. Nach dem Neustart steht kein Banner im Log.
- COM6 und COM3 waren um 17:31 frei. Die beiden Fenster mit `pio device monitor` hatten sich nach dem Ab- und Einstecken nicht wieder mit den Picos verbunden. Darum fehlen Banner und `[KASSE] Uhr gestellt` im Log.
- Kasse-Seite in einem zweiten Chrome 157.0.8103.79 im Hintergrundmodus mit leerem Profil um 17:31: Karte "Testwerkzeuge (nur im Test-Build)" sichtbar (`display: block`), Belegung "0 / 3", Uhr "13.11.2026 17:31:44".
- Ergebnis der Prüfung: Auf Pico 1 läuft der Test-Build. Die Seite blendet die Karte erst ein, wenn sie /api/vehicles erfolgreich abgerufen hat (src/cashdesk/web_page_cashdesk.h:269-281). Warum die Karte in Adams Chrome-Tab fehlte, ist noch offen.
- Die pio-Monitore wurden um 17:33 beendet. Ab 17:34:02 laufen die Monitore mit tools/serial_monitor.py (Abweichung von Testkonzept 1.1, Kapitel 4.3). Grund: `pio device monitor` lässt sich nicht im Hintergrund starten ("requires an interactive terminal on stdin") und verbindet sich nach einem Neustart des Picos nicht selbst wieder.

## Eingangskriterien

Zweiter Durchgang. Adam hat laut eigener Angabe Pico 1 und Pico 2 ab- und wieder eingesteckt, den Kasse-Tab geschlossen, `http://192.168.137.211` in einem neuen Tab geöffnet und danach den Schranken-Tab mit F5 neu geladen.

- Uhrzeit: 17:36:38 (Trennung Pico 1 im Log) bis 17:45:54 (Reset im Log)
- Banner beider Picos mit "Firmware 1.0" gesehen: Pico 1 `17:36:45.225 > === Parksystem M450 - Pico 1 (Kasse) - Firmware 1.0 ===` (Monitor_Pico1_Z1.log). Pico 2: Im Log von Pico 2 steht zwischen 17:34:02 und 17:49 weder eine Trennung noch ein Banner. Pico 2 hat in diesem Durchgang nicht neu gestartet. Nachgeholt: Adam hat nur Pico 2 ab- und wieder eingesteckt, Log `17:53:50.904 --- Verbindung getrennt`, dann `17:53:58.343 > === Parksystem M450 - Pico 2 (Schranken) - Firmware 1.0 ===`, `17:53:59.991 > [WLAN] Verbunden. Weboberflaeche: http://192.168.137.212`, `17:54:01.996 > [KASSE-CLIENT] Kasse nicht erreichbar, Fehler -1`. Adam danach auf der Schranken-Seite nach F5: Belegung "0 / 3", "Kasse erreichbar"
- Monitore mit Zeitstempel und Logdatei offen: ja, seit 17:34:02 (tools/serial_monitor.py)
- Kasse-Seite zuerst geöffnet, Monitorzeile `[KASSE] Uhr gestellt: ...` (Zeile abschreiben): `17:39:16.943 > [KASSE] Uhr gestellt: 13.11.2026 17:39:16`. Adam: Karte "Testwerkzeuge (nur im Test-Build)" ist im neuen Tab sichtbar
- Schranken-Seite danach geöffnet: Adam: mit F5 neu geladen
- Ladezeit Kasse-Seite (höchstens 3 s): Messung durch Claude mit PowerShell `Invoke-WebRequest`, Zeit bis die HTML-Seite vollständig empfangen ist, ohne Ausführung von JavaScript. 17:32:24 bis 17:32:26: 0,121 s, 0,085 s, 0,026 s (HTTP 200, 13018 Bytes). Nachweis Ladezeit_EK_Z1.txt
- Ladezeit Schranken-Seite (höchstens 3 s): gleiche Messung, 17:32:26 bis 17:32:27: 0,040 s, 0,084 s, 0,030 s (HTTP 200, 11175 Bytes). Nachweis Ladezeit_EK_Z1.txt
- Servo-Test: Meldung (Soll M23) und Armbewegung: Adam: grüne Meldung "Servo an GP8 öffnet die Schranke und schliesst sie nach 5 Sekunden wieder.", Arm ging hoch und nach 5 s wieder runter. Log Pico 2: `17:42:10.468 > [SCHRANKE] Schranke oeffnet (0 Grad).` und `17:42:15.501 > [SCHRANKE] Schranke schliesst (90 Grad).`
- Reset: Meldung (Soll M22): Adam: "Alle Parkvorgänge gelöscht." Log Pico 1: `17:45:54.050 > [DEBUG] Alle Parkvorgaenge geloescht.`
- Belegung auf beiden Seiten (Soll 0 / 3): Adam: "0 / 3" auf beiden Seiten
- Nachweis: EK_Z1_1.png (Kasse-Seite mit "Alle Parkvorgänge gelöscht.", Belegung 0 / 3, Uhr 13.11.2026 17:46:16), Ladezeit_EK_Z1.txt, Monitor_Pico1_Z1.log, Monitor_Pico2_Z1.log. EK_Z1_2.png (Schranken-Seite um 17:54:38 mit Belegung 0 / 3, "Kasse erreichbar", Schrankenarm "geschlossen"). Die erste Fassung von EK_Z1_2.png zeigte die Kasse-Seite und wurde durch die Aufnahme von 17:54:38 ersetzt
- Bemerkung: Log Pico 1 nach dem Neustart: `17:36:50.498 > [WLAN] Verbindung verloren. Grund: Anmeldung fehlgeschlagen. Der Router hat den Pico abgewiesen.`, `17:37:01.844 > [WLAN] Neuer Verbindungsversuch. ...`, `17:37:02.033 > [WLAN] Verbunden. Weboberflaeche: http://192.168.137.211`. Log Pico 2, Zeilen `[KASSE-CLIENT] Kasse nicht erreichbar, Fehler N`: 17:35:21.997 (-11), 17:36:40.712 (-1), 17:36:50.673 (-1), 17:37:00.697 (-1), 17:38:46.704 (-1), 17:39:00.104 (-11), 17:39:12.807 (-11), 17:43:40.760 (-11), 17:43:50.732 (-11), 17:44:00.740 (-11), 17:44:10.708 (-1)

## T29 Start (Schritte 1 bis 3)

Soll: M04. Meldung der Testwerkzeuge "Einfahrtszeit um 50 Minuten zurückgesetzt. Gebühr für genau 0 h 50 min: CHF 2.00." Vor dem Bezahlen Parkdauer 0 h 50 min bis 0 h 59 min und M16 CHF 2.00, danach M18 CHF 2.00. Bezahlen innerhalb von 9 Minuten nach der Verschiebung.

- Uhrzeit Einfahrt: 17:58:25 (Log Pico 1)
- Kennzeichen: SG 11122
- Ausgangszustand (Reset gemacht): Reset um 17:45:54 (Eingangskriterien), danach kein Fahrzeug eingefahren. Pico 2 um 17:53:50 neu gestartet
- Meldung nach der Einfahrt: Adam: grüne Meldung "SG 11122 ist registriert. Die Einfahrtsschranke öffnet und schliesst nach 5 Sekunden wieder.", Arm ging hoch und nach 5 s wieder runter
- Uhrzeit der Verschiebung: 17:59:17 (Log Pico 1)
- Meldung der Testwerkzeuge: Adam: "Einfahrtszeit um 50 Minuten zurückgesetzt. Gebühr für genau 0 h 50 min: CHF 2.00."
- Parkdauer vor dem Bezahlen (P): Adam: "0 h 51 min"
- Offener Betrag vor dem Bezahlen: Adam: orange Meldung "Offener Betrag: CHF 2.00"
- Uhrzeit der Zahlung (Z): 18:01:01.778 (Log Pico 1, Zeile `[KASSE] Bezahlt`)
- Meldung nach dem Bezahlen: Adam: "Bezahlung erfolgt. Betrag: CHF 2.00." Screenshot: Einfahrt "13.11.2026 17:09:16", Parkdauer "0 h 51 min", Gebühr "CHF 2.00", Status "bezahlt", Belegung "1 / 3", Uhr der Kasse "13.11.2026 18:01:04"
- Timer gestellt auf Z plus 14 Minuten: 18:15:02 (Signalton und Meldungsfenster auf dem Laptop)
- Timer gestellt auf Z plus 16 Minuten: 18:17:02 (Signalton und Meldungsfenster auf dem Laptop)
- Monitorzeilen:
  - Pico 2: `17:58:25.593 > [SCHRANKE] Schranke oeffnet (0 Grad).`
  - Pico 1: `17:58:25.593 > [KASSE] Einfahrt: SG 11122  Belegung: 1/3`
  - Pico 2: `17:58:30.626 > [SCHRANKE] Schranke schliesst (90 Grad).`
  - Pico 1: `17:59:17.461 > [DEBUG] Einfahrtszeit verschoben: SG 11122 -50 Minuten`
  - Pico 1: `18:01:01.778 > [KASSE] Bezahlt: SG 11122 CHF 2.00`
- Nachweis: T29_Z1_1.png (Kasse-Seite nach dem Bezahlen, von Adam gespeichert)
- Bemerkung: Log Pico 2 vor der Einfahrt: `17:57:26.968 > [KASSE-CLIENT] Kasse nicht erreichbar, Fehler -11`

## T06 Leeres Kennzeichen

Soll: M01 "Bitte ein Kennzeichen eingeben.", Arm bleibt zu, Monitor Pico 2 `[EINGABE] Abgelehnt, ungueltiges Kennzeichen. Laenge: 0`, Kasse unverändert.

- Uhrzeit: 18:04:52 (Log Pico 2)
- Kennzeichen: leeres Feld
- Ausgangszustand: SG 11122 aus T29 im Parkhaus, bezahlt, Belegung 1 / 3
- Meldungstext: Adam: rote Meldung "Bitte ein Kennzeichen eingeben."
- Servo: Adam: Arm hat sich nicht bewegt
- Monitorzeile Pico 2: `18:04:52.745 > [EINGABE] Abgelehnt, ungueltiges Kennzeichen. Laenge: 0`
- Monitor Pico 1 (neue Zeile ja oder nein): nein, zwischen 18:01:02 und 18:07:16 keine Zeile
- Kasse-Seite: Adam: nur SG 11122 unter "Fahrzeuge im Parkhaus", Belegung "1 / 3". Screenshot 18:06:56: Belegung 1 / 3, einzige Zeile SG 11122, Parkdauer 0 h 57 min, Status bezahlt
- Status:
- Nachweis: T06_Z1_2.png (Kasse-Seite 18:06:56). T06_Z1_1.png (Schranken-Seite mit Meldung) fehlt noch
- Bemerkung: Log Pico 2: `18:03:55.372 > [KASSE-CLIENT] Kasse nicht erreichbar, Fehler -11`

## T07 Sonderzeichen

Soll: M03 "Das ist kein gültiges Kennzeichen. Erwartet wird zum Beispiel LU 12345.", Arm bleibt zu, Monitor Pico 2 `... Laenge: 10`, Kasse unverändert.

Erster Versuch (Adam im Browser, vor 18:18, Uhrzeit nicht notiert): Adam hat die Eingabe aus der Zwischenablage eingefügt und sah danach die rote Meldung "Bitte ein Kennzeichen eingeben." Abklärung durch Claude:

- Im Log von Pico 2 steht zwischen 18:07:00 und 18:18:47 keine Zeile `[EINGABE] ...` und keine Trennung.
- Ein Klick auf "Einfahrt bestätigen" ersetzt die Meldung sofort durch "Anfrage läuft ..." und schickt die Eingabe an Pico 2 (src/barriers/web_page_barriers.h:137-161). Pico 2 schreibt jede abgelehnte Eingabe ins Log (src/barriers/main.cpp:127-141).
- Ergebnis: Der erste Versuch hat Pico 2 nicht erreicht. Die rote Meldung war die stehen gebliebene Meldung aus T06 (18:04:52). Kein Befund zur Firmware.

Zweiter Versuch, per HTTP-Abfrage durch Claude statt im Browser (Adam abwesend):

- Uhrzeit: 18:18:47.878 (Aufruf)
- Kennzeichen: `!@#$%^&*()`
- Ausgangszustand: SG 11122 aus T29 im Parkhaus, bezahlt. `/api/vehicles` um 18:18:47: `"occupied":1`, einziges Fahrzeug SG 11122
- Meldungstext: Aufruf `http://192.168.137.212/api/entry?plate=!%40%23%24%25%5E%26*()` (gleiche Kodierung wie die Seite mit `encodeURIComponent`). Antwort HTTP 200 `{"ok":false,"message":"Das ist kein gültiges Kennzeichen. Erwartet wird zum Beispiel LU 12345."}`. Im Browser nicht beobachtet
- Servo: nicht beobachtet (Adam abwesend). Im Log von Pico 2 keine Zeile `[SCHRANKE] Schranke oeffnet`. `/api/state` um 18:18:47: `"entryBarrierOpen":false`
- Monitorzeile Pico 2: `18:18:47.988 > [EINGABE] Abgelehnt, ungueltiges Kennzeichen. Laenge: 10`
- Kasse-Seite: im Browser nicht beobachtet. `/api/vehicles` vor und nach dem Aufruf gleich: `"occupied":1`, einziges Fahrzeug SG 11122. Keine Zeile im Log von Pico 1
- Status:
- Nachweis: Monitor_Pico2_Z1.log, diese Notizen
- Bemerkung: Ausführung und Ablesung durch Claude per HTTP

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

Adam war ab 18:15 abwesend. Die Schritte 5 bis 8 hat Claude per HTTP-Abfrage ausgeführt und abgelesen, nicht im Browser. Die Aufrufe sind dieselben, die die Seiten bei Suche, Ausfahrt und Bezahlung schicken.

- Uhrzeit Suche bei Z plus 14 Minuten: nicht durchgeführt. Der Timer hat um 18:15:03 ausgelöst. Schritt 4 hätte bis Z plus 15 Minuten (18:16:01) laufen müssen. Adam: "Für den nächsten Schritt von T29 ist es zu spät."
- Parkdauer, Status, Meldung: keine Beobachtung
- Uhrzeit Suche bei Z plus 16 Minuten: 18:18:04 (Z plus 17 min 02 s), HTTP-Abfrage durch Claude
- Parkdauer, Status, Meldung, Betrag: Aufruf `http://192.168.137.211/api/status?plate=SG%2011122`, Antwort HTTP 200 `{"plate":"SG 11122","status":"paid","entry":"13.11.2026 17:09:16","duration":"1 h 08 min","fee":"CHF 2.00","feeInCents":200}`. Die Meldung auf der Kasse-Seite wurde nicht beobachtet
- Uhrzeit Ausfahrt: 18:18:55.726, HTTP-Abfrage durch Claude statt Klick auf "Ausfahrt bestätigen"
- Meldungstext Ausfahrt: Aufruf `http://192.168.137.212/api/exit?plate=SG%2011122`, Antwort HTTP 200 `{"ok":true,"message":"Bezahlung bestätigt. Die Ausfahrtsschranke öffnet für SG 11122."}`
- Servo: nicht beobachtet (Adam abwesend). Log Pico 2: `18:18:55.953 > [SCHRANKE] Schranke oeffnet (0 Grad).` und `18:19:00.985 > [SCHRANKE] Schranke schliesst (90 Grad).` `/api/state` direkt nach dem Aufruf: `"entryBarrierOpen":true`, `"exitBarrierOpen":true`, `"exitBarrierMsLeft":4810`, `"occupied":0`
- Monitorzeilen Pico 1 und Pico 2:
  - Pico 1: `18:18:55.837 > [KASSE] Statusabfrage von Pico 2: SG 11122 -> paid`
  - Pico 2: `18:18:55.897 > [AUSFAHRT] Kasse meldet fuer SG 11122: paid`
  - Pico 1: `18:18:55.953 > [KASSE] Ausfahrt: SG 11122  Belegung: 0/3`
  - Pico 2: `18:18:55.953 > [SCHRANKE] Schranke oeffnet (0 Grad).`
  - Pico 2: `18:19:00.985 > [SCHRANKE] Schranke schliesst (90 Grad).`
- Kasse nach der Ausfahrt: `/api/vehicles` um 18:18:56: `"occupied":0`, keine Fahrzeuge im Parkhaus, unter "history" SG 11122 mit `"status":"gone"`, `"exit":"13.11.2026 18:18:55"`, `"fee":"CHF 2.00"`
- Nachzahlung: Meldung und Betrag: Suche um 18:19:03.569, Aufruf `http://192.168.137.211/api/status?plate=SG%2011122`, Antwort HTTP 200 `{"plate":"SG 11122","status":"unknown","entry":"-","duration":"-","fee":"-","feeInCents":0}`. Bei `"status":"unknown"` sperrt die Kasse-Seite den Knopf "Bezahlen" (src/cashdesk/web_page_cashdesk.h:191-198). `/api/pay` wurde deshalb nicht aufgerufen. Keine Nachzahlung
- Zweite Ausfahrt: Meldung und Servo: 18:19:03.803, Aufruf `http://192.168.137.212/api/exit?plate=SG%2011122`, Antwort HTTP 200 `{"ok":false,"message":"Zu dem Kennzeichen SG 11122 ist keine Einfahrt bekannt. Die Schranke bleibt geschlossen."}`. Servo nicht beobachtet, im Log keine Zeile `Schranke oeffnet`, `/api/state` danach `"exitBarrierOpen":false`. Log: Pico 1 `18:19:04.031 > [KASSE] Statusabfrage von Pico 2: SG 11122 -> unknown`, Pico 2 `18:19:04.031 > [AUSFAHRT] Kasse meldet fuer SG 11122: unknown` und `18:19:04.031 > [AUSFAHRT] Unbekanntes Kennzeichen: SG 11122`
- Status:
- Nachweis: Monitor_Pico1_Z1.log, Monitor_Pico2_Z1.log, diese Notizen
- Bemerkung: Schritt 4 zu spät, nicht durchgeführt. Schritte 5 bis 8 per HTTP-Abfrage durch Claude statt im Browser beobachtet. Der Servo-Arm wurde in den Schritten 6 und 8 von niemandem beobachtet

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
