# Projekt einrichten und starten

Diese Anleitung führt dich vom ausgepackten Material bis zum laufenden Parkhaus. Du musst
dafür nicht programmieren können. Du musst nur an einer Stelle im Code die Daten deines
eigenen WLANs eintragen.

**Der Ablauf im Überblick:**

1. Werkzeuge installieren
2. Den Servo an einen der beiden Picos anschliessen
3. Die eigene WLAN-Konfiguration eintragen
4. Die Firmware bauen
5. Die Firmware auf die beiden Picos spielen, zuerst die Kasse, dann den Pico mit dem Servo
6. Prüfen, ob beide Picos im WLAN sind
7. Den Servo-Test durchführen
8. Das ganze Projekt ausführen

Ganz am Ende steht eine Tabelle zur Fehlerbehebung.

---

## 1. Werkzeuge installieren

- [Visual Studio Code](https://code.visualstudio.com/) installieren.
- In VS Code links auf das Symbol für **Erweiterungen** klicken, nach **PlatformIO IDE**
  suchen und installieren.
- Den Projektordner in VS Code öffnen: **Datei, Ordner öffnen**.

Beim ersten Bauen lädt PlatformIO die Werkzeuge für den Pico selbst herunter. Das dauert
einige Minuten.

**Material:**

| Anzahl | Teil |
| --- | --- |
| 2 | Raspberry Pi Pico W mit aufgelöteten Stiftleisten, je auf einem Steckbrett |
| 1 | SG90-Servo mit Schrankenarm und drei Jumper-Kabeln |
| 2 | Micro-USB-Kabel |
| 1 | Laptop mit Browser |

---

## 2. Den Servo anschliessen

Der Servo kommt an **einen** der beiden Picos. Dieser Pico wird die **Schranke**. Der andere
wird die **Kasse** und braucht nur ein USB-Kabel.

Damit du sie nicht verwechselst: Klebe auf den Pico mit Servo einen Zettel „2 Schranke"
und auf den anderen „1 Kasse".

**Beim Verkabeln ist kein USB-Kabel eingesteckt.** So sieht es aus:

![Verkabelung von Pico und SG90 auf dem Steckbrett](docs/verkabelung.png)

| Ader am Servo | Pin am Pico | Loch im Steckbrett |
| --- | --- | --- |
| rot | Pin 40, **VBUS** (5 Volt) | **J1** |
| braun | Pin 8, **GND** (Masse) | **A8** |
| orange | Pin 11, **GP8** (Signal) | **A11** |

Die Löcher gelten für den Aufbau, wie er bei uns auf dem Tisch steht: Steckbrett **hochkant**,
die Reihen 1 bis 30 von oben nach unten, der Pico mit der **USB-Buchse nach oben**. Seine linken
Beinchen (Pin 1 bis 20) stecken in Spalte **C**, seine rechten (Pin 40 bis 21) in Spalte **H**,
das oberste Beinchen in Reihe 1. Damit liegen alle GP-Pins und Pin 8 (GND) auf der **linken**
Hälfte (A bis E) und VBUS auf der **rechten** (F bis J).

In jeder Reihe sind die Löcher A bis E miteinander verbunden, und getrennt davon F bis J. Ein
Kabel ist also mit einem Pin verbunden, wenn es in **derselben Reihe** und auf **derselben
Hälfte** steckt wie dieser Pin.

**Achtung beim Bild:** Im Bild liegt das Steckbrett **quer**, mit der USB-Buchse links. Seine
Buchstaben und Zahlen passen deshalb nicht zu einem hochkant liegenden Steckbrett. Richte dich
nach den Pin-Namen im Bild (VBUS, GND, GP8), nicht nach den Lochangaben darin.

Der Code schickt das Signal auf **GP8**. Das steht in `src/common/config.h` bei
`SERVO_PIN_ENTRY`. Steckst du das orange Kabel woanders ein, musst du dort die passende
**GPIO-Nummer** eintragen (zum Beispiel `8` für GP8, nicht die Pin-Nummer 11).

---

## 3. Die eigene WLAN-Konfiguration

Die Picos haben **keinen Ethernet-Anschluss**, sie kommen nur über WLAN ins Netz. Damit sie
funktionieren, trägst du dein eigenes WLAN und zwei Adressen in eine Datei ein. Diese
Werte kennt niemand ausser dir, deshalb musst du sie selbst herausfinden.

### 3.1 Welches WLAN geht?

Das WLAN muss **alle** diese Bedingungen erfüllen:

| Bedingung | Warum |
| --- | --- |
| **2,4 GHz** | Der Funkchip des Pico kann kein 5 GHz. Viele Router haben für 2,4 GHz einen eigenen Namen, oft mit dem Zusatz `-2.4GHz` oder `_2G`. Genau diesen Namen musst du nehmen. |
| **mit Passwort (WPA2)** | Netze mit Anmeldeseite im Browser oder mit Benutzername und Passwort (z. B. Firmen- oder Hochschulnetze) gehen nicht. |
| **Dein Laptop ist im selben Netz** | Sonst kann der Browser die Picos nicht erreichen. Der Laptop darf dabei per 5 GHz oder per Kabel verbunden sein, solange es derselbe Router ist. |
| **Geräte dürfen sich gegenseitig erreichen** | Gäste- und Schulnetze verbieten das oft. Dann sind die Picos im WLAN, aber der Browser findet sie trotzdem nicht. |

**Wenn du kein passendes WLAN hast**, zum Beispiel weil dein Laptop nur per Kabel im Netz
hängt: Mach mit dem Laptop einen **mobilen Hotspot** auf. Die Picos verbinden sich dann mit
deinem Laptop.

1. Windows-Einstellungen, **Netzwerk und Internet**, **Mobiler Hotspot**.
2. Unter **Eigenschaften**, **Bearbeiten** das **Netzwerkband auf 2,4 GHz** stellen. Dort
   siehst du auch Name und Passwort des Hotspots.
3. Den Hotspot einschalten.

Mit dem Hotspot sind die Netzwerkwerte immer gleich, du kannst Abschnitt 3.2 überspringen:

| Wert | beim Windows-Hotspot |
| --- | --- |
| Gateway | `192.168.137.1` |
| Subnetzmaske | `255.255.255.0` |
| Adressen für die Picos | z. B. `192.168.137.211` und `192.168.137.212` |

Auf einem Firmenlaptop kann ein Hotspot durch die IT verboten oder gesperrt sein. Klär das
vorher.

### 3.2 Die Daten deines Netzes herausfinden

Verbinde den Laptop mit dem WLAN, das du benutzen willst. Öffne in VS Code ein Terminal
(**Terminal, Neues Terminal**) und tippe:

```text
ipconfig
```

Such den Abschnitt deines WLAN-Adapters (oder bei Kabel den Ethernet-Adapter) und notiere
drei Zeilen. So sieht das zum Beispiel aus:

```text
Drahtlos-LAN-Adapter WLAN:
   IPv4-Adresse  . . . . . . . . . . : 192.168.1.57
   Subnetzmaske  . . . . . . . . . . : 255.255.255.0
   Standardgateway . . . . . . . . . : 192.168.1.1
```

| Was | Beispiel | Wofür |
| --- | --- | --- |
| IPv4-Adresse | `192.168.1.57` | die Adresse deines Laptops, zeigt den Bereich deines Netzes |
| Subnetzmaske | `255.255.255.0` | kommt in `IP_SUBNET_MASK` |
| Standardgateway | `192.168.1.1` | die Adresse des Routers, kommt in `IP_GATEWAY` |

Deine Werte sind mit grosser Wahrscheinlichkeit andere. Nimm immer deine eigenen.

### 3.3 Zwei freie Adressen für die Picos wählen

Jeder Pico bekommt eine **feste Adresse**. So findet die Schranke die Kasse immer an
derselben Stelle.

Bei der Subnetzmaske `255.255.255.0` gilt: Die ersten drei Zahlen übernimmst du von deinem
Laptop, die letzte wählst du selbst. Nimm hohe Zahlen, die der Router selten vergibt, zum
Beispiel 201 und 202:

| Pico | Beispiel |
| --- | --- |
| Kasse | `192.168.1.201` |
| Schranke | `192.168.1.202` |

**Prüf, ob die Adressen frei sind.** Im Terminal:

```text
ping 192.168.1.201
ping 192.168.1.202
```

Kommt **keine Antwort** („Zeitüberschreitung" oder „Zielhost nicht erreichbar"), ist die
Adresse frei. Kommt eine Antwort, benutzt sie schon ein anderes Gerät. Dann nimm eine
andere Zahl.

Hat deine Subnetzmaske eine andere Form, zum Beispiel `255.255.254.0`, ist dein Netz grösser.
Am einfachsten nimmst du dann trotzdem Adressen, bei denen nur die letzte Zahl anders ist
als bei deinem Laptop.

### 3.4 Die Werte eintragen

Öffne die Datei **`src/common/config.h`**. Ganz oben stehen zwei Abschnitte: **WLAN** und
**Feste IP-Adressen**.

Dort sind schon Konfigurationen anderer Netze vorhanden. Die aktive Konfiguration ist die
ohne `//` am Zeilenanfang. **Setz vor jede Zeile der bisher aktiven Konfiguration zwei
Schrägstriche `//`**, dann ist sie abgeschaltet. Darunter fügst du deine eigene ein:

```cpp
// Mein Netz:
#define WIFI_SSID     "NAME-DEINES-WLANS"
#define WIFI_PASSWORD "PASSWORT-DEINES-WLANS"
```

```cpp
// Mein Netz:
#define IP_CASHDESK    IPAddress(192, 168, 1, 201)   // Pico 1, die Kasse
#define IP_BARRIERS    IPAddress(192, 168, 1, 202)   // Pico 2, die Schranke
#define IP_GATEWAY     IPAddress(192, 168, 1, 1)     // dein Router
#define IP_SUBNET_MASK IPAddress(255, 255, 255, 0)   // deine Subnetzmaske
```

Die Zahlen sind nur ein Beispiel. Setz deine eigenen aus 3.2 und 3.3 ein.

Achte auf diese Punkte:

- Name und Passwort stehen in **Anführungszeichen**, Gross- und Kleinschreibung muss genau
  stimmen, auch Leerzeichen im Namen.
- Bei den Adressen stehen **Kommas** statt Punkte: `192.168.1.201` wird zu
  `IPAddress(192, 168, 1, 201)`.
- Von jeder der sechs Zeilen darf es **nur eine aktive** geben, also ohne `//` davor. Sonst
  bricht das Bauen mit einer Fehlermeldung ab.
- Die Datei danach speichern (**Strg + S**).

### 3.5 Deine beiden Adressen

Alles Weitere in dieser Anleitung bezieht sich auf die beiden Adressen, die du eben
eingetragen hast. Ich schreibe dafür:

| Platzhalter | bedeutet | im Beispiel |
| --- | --- | --- |
| `<IP-KASSE>` | was du bei `IP_CASHDESK` eingetragen hast | `192.168.1.201` |
| `<IP-SCHRANKE>` | was du bei `IP_BARRIERS` eingetragen hast | `192.168.1.202` |

Wo also `http://<IP-KASSE>` steht, tippst du zum Beispiel `http://192.168.1.201` in den
Browser.

**Das Passwort steht jetzt im Code.** Wenn du das Projekt mit Git hochlädst oder
weitergibst, kann es jeder lesen, der Zugriff hat. Entferne es vorher wieder.

---

## 4. Die Firmware bauen

Aus dem Projekt entstehen zwei Programme (Firmwares), eines für jeden Pico.

**Weg A, mit der Maus:** Links in VS Code auf das PlatformIO-Symbol (der Ameisenkopf)
klicken. Unter **Project Tasks** bei **pico1_cashdesk** auf **Build** klicken, danach bei
**pico2_barriers** ebenfalls auf **Build**.

**Weg B, im Terminal** (Terminal, Neues Terminal):

```powershell
& "$env:USERPROFILE\.platformio\penv\Scripts\pio.exe" run
```

In beiden Fällen musst du warten, bis **SUCCESS** erscheint, bei Weg B zweimal:

```text
Environment     Status    Duration
--------------  --------  ------------
pico1_cashdesk  SUCCESS   00:00:23.077
pico2_barriers  SUCCESS   00:00:09.004
```

Danach liegen die beiden fertigen Dateien hier:

| Datei | gehört auf |
| --- | --- |
| `.pio\build\pico1_cashdesk\firmware.uf2` | den Pico **ohne** Servo (Kasse) |
| `.pio\build\pico2_barriers\firmware.uf2` | den Pico **mit** Servo (Schranke) |

Steht dort `FAILED`, ist meistens in `config.h` etwas falsch eingetragen: ein fehlendes
Anführungszeichen oder eine Zeile doppelt aktiv.

---

## 5. Die Firmware auf die Picos spielen

**Zieh zuerst beide Picos ab.** Du schliesst sie einzeln an, damit nichts verwechselt wird.

### 5.1 Zuerst die Kasse (Pico ohne Servo)

1. Den **weissen Knopf BOOTSEL** auf dem Pico gedrückt halten.
2. Knopf gedrückt lassen und das **USB-Kabel einstecken**. Dann loslassen.
3. Im Explorer erscheint ein neues Laufwerk **RPI-RP2**, meistens als `D:`. Merk dir den
   Buchstaben.
4. Im Terminal, mit deinem Laufwerksbuchstaben:
   ```powershell
   Copy-Item .pio\build\pico1_cashdesk\firmware.uf2 D:\
   ```
   Oder im Explorer die Datei aus `.pio\build\pico1_cashdesk` auf das Laufwerk ziehen.
5. Das Laufwerk verschwindet nach ein paar Sekunden von selbst. Der Pico hat die Firmware
   übernommen und startet.
6. **Den Pico stecken lassen.**

### 5.2 Danach die Schranke (Pico mit Servo)

1. **BOOTSEL** am Pico mit dem Servo gedrückt halten, USB einstecken, loslassen.
2. Wieder erscheint **RPI-RP2**, eventuell mit einem anderen Buchstaben.
3. Im Terminal, mit dem richtigen Buchstaben:
   ```powershell
   Copy-Item .pio\build\pico2_barriers\firmware.uf2 D:\
   ```
4. Das Laufwerk verschwindet, der Pico startet. Dabei kann der Arm kurz in die geschlossene
   Stellung fahren.

**Ab jetzt nie mehr BOOTSEL drücken**, ausser du willst eine neue Firmware aufspielen.
Steckst du einen Pico mit gedrücktem Knopf ein, startet er nicht sein Programm, sondern
wartet als Laufwerk auf eine neue Firmware.

---

## 6. Prüfen, ob beide Picos im WLAN sind

### 6.1 Die LED

Auf jedem Pico sitzt neben der USB-Buchse eine kleine LED. Sie leuchtet, **sobald der Pico
im WLAN ist**. Das dauert nach dem Einstecken etwa 5 bis 15 Sekunden.

- Beide LEDs leuchten: weiter mit Schritt 7.
- Eine LED bleibt aus: weiter mit 6.2.

### 6.2 Die Meldungen des Pico lesen

Ein Pico schreibt beim Start über das USB-Kabel, was er tut. So liest du es:

1. **Nur den betroffenen Pico** eingesteckt lassen.
2. Im Terminal:

   ```powershell
   & "$env:USERPROFILE\.platformio\penv\Scripts\pio.exe" device monitor
   ```

3. Bis zu 15 Sekunden warten. Ist der Pico nicht im WLAN, schreibt er in diesem Abstand
   `[WLAN] Neuer Verbindungsversuch. Letzter Stand: ...` und dahinter den Grund.
4. Beenden mit **Strg + C**.

Willst du auch die komplette Startmeldung mit der Liste aller WLANs sehen: Monitor beenden,
den Pico abziehen, **ohne Knopf** wieder einstecken und den Befehl aus Punkt 2 sofort
danach starten.

Wenn alles stimmt, sieht der Start so aus:

```text
[WLAN] Suche Netze in der Umgebung ...
  --> NAME-DEINES-WLANS  Signal -55 dBm  Kanal 6  WPA2
[WLAN] Verbinde mit NAME-DEINES-WLANS
[WLAN] Verbunden. Weboberflaeche: http://<IP-SCHRANKE>
```

Der Pico listet alle WLANs auf, die er sieht, und markiert deines mit einem Pfeil. Wenn
nicht, steht dort der Grund:

| Meldung | Bedeutung |
| --- | --- |
| `ACHTUNG: ... ist nicht in der Liste` | Der Pico sieht dein WLAN nicht. Meistens funkt es nur auf 5 GHz. Nimm den 2,4-GHz-Namen. |
| `Netz nicht gefunden` | dasselbe |
| `Anmeldung abgelehnt` | Passwort falsch, oder das Netz verlangt WPA3 |
| `Anmeldung fehlgeschlagen` | Der Router weist den Pico ab |

---

## 7. Der Servo-Test

Der Servo-Test prüft, ob der Arm sich bewegt, **bevor** du das Parkhaus benutzt. Er fährt
die Schranke einmal auf und wieder zu, ohne dass ein Auto erfasst wird. So weisst du sicher,
dass Verkabelung und Firmware stimmen.

### 7.1 Durchführen

1. Im Browser öffnen: **`http://<IP-SCHRANKE>`**
2. Ganz nach unten scrollen bis zum Kasten **„Smoke-Test der Servos (nur im Test-Build)"**.
3. Auf **„Servo testen"** klicken.

### 7.2 Was passieren muss

| Wo | Was du siehst |
| --- | --- |
| auf der Seite, im Kasten Einfahrt | grüne Meldung: „Servo an GP8 öffnet die Schranke und schliesst sie nach 5 Sekunden wieder." |
| auf der Seite, beim Schrankenarm | „offen", darunter ein grüner Balken, der in 5 Sekunden abläuft |
| **am Servo** | **der rote Arm geht hoch**, bleibt 5 Sekunden oben und **geht wieder runter** |
| danach auf der Seite | „geschlossen" |

Den Test kannst du auch ohne Knopf starten, indem du diese Adresse direkt aufrufst:
`http://<IP-SCHRANKE>/api/servotest`

### 7.3 Wenn es nicht klappt

| Was passiert | Ursache und Lösung |
| --- | --- |
| Die Seite lädt nicht | Netzwerkproblem, siehe Schritt 6 und die Fehlertabelle unten |
| Den Kasten „Smoke-Test" gibt es nicht | Die Firmware wurde ohne Testwerkzeuge gebaut. In `platformio.ini` muss bei `pico2_barriers` `-DTEST_BUILD=1` stehen. |
| Grüne Meldung, der Balken läuft, **aber der Arm bewegt sich nicht** | Die Software arbeitet, das Signal kommt nicht an. Entweder sind die beiden Picos **vertauscht** (die Schranken-Firmware ist auf dem Pico ohne Servo), oder die **Kabel** stecken falsch. Prüf die Tabelle in Schritt 2 und ob der Pico wirklich mit seinen Beinchen im Steckbrett steckt. |
| Der Pico startet neu, sobald der Arm loslegt | Das rote Kabel steckt am falschen Pin. Es muss auf Pin 40 (VBUS), nicht auf einen 3,3-Volt-Pin. |
| Der Arm fährt **falsch herum** (öffnet, wenn er schliessen soll) | In `src/common/config.h` die Zahlen bei `SERVO_ANGLE_CLOSED` und `SERVO_ANGLE_OPEN` tauschen, neu bauen und `pico2_barriers` aufspielen. |
| Der Arm steht schief | Mechanik: Schraube am Arm lösen, Arm abziehen und bei geschlossener Schranke waagrecht wieder aufstecken. |

Sind die Picos vertauscht, spielst du einfach die Firmwares über Kreuz neu auf (Schritt 5).

### 7.4 Hardware-Test ohne Webseite und ohne WLAN

Bewegt sich der Arm auf der Webseite nicht, prüfst du die Hardware mit einem eigenen
kleinen Testprogramm. Es fährt den Arm nur jede Sekunde hin und her, sonst nichts. Die LED
blinkt im selben Takt.

1. Bauen, im Terminal:

   ```powershell
   & "$env:USERPROFILE\.platformio\penv\Scripts\pio.exe" run -e servo_test
   ```

2. Den Pico mit Servo mit **BOOTSEL** einstecken und die Datei
   `.pio\build\servo_test\firmware.uf2` auf das Laufwerk **RPI-RP2** kopieren.
3. Hinschauen: Der Arm muss jede Sekunde die Stellung wechseln.

| Ergebnis | Bedeutung |
| --- | --- |
| Arm bewegt sich | Verkabelung stimmt. Dann `pico2_barriers` wieder aufspielen. |
| Arm bewegt sich nicht | Den Test mit `servo_test_all_pins` statt `servo_test` wiederholen (Datei dann aus `.pio\build\servo_test_all_pins`). Dieser schickt das Signal auf **alle** Pins. |
| nur bei `servo_test_all_pins` bewegt er sich | Das **orange** Kabel steckt an einem anderen Pin als in `config.h` eingetragen. Es gehört auf **Pin 11 (GP8)**, oder du trägst bei `SERVO_PIN_ENTRY` die GPIO-Nummer des Pins ein, an dem es steckt. |
| bei keinem bewegt er sich | Der Servo hat keinen Strom: **rot** und **braun** prüfen, und ob der Pico fest im Steckbrett steckt. |

**Danach immer die richtige Firmware wieder aufspielen** (`pico2_barriers` auf den Pico
mit Servo). Sonst läuft weiterhin nur der Test, und die Webseite ist nicht erreichbar.

---

## 8. Das ganze Projekt ausführen

### 8.1 Die beiden Seiten öffnen

1. **Zuerst die Kasse** öffnen: **`http://<IP-KASSE>`**
   Beim Öffnen stellt dein Browser automatisch die Uhr des Pico. Der Pico hat keine eigene
   Uhr. Öffnest du die Kasse nicht zuerst, stehen bei den Zeiten nur zwei Striche.
2. Dann die **Schranke** in einem zweiten Tab: **`http://<IP-SCHRANKE>`**
   Oben muss **„Kasse erreichbar"** mit einem grünen Punkt stehen. Steht dort „Kasse nicht
   erreichbar", warte 10 Sekunden. Die Schranke fragt in diesem Abstand nach.

Am besten legst du beide Tabs nebeneinander.

### 8.2 Der normale Ablauf

| Schritt | Wo | Was du tust | Was passieren muss |
| --- | --- | --- | --- |
| 1 | Schranke | bei **Einfahrt** `LU 12345` eintippen, „Einfahrt bestätigen" | grüne Meldung, **der Arm geht hoch** und nach 5 Sekunden runter |
| 2 | Kasse | nichts, nur hinschauen | `LU 12345` erscheint unter „Fahrzeuge im Parkhaus" mit Einfahrtszeit, Belegung `1 / 3` |
| 3 | Kasse | `LU 12345` suchen | Einfahrt, Parkdauer und Gebühr `CHF 2.00`, orange Meldung „Offener Betrag" |
| 4 | Kasse | „Bezahlen" | grüne Meldung „Bezahlung erfolgt. Betrag: CHF 2.00." |
| 5 | Schranke | bei **Ausfahrt** `LU 12345` eintippen, „Ausfahrt bestätigen" | grüne Meldung, **der Arm geht hoch** |
| 6 | Kasse | nichts | `LU 12345` steht jetzt unter „Abgeschlossene Vorgänge", Belegung `0 / 3` |

Mit einem einzigen Servo ist der Arm für Einfahrt und Ausfahrt derselbe. Die Seite der
Schranke zeigt ihn deshalb als „Schrankenarm (gemeinsam)".

### 8.3 Fälle, in denen die Schranke zubleiben muss

Probier diese Fälle aus. Bei allen bleibt der Arm unten:

| Was du tust | Erwartete Meldung |
| --- | --- |
| Einfahrt mit `12345` | „Das ist kein gültiges Kennzeichen. Erwartet wird zum Beispiel LU 12345." |
| Einfahrt mit leerem Feld | „Bitte ein Kennzeichen eingeben." |
| Einfahrt mit einem Auto, das schon drin ist | „Dieses Fahrzeug steht bereits im Parkhaus." |
| vier verschiedene Autos einfahren lassen, z. B. `LU 1`, `LU 2`, `LU 3`, `LU 4` | beim vierten: „Das Parkhaus ist voll. Keine weitere Einfahrt möglich." |
| Ausfahrt **vor** dem Bezahlen | „Die Parkgebühr für ... ist noch offen (CHF 2.00). Bitte zuerst an der Kasse bezahlen." |
| unter `BE 55511` einfahren und bezahlen, dann Ausfahrt mit `BE 55512` | „Zu dem Kennzeichen BE 55512 ist keine Einfahrt bekannt." Mit `BE 55511` geht die Schranke danach auf. |
| Kassen-Pico abziehen, dann eine Ausfahrt versuchen | „Die Kasse ist zurzeit nicht erreichbar. ... die Schranke bleibt geschlossen." |

Kleinschreibung und ein fehlendes Leerzeichen sind erlaubt: `lu12345` wird automatisch zu
`LU 12345`.

### 8.4 Die Gebühr testen, ohne eine Stunde zu warten

Der Tarif ist **CHF 2.00 pro angefangene Stunde**. Damit du nicht 61 Minuten warten musst,
hat die Kasse unten den Kasten **„Testwerkzeuge (nur im Test-Build)"**:

1. Ein Auto einfahren lassen, zum Beispiel `BE 55511`.
2. An der Kasse unten im Kasten das Kennzeichen eintragen, daneben die Minuten, zum Beispiel
   `61`.
3. **„Einfahrt zurücksetzen"** klicken. Die Kasse tut jetzt so, als wäre das Auto vor
   61 Minuten eingefahren.
4. Das Kennzeichen oben suchen.

| Minuten | erwartete Gebühr |
| --- | --- |
| 1 | CHF 2.00 |
| 60 | CHF 2.00 |
| 61 | CHF 4.00 |
| 121 | CHF 6.00 |

Mit **„Alle Vorgänge löschen"** fängst du wieder bei einem leeren Parkhaus an.

### 8.5 Im Alltag

- Die Picos brauchen nur **Strom über USB**. Laptop oder Handy-Ladegerät reicht. Sie starten
  ihr Programm von selbst.
- Nach einem Neustart der Kasse ist das Parkhaus leer. Alles liegt im Arbeitsspeicher.
- Änderst du etwas in `config.h`, musst du **beide** Firmwares neu bauen und auf **beide**
  Picos spielen.
- Wechselst du das WLAN (zum Beispiel von zuhause zur Schule), wiederholst du Schritt 3
  bis 6.

---

## Fehlerbehebung

| Problem | Ursache | Lösung |
| --- | --- | --- |
| Beim Bauen steht `FAILED` | Tippfehler in `config.h` | Anführungszeichen, Kommas und doppelt aktive Zeilen prüfen |
| Im Terminal: Befehl `pio` nicht gefunden | PlatformIO ist nicht im Suchpfad | den langen Befehl mit `$env:USERPROFILE` aus Schritt 4 verwenden, oder Weg A |
| Kein Laufwerk RPI-RP2 erscheint | Knopf zu früh losgelassen, oder ein Ladekabel ohne Datenleitung | Knopf gedrückt halten **während** du einsteckst, anderes USB-Kabel probieren |
| Am Laptop erscheint ein Laufwerk RPI-RP2, obwohl du nichts flashen willst | Knopf beim Einstecken gedrückt | abziehen, ohne Knopf neu einstecken |
| LED bleibt aus | Pico kommt nicht ins WLAN | Meldungen lesen, Schritt 6.2 |
| LED leuchtet, Seite lädt nicht | falsche Adresse, Laptop in einem anderen Netz, oder das WLAN verbietet Verbindungen zwischen Geräten | Adresse in den Meldungen prüfen, Laptop ins selbe Netz, sonst Hotspot verwenden (3.1) |
| „Kasse nicht erreichbar" auf der Seite der Schranke | Kassen-Pico aus, nicht im WLAN, oder `IP_CASHDESK` stimmt nicht | LED der Kasse prüfen, `http://<IP-KASSE>` im Browser öffnen |
| Zeiten werden als `--` angezeigt | Uhr nicht gestellt | die Seite der Kasse einmal öffnen |
| Arm bewegt sich nicht | siehe 7.3 | |

Mehr Hintergrund zum Projekt steht im [README](README.md). Eine ausführliche Erklärung, wie
alles im Code funktioniert, findest du in [docs/Projekt-Erklaerung.md](docs/Projekt-Erklaerung.md).
