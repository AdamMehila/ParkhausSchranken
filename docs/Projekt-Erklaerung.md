# Das Parkhaus-Projekt verstehen

Dieses Dokument erklärt das ganze Projekt von Anfang bis Ende: worum es geht, wie die
Geräte miteinander reden und was jede einzelne Zeile Code macht.

Es ist für jemanden geschrieben, der noch nie C++ programmiert hat. Jedes Fachwort und
jedes Sprachmittel wird genau dort erklärt, wo es zum ersten Mal auftaucht. Du musst
also nichts vorher wissen. Lies einfach von oben nach unten und halte den Code in
VS Code daneben offen.

**So ist das Dokument aufgebaut:**

1. **Worum es geht.** Was das System macht, welche Geräte beteiligt sind und welche
   Regeln gelten. Noch ganz ohne Code.
2. **Wie die Geräte miteinander reden.** WLAN, Adressen, Webseiten, Anfragen und
   Antworten. Am Ende weisst du genau, welche Nachricht wann von wo nach wo geht.
3. **Der Code, Datei für Datei.** In einer Reihenfolge, in der jede Datei nur auf
   Dingen aufbaut, die du schon kennst.
4. **Eine Fahrt durch den ganzen Code.** Wir verfolgen ein Auto von der Einfahrt bis
   zur Ausfahrt durch alle Funktionen. Hier fügt sich alles zusammen.

## Inhalt

- [Teil 1: Worum es geht](#teil-1-worum-es-geht)
  - [1.1 Die Aufgabe](#11-die-aufgabe)
  - [1.2 Eine Geschichte: Ein Auto im Parkhaus](#12-eine-geschichte-ein-auto-im-parkhaus)
  - [1.3 Die Geräte](#13-die-geräte)
  - [1.4 Warum zwei Picos?](#14-warum-zwei-picos)
  - [1.5 Die Regeln](#15-die-regeln)
  - [1.6 Vom Code zum laufenden Pico](#16-vom-code-zum-laufenden-pico)
- [Teil 2: Wie die Geräte miteinander reden](#teil-2-wie-die-geräte-miteinander-reden)
  - [2.1 Das Netz: WLAN und IP-Adressen](#21-das-netz-wlan-und-ip-adressen)
  - [2.2 Fragen und Antworten: HTTP](#22-fragen-und-antworten-http)
  - [2.3 Die Sprache der Antworten: JSON](#23-die-sprache-der-antworten-json)
  - [2.4 Wer redet mit wem?](#24-wer-redet-mit-wem)
  - [2.5 Alle Gespräche Schritt für Schritt](#25-alle-gespräche-schritt-für-schritt)
  - [2.6 Alle Adressen auf einen Blick](#26-alle-adressen-auf-einen-blick)
  - [2.7 Der Draht zum Laptop: die serielle Ausgabe](#27-der-draht-zum-laptop-die-serielle-ausgabe)
- [Teil 3: Der Code, Datei für Datei](#teil-3-der-code-datei-für-datei)
  - [3.0 Bevor es losgeht](#30-bevor-es-losgeht)
  - [3.1 platformio.ini: Die Bauanleitung](#31-platformioini-die-bauanleitung)
  - [3.2 config.h: Alle Einstellungen](#32-configh-alle-einstellungen)
  - [3.3 license_plate: Kennzeichen prüfen](#33-license_plate-kennzeichen-prüfen)
  - [3.4 clock_time: Zeit messen und anzeigen](#34-clock_time-zeit-messen-und-anzeigen)
  - [3.5 json_text.h: Text sicher verpacken](#35-json_texth-text-sicher-verpacken)
  - [3.6 network: Ins WLAN kommen und drin bleiben](#36-network-ins-wlan-kommen-und-drin-bleiben)
  - [3.7 parking_garage: Das Gedächtnis der Kasse](#37-parking_garage-das-gedächtnis-der-kasse)
  - [3.8 web_page_cashdesk.h: Die Webseite der Kasse](#38-web_page_cashdeskh-die-webseite-der-kasse)
  - [3.9 cashdesk/main.cpp: Das Hauptprogramm der Kasse](#39-cashdeskmaincpp-das-hauptprogramm-der-kasse)
  - [3.10 barrier: Die Schranke als Objekt](#310-barrier-die-schranke-als-objekt)
  - [3.11 cashdesk_client: Pico 2 ruft die Kasse an](#311-cashdesk_client-pico-2-ruft-die-kasse-an)
  - [3.12 web_page_barriers.h: Die Webseite der Schranke](#312-web_page_barriersh-die-webseite-der-schranke)
  - [3.13 barriers/main.cpp: Das Hauptprogramm der Schranke](#313-barriersmaincpp-das-hauptprogramm-der-schranke)
- [Teil 4: Eine Fahrt durch den ganzen Code](#teil-4-eine-fahrt-durch-den-ganzen-code)
- [Anhang A: Begriffe von A bis Z](#anhang-a-begriffe-von-a-bis-z)
- [Anhang B: Wo ändere ich was?](#anhang-b-wo-ändere-ich-was)

---

## Teil 1: Worum es geht

### 1.1 Die Aufgabe

Im Modul M450 lautet der Auftrag A01-03: **Baue ein Parkhaus mit Schranke** auf zwei
Raspberry Pi Pico W. Das System soll:

- ein Auto bei der Einfahrt erfassen und sich merken, **wann** es reingefahren ist,
- die Schranke öffnen und nach 5 Sekunden von selbst wieder schliessen,
- ungültige Kennzeichen und ein volles Parkhaus abweisen,
- die **Parkgebühr** aus der Parkdauer berechnen,
- an einer **Kasse** die Bezahlung entgegennehmen,
- das Auto nur dann wieder hinauslassen, wenn es **bezahlt** hat.

Es gibt keine echte Kamera, die Kennzeichen liest. Stattdessen tippt man das
Kennzeichen auf einer Webseite ein. Für das Programm macht das keinen Unterschied: Ob
der Text `LU 12345` von einer Kamera kommt oder von einer Tastatur, ist für alles
Weitere egal.

### 1.2 Eine Geschichte: Ein Auto im Parkhaus

Bevor wir auf Technik schauen, spielen wir einmal durch, was ein Benutzer erlebt.
Anna fährt mit dem Auto `LU 12345` vor das Parkhaus.

1. **Einfahrt.** An der Einfahrt steht eine Webseite (die von Pico 2). Dort wird
   `LU 12345` eingetippt und „Einfahrt bestätigen" gedrückt.
2. **Prüfung.** Pico 2 schaut zuerst selbst, ob das überhaupt wie ein Kennzeichen
   aussieht. Dann fragt er die Kasse (Pico 1): „Darf `LU 12345` rein?" Die Kasse
   prüft, ob noch ein Platz frei ist und ob dieses Auto nicht schon drinsteht. Wenn
   alles passt, notiert sie das Kennzeichen mit der aktuellen Uhrzeit und sagt „Ja".
3. **Schranke auf.** Pico 2 fährt den Schrankenarm hoch. Nach 5 Sekunden fährt er
   von selbst wieder runter.
4. **Parken.** Anna kauft ein. Die Zeit läuft.
5. **Bezahlen.** Vor dem Gehen sucht Anna an der Kasse (Webseite von Pico 1) ihr
   Kennzeichen. Die Kasse zeigt Einfahrtszeit, Parkdauer und Gebühr an, zum Beispiel
   `CHF 4.00` für 1 Stunde und 20 Minuten. Anna drückt „Bezahlen".
6. **Ausfahrt.** An der Ausfahrt wird `LU 12345` eingetippt. Pico 2 fragt die Kasse:
   „Hat `LU 12345` bezahlt?" Die Kasse sagt „Ja". Pico 2 meldet der Kasse, dass das
   Auto jetzt geht (der Platz wird frei), und öffnet dann die Schranke.

Hätte Anna nicht bezahlt, wäre bei Schritt 6 eine Meldung erschienen: „Die Parkgebühr
für LU 12345 ist noch offen (CHF 4.00). Bitte zuerst an der Kasse bezahlen." Die
Schranke wäre zugeblieben.

Diese Geschichte ist der rote Faden für alles Weitere. Jedes Stück Code, das du später
siehst, gehört zu einem dieser sechs Schritte.

### 1.3 Die Geräte

#### Der Raspberry Pi Pico W

Ein Pico W ist ein **Mikrocontroller**: ein winziger Computer auf einer Platine, etwa
so gross wie ein Kaugummistreifen. Er unterscheidet sich stark von einem Laptop:

| | Laptop | Pico W |
|---|---|---|
| Bildschirm, Tastatur | ja | nein |
| Betriebssystem (Windows) | ja | **nein**, nur unser eigenes Programm |
| Arbeitsspeicher | etwa 16 000 000 KB | **264 KB** |
| Speicher für Programme | etwa 1 000 000 000 KB | **2 048 KB** |
| Rechentakt | etwa 3 000 MHz | **133 MHz** |
| WLAN | ja | ja, aber **nur 2,4 GHz** |
| Anschlüsse für Elektronik | nein | **ja**, 26 sogenannte GPIO-Pins |

Weil er keinen Bildschirm hat, zeigt der Pico seine Oberfläche **als Webseite** an.
Man öffnet sie im Browser des Laptops. Der Laptop ist also nur das Display, die ganze
Logik läuft auf den Picos.

**GPIO** heisst „General Purpose Input/Output", also Allzweck-Ein- und Ausgang. Das
sind die Metallbeinchen an den Rändern der Platine. Das Programm kann auf jedem davon
Strom ein- und ausschalten oder messen, ob dort Strom anliegt. So steuert der Pico
Elektronik wie unseren Servo.

#### Die zwei Picos und ihre Rollen

| | Pico 1 | Pico 2 |
|---|---|---|
| Rolle | **die Kasse** | **die Schranke** |
| Weiss, welche Autos drin sind | **ja**, nur er | nein, fragt immer bei Pico 1 nach |
| Webseite zeigt | Suchen, Gebühr, Bezahlen, Fahrzeugliste | Felder für Einfahrt und Ausfahrt |
| Angeschlossen | nur USB-Kabel (Strom) | USB-Kabel und der Servo |
| Ordner im Code | `src/cashdesk/` | `src/barriers/` |

(„cashdesk" ist Englisch für Kasse, „barrier" für Schranke. Alle Namen im Code sind
englisch, die Erklärungen in den Kommentaren deutsch.)

#### Der Servo und die blaue Schranke

Das blaue Teil mit dem roten Arm enthält einen **SG90-Servo**. Ein Servo ist ein kleiner
Motor, dem man nicht sagt „dreh dich", sondern „**geh auf diesen Winkel**". Er fährt
dorthin und hält die Position.

Der Servo hat drei Adern:

| Ader | Bedeutung |
|---|---|
| braun | Masse (GND, der Minuspol) |
| rot | Strom, 5 Volt |
| orange | Signal: hier sagt der Pico, welchen Winkel er will |

Auf der Signalleitung schickt der Pico alle 20 Millisekunden einen kurzen Stromstoss.
Wie **lang** dieser Stoss ist, bestimmt den Winkel: etwa 1 Millisekunde bedeutet 0 Grad,
etwa 1,5 Millisekunden bedeuten 90 Grad. Das musst du nie selbst ausrechnen, das macht
eine fertige Bibliothek für uns. Im Code steht nur `servo.write(90)`.

In unserem Aufbau bedeuten:
- **90 Grad:** Arm liegt waagrecht, die Schranke ist **zu**.
- **0 Grad:** Arm steht senkrecht, die Schranke ist **offen**.

Welcher Winkel was bedeutet, hängt nur davon ab, wie der rote Arm auf die Achse des Servos
gesteckt ist. Im Testkonzept ist es genau umgekehrt. Darum stehen die beiden Winkel in
`config.h` und nicht fest im Code.

#### Die Verkabelung

Der Servo hängt an **Pico 2**. So sieht es auf dem Steckbrett aus:

![Verkabelung von Pico und SG90 auf dem Steckbrett](verkabelung.png)

| Servo-Ader | Pin am Pico | Name des Pins | Loch im Steckbrett |
|---|---|---|---|
| rot | Pin 40 | VBUS (5 Volt direkt vom USB-Kabel) | **J1** |
| braun | Pin 8 | GND (Masse) | **A8** |
| orange | Pin 11 | GP8 (Signal) | **A11** |

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

**Achtung, Verwechslungsgefahr:** Die **Pin-Nummer** zählt einfach die Beinchen auf der
Platine durch (1 bis 40). Die **GPIO-Nummer** ist der Name, unter dem das Programm einen
Anschluss kennt. Pin 11 heisst im Programm GP8 (GPIO 8). Im Code steht deshalb `8`,
beim Verkabeln zählst du aber bis 11.

Für das Signalkabel kommt fast jeder GPIO-Pin in Frage. Wichtig ist nur, dass im Code
dieselbe GPIO-Nummer steht, an der das Kabel wirklich steckt. Steht im Code eine andere
Nummer, schickt der Pico sein Signal ins Leere, und der Arm bewegt sich nicht, obwohl die
Webseite „offen" meldet. Genau das ist bei der Einrichtung passiert.

### 1.4 Warum zwei Picos?

Die Aufgabe schreibt zwei Geräte vor. Es ist aber auch eine sinnvolle Aufteilung, wie
sie in echten Parkhäusern vorkommt:

- Die **Kasse** ist die Buchhaltung. Sie allein weiss, welches Auto wann gekommen ist
  und ob es bezahlt hat.
- Die **Schranke** ist die Mechanik. Sie weiss nichts über Autos und Geld. Bei jeder
  Entscheidung fragt sie die Kasse.

Diese Regel hat einen Namen: **eine einzige Quelle der Wahrheit**. Würden beide Picos
eine eigene Liste der Autos führen, wären diese Listen früher oder später verschieden.
Etwa wenn eine Nachricht unterwegs verloren geht. Dann wüsste niemand mehr, welche
Liste stimmt. Mit nur einer Liste kann das nicht passieren.

### 1.5 Die Regeln

Diese Regeln stehen im Testkonzept und sind im Code genau so umgesetzt:

| Regel | Wert |
|---|---|
| Tarif | **CHF 2.00 pro angefangene Stunde** |
| Gratiszeit | keine. Schon 1 Minute kostet CHF 2.00 |
| Tagesmaximum | keines |
| Anzahl Parkplätze | **3** (absichtlich wenig, damit „voll" schnell testbar ist) |
| Offenhaltezeit der Schranke | **5 Sekunden** |
| Gültiges Kennzeichen | 2 Buchstaben, Leerzeichen, 1 bis 6 Ziffern, z. B. `LU 12345` |
| Höchstlänge | 10 Zeichen |
| Kleinbuchstaben, fehlendes Leerzeichen | werden korrigiert: `lu12345` wird zu `LU 12345` |
| Doppelt bezahlen | nicht möglich, die Kasse meldet „bereits bezahlt" |
| Betrag nach dem Bezahlen | bleibt eingefroren, auch wenn die Zeit weiterläuft |
| Ausfahrt | nur, wenn die Kasse „bezahlt" meldet |

Was „angefangene Stunde" heisst, zeigt diese Tabelle:

| Parkdauer | Gebühr |
|---|---|
| 1 Minute | CHF 2.00 |
| 60 Minuten | CHF 2.00 |
| 61 Minuten | CHF 4.00 |
| 2 Stunden | CHF 4.00 |
| 2 Stunden 1 Minute | CHF 6.00 |

### 1.6 Vom Code zum laufenden Pico

Der Code, den du in VS Code siehst, ist **Quellcode**: Text, den Menschen lesen können.
Der Pico versteht diesen Text nicht. Er versteht nur Maschinenbefehle, also Zahlen.
Dazwischen liegen drei Schritte:

```text
  Quellcode (.cpp, .h)
        |
        |  1. bauen (kompilieren): PlatformIO übersetzt den Text in Maschinenbefehle
        v
  firmware.uf2   (die fertige "Firmware", eine einzige Datei)
        |
        |  2. flashen: die Datei auf den Pico übertragen
        v
  Pico speichert sie dauerhaft
        |
        |  3. Strom an: das Programm startet von selbst
        v
  Pico läuft
```

Das Wort **Firmware** bedeutet: das Programm, das fest auf einem Gerät liegt. Der Pico
behält es auch ohne Strom.

Aus **einem** Projekt entstehen **zwei** Firmwares: eine für die Kasse, eine für die
Schranke. Sie teilen sich manche Dateien (den Ordner `common`), aber jede hat ihr
eigenes Hauptprogramm. Wie das funktioniert, steht in [Abschnitt 3.1](#31-platformioini-die-bauanleitung).

**Flashen** geht so: Man hält den weissen Knopf **BOOTSEL** auf dem Pico gedrückt und
steckt dabei das USB-Kabel ein. Der Pico meldet sich dann am Laptop wie ein USB-Stick
mit dem Namen `RPI-RP2`. Kopiert man die Datei `firmware.uf2` darauf, übernimmt der
Pico sie und startet neu mit dem neuen Programm.

---

## Teil 2: Wie die Geräte miteinander reden

### 2.1 Das Netz: WLAN und IP-Adressen

Alle drei Geräte (Laptop, Pico 1, Pico 2) sind im **selben WLAN**. Darüber reden sie
miteinander.

#### Nur 2,4 GHz

WLAN funkt auf zwei Frequenzbereichen: **2,4 GHz** und **5 GHz**. Der Funkchip des Pico
kann **nur 2,4 GHz**. Viele Router geben den beiden Bereichen eigene Namen, zum Beispiel
`MeinWLAN` (5 GHz) und `MeinWLAN-2.4GHz` (2,4 GHz). Der Pico muss den
2,4-GHz-Namen bekommen, sonst findet er das Netz gar nicht.

Der Laptop darf dabei ruhig im 5-GHz-Netz bleiben. Beide Namen gehören zum selben
Router und zum selben Heimnetz. Die Geräte können sich gegenseitig erreichen, egal über
welchen Namen sie verbunden sind.

#### IP-Adressen

Damit sich Geräte im Netz finden, hat jedes eine **IP-Adresse**. Das ist wie eine
Hausnummer: vier Zahlen, durch Punkte getrennt.

| Gerät | IP-Adresse |
|---|---|
| Router | `192.168.88.1` |
| Pico 1, Kasse | `192.168.89.211` |
| Pico 2, Schranke | `192.168.89.212` |
| Laptop | bekommt eine vom Router, zum Beispiel `192.168.88.248` |

Normalerweise verteilt der Router die Adressen selbst, und ein Gerät kann nach jedem
Neustart eine andere bekommen. Bei den Picos wollen wir das nicht: Pico 2 muss die Kasse
**immer unter derselben Adresse** finden. Deshalb geben sich die Picos ihre Adresse
selbst. Das nennt man **feste** oder **statische IP-Adresse**. Sie steht in der Datei
`config.h`.

Dazu gehören noch zwei Angaben:

- **Gateway:** die Adresse des Routers. An ihn schickt ein Gerät alles, was nicht im
  eigenen Netz liegt. Bei uns `192.168.88.1`.
- **Subnetzmaske:** sagt, welche Adressen zum eigenen Netz gehören. `255.255.254.0`
  bedeutet hier: alles von `192.168.88.0` bis `192.168.89.255`. Unsere Picos liegen mit
  `.89.211` und `.89.212` also im gleichen Netz wie der Router.

Die Adressen `.211` und `.212` sind frei gewählt. Wichtig ist nur, dass kein anderes
Gerät sie schon benutzt.

### 2.2 Fragen und Antworten: HTTP

Die Geräte reden miteinander über **HTTP**. Das ist dieselbe Technik, mit der dein
Browser Webseiten aus dem Internet lädt. Das Prinzip ist immer gleich:

- Einer **fragt** (der **Client**).
- Einer **antwortet** (der **Server**).

Beide Picos sind **Webserver**: Sie warten auf Fragen und beantworten sie. Der Browser
ist ein Client. Und auch Pico 2 ist ein Client, wenn er die Kasse etwas fragt.

#### Eine Adresse auseinandergenommen

So sieht eine typische Frage aus, als Adresse im Browser geschrieben:

```text
http://192.168.89.211/api/status?plate=LU%2012345
└─┬─┘ └──────┬──────┘└────┬────┘└───────┬───────┘
  │          │            │             └── Parameter: plate hat den Wert "LU 12345"
  │          │            └── Pfad: welche Funktion auf dem Pico gemeint ist
  │          └── wer gefragt wird: die Kasse
  └── die Technik: HTTP
```

- Der **Pfad** wählt aus, was der Pico tun soll. Jeder Pfad gehört zu einer Funktion im
  Code. So einen Pfad nennt man **Endpunkt**.
- Nach dem **Fragezeichen** kommen **Parameter**, also Zusatzangaben. Mehrere werden mit
  `&` getrennt: `?plate=LU%2012345&minutes=61`.
- `%20` ist ein **Leerzeichen**. In einer Webadresse dürfen keine Leerzeichen stehen,
  deshalb werden sie umgeschrieben. Aus `LU 12345` wird `LU%2012345`.

#### Was genau übertragen wird

Hinter den Kulissen schickt der Client einen kurzen Text, die **Anfrage**:

```text
GET /api/status?plate=LU%2012345 HTTP/1.1
Host: 192.168.89.211
```

`GET` heisst „gib mir". Der Server schickt eine **Antwort** zurück:

```text
HTTP/1.1 200 OK
Content-Type: application/json; charset=utf-8
Cache-Control: no-store

{"plate":"LU 12345","status":"paid","fee":"CHF 4.00", ...}
```

Die erste Zeile enthält einen **Statuscode**. Wir benutzen drei:

| Code | Bedeutung | Wann bei uns |
|---|---|---|
| 200 | alles in Ordnung | fast immer |
| 400 | deine Anfrage ist kaputt | Uhrzeit fehlt oder ist unsinnig |
| 404 | diesen Pfad gibt es nicht | Tippfehler in der Adresse |

Wichtig: Code 200 heisst nur „die Frage ist angekommen und wurde beantwortet". Ob die
**Antwort** „ja" oder „nein" ist, steht im Inhalt. Eine abgelehnte Einfahrt kommt also
mit Code 200 und `"ok":false` zurück.

`Cache-Control: no-store` bittet den Browser, die Antwort **nicht** zu speichern. Sonst
würde er beim nächsten Mal die alte Belegung anzeigen, ohne neu zu fragen.

### 2.3 Die Sprache der Antworten: JSON

Die Antworten sind im Format **JSON** geschrieben. Das ist eine einfache Schreibweise für
Daten, die Menschen und Programme gleichermassen lesen können:

```json
{
  "ok": true,
  "plate": "LU 12345",
  "occupied": 1,
  "totalSpots": 3,
  "vehicles": [ { "plate": "LU 12345", "fee": "CHF 2.00" } ]
}
```

Die Regeln sind wenige:

- `{ }` umschliesst ein Objekt, also eine Sammlung von Feldern.
- Jedes Feld hat einen **Namen** in Anführungszeichen, einen Doppelpunkt und einen
  **Wert**. Felder werden mit Kommas getrennt.
- **Text** steht in Anführungszeichen: `"LU 12345"`.
- **Zahlen** und `true` / `false` stehen **ohne** Anführungszeichen.
- `[ ]` umschliesst eine **Liste**.

Weil Text in Anführungszeichen steht, wird es heikel, wenn der Text selbst ein
Anführungszeichen enthält. Tippt jemand als Kennzeichen `"` ein und die Kasse schreibt
das ungeschützt zurück, entsteht kaputtes JSON. Wie wir das verhindern, steht in
[Abschnitt 3.5](#35-json_texth-text-sicher-verpacken).

### 2.4 Wer redet mit wem?

```text
   ┌─────────────────────────┐                       ┌─────────────────────────┐
   │  Browser am Laptop      │                       │  Browser am Laptop      │
   │  Seite der KASSE        │                       │  Seite der SCHRANKE     │
   └───────────┬─────────────┘                       └───────────┬─────────────┘
               │ alle 2 s: Fahrzeugliste                         │ jede 1 s: Zustand
               │ bei Klick: suchen, bezahlen                     │ bei Klick: Einfahrt, Ausfahrt
               v                                                 v
   ┌─────────────────────────┐   fragt bei Bedarf    ┌─────────────────────────┐
   │  PICO 1: KASSE          │ <──────────────────── │  PICO 2: SCHRANKE       │
   │  192.168.89.211         │   und alle 3 s die    │  192.168.89.212         │
   │  speichert alle Autos   │   Belegung            │  steuert den Servo      │
   └─────────────────────────┘                       └───────────┬─────────────┘
                                                                 │ Signal auf GP8    
                                                                 v
                                                          ┌──────────────┐
                                                          │  Servo / Arm │
                                                          └──────────────┘
```

Drei Dinge fallen auf:

1. **Pico 1 fragt nie Pico 2.** Die Kasse antwortet nur. Sie muss nichts über die
   Schranke wissen.
2. **Die Seite der Schranke redet nie direkt mit der Kasse.** Sie fragt immer Pico 2,
   und Pico 2 fragt bei Bedarf weiter.
3. **Pico 2 ist gleichzeitig Server und Client.** Server für seinen Browser, Client
   gegenüber der Kasse.

### 2.5 Alle Gespräche Schritt für Schritt

In den folgenden Ablaufdiagrammen läuft die Zeit von oben nach unten. Pfeile sind
Nachrichten.

#### a) Die Kassenseite wird geöffnet

```text
Browser (Kasse)                                   Pico 1 (Kasse)
     │  GET /                                          │
     │────────────────────────────────────────────────>│
     │  die Webseite (HTML, CSS, JavaScript)           │
     │<────────────────────────────────────────────────│
     │                                                 │
     │  GET /api/setclock?epoch=1792710000             │   Uhr stellen, nur beim
     │────────────────────────────────────────────────>│   allerersten Mal
     │  {"ok":true}                                    │
     │<────────────────────────────────────────────────│
     │                                                 │
     │  GET /api/vehicles      (danach alle 2 Sekunden)│
     │────────────────────────────────────────────────>│
     │  {"occupied":0,"totalSpots":3,"vehicles":[],...}│
     │<────────────────────────────────────────────────│
```

Der Pico hat keine Uhr mit Batterie. Nach dem Einschalten weiss er nicht, welches Datum
ist. Deshalb schickt ihm die Webseite einmal die Uhrzeit des Laptops. Die Zahl hinter
`epoch=` ist die Anzahl Sekunden seit dem 1. Januar 1970, so zählen Computer
üblicherweise die Zeit.

#### b) Einfahrt

```text
Browser (Schranke)          Pico 2 (Schranke)                    Pico 1 (Kasse)
     │                            │                                     │
     │ GET /api/entry?plate=lu12345                                     │
     │───────────────────────────>│                                     │
     │                            │ 1. Kennzeichen prüfen               │
     │                            │    "lu12345" wird "LU 12345"        │
     │                            │                                     │
     │                            │ GET /api/entry?plate=LU%2012345     │
     │                            │────────────────────────────────────>│
     │                            │                                     │ 2. nochmals prüfen
     │                            │                                     │ 3. schon drin? nein
     │                            │                                     │ 4. Platz frei? ja
     │                            │                                     │ 5. speichern mit Zeit
     │                            │ {"ok":true,"code":"ok",             │
     │                            │  "occupied":1,"totalSpots":3}       │
     │                            │<────────────────────────────────────│
     │                            │ 6. Arm hoch                         │
     │ {"ok":true,"message":      │                                     │
     │  "LU 12345 ist registriert │                                     │
     │  ..."}                     │                                     │
     │<───────────────────────────│                                     │
     │                            │ 7. nach 5 Sekunden: Arm runter      │
```

Wäre das Kennzeichen ungültig, hätte Pico 2 schon bei Schritt 1 abgebrochen. Die Kasse
hätte dann gar nichts davon erfahren, und der Arm hätte sich nicht bewegt.

#### c) Suchen und Bezahlen an der Kasse

```text
Browser (Kasse)                                   Pico 1 (Kasse)
     │  GET /api/status?plate=LU%2012345               │
     │────────────────────────────────────────────────>│  Auto suchen, Gebühr
     │  {"status":"unpaid","fee":"CHF 4.00",...}       │  aus der Dauer berechnen
     │<────────────────────────────────────────────────│
     │                                                 │
     │  (Benutzer klickt "Bezahlen")                   │
     │  GET /api/pay?plate=LU%2012345                  │
     │────────────────────────────────────────────────>│  Betrag festhalten,
     │  {"ok":true,"amount":"CHF 4.00",...}            │  Status auf bezahlt
     │<────────────────────────────────────────────────│
```

#### d) Ausfahrt

```text
Browser (Schranke)          Pico 2 (Schranke)                    Pico 1 (Kasse)
     │ GET /api/exit?plate=LU%2012345                                   │
     │───────────────────────────>│                                     │
     │                            │ 1. Kennzeichen prüfen               │
     │                            │ GET /api/status?plate=LU%2012345    │
     │                            │────────────────────────────────────>│
     │                            │ {"status":"paid",...}               │
     │                            │<────────────────────────────────────│
     │                            │ 2. hat geantwortet? ja              │
     │                            │ 3. kennt das Auto? ja               │
     │                            │ 4. bezahlt? ja                      │
     │                            │                                     │
     │                            │ GET /api/exit?plate=LU%2012345      │
     │                            │────────────────────────────────────>│ Vorgang abschliessen,
     │                            │ {"ok":true,...}                     │ Platz frei
     │                            │<────────────────────────────────────│
     │                            │ 5. ERST JETZT Arm hoch              │
     │ {"ok":true,"message":...}  │                                     │
     │<───────────────────────────│                                     │
```

Achte auf die Reihenfolge am Ende: Pico 2 meldet die Ausfahrt der Kasse, **bevor** er den
Arm öffnet. Warum das wichtig ist, steht in [Abschnitt 3.13](#313-barriersmaincpp-das-hauptprogramm-der-schranke).

#### e) Was im Hintergrund ständig passiert

Auch wenn niemand etwas anklickt, ist im Netz Betrieb:

| Wer | fragt wen | wie oft | warum |
|---|---|---|---|
| Browser (Kasse) | Pico 1, `/api/vehicles` | alle 2 Sekunden | Liste und Parkdauer aktuell halten |
| Browser (Schranke) | Pico 2, `/api/state` | jede Sekunde | Belegung, Zustand des Arms, ablaufender Balken |
| Pico 2 | Pico 1, `/api/vehicles` | alle 3 Sekunden (bei Ausfall alle 10) | Belegung für die Anzeige der Schranke |

Die Seite der Schranke fragt jede Sekunde, aber Pico 2 gibt diese Frage **nicht** jedes
Mal an die Kasse weiter. Er antwortet mit dem, was er sich vom letzten Mal gemerkt hat.
Das nennt man einen **Zwischenspeicher** oder **Cache**. Ohne ihn würde ein Ausfall der
Kasse die Seite der Schranke jede Sekunde für zwei Sekunden einfrieren.

#### f) Wenn die Kasse ausfällt

```text
Browser (Schranke)          Pico 2 (Schranke)                    Pico 1 (Kasse)
     │ GET /api/exit?plate=...    │                                     │  (ausgeschaltet)
     │───────────────────────────>│ GET /api/status?plate=...           │
     │                            │─────────────────────────────────X   │
     │                            │ ... wartet höchstens 2 Sekunden ... │
     │                            │ keine Antwort: Schranke bleibt ZU   │
     │ {"ok":false,"message":     │                                     │
     │  "Die Kasse ist zurzeit    │                                     │
     │  nicht erreichbar ..."}    │                                     │
     │<───────────────────────────│                                     │
```

Ohne Kasse kann niemand prüfen, ob bezahlt wurde. Also bleibt die Schranke zu. Dieses
Prinzip heisst **im Zweifel sicher** (englisch fail-safe): Wenn etwas schiefgeht, wählt
das System den Zustand, in dem kein Schaden entsteht.

### 2.6 Alle Adressen auf einen Blick

**Pico 1, die Kasse (`192.168.89.211`)**

| Endpunkt | Wer ruft ihn auf | Was er macht |
|---|---|---|
| `/` | Browser | liefert die Webseite der Kasse |
| `/api/vehicles` | Browser, Pico 2 | Belegung, Autos im Parkhaus, ausgefahrene Autos |
| `/api/status?plate=...` | Browser, Pico 2 | Auskunft zu einem Kennzeichen: `unknown`, `unpaid` oder `paid` |
| `/api/entry?plate=...` | Pico 2 | Einfahrt eintragen |
| `/api/pay?plate=...` | Browser | Gebühr bezahlen |
| `/api/exit?plate=...` | Pico 2 | Parkvorgang abschliessen, Platz freigeben |
| `/api/setclock?epoch=...` | Browser | Uhr stellen |
| `/api/debug/time?plate=...&minutes=61` | Browser | nur zum Testen: Einfahrt in die Vergangenheit verschieben |
| `/api/debug/reset` | Browser | nur zum Testen: alles löschen |

**Pico 2, die Schranke (`192.168.89.212`)**

| Endpunkt | Wer ruft ihn auf | Was er macht |
|---|---|---|
| `/` | Browser | liefert die Webseite der Schranke |
| `/api/entry?plate=...` | Browser | Einfahrt durchführen |
| `/api/exit?plate=...` | Browser | Ausfahrt durchführen |
| `/api/state` | Browser | Belegung, Kasse erreichbar, Zustand des Arms |
| `/api/servotest` | Browser | nur zum Testen: Arm einmal auf und zu |

Du kannst jeden dieser Endpunkte auch direkt in die Adresszeile des Browsers tippen und
siehst dann die rohe JSON-Antwort. Das ist sehr praktisch zur Fehlersuche.

Hier ein paar echte Antworten aus einem Testlauf:

```text
/api/status?plate=LU%2012345    (Kasse)
{"plate":"LU 12345","status":"unpaid","entry":"22.10.2026 23:02:56",
 "duration":"0 h 00 min","fee":"CHF 2.00","feeInCents":200}

/api/exit?plate=LU%2012345      (Schranke, noch nicht bezahlt)
{"ok":false,"message":"Die Parkgebühr für LU 12345 ist noch offen (CHF 2.00).
 Bitte zuerst an der Kasse bezahlen. Die Schranke bleibt geschlossen."}

/api/state                      (Schranke)
{"cashDeskOnline":true,"wifi":true,"occupied":0,"totalSpots":3,
 "entryBarrierOpen":false,"entryBarrierMsLeft":0,"exitBarrierOpen":false,
 "exitBarrierMsLeft":0,"singleServo":true,"testBuild":true}
```

### 2.7 Der Draht zum Laptop: die serielle Ausgabe

Neben dem WLAN gibt es noch einen zweiten Weg, auf dem der Pico mit dem Laptop redet:
das **USB-Kabel**. Darüber schreibt der Pico Textzeilen, die man im **seriellen Monitor**
lesen kann (in VS Code mit `pio device monitor`). So sieht das beim Start der Schranke aus:

```text
=== Parksystem M450 - Pico 2 (Schranken) ===
[SCHRANKE] Schranke bereit an GPIO 8
[SCHRANKE] Ein-Servo-Betrieb: Ein- und Ausfahrt teilen GP8
[WLAN] Suche Netze in der Umgebung ...
  --> DEIN-WLAN-NAME  Signal -50 dBm  Kanal 1  WPA2
[WLAN] Verbinde mit DEIN-WLAN-NAME
[WLAN] Verbunden. Weboberflaeche: http://192.168.89.212
[SCHRANKEN] Webserver laeuft. Kasse erwartet unter 192.168.89.211
```

Jede Zeile beginnt mit einem Vermerk in eckigen Klammern, der sagt, aus welchem Teil des
Programms sie stammt: `[WLAN]`, `[KASSE]`, `[SCHRANKE]`, `[AUSFAHRT]` und so weiter. Für
die Benutzer ist diese Ausgabe unsichtbar. Sie ist nur für dich da, um zu sehen, was im
Pico passiert. Umlaute stehen dort absichtlich als `ae`, `oe`, `ue`, weil manche Monitore
Umlaute falsch darstellen.

---

## Teil 3: Der Code, Datei für Datei

### 3.0 Bevor es losgeht

#### Welche Dateien es gibt

Im Projekt kommen drei Arten von Dateien vor:

| Endung | Was drin steht |
|---|---|
| `.ini` | Einstellungen für das Bauen. Bei uns nur `platformio.ini`. |
| `.h` | eine **Header-Datei**: eine Übersicht, was es gibt (wie ein Inhaltsverzeichnis) |
| `.cpp` | eine **Quelldatei**: der eigentliche Code (die Kapitel dazu) |

Zu den meisten Themen gibt es ein Paar aus `.h` und `.cpp` mit gleichem Namen, zum
Beispiel `license_plate.h` und `license_plate.cpp`. Warum das so ist, erkläre ich in
[Abschnitt 3.3](#33-license_plate-kennzeichen-prüfen), wo es zum ersten Mal vorkommt.

#### Die Landkarte

```text
platformio.ini            Bauanleitung: welche Dateien in welche Firmware kommen

src/
  common/                 GEMEINSAM, landet in beiden Firmwares
    config.h              alle Einstellungen (WLAN, Adressen, Tarif, Pins ...)
    license_plate.h/.cpp  Kennzeichen prüfen und vereinheitlichen
    clock_time.h/.cpp     Zeit messen, Datum, Dauer und Geld als Text
    json_text.h           Text sicher in JSON einpacken
    network.h/.cpp        ins WLAN kommen und drin bleiben

  cashdesk/               NUR PICO 1, DIE KASSE
    parking_garage.h/.cpp die Liste aller Parkvorgänge, Gebühr, Bezahlung
    web_page_cashdesk.h   die Webseite der Kasse
    main.cpp              Hauptprogramm: Webserver und alle Endpunkte

  barriers/               NUR PICO 2, DIE SCHRANKE
    barrier.h/.cpp        eine Schranke: Servo öffnen und von selbst schliessen
    cashdesk_client.h/.cpp  Anfragen an die Kasse schicken
    web_page_barriers.h   die Webseite der Schranke
    main.cpp              Hauptprogramm: Webserver, Einfahrt, Ausfahrt
```

Wir gehen die Dateien **genau in dieser Reihenfolge** durch, von oben nach unten. Jede
Datei benutzt nur Dinge aus Dateien, die vor ihr kommen. So musst du nie etwas
verstehen, das du noch nicht gesehen hast.

Wer welche Datei benutzt, zeigt dieses Bild. Ein Pfeil heisst „benutzt":

```text
                            config.h
            ┌──────────┬───────┴──────┬───────────┐
            v          v              v           v
     license_plate  clock_time     network    json_text.h
            │          │              │           │
            └────┬─────┘              │           │
                 v                    │           │
          parking_garage              │           │
                 │                    │           │
                 v                    v           v
          cashdesk/main.cpp <── web_page_cashdesk.h        KASSE

     license_plate  network  json_text.h   barrier   cashdesk_client
            │          │          │           │            │
            └──────────┴────┬─────┴───────────┴────────────┘
                            v
                   barriers/main.cpp <── web_page_barriers.h    SCHRANKE
```

#### Wie C++-Code aussieht

Ein paar Dinge gelten überall. Mehr brauchst du am Anfang nicht:

```cpp
// Alles nach zwei Schrägstrichen ist ein Kommentar. Der Computer ignoriert es.
// Kommentare sind nur für Menschen da.

int count = 3;        // Jede Anweisung endet mit einem Strichpunkt.

if (count > 2) {      // Geschweifte Klammern fassen mehrere Zeilen zu einem
  count = 0;          // Block zusammen. Die Einrückung ist nur für die Lesbarkeit,
}                     // dem Computer ist sie egal.
```

- **Gross- und Kleinschreibung zählt.** `count` und `Count` sind zwei verschiedene Namen.
- **Zeilenumbrüche und Leerzeichen sind egal.** Man könnte alles in eine Zeile schreiben.
  Man tut es nicht, weil man es dann nicht mehr lesen kann.
- **Namen in `GROSSBUCHSTABEN`** sind in diesem Projekt Konstanten, also Werte, die sich
  nie ändern.

Alles Weitere erkläre ich, wenn es im Code auftaucht.

#### Wie ein Programm auf dem Pico abläuft

Ein Programm für den Pico hat immer zwei besondere Funktionen. Eine **Funktion** ist ein
benannter Block Code, den man aufrufen kann. Hier ein Mini-Programm, das die kleine LED
auf dem Pico blinken lässt:

```cpp
#include <Arduino.h>

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);     // die eingebaute LED als Ausgang einstellen
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  // LED an
  delay(500);                       // eine halbe Sekunde warten
  digitalWrite(LED_BUILTIN, LOW);   // LED aus
  delay(500);                       // eine halbe Sekunde warten
}
```

- **`setup()`** läuft **genau einmal**, direkt nach dem Einschalten. Hier wird alles
  vorbereitet.
- **`loop()`** läuft danach **immer wieder**, ohne Ende, so lange der Pico Strom hat.
  Ist die letzte Zeile erreicht, beginnt `loop()` wieder von vorn.

Diese beiden Funktionen schreibt man selbst. Aufgerufen werden sie vom **Arduino-
Framework**. Das ist eine Sammlung von fertigem Code, die uns viel Arbeit abnimmt: Sie
startet den Pico, ruft `setup()` auf und dann endlos `loop()`. Ausserdem bringt sie
Befehle wie `digitalWrite()` und `delay()` mit.

**Eine wichtige Warnung zu `delay()`:** Während `delay(500)` macht der Pico eine halbe
Sekunde lang **gar nichts**. Im Blink-Beispiel ist das egal. In unserem Projekt wäre es
schlimm: Der Pico muss ständig Webanfragen beantworten. Würde er 5 Sekunden lang auf das
Schliessen der Schranke warten, wäre die Webseite 5 Sekunden lang tot. Deshalb kommt
`delay()` in unseren `loop()`-Funktionen **nie** vor. Wie wir stattdessen warten, siehst
du bei der Schranke in [Abschnitt 3.10](#310-barrier-die-schranke-als-objekt).

**Bibliothek** nennt man fertigen Code, den andere geschrieben haben und den man
einbindet. Wir benutzen Bibliotheken für WLAN (`WiFi.h`), für den Webserver
(`WebServer.h`), für Anfragen an andere Geräte (`HTTPClient.h`) und für den Servo
(`Servo.h`). Sie kommen alle mit dem Arduino-Framework für den Pico mit.

---

### 3.1 platformio.ini: Die Bauanleitung

**Wozu:** Diese Datei sagt **PlatformIO** (dem Werkzeug in VS Code, das den Code baut),
für welches Gerät gebaut wird und welche Dateien in welche Firmware gehören.

Die Datei ist in **Abschnitte** gegliedert. Ein Abschnitt beginnt mit einem Namen in
eckigen Klammern. Zeilen mit `;` am Anfang sind Kommentare. Ohne Kommentare sieht die
Datei so aus:

```ini
[env]
platform = https://github.com/maxgerhardt/platform-raspberrypi.git
board = rpipicow
framework = arduino
board_build.core = earlephilhower
monitor_speed = 115200
build_flags =
    -Wall
    -DPIO_FRAMEWORK_ARDUINO_ENABLE_IPV6=0

[env:pico1_cashdesk]
build_src_filter = -<*> +<common/> +<cashdesk/>
build_flags =
    ${env.build_flags}
    -DTEST_BUILD=1

[env:pico2_barriers]
build_src_filter = -<*> +<common/> +<barriers/>
build_flags =
    ${env.build_flags}
    -DTEST_BUILD=1
```

#### Der gemeinsame Abschnitt `[env]`

Alles in `[env]` gilt für **beide** Firmwares. So steht nichts doppelt da.

| Zeile | Bedeutung |
|---|---|
| `platform = ...` | Welche **Chipfamilie**. Hier die Raspberry-Pi-Chips. Die Adresse zeigt auf eine Version, die das Framework von „earlephilhower" mitbringt (siehe unten). |
| `board = rpipicow` | Welches **Board** genau: der Pico **W**, also mit WLAN. Daraus weiss PlatformIO, wie viel Speicher da ist. |
| `framework = arduino` | Wir programmieren im Arduino-Stil, also mit `setup()` und `loop()`. |
| `board_build.core = earlephilhower` | Welche **Umsetzung** des Arduino-Frameworks. Es gibt zwei für den Pico. Nur diese bringt `WebServer.h` und `Servo.h` für den Pico W mit. Fehlt die Zeile, findet der Compiler diese Dateien nicht. |
| `monitor_speed = 115200` | Die Geschwindigkeit der seriellen Ausgabe. Im Programm steht dieselbe Zahl bei `Serial.begin(115200)`. Stimmen die beiden nicht überein, erscheint im Monitor nur Zeichensalat. |
| `build_flags` | Zusätzliche Schalter für den Compiler, siehe gleich. |

Der **Compiler** ist das Programm, das deinen Quellcode in Maschinenbefehle übersetzt.

Die **Build-Flags**:

- `-Wall` heisst „warn all": Der Compiler soll bei allem warnen, was verdächtig aussieht.
  Eine Warnung ist kein Fehler, das Programm wird trotzdem gebaut. Aber Warnungen
  zeigen oft echte Fehler. Unser Projekt baut ohne eine einzige Warnung.
- `-D` heisst **definiere**. `-DPIO_FRAMEWORK_ARDUINO_ENABLE_IPV6=0` legt fest, dass der
  Name `PIO_FRAMEWORK_ARDUINO_ENABLE_IPV6` den Wert `0` hat. Das schaltet IPv6 ab, eine
  neuere Art von IP-Adressen, die wir nicht brauchen. Es spart Speicher.

#### Die beiden Firmwares

`[env:pico1_cashdesk]` und `[env:pico2_barriers]` sind zwei **Umgebungen**. Jede ergibt
eine eigene Firmware. Beide erben alles aus `[env]`.

Die wichtigste Zeile ist **`build_src_filter`**. Sie bestimmt, welche Dateien aus `src/`
in diese Firmware kommen. Man liest sie von links nach rechts:

```text
-<*>          erst einmal ALLES abwählen
+<common/>    dann den gemeinsamen Ordner dazunehmen
+<cashdesk/>  und den Ordner der Kasse
```

Der Ordner `barriers/` bleibt bei der Kasse also draussen, und umgekehrt. Das ist nicht
nur ordentlich, sondern **zwingend**: Beide Ordner haben eine `main.cpp`, und beide
enthalten eine Funktion `setup()` und eine Funktion `loop()`. Ein Programm darf aber nur
**eine** `setup()` haben. Kämen beide Ordner in dieselbe Firmware, würde das Bauen mit
der Fehlermeldung `multiple definition of 'setup'` abbrechen.

`${env.build_flags}` heisst: „übernimm hier die Build-Flags aus `[env]`". Ohne diese Zeile
würden die Flags aus `[env]` ersetzt statt ergänzt.

`-DTEST_BUILD=1` schaltet die **Testwerkzeuge** ein. Das sind Endpunkte, mit denen man
zum Beispiel die Einfahrtszeit eines Autos künstlich verschieben kann, um eine Parkdauer
von drei Stunden in drei Sekunden zu testen. Im Code stehen diese Teile zwischen
`#if TEST_BUILD` und `#endif`. Setzt man den Wert auf `0`, verschwinden sie vollständig
aus der Firmware. Wie das genau funktioniert, siehst du in [Abschnitt 3.7](#37-parking_garage-das-gedächtnis-der-kasse).

Gebaut wird mit diesen Befehlen:

```text
pio run -e pico1_cashdesk     baut die Kasse
pio run -e pico2_barriers     baut die Schranke
pio run                       baut beide
```

Das Ergebnis liegt danach in `.pio/build/pico1_cashdesk/firmware.uf2` und
`.pio/build/pico2_barriers/firmware.uf2`.

---

### 3.2 config.h: Alle Einstellungen

**Wozu:** Hier stehen alle Zahlen und Texte, die man vielleicht einmal ändern möchte.
Wer den Tarif, die Anzahl Plätze oder das WLAN ändern will, muss nur diese eine Datei
anfassen. **Wer benutzt sie:** praktisch jede andere Datei, auf beiden Picos. Dadurch
rechnen Kasse und Schranke garantiert mit denselben Werten.

Datei: [src/common/config.h](../src/common/config.h)

#### Die ersten drei Zeilen

```cpp
#pragma once
#include <Arduino.h>
#include <IPAddress.h>
```

**`#include`** heisst: „Füge hier den Inhalt einer anderen Datei ein." Das ist wörtlich
gemeint. Bevor der Compiler loslegt, ersetzt ein Vorprogramm (der **Präprozessor**) diese
Zeile durch den kompletten Text der genannten Datei. So bekommt unser Code Zugriff auf
alles, was dort steht.

- Spitze Klammern `<Arduino.h>` bedeuten: eine Datei aus einer Bibliothek.
- Anführungszeichen `"config.h"` bedeuten: eine Datei aus unserem eigenen Projekt.

`Arduino.h` bringt die Grundlagen wie `millis()` und `digitalWrite()`. `IPAddress.h`
bringt den Typ `IPAddress`, mit dem man IP-Adressen speichert.

**`#pragma once`** steht am Anfang jeder `.h`-Datei. Es heisst: „Füge mich höchstens
einmal ein." Ohne diese Zeile könnte es passieren, dass `config.h` über Umwege zweimal in
dieselbe Datei eingefügt wird. Dann stünde alles doppelt da, und der Compiler würde sich
beschweren, dass zum Beispiel `MAX_PARKING_SPOTS` zweimal festgelegt wurde.

#### WLAN-Zugangsdaten

```cpp
#define WIFI_SSID     "DEIN-WLAN-NAME"
#define WIFI_PASSWORD "********"
```

(Das echte Passwort steht hier nicht, es gehört nicht in eine Dokumentation.)

**`#define NAME WERT`** ist eine reine **Textersetzung**. Der Präprozessor ersetzt vor dem
Kompilieren überall im Code `WIFI_SSID` durch `"DEIN-WLAN-NAME"`. Das ist, als
würdest du in Word „Suchen und Ersetzen" drücken. **SSID** ist übrigens das Fachwort für
den Namen eines WLANs.

Text in doppelten Anführungszeichen nennt man **Zeichenkette** oder **String**.

#### Feste IP-Adressen

```cpp
#define IP_CASHDESK    IPAddress(192, 168, 89, 211)  // Pico 1, die Kasse
#define IP_BARRIERS    IPAddress(192, 168, 89, 212)  // Pico 2, die Schranke
#define IP_GATEWAY     IPAddress(192, 168, 88, 1)    // der Router
#define IP_SUBNET_MASK IPAddress(255, 255, 254, 0)
```

`IPAddress(192, 168, 89, 211)` erzeugt eine IP-Adresse aus vier Zahlen. Was die
einzelnen Adressen bedeuten, steht in [Abschnitt 2.1](#21-das-netz-wlan-und-ip-adressen).
In der Datei stehen darüber noch die alten Werte für das Schulnetz, mit `//` auskommentiert.
Auskommentierter Code wird ignoriert, man hebt ihn nur auf, um schnell zurückwechseln zu
können.

#### Konstanten mit `constexpr`

```cpp
constexpr uint16_t WEB_SERVER_PORT = 80;
constexpr uint8_t  MAX_PARKING_SPOTS = 3;
constexpr uint32_t CENTS_PER_HOUR = 200;
```

Eine solche Zeile hat immer denselben Aufbau. Nehmen wir die zweite auseinander:

```text
constexpr   uint8_t   MAX_PARKING_SPOTS   =   3   ;
    │          │              │           │   │   └── Ende der Anweisung
    │          │              │           │   └── der Wert
    │          │              │           └── "bekommt den Wert"
    │          │              └── der Name
    │          └── der Typ: welche Art von Wert (hier: kleine ganze Zahl)
    └── "steht beim Bauen fest und ändert sich nie"
```

**`constexpr`** heisst: Der Wert steht schon beim Bauen fest und kann im laufenden
Programm nie verändert werden. Würde irgendwo im Code `MAX_PARKING_SPOTS = 5;` stehen,
würde der Compiler mit einem Fehler abbrechen. Das schützt vor Versehen.

**Warum zwei Schreibweisen, `#define` und `constexpr`?** `constexpr` ist die bessere,
weil der Compiler dabei den Typ kennt und prüfen kann. `#define` brauchen wir trotzdem
für zwei Fälle: für Texte, die mit dem Befehl `F()` verwendet werden (kommt später), und
für Werte, die man beim Bauen von aussen überschreiben können soll.

#### Typen: welche Art von Wert

Jeder Wert in C++ hat einen **Typ**. Der Typ sagt, was für ein Wert das ist und wie viel
Speicher er braucht. In unserem Projekt kommen diese vor:

| Typ | Was | Wertebereich | Speicher |
|---|---|---|---|
| `bool` | wahr oder falsch | `true`, `false` | 1 Byte |
| `char` | ein einzelnes Zeichen | `'A'`, `'7'`, `' '` | 1 Byte |
| `uint8_t` | kleine ganze Zahl ohne Minus | 0 bis 255 | 1 Byte |
| `uint16_t` | ganze Zahl ohne Minus | 0 bis 65 535 | 2 Byte |
| `uint32_t` | grosse ganze Zahl ohne Minus | 0 bis rund 4,29 Milliarden | 4 Byte |
| `uint64_t` | riesige ganze Zahl ohne Minus | 0 bis 18 Trillionen | 8 Byte |
| `int` | ganze Zahl mit Minus | rund minus 2 bis plus 2 Milliarden | 4 Byte |
| `int64_t` | riesige ganze Zahl mit Minus | sehr gross | 8 Byte |
| `String` | Text beliebiger Länge | `"LU 12345"` | je nach Länge |

So liest du die Namen: **`u`** steht für unsigned, also **ohne Vorzeichen**, es gibt keine
negativen Zahlen. Die **Zahl** ist die Anzahl Bits (8 Bits sind 1 Byte). `_t` heisst nur
„Typ". `uint8_t` ist also „vorzeichenlose Zahl mit 8 Bit".

Warum nicht einfach überall `int`? Weil man mit dem passenden Typ ausdrückt, was ein Wert
sein kann. `MAX_PARKING_SPOTS` als `uint8_t` sagt jedem Leser: Das ist eine kleine,
positive Zahl. Und auf einem Gerät mit nur 264 KB Arbeitsspeicher zählt jedes Byte.

#### Geld als ganze Zahl

```cpp
constexpr uint32_t CENTS_PER_HOUR = 200;   // 200 Rappen sind CHF 2.00
```

Warum 200 Rappen und nicht 2.00 Franken? Weil Computer Kommazahlen nicht exakt speichern
können. Rechnet ein Computer `0.1 + 0.2`, kommt `0.30000000000000004` heraus. Bei Geld
sind solche Rundungsfehler nicht akzeptabel. Mit **ganzen Rappen** kann das nicht
passieren: 200 + 200 ist immer genau 400. Erst bei der Anzeige wird daraus `CHF 4.00`.
Diese Regel gilt in jeder ernsthaften Software: **Geld immer als ganze Zahl der kleinsten
Einheit speichern.**

#### Ein Wert, den man von aussen ändern kann

```cpp
#ifndef SERVO_COUNT
  #define SERVO_COUNT 1
#endif
```

Das liest man so: „**Wenn** `SERVO_COUNT` noch **nicht** definiert ist (`#ifndef` = if
not defined), dann lege es auf 1 fest. **Ende** der Bedingung." Normalerweise ist es
nicht definiert, also gilt 1. Schreibt man aber in `platformio.ini` den Schalter
`-DSERVO_COUNT=2`, ist der Name schon definiert, bevor diese Zeile kommt, und der Wert 2
bleibt. So kann man die Anzahl Servos umstellen, ohne den Code anzufassen.

`SERVO_COUNT` sagt, wie viele Servos angeschlossen sind. Mit **1** (unser Fall) bedient
ein einziger Servo Einfahrt und Ausfahrt gemeinsam. Mit **2** hätte jede ihren eigenen.

#### Die restlichen Einstellungen

```cpp
constexpr uint8_t  SERVO_PIN_ENTRY      = 8;     // GP8, physischer Pin 11
constexpr uint8_t  SERVO_PIN_EXIT       = 9;     // GP9, nur bei zwei Servos
constexpr uint8_t  SERVO_ANGLE_CLOSED   = 90;    // Arm waagrecht
constexpr uint8_t  SERVO_ANGLE_OPEN     = 0;     // Arm senkrecht
constexpr uint32_t BARRIER_OPEN_TIME_MS = 5000;  // 5 Sekunden
constexpr uint16_t HTTP_TIMEOUT_MS      = 2000;  // höchstens 2 s auf die Kasse warten
constexpr uint8_t  PLATE_MAX_LENGTH     = 10;    // Höchstlänge eines Kennzeichens
```

`MS` am Ende eines Namens steht für **Millisekunden**. 1000 Millisekunden sind eine
Sekunde. Fast alle Zeiten im Projekt werden in Millisekunden angegeben, weil die
eingebaute Uhr des Pico in Millisekunden zählt.

`HTTP_TIMEOUT_MS` ist absichtlich klein. Antwortet die Kasse nicht, gibt Pico 2 nach
spätestens 2 Sekunden auf und zeigt „Kasse nicht erreichbar". Bei 30 Sekunden würde die
Seite der Schranke eine halbe Minute lang einfrieren.

**Zusammenfassung:** `config.h` enthält nur Einstellungen, keinen ablaufenden Code. Du
hast gelernt, was `#include`, `#pragma once`, `#define`, `constexpr` und Typen sind.

---

### 3.3 license_plate: Kennzeichen prüfen

**Wozu:** Diese Dateien prüfen, ob ein eingetipptes Kennzeichen gültig ist, und bringen
es in eine einheitliche Schreibweise. **Wer benutzt sie:** beide Picos. Pico 2 prüft
sofort bei der Eingabe, und die Kasse prüft **nochmals**, bevor sie etwas speichert.

Warum doppelt? Weil jemand die Adresse der Kasse auch direkt im Browser aufrufen und
Pico 2 damit überspringen könnte. Die Kasse darf sich nicht darauf verlassen, dass schon
jemand vor ihr geprüft hat. Das nennt man **gestaffelte Absicherung**.

#### Warum es zwei Dateien gibt: `.h` und `.cpp`

Stell dir ein Restaurant vor. Die **Speisekarte** sagt dir, welche Gerichte es gibt und
was sie kosten. Wie sie gekocht werden, steht nicht drauf. Das passiert in der **Küche**.

- Die **`.h`-Datei** ist die Speisekarte. Sie listet auf, welche Funktionen es gibt, wie
  sie heissen und was man ihnen geben muss. Das nennt man **Deklaration**.
- Die **`.cpp`-Datei** ist die Küche. Dort steht, was die Funktionen tatsächlich tun. Das
  nennt man **Definition**.

Wer eine Funktion benutzen will, bindet nur die `.h`-Datei ein. Er muss nicht wissen, wie
sie innen funktioniert. So bleiben die Dateien übersichtlich, und man kann das Innere
einer Funktion ändern, ohne dass sich für die Benutzer etwas ändert.

#### Die Speisekarte: license_plate.h

Datei: [src/common/license_plate.h](../src/common/license_plate.h)

```cpp
enum class PlateCheckResult : uint8_t {
  VALID,         // alles in Ordnung
  EMPTY,         // es wurde gar nichts eingegeben
  TOO_LONG,      // mehr Zeichen als ein Kennzeichen haben kann
  WRONG_FORMAT   // falscher Aufbau
};
```

Ein **`enum class`** ist eine Liste von benannten Möglichkeiten. Hier: Was kann bei der
Prüfung eines Kennzeichens herauskommen? Genau eine dieser vier Antworten. Im Code
schreibt man dann `PlateCheckResult::VALID` oder `PlateCheckResult::TOO_LONG`. Die zwei
Doppelpunkte heissen „aus der Liste".

Warum nicht einfach `true` und `false`? Weil „falsch" nicht sagt, **was** falsch ist. Mit
vier Möglichkeiten kann die Webseite einen passenden Satz anzeigen: „Bitte ein
Kennzeichen eingeben" ist etwas anderes als „Das Kennzeichen ist zu lang".

Warum nicht Zahlen wie 0, 1, 2, 3? Weil niemand sich merken kann, dass 2 „zu lang"
bedeutet. `PlateCheckResult::TOO_LONG` liest sich von selbst.

`: uint8_t` legt fest, dass der Wert intern als kleine Zahl gespeichert wird. Das spart
Speicher.

Dann folgen die drei Funktionen, die diese Datei anbietet:

```cpp
String normalizePlate(const String& userInput);
PlateCheckResult checkPlate(const String& userInput, String& normalizedPlateOut);
String plateErrorMessage(PlateCheckResult result);
```

So liest man eine **Funktionsdeklaration**:

```text
String      normalizePlate   (const String& userInput)   ;
  │               │                     │                 └── Strichpunkt statt Rumpf:
  │               │                     │                     nur angekündigt, nicht ausgeführt
  │               │                     └── Parameter: was man der Funktion geben muss
  │               └── der Name der Funktion
  └── der Rückgabetyp: was die Funktion zurückgibt (hier: einen Text)
```

Ein **Parameter** ist ein Wert, den man der Funktion beim Aufruf mitgibt. Der **Rückgabe-
wert** ist das Ergebnis, das sie zurückliefert. Aufgerufen wird sie so:

```cpp
String clean = normalizePlate("lu12345");   // clean enthält danach "LU 12345"
```

#### Was `const String&` bedeutet

In fast jeder Funktion des Projekts steht `const String&`. Das ist wichtig, also langsam:

- **`String`** ist der Typ: ein Text.
- **`&`** macht daraus eine **Referenz**. Die Funktion bekommt **keine Kopie** des Textes,
  sondern arbeitet direkt mit dem Original. Vergleich: Du zeigst jemandem dein Heft,
  statt es zu fotokopieren. Das spart Zeit und Speicher.
- **`const`** ist ein Versprechen: „Ich schaue dein Heft nur an, ich schreibe nichts
  hinein." Der Compiler überwacht das. Versucht die Funktion trotzdem, den Text zu
  ändern, bricht das Bauen mit einem Fehler ab.

Zusammen heisst `const String& userInput`: „Gib mir deinen Text zum Anschauen, ohne Kopie
und ohne dass ich ihn verändern darf."

#### Zwei Ergebnisse aus einer Funktion

```cpp
PlateCheckResult checkPlate(const String& userInput, String& normalizedPlateOut);
```

Hier ist der zweite Parameter `String&`, **ohne** `const`. Das heisst: Die Funktion
**darf** in diesen Text hineinschreiben. Damit kann sie zwei Dinge zurückgeben:

- als **Rückgabewert** das Prüfergebnis, zum Beispiel `VALID`,
- **nebenbei** über den zweiten Parameter das bereinigte Kennzeichen.

So sieht ein Aufruf aus:

```cpp
String plate;                                        // leerer Text
PlateCheckResult result = checkPlate("lu12345", plate);
// result ist jetzt PlateCheckResult::VALID
// plate  ist jetzt "LU 12345"
```

Solche Parameter, die von der Funktion befüllt werden, heissen **Ausgabeparameter**. Im
Projekt enden ihre Namen immer auf `Out`, damit man sie sofort erkennt.

#### Die Küche: license_plate.cpp

Datei: [src/common/license_plate.cpp](../src/common/license_plate.cpp)

Die Datei beginnt mit:

```cpp
#include "license_plate.h"
#include "config.h"

namespace {
  // ... fünf Hilfsfunktionen ...
}
```

Eine `.cpp`-Datei bindet immer zuerst ihre eigene `.h`-Datei ein. Dazu `config.h`, weil
wir `PLATE_MAX_LENGTH` brauchen.

**`namespace { ... }`** ohne Namen ist ein **anonymer Namensraum**. Alles darin ist nur
innerhalb dieser einen Datei sichtbar. Die Hilfsfunktionen darin sind also privat: Keine
andere Datei kann sie aufrufen oder versehentlich eine gleichnamige Funktion haben. Nach
aussen gibt es nur die drei Funktionen aus der Speisekarte.

#### Hilfsfunktion 1: Grossbuchstaben und einzelne Leerzeichen

[license_plate.cpp:11](../src/common/license_plate.cpp#L11)

```cpp
String toUpperCaseWithSingleSpaces(const String& text) {
  String trimmed = text;
  trimmed.trim();

  String result;
  result.reserve(trimmed.length() + 1);

  bool previousCharWasSpace = false;

  for (unsigned int i = 0; i < trimmed.length(); i++) {
    char currentChar = trimmed.charAt(i);
    bool isSpace = (currentChar == ' ' || currentChar == '\t');

    if (isSpace) {
      if (!previousCharWasSpace && result.length() > 0) {
        result += ' ';
        previousCharWasSpace = true;
      }
    } else {
      result += (char)toupper(currentChar);
      previousCharWasSpace = false;
    }
  }

  result.trim();
  return result;
}
```

Das ist die längste Funktion dieser Datei. Gehen wir sie Zeile für Zeile durch.

**`String trimmed = text;`** legt eine neue **Variable** an. Eine Variable ist ein
benannter Speicherplatz für einen Wert. Hier: ein Text namens `trimmed`, der zuerst eine
Kopie von `text` enthält. Eine Kopie brauchen wir, weil wir `text` nicht ändern dürfen
(es ist ja `const`).

**`trimmed.trim();`** Der Punkt heisst „von diesem Ding". `trimmed.trim()` ruft die
Funktion `trim` **von** `trimmed` auf. `trim` entfernt Leerzeichen am Anfang und am Ende.
Aus `"  lu  12345 "` wird `"lu  12345"`. Funktionen, die zu einem Ding gehören und mit
Punkt aufgerufen werden, nennt man **Methoden**. Ein `String` hat viele davon:

| Methode | Was sie tut | Beispiel mit `"LU 12345"` |
|---|---|---|
| `.length()` | Anzahl Zeichen | `8` |
| `.charAt(i)` | das Zeichen an Stelle `i` | `.charAt(0)` ist `'L'` |
| `.trim()` | Leerzeichen am Rand entfernen | |
| `.indexOf(' ')` | an welcher Stelle steht das Zeichen, `-1` wenn gar nicht | `2` |
| `.substring(a, b)` | Ausschnitt von Stelle `a` bis vor Stelle `b` | `.substring(0, 2)` ist `"LU"` |
| `.reserve(n)` | Platz für `n` Zeichen im Voraus bestellen | |
| `.equals(x)` | ist der Text gleich `x` | |

**Wichtig: Stellen werden ab 0 gezählt.** Das erste Zeichen steht an Stelle 0, das zweite
an Stelle 1. Das ist in fast allen Programmiersprachen so.

```text
Text:   L   U       1   2   3   4   5
Stelle: 0   1   2   3   4   5   6   7
```

**`result.reserve(...)`** bestellt Speicher im Voraus. Ohne das müsste sich der Text bei
jedem angehängten Zeichen neuen Speicher suchen. Das wäre langsamer, und auf dem Pico
würde der Speicher dabei in kleine Stücke zerfallen.

**`bool previousCharWasSpace = false;`** merkt sich, ob das letzte Zeichen ein Leerzeichen
war. Damit erkennen wir mehrere Leerzeichen hintereinander.

**Die `for`-Schleife.** Eine **Schleife** wiederholt einen Block mehrmals. Die `for`-
Schleife hat drei Teile in der Klammer, durch Strichpunkte getrennt:

```text
for ( unsigned int i = 0 ;   i < trimmed.length()  ;   i++  ) { ... }
      └──── Start ────┘      └──── Bedingung ────┘     └Schritt┘
      einmal am Anfang:      vor jeder Runde: nur       nach jeder Runde:
      i beginnt bei 0        weiter, solange das        i um 1 erhöhen
                             noch stimmt
```

Bei einem Text mit 9 Zeichen läuft der Block also 9 Mal, mit `i` gleich 0, 1, 2 bis 8.
`i++` ist eine Kurzform für „i um eins erhöhen". `unsigned int` ist eine ganze Zahl ohne
Minus.

**`char currentChar = trimmed.charAt(i);`** holt das Zeichen an Stelle `i`. Ein einzelnes
Zeichen steht in **einfachen** Anführungszeichen: `' '` ist ein Leerzeichen, `'A'` ein A.
Text aus mehreren Zeichen steht in **doppelten**: `"LU"`.

**`bool isSpace = (currentChar == ' ' || currentChar == '\t');`** Hier stecken zwei
Operatoren drin:

| Zeichen | Bedeutung | Beispiel |
|---|---|---|
| `==` | ist gleich (Vergleich!) | `a == 3` |
| `=` | bekommt den Wert (Zuweisung!) | `a = 3` |
| `!=` | ist ungleich | `a != 3` |
| `<`, `>`, `<=`, `>=` | kleiner, grösser, kleiner gleich, grösser gleich | |
| `&&` | **und**: beides muss stimmen | `a > 0 && a < 10` |
| `\|\|` | **oder**: eines genügt | `a == 1 \|\| a == 2` |
| `!` | **nicht**: dreht wahr und falsch um | `!isSpace` |

`'\t'` ist ein Tabulator. Die Zeile heisst also: „Ist das Zeichen ein Leerzeichen oder
ein Tabulator?" Das Ergebnis (`true` oder `false`) landet in `isSpace`.

**`if (...) { ... } else { ... }`** ist eine **Bedingung**. Stimmt der Ausdruck in der
Klammer, läuft der erste Block, sonst der zweite.

- **Ist es ein Leerzeichen**, wird es nur übernommen, wenn davor **kein** Leerzeichen war
  (`!previousCharWasSpace`) **und** schon etwas im Ergebnis steht (`result.length() > 0`).
  `result += ' '` hängt ein Zeichen hinten an. `+=` heisst „hänge an" oder bei Zahlen
  „zähle dazu".
- **Ist es kein Leerzeichen**, wird es als Grossbuchstabe angehängt. `toupper(c)` macht aus
  `'l'` ein `'L'`. Ziffern bleiben, wie sie sind. `(char)` davor sagt: „Behandle das
  Ergebnis als Zeichen." Das ist nötig, weil `toupper` eine Zahl zurückgibt. Eine solche
  Umwandlung in Klammern nennt man **Cast**.

**`return result;`** beendet die Funktion und gibt `result` als Ergebnis zurück.

So läuft die Funktion mit der Eingabe `"  lu  12345 "`. Nach `trim()` ist daraus
`"lu  12345"` geworden:

| `i` | Zeichen | Leerzeichen? | davor Leerzeichen? | Was passiert | `result` danach |
|---|---|---|---|---|---|
| 0 | `l` | nein | | als `L` anhängen | `L` |
| 1 | `u` | nein | | als `U` anhängen | `LU` |
| 2 | ` ` | ja | nein | Leerzeichen anhängen | `LU ` |
| 3 | ` ` | ja | **ja** | nichts | `LU ` |
| 4 | `1` | nein | | anhängen | `LU 1` |
| 5 bis 8 | `2345` | nein | | anhängen | `LU 12345` |

Ergebnis: `"LU 12345"`.

#### Hilfsfunktionen 2 und 3: Das fehlende Leerzeichen

[license_plate.cpp:46](../src/common/license_plate.cpp#L46)

```cpp
bool looksLikePlateWithMissingSpace(const String& text) {
  if (text.indexOf(' ') >= 0) {
    return false;
  }
  if (text.length() < 3) {
    return false;
  }
  return isAlpha(text.charAt(0))
      && isAlpha(text.charAt(1))
      && isDigit(text.charAt(2));
}

String insertSpaceAfterCantonCode(const String& text) {
  return text.substring(0, 2) + " " + text.substring(2);
}
```

Die erste Funktion fragt: „Sieht der Text aus wie ein Kennzeichen, bei dem nur das
Leerzeichen fehlt?" Sie prüft drei Dinge nacheinander:

1. Gibt es schon ein Leerzeichen? `indexOf` liefert die Stelle oder `-1`, wenn keines da
   ist. Eine Stelle `>= 0` heisst: Es gibt eins, also nichts zu tun.
2. Ist der Text zu kurz (unter 3 Zeichen)? Dann auch nicht.
3. Sind die ersten zwei Zeichen Buchstaben und das dritte eine Ziffer?
   `isAlpha` prüft auf Buchstabe, `isDigit` auf Ziffer.

Bemerkenswert ist das **frühe `return`**: Sobald eine Prüfung scheitert, verlässt die
Funktion sofort mit `false`. Der Rest wird gar nicht mehr angeschaut. Das ist übersichtlicher,
als alles in verschachtelte `if`-Blöcke zu packen. Du wirst dieses Muster im ganzen
Projekt sehen.

Die zweite Funktion setzt das Leerzeichen ein. Mit `"LU12345"`:

```text
text.substring(0, 2)   ist  "LU"       (Stelle 0 bis vor Stelle 2)
" "                    ist  " "
text.substring(2)      ist  "12345"    (ab Stelle 2 bis zum Ende)
zusammen mit +         ist  "LU 12345"
```

Bei Texten fügt `+` sie aneinander.

#### Hilfsfunktionen 4 und 5: Die beiden Teile eines Kennzeichens

[license_plate.cpp:69](../src/common/license_plate.cpp#L69)

```cpp
bool hasValidCantonCode(const String& plate) {
  if (plate.indexOf(' ') != 2) {
    return false;
  }
  return isAlpha(plate.charAt(0)) && isAlpha(plate.charAt(1));
}

bool hasValidNumberPart(const String& plate) {
  String numberPart = plate.substring(3);

  if (numberPart.length() < 1 || numberPart.length() > 6) {
    return false;
  }
  for (unsigned int i = 0; i < numberPart.length(); i++) {
    if (!isDigit(numberPart.charAt(i))) {
      return false;
    }
  }
  return true;
}
```

Ein Kennzeichen besteht aus zwei Teilen: dem **Kantonskürzel** (zwei Buchstaben) und der
**Nummer** (eine bis sechs Ziffern), dazwischen ein Leerzeichen.

- `hasValidCantonCode` verlangt: Das (erste) Leerzeichen steht genau an Stelle 2, davor
  stehen zwei Buchstaben.
- `hasValidNumberPart` nimmt alles ab Stelle 3 (also nach `"LU "`), prüft die Länge und
  dann in einer Schleife jedes Zeichen. Ist auch nur eines keine Ziffer, kommt sofort
  `false`. Nur wenn die Schleife ganz durchläuft, ist alles in Ordnung.

#### Die drei öffentlichen Funktionen

[license_plate.cpp:95](../src/common/license_plate.cpp#L95)

```cpp
String normalizePlate(const String& userInput) {
  String result = toUpperCaseWithSingleSpaces(userInput);

  if (looksLikePlateWithMissingSpace(result)) {
    result = insertSpaceAfterCantonCode(result);
  }

  return result;
}
```

Hier sieht man, warum es die kleinen Hilfsfunktionen gibt: `normalizePlate` liest sich
fast wie ein Satz. „Mach Grossbuchstaben und einzelne Leerzeichen. Wenn das Leerzeichen
fehlt, füge es ein." Funktionen mit sprechenden Namen machen Code lesbar.

[license_plate.cpp:109](../src/common/license_plate.cpp#L109)

```cpp
PlateCheckResult checkPlate(const String& userInput, String& normalizedPlateOut) {
  normalizedPlateOut = normalizePlate(userInput);

  if (normalizedPlateOut.length() == 0) {
    return PlateCheckResult::EMPTY;
  }
  if (normalizedPlateOut.length() > PLATE_MAX_LENGTH) {
    return PlateCheckResult::TOO_LONG;
  }
  if (!hasValidCantonCode(normalizedPlateOut)) {
    return PlateCheckResult::WRONG_FORMAT;
  }
  if (!hasValidNumberPart(normalizedPlateOut)) {
    return PlateCheckResult::WRONG_FORMAT;
  }

  return PlateCheckResult::VALID;
}
```

Zuerst wird vereinheitlicht und das Ergebnis direkt in den Ausgabeparameter geschrieben.
Dann kommen vier Prüfungen, jede mit eigenem frühen `return`. Nur wer alle übersteht,
bekommt `VALID`.

**Die Reihenfolge ist Absicht.** Gibt jemand 30 Buchstaben ein, stimmen sowohl Länge als
auch Format nicht. Der hilfreichere Satz für den Benutzer ist aber „zu lang". Deshalb
kommt die Längenprüfung vor der Formatprüfung.

Ein paar Eingaben und was herauskommt:

| Eingabe | vereinheitlicht | Ergebnis |
|---|---|---|
| `LU 12345` | `LU 12345` | `VALID` |
| `lu12345` | `LU 12345` | `VALID` |
| `  zh   1 ` | `ZH 1` | `VALID` |
| (leer) | (leer) | `EMPTY` |
| `LU 1234567` | `LU 1234567` | `TOO_LONG` (11 Zeichen) |
| `12345` | `12345` | `WRONG_FORMAT` |
| `L 12345` | `L 12345` | `WRONG_FORMAT` (nur ein Buchstabe) |
| `!@#$` | `!@#$` | `WRONG_FORMAT` |

[license_plate.cpp:136](../src/common/license_plate.cpp#L136)

```cpp
String plateErrorMessage(PlateCheckResult result) {
  switch (result) {
    case PlateCheckResult::EMPTY:
      return F("Bitte ein Kennzeichen eingeben.");
    case PlateCheckResult::TOO_LONG:
      return F("Das Kennzeichen ist zu lang. Erlaubt sind höchstens 10 Zeichen, zum Beispiel LU 12345.");
    case PlateCheckResult::WRONG_FORMAT:
      return F("Das ist kein gültiges Kennzeichen. Erwartet wird zum Beispiel LU 12345.");
    default:
      return String();
  }
}
```

**`switch`** wählt anhand eines Wertes einen von mehreren Wegen. Für jeden möglichen
Wert gibt es einen **`case`**. **`default`** fängt alles ab, was in keinem `case` steht,
hier also `VALID`. Dann gibt es keinen Fehler, und `String()` liefert einen leeren Text.

Ein `switch` ist kürzer und übersichtlicher als eine lange Kette von `if`-Abfragen, wenn
man einen Wert mit vielen Möglichkeiten vergleicht.

**`F("...")`** ist ein Befehl speziell für Mikrocontroller. Normalerweise wird jeder Text
im Programm beim Start in den Arbeitsspeicher kopiert. Der ist aber knapp (264 KB). Mit
`F()` bleibt der Text im grossen Programmspeicher (2 MB) und wird erst gelesen, wenn er
gebraucht wird. Fast alle festen Texte im Projekt stehen deshalb in `F()`.

**Zusammenfassung:** Du kennst jetzt Header und Quelldatei, Funktionen, Parameter,
Rückgabewerte, Referenzen mit `const`, Ausgabeparameter, `enum class`, Variablen, `if`,
`for`, `switch`, die Vergleichsoperatoren und `F()`. Das ist schon ein grosser Teil von
allem, was im Projekt vorkommt.

---

### 3.4 clock_time: Zeit messen und anzeigen

**Wozu:** Der Pico hat keine Uhr mit Batterie. Nach dem Einschalten weiss er nicht,
welcher Tag ist. Er weiss nur, wie viele Millisekunden seit dem Einschalten vergangen
sind. Diese Dateien machen daraus zweierlei:

1. eine **Laufzeit in Sekunden**, mit der gerechnet wird (Parkdauer, Gebühr). Sie ist
   immer korrekt, auch wenn nie jemand die Uhr gestellt hat.
2. ein **echtes Datum mit Uhrzeit** für die Anzeige. Dafür schickt die Webseite der Kasse
   einmal die Uhrzeit des Laptops.

Ausserdem bringen sie drei Funktionen mit, die Zahlen in lesbaren Text verwandeln: ein
Datum, eine Dauer und einen Geldbetrag. **Wer benutzt sie:** die Kasse.

Datei: [src/common/clock_time.cpp](../src/common/clock_time.cpp)

#### Variablen, die das ganze Programm über leben

```cpp
namespace {
  uint64_t g_totalMilliseconds = 0;
  uint32_t g_lastMillisReading = 0;

  bool    g_wallClockIsSet = false;
  int64_t g_unixTimeAtSync = 0;
  int64_t g_uptimeAtSync   = 0;
}
```

Bisher kamen Variablen nur **innerhalb** von Funktionen vor. Solche **lokalen**
Variablen gibt es nur, solange die Funktion läuft. Danach sind sie weg.

Diese hier stehen **ausserhalb** jeder Funktion. Sie entstehen beim Start und leben, bis
der Strom ausgeht. Jede Funktion dieser Datei kann sie lesen und ändern. Weil sie im
anonymen Namensraum stehen, kommen andere Dateien nicht an sie heran.

Das **`g_`** am Anfang ist eine Konvention in diesem Projekt: Es markiert solche
langlebigen Variablen (g wie global). Beim Lesen siehst du so sofort, dass sich ein Wert
über einen Funktionsaufruf hinaus hält.

#### Die Laufzeit in Sekunden, und das Problem mit 49 Tagen

[clock_time.cpp:24](../src/common/clock_time.cpp#L24)

```cpp
uint32_t uptimeInSeconds() {
  uint32_t now = millis();

  g_totalMilliseconds += (uint32_t)(now - g_lastMillisReading);
  g_lastMillisReading = now;

  return (uint32_t)(g_totalMilliseconds / 1000ULL);
}
```

**`millis()`** liefert die Millisekunden seit dem Einschalten, als `uint32_t`. Diese Zahl
kann höchstens bis rund 4,29 Milliarden gehen. 4,29 Milliarden Millisekunden sind etwa
**49,7 Tage**. Danach springt `millis()` zurück auf 0, wie ein Kilometerzähler, der
überläuft. Das nennt man **Überlauf**.

Würde man einfach `millis() / 1000` rechnen, wäre die Laufzeit nach 49 Tagen plötzlich
wieder null. Ein Auto, das vorher eingefahren ist, hätte dann eine negative Parkdauer.

Die Lösung steckt in einer einzigen Zeile:

```cpp
g_totalMilliseconds += (uint32_t)(now - g_lastMillisReading);
```

Wir zählen nicht den absoluten Wert, sondern nur den **Unterschied** zum letzten Aufruf,
und addieren ihn auf eine riesige 64-Bit-Zahl. Das Geniale: Dieser Unterschied ist **auch
beim Überlauf richtig**. Bei Zahlen ohne Vorzeichen läuft nämlich auch das Rechenergebnis
über.

Stell dir eine Uhr mit nur 12 Stunden vor. Es ist 11 Uhr, zwei Stunden später ist es
1 Uhr. Wie viele Stunden sind vergangen? „1 minus 11" wäre minus 10. Auf der Uhr gezählt
sind es aber 2, denn von 11 aus weitergezählt kommt man über 12 auf 1. Genau so rechnet
der Computer mit `uint32_t`: Minus 10 gibt es nicht, also läuft das Ergebnis über und
landet bei der richtigen Zahl.

Das klappt aber nur, wenn zwischen zwei Aufrufen keine ganze Runde (49 Tage) vergeht.
Deshalb ruft das Hauptprogramm der Kasse `uptimeInSeconds()` in **jedem** Durchlauf von
`loop()` auf, also viele tausend Mal pro Sekunde.

`/ 1000ULL` teilt durch 1000, macht also aus Millisekunden Sekunden. **Bei ganzen Zahlen
wird beim Teilen der Rest abgeschnitten**: 2999 / 1000 ist 2, nicht 2,999. `ULL` hinter
der 1000 sagt dem Compiler, dass die 1000 als riesige Zahl (64 Bit) behandelt werden soll,
passend zu `g_totalMilliseconds`.

#### Die Uhr stellen

[clock_time.cpp:50](../src/common/clock_time.cpp#L50)

```cpp
void setWallClock(uint32_t localUnixTime) {
  g_unixTimeAtSync = (int64_t)localUnixTime;
  g_uptimeAtSync   = (int64_t)uptimeInSeconds();
  g_wallClockIsSet = true;
}
```

**`void`** als Rückgabetyp heisst: Diese Funktion gibt nichts zurück. Sie tut nur etwas.

Die **Unix-Zeit** ist die Anzahl Sekunden seit dem 1. Januar 1970. So zählen fast alle
Computer die Zeit. Am 22. Oktober 2026 liegt sie bei rund 1,79 Milliarden.

Beim Stellen merken wir uns ein **Paar**: „Bei Laufzeit-Sekunde 100 war es 14:00 Uhr."
Aus diesem Paar lässt sich jede andere Laufzeit-Sekunde in eine Uhrzeit umrechnen:

[clock_time.cpp:65](../src/common/clock_time.cpp#L65)

```cpp
int64_t uptimeSecondToUnixTime(int64_t uptimeSecond) {
  return g_unixTimeAtSync + (uptimeSecond - g_uptimeAtSync);
}
```

Beispiel: Gestellt bei Sekunde 100, da war es 14:00:00. Gefragt ist Sekunde 40.
Die Rechnung ist „14:00:00 plus (40 minus 100) Sekunden", also 14:00:00 minus 60 Sekunden,
also **13:59:00**.

Daraus folgt etwas Schönes: Die Kasse speichert bei jedem Auto nur die **Laufzeit-Sekunde**
der Einfahrt, nicht die Uhrzeit. Fährt ein Auto ein, bevor jemand die Kassenseite
geöffnet hat, ist die Uhr noch nicht gestellt. Sobald sie gestellt wird, kann auch diese
alte Einfahrt in die richtige Uhrzeit umgerechnet werden.

`(int64_t)` vor einem Wert ist wieder ein **Cast**: Der Wert wird in eine Zahl mit
Vorzeichen umgewandelt. Das ist nötig, weil `uptimeSecond - g_uptimeAtSync` auch negativ
sein kann (wie im Beispiel: minus 60).

#### Datum und Uhrzeit als Text

[clock_time.cpp:74](../src/common/clock_time.cpp#L74)

```cpp
String formatDateAndTime(int64_t uptimeSecond) {
  if (!g_wallClockIsSet) {
    return F("--");
  }

  time_t unixTime = (time_t)uptimeSecondToUnixTime(uptimeSecond);

  struct tm parts;
  gmtime_r(&unixTime, &parts);

  char buffer[40];
  snprintf(buffer, sizeof(buffer), "%02d.%02d.%04d %02d:%02d:%02d",
           parts.tm_mday,
           parts.tm_mon + 1,
           parts.tm_year + 1900,
           parts.tm_hour,
           parts.tm_min,
           parts.tm_sec);

  return String(buffer);
}
```

Ohne gestellte Uhr gibt es nur zwei Striche zurück.

**`struct tm parts;`** legt eine Variable an, die ein Datum in seine Teile zerlegt
aufnehmen kann: Tag, Monat, Jahr, Stunde, Minute, Sekunde. Ein `struct` ist ein Bündel
mehrerer Werte unter einem Namen. Auf die einzelnen Teile greift man mit Punkt zu:
`parts.tm_hour` ist die Stunde. Mehr zu `struct` in [Abschnitt 3.7](#37-parking_garage-das-gedächtnis-der-kasse).

**`gmtime_r(&unixTime, &parts);`** zerlegt die Unix-Zeit und schreibt die Teile in
`parts`. Das **`&`** vor einem Variablennamen heisst hier „die Adresse von", also „wo
diese Variable im Speicher liegt". Wir sagen der Funktion damit: „Schreib das Ergebnis
dorthin, wo `parts` liegt." (Das ist ein anderes `&` als bei der Referenz in einem
Parameter. Gleiches Zeichen, andere Stelle, andere Bedeutung. C++ ist da leider nicht
sehr konsequent.)

Zwei Eigenheiten von `struct tm`: Der Monat zählt **ab 0** (Januar ist 0), deshalb `+ 1`.
Das Jahr zählt **ab 1900** (2026 ist 126), deshalb `+ 1900`.

**`char buffer[40];`** legt ein **Array** an: 40 Zeichen-Plätze hintereinander, ein
fester Puffer für Text. Mehr zu Arrays in Abschnitt 3.7.

**`snprintf`** schreibt formatierten Text in diesen Puffer. Im Vorlagetext stehen
**Platzhalter**, die der Reihe nach durch die Werte danach ersetzt werden:

| Platzhalter | Bedeutung | Beispiel |
|---|---|---|
| `%d` | ganze Zahl | `5` |
| `%02d` | ganze Zahl, mindestens 2 Stellen, vorne mit 0 aufgefüllt | `05` |
| `%04d` | mindestens 4 Stellen | `2026` |
| `%lu` | grosse ganze Zahl ohne Vorzeichen | `123456` |
| `%%` | ein Prozentzeichen | `%` |

Aus `"%02d.%02d.%04d %02d:%02d:%02d"` mit den Werten 22, 10, 2026, 14, 5, 3 wird
`"22.10.2026 14:05:03"`.

`sizeof(buffer)` ist die Grösse des Puffers, also 40. Damit weiss `snprintf`, dass es
höchstens 40 Zeichen schreiben darf. Würde es weiterschreiben, überschriebe es fremden
Speicher, und das Programm würde unberechenbar abstürzen. Deshalb gibt man die Grösse
immer mit.

`String(buffer)` macht aus dem Puffer einen normalen `String` zum Zurückgeben.

#### Dauer und Geld als Text

[clock_time.cpp:104](../src/common/clock_time.cpp#L104)

```cpp
String formatDuration(uint32_t seconds) {
  uint32_t hours = seconds / 3600;
  uint32_t minutes = (seconds % 3600) / 60;

  char buffer[24];
  snprintf(buffer, sizeof(buffer), "%lu h %02lu min",
           (unsigned long)hours, (unsigned long)minutes);
  return String(buffer);
}

String formatMoney(uint32_t cents) {
  char buffer[20];
  snprintf(buffer, sizeof(buffer), "CHF %lu.%02lu",
           (unsigned long)(cents / 100), (unsigned long)(cents % 100));
  return String(buffer);
}
```

Hier kommt der **Modulo-Operator `%`** vor. Er liefert den **Rest** einer Division.
Zusammen mit der ganzzahligen Division `/` kann man damit eine Zahl zerlegen:

```text
4380 Sekunden:
  4380 / 3600         = 1      volle Stunden
  4380 % 3600         = 780    übrige Sekunden
  780 / 60            = 13     volle Minuten
  Ergebnis:           "1 h 13 min"

450 Rappen:
  450 / 100           = 4      Franken
  450 % 100           = 50     Rappen
  Ergebnis:           "CHF 4.50"

205 Rappen:
  205 / 100 = 2, 205 % 100 = 5, mit %02lu wird daraus "05"
  Ergebnis:           "CHF 2.05"   (ohne die 02 stünde da "CHF 2.5")
```

`(unsigned long)` ist ein Cast, weil `%lu` genau diesen Typ erwartet.

**Zusammenfassung:** Laufzeit für Berechnungen, Wanduhr nur für die Anzeige. Neu waren
langlebige Variablen, `millis()` und der Überlauf, `void`, `&` als Adresse, Arrays als
Puffer, `snprintf` mit Platzhaltern, `/` und `%`.

---

### 3.5 json_text.h: Text sicher verpacken

**Wozu:** Wenn ein Pico einen Text in eine JSON-Antwort schreibt, darf dieser Text die
Antwort nicht kaputt machen. **Wer benutzt sie:** beide Hauptprogramme, bei jeder Antwort,
die Text enthält.

Datei: [src/common/json_text.h](../src/common/json_text.h)

#### Das Problem

In JSON steht Text zwischen Anführungszeichen:

```json
{"message":"Einfahrt registriert"}
```

Jetzt tippt jemand als Kennzeichen ein einziges `"` ein. Die Kasse schreibt es in die
Fehlermeldung zurück:

```json
{"message":"Ungültig: ""}
```

Das ist kaputt. Das zweite `"` beendet den Text zu früh, das dritte steht sinnlos herum.
Der Browser kann das nicht lesen und zeigt eine völlig falsche Fehlermeldung an. Schlimmer
noch: Mit geschickt gewählten Eingaben könnte jemand eigene Felder in die Antwort
schmuggeln. Diese Art Angriff heisst **Injection**.

Die Lösung: Alle Zeichen, die in JSON eine besondere Bedeutung haben, werden durch eine
harmlose Schreibweise ersetzt. Aus `"` wird `\"`. Der Rückwärtsstrich sagt dem Leser:
„Das nächste Zeichen gehört zum Text, es beendet ihn nicht." Das nennt man **Escaping**.

#### Der Code

```cpp
inline String escapeForJson(const String& text) {
  String result;
  result.reserve(text.length() + 8);

  for (unsigned int i = 0; i < text.length(); i++) {
    char currentChar = text.charAt(i);

    switch (currentChar) {
      case '"':  result += "\\\"";  break;
      case '\\': result += "\\\\";  break;
      case '\n': result += "\\n";   break;
      case '\r': result += "\\r";   break;
      case '\t': result += "\\t";   break;

      default:
        if ((uint8_t)currentChar < 0x20) {
          char escapeBuffer[8];
          snprintf(escapeBuffer, sizeof(escapeBuffer), "\\u%04x", (uint8_t)currentChar);
          result += escapeBuffer;
        } else {
          result += currentChar;
        }
    }
  }

  return result;
}
```

Die Funktion geht Zeichen für Zeichen durch und baut einen neuen Text auf. Normale
Zeichen werden einfach übernommen, besondere werden ersetzt.

**`break;`** beendet einen `case`. Ohne `break` würde das Programm einfach in den nächsten
`case` weiterlaufen. Das ist eine bekannte Falle. (In `plateErrorMessage` war kein `break`
nötig, weil dort jeder `case` mit `return` die ganze Funktion verlässt.)

#### Die verwirrenden Rückwärtsstriche

Der schwierigste Teil ist `"\\\""`. Das liegt daran, dass der Rückwärtsstrich **auch in
C++ selbst** eine besondere Bedeutung hat. In einem C++-Text gilt:

| Im C++-Code geschrieben | Tatsächlich gespeichert | Bedeutung |
|---|---|---|
| `"\""` | `"` | ein Anführungszeichen |
| `"\\"` | `\` | ein Rückwärtsstrich |
| `"\n"` | (Zeilenumbruch) | ein Zeichen |
| `"\\\""` | `\"` | Rückwärtsstrich und Anführungszeichen, **2 Zeichen** |
| `"\\n"` | `\n` | Rückwärtsstrich und n, **2 Zeichen** |

`"\\\""` zerlegt: `\\` wird zu `\`, und `\"` wird zu `"`. Zusammen landen also genau die
zwei Zeichen `\"` im Ergebnis. Und genau das will JSON sehen.

Beispiel: Aus dem Text `Ab"c` wird `Ab\"c`.

`0x20` ist die Zahl 32 in **Hexadezimalschreibweise** (Zahlen zur Basis 16, erkennbar am
`0x`). Alle Zeichen mit einer Nummer unter 32 sind unsichtbare Steuerzeichen. Sie werden
als `\u0001` und so weiter geschrieben.

#### Warum die ganze Funktion in der `.h`-Datei steht

Normalerweise steht in einer `.h`-Datei nur die Speisekarte, und die Küche ist in der
`.cpp`-Datei. Diese Funktion ist aber kurz, und es gibt sonst nichts zum Thema JSON. Dafür
eine eigene `.cpp`-Datei anzulegen, lohnt sich nicht.

Das Problem: Die `.h`-Datei wird in mehrere `.cpp`-Dateien eingefügt. Dann stünde der
Rumpf der Funktion mehrfach im fertigen Programm, und das Bauen würde mit
`multiple definition` abbrechen. Das Wort **`inline`** erlaubt genau das: „Diese Funktion
darf mehrfach vorkommen, es ist immer dieselbe."

**Zusammenfassung:** Escaping schützt die Antworten vor kaputtem JSON und vor Injection.
Neu waren `break`, Escape-Zeichen in C++, Hexadezimalzahlen und `inline`.

---

### 3.6 network: Ins WLAN kommen und drin bleiben

**Wozu:** Den Pico mit dem WLAN verbinden, ihm seine feste IP-Adresse geben und die
Verbindung nach einem Ausfall von selbst wieder aufbauen. **Wer benutzt sie:** beide
Picos.

#### Die Speisekarte: network.h

Datei: [src/common/network.h](../src/common/network.h)

```cpp
void startWifi(IPAddress ownIp, const char* hostname);
void keepWifiAlive();
bool isWifiConnected();
```

- `startWifi` wird einmal in `setup()` aufgerufen. `ownIp` ist die feste Adresse, die
  der Pico bekommen soll. `hostname` ist ein Name, unter dem er im Router auftaucht, zum
  Beispiel `"parkhaus-kasse"`. `const char*` ist eine einfache, alte Art, einen festen
  Text zu übergeben. Für uns heisst es einfach: „ein Text, der sich nicht ändert".
- `keepWifiAlive` wird in **jedem** Durchlauf von `loop()` aufgerufen und kümmert sich um
  Ausfälle.
- `isWifiConnected` beantwortet die Frage „Bin ich gerade im WLAN?" mit `true` oder
  `false`.

#### Die Küche: network.cpp

Datei: [src/common/network.cpp](../src/common/network.cpp)

Zuerst kommen ein paar feste Werte und langlebige Variablen, alle im anonymen Namensraum:

```cpp
constexpr uint32_t CONNECT_TIMEOUT_MS     = 20000;  // beim Start höchstens 20 s warten
constexpr uint32_t RETRY_INTERVAL_MS      = 15000;  // nach Ausfall alle 15 s neu versuchen
constexpr uint32_t JOIN_STATE_ACTIVE      = 0x0001;
constexpr uint32_t LINK_CHECK_INTERVAL_MS = 500;

IPAddress g_ownIp;
uint32_t  g_lastAttemptMs   = 0;
bool      g_wasConnected    = false;
bool      g_lastLinkResult  = false;
uint32_t  g_lastLinkCheckMs = 0;
```

#### Der Start: startWifi

[network.cpp:210](../src/common/network.cpp#L210)

```cpp
void startWifi(IPAddress ownIp, const char* hostname) {
  g_ownIp = ownIp;

  WiFi.setHostname(hostname);
  WiFi.mode(WIFI_STA);

  printVisibleNetworks();

  Serial.print(F("[WLAN] Verbinde mit "));
  Serial.println(F(WIFI_SSID));

  beginConnecting();

  if (waitForConnection(CONNECT_TIMEOUT_MS)) {
    g_wasConnected = true;
    Serial.print(F("[WLAN] Verbunden. Weboberflaeche: http://"));
    Serial.println(WiFi.localIP());
  } else {
    Serial.print(F("[WLAN] Keine Verbindung. Grund: "));
    Serial.println(describeWifiProblem());
    Serial.println(F("[WLAN] Es wird im Hintergrund weiter versucht."));
  }

  g_lastAttemptMs = millis();
}
```

**`WiFi`** ist ein fertiges **Objekt** aus der WLAN-Bibliothek. Ein Objekt ist ein Ding,
das Daten und Methoden zusammenpackt. Mit dem Punkt ruft man seine Methoden auf, genau
wie bei `String`. `WiFi.mode(WIFI_STA)` sagt: Der Pico ist eine **Station**, also ein
Teilnehmer in einem fremden WLAN, und nicht selbst ein Router.

**`Serial.print(...)`** schreibt Text in die serielle Ausgabe (siehe [2.7](#27-der-draht-zum-laptop-die-serielle-ausgabe)).
**`Serial.println(...)`** macht dasselbe und beginnt danach eine neue Zeile.

Der Ablauf ist: erst die Umgebung nach WLANs absuchen (für die Fehlersuche), dann die
Verbindung starten, dann höchstens 20 Sekunden warten. Klappt es, wird die Adresse
ausgegeben. Klappt es nicht, der Grund. **In beiden Fällen läuft das Programm weiter.**
Ein Pico, der ewig auf das WLAN wartet, wirkt für den Benutzer wie abgestürzt.

#### Die Verbindung starten

[network.cpp:157](../src/common/network.cpp#L157)

```cpp
void beginConnecting() {
  WiFi.config(g_ownIp, IP_GATEWAY, IP_GATEWAY, IP_SUBNET_MASK);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}
```

`WiFi.config` setzt die feste IP-Adresse. **Achtung, eine echte Falle:** Die Reihenfolge
der Werte ist in dieser Bibliothek `config(Adresse, DNS-Server, Gateway, Subnetzmaske)`.
Viele erwarten eine andere Reihenfolge. Vertauscht man zwei, scheint sich der Pico zu
verbinden, erreicht aber niemanden. Als DNS-Server (der Namen wie `google.com` in Adressen
übersetzt) nehmen wir einfach den Router, deshalb steht `IP_GATEWAY` zweimal da.

`WiFi.begin` startet die Anmeldung mit Name und Passwort aus `config.h`.

#### Warten mit Obergrenze

[network.cpp:171](../src/common/network.cpp#L171)

```cpp
bool waitForConnection(uint32_t timeoutMs) {
  uint32_t startedAt = millis();

  while (!checkWifiLinkNow() && (millis() - startedAt) < timeoutMs) {
    delay(250);
    Serial.print('.');
  }
  Serial.println();

  return checkWifiLinkNow();
}
```

Eine **`while`-Schleife** wiederholt ihren Block, **solange** die Bedingung stimmt. Hier:
„Solange noch keine Verbindung besteht **und** noch keine 20 Sekunden um sind." Pro Runde
wird eine Viertelsekunde gewartet und ein Punkt ausgegeben, damit man im Monitor sieht,
dass etwas passiert.

Hier ist `delay()` ausnahmsweise in Ordnung: Das passiert beim Start in `setup()`, und
der Pico hat ohne WLAN sowieso noch nichts anderes zu tun.

**`millis() - startedAt`** ist ein Muster, das du im Projekt noch oft siehst. Es heisst:
„Wie viele Millisekunden sind seit `startedAt` vergangen?" Dank des Tricks mit den
vorzeichenlosen Zahlen aus [Abschnitt 3.4](#34-clock_time-zeit-messen-und-anzeigen) stimmt
das sogar dann, wenn `millis()` zwischendurch übergelaufen ist.

#### Im laufenden Betrieb: keepWifiAlive

[network.cpp:238](../src/common/network.cpp#L238)

```cpp
void keepWifiAlive() {
  bool isConnectedNow = hasRealWifiLink();

  logOnlyWhenStateChanges(isConnectedNow);

  if (isConnectedNow) {
    return;
  }

  if (millis() - g_lastAttemptMs > RETRY_INTERVAL_MS) {
    g_lastAttemptMs = millis();
    Serial.print(F("[WLAN] Neuer Verbindungsversuch. Letzter Stand: "));
    Serial.println(describeWifiProblem());
    beginConnecting();
  }
}
```

Diese Funktion wird tausende Male pro Sekunde aufgerufen. Sie darf deshalb **nie warten**.
Sie schaut nur kurz nach und kehrt sofort zurück:

1. Bin ich verbunden? Wenn ja, fertig.
2. Wenn nein: Ist der letzte Versuch mehr als 15 Sekunden her? Wenn ja, neuer Versuch
   und die Zeit merken. Wenn nein, nichts tun.

Das ist das wichtigste Muster des ganzen Projekts: **Statt zu warten, merkt man sich, wann
etwas zuletzt passiert ist, und schaut bei jedem Durchlauf, ob es wieder Zeit ist.** So
bleibt der Pico immer ansprechbar.

#### Nur melden, wenn sich etwas ändert

[network.cpp:191](../src/common/network.cpp#L191)

```cpp
void logOnlyWhenStateChanges(bool isConnectedNow) {
  if (isConnectedNow == g_wasConnected) {
    return;
  }

  if (isConnectedNow) {
    Serial.print(F("[WLAN] Verbunden. Weboberflaeche: http://"));
    Serial.println(WiFi.localIP());
  } else {
    Serial.print(F("[WLAN] Verbindung verloren. Grund: "));
    Serial.println(describeWifiProblem());
  }

  g_wasConnected = isConnectedNow;
}
```

Würde man bei jedem Durchlauf „verbunden" schreiben, kämen tausende Zeilen pro Sekunde.
Stattdessen vergleicht die Funktion den jetzigen Zustand mit dem gemerkten. Nur bei einem
**Wechsel** schreibt sie eine Zeile und merkt sich den neuen Zustand. Wie eine Türglocke,
die einmal klingelt, wenn die Tür aufgeht, und nicht dauernd, solange sie offen steht.

#### Die ehrliche Verbindungsprüfung

[network.cpp:48](../src/common/network.cpp#L48)

```cpp
bool askChipIfConnected() {
  if (cyw43_state.wifi_join_state != JOIN_STATE_ACTIVE) {
    return false;
  }

  uint8_t routerAddress[6];
  return cyw43_wifi_get_bssid(&cyw43_state, routerAddress) == 0;
}
```

Diese Funktion hat eine Geschichte. Die WLAN-Bibliothek hat eine eigene Abfrage
`WiFi.status()`. Bei einer festen IP-Adresse **lügt** diese aber: Sie meldet „verbunden",
sobald die Anmeldung **beginnt**, nicht erst, wenn sie geklappt hat. Bei der Einrichtung
hat das lange in die Irre geführt: Die Picos meldeten „verbunden" und eine Sekunde später
„verloren", immer wieder, obwohl sie nie wirklich drin waren. Der echte Grund war, dass
sie den 5-GHz-Namen bekommen hatten.

Deshalb fragen wir direkt beim **Funkchip** des Pico (er heisst CYW43). Zwei Bedingungen
müssen erfüllt sein:

1. Der Treiber des Chips ist mit der Anmeldung fertig. Während sie läuft, steht in
   `cyw43_state.wifi_join_state` eine andere Zahl als 1.
2. Der Chip kann die **BSSID** nennen. Das ist die Hardware-Adresse des Routers, mit dem
   er verbunden ist (6 Zahlen). Ohne Verbindung gibt es keine BSSID, und die Funktion
   meldet einen Fehler (eine Zahl ungleich 0).

`uint8_t routerAddress[6];` ist ein Array für diese 6 Zahlen. Wir brauchen die Adresse
gar nicht, nur die Antwort, **ob** es eine gibt.

[network.cpp:60](../src/common/network.cpp#L60)

```cpp
bool hasRealWifiLink() {
  if (millis() - g_lastLinkCheckMs >= LINK_CHECK_INTERVAL_MS) {
    g_lastLinkCheckMs = millis();
    g_lastLinkResult  = askChipIfConnected();
  }
  return g_lastLinkResult;
}
```

Jede Frage an den Funkchip läuft über eine Datenleitung und dauert etwas. Bei tausenden
Durchläufen pro Sekunde wäre das zu viel. Deshalb wird höchstens alle 500 Millisekunden
wirklich gefragt. Dazwischen kommt die gemerkte Antwort zurück. Wieder dasselbe Muster:
merken, wann zuletzt, und nur bei Bedarf neu.

`checkWifiLinkNow()` macht dasselbe ohne diese Bremse. Es wird beim Start gebraucht, wo
jede Viertelsekunde zählt.

#### Fehlersuche: was sieht der Pico?

[network.cpp:115](../src/common/network.cpp#L115)

```cpp
void printVisibleNetworks() {
  Serial.println(F("[WLAN] Suche Netze in der Umgebung ..."));

  int networkCount = WiFi.scanNetworks();

  if (networkCount <= 0) {
    Serial.println(F("[WLAN] Kein einziges Netz gefunden."));
    return;
  }

  bool ownNetworkFound = false;

  for (int i = 0; i < networkCount; i++) {
    bool isOwnNetwork = (strcmp(WiFi.SSID(i), WIFI_SSID) == 0);
    if (isOwnNetwork) {
      ownNetworkFound = true;
    }

    Serial.print(isOwnNetwork ? F("  --> ") : F("      "));
    Serial.print(WiFi.SSID(i));
    // ... Signalstärke, Kanal, Verschlüsselung ...
  }

  if (!ownNetworkFound) {
    // Warnung: das eigene Netz ist nicht in der Liste
  }

  WiFi.scanDelete();
}
```

`WiFi.scanNetworks()` sucht alle WLANs in der Umgebung und gibt zurück, wie viele es
gefunden hat. Mit `WiFi.SSID(i)` holt man den Namen des Netzes Nummer `i`.

**`strcmp(a, b)`** vergleicht zwei Texte im alten C-Stil. Sie gibt **0** zurück, wenn
beide gleich sind. Deshalb `== 0`.

**`bedingung ? A : B`** ist der **Bedingungsoperator**, eine Kurzform von `if`/`else`
innerhalb einer Zeile: „Wenn die Bedingung stimmt, nimm A, sonst B." Hier: Ist es das
eigene Netz, wird ein Pfeil davorgeschrieben, sonst Leerzeichen.

Diese Liste hat bei der Einrichtung das Problem gelöst: Ein Netz wie `MeinWLAN`
tauchte darin gar nicht auf, nur `MeinWLAN-2.4GHz`.

`describeWifiProblem()` ([network.cpp:80](../src/common/network.cpp#L80)) übersetzt den
Zustand des Funkchips mit einem `switch` in einen deutschen Satz, zum Beispiel „Netz nicht
gefunden. Funkt der Router nur auf 5 GHz?". `encryptionToText()` macht dasselbe für die
Verschlüsselungsart in der Liste.

**Zusammenfassung:** Starten mit Obergrenze, im Betrieb nie warten, Ausfälle selbst
reparieren, nur Änderungen melden, und den Funkchip direkt fragen, weil die Bibliothek
bei fester IP falsch meldet. Neu waren Objekte, `while`, `Serial`, `strcmp` und der
Bedingungsoperator `? :`.

---

### 3.7 parking_garage: Das Gedächtnis der Kasse

**Wozu:** Das ist das Herzstück. Hier wird gespeichert, welches Auto wann eingefahren ist
und ob es bezahlt hat. Hier wird die Gebühr berechnet und entschieden, ob eine Einfahrt
erlaubt ist. **Wer benutzt sie:** nur die Kasse. Diese Dateien sind die „einzige Quelle
der Wahrheit" aus [Abschnitt 1.4](#14-warum-zwei-picos).

Alles liegt im **Arbeitsspeicher**. Geht der Strom weg, ist alles vergessen. Für dieses
Schulprojekt ist das so gewollt.

#### Die Speisekarte: parking_garage.h

Datei: [src/cashdesk/parking_garage.h](../src/cashdesk/parking_garage.h)

```cpp
constexpr uint8_t MAX_PARKING_RECORDS = 24;
```

Die Kasse kann 24 **Parkvorgänge** speichern. Ein Parkvorgang ist ein Eintrag „dieses Auto,
eingefahren um diese Zeit, so viel bezahlt". Warum 24, wenn es nur 3 Plätze gibt? Weil auch
**ausgefahrene** Autos als Verlauf in der Liste bleiben. Die Kassenseite zeigt sie unter
„Abgeschlossene Vorgänge" an.

##### Die vier Zustände eines Eintrags

```cpp
enum class RecordStatus : uint8_t {
  UNUSED,          // dieser Platz in der Liste ist noch nie benutzt worden
  PARKED_UNPAID,   // Auto steht im Parkhaus, Gebühr noch offen
  PARKED_PAID,     // Auto steht im Parkhaus, Gebühr bezahlt
  FINISHED         // Auto ist ausgefahren, Eintrag bleibt als Verlauf
};
```

Jeder Eintrag durchläuft diese Zustände immer in derselben Reihenfolge:

```text
             registerEntry()            payParkingFee()           finishParking()
  UNUSED ─────────────────> PARKED_UNPAID ─────────────> PARKED_PAID ─────────────> FINISHED
    ^                                                                                  │
    │                                                                                  │
    └─── ist die Liste voll, wird der älteste FINISHED-Eintrag für ein neues Auto ─────┘
         wiederverwendet (findSlotForNewRecord)
```

Ein Auto „steht im Parkhaus", wenn sein Eintrag `PARKED_UNPAID` oder `PARKED_PAID` ist.

##### Ein Parkvorgang als `struct`

```cpp
struct ParkingRecord {
  char         plate[PLATE_MAX_LENGTH + 2];
  int64_t      entryUptimeSecond;   // Laufzeit-Sekunde der Einfahrt
  int64_t      exitUptimeSecond;    // Laufzeit-Sekunde der Ausfahrt
  uint32_t     paidCents;           // bezahlter Betrag in Rappen
  RecordStatus status;
};
```

Ein **`struct`** bündelt mehrere Werte, die zusammengehören, unter einem Namen. Stell dir
eine Karteikarte vor, auf der fünf Felder vorgedruckt sind. `ParkingRecord` ist die
Vorlage für so eine Karte. Hat man eine Karte namens `record`, kommt man mit Punkt an ihre
Felder: `record.status`, `record.paidCents`.

Statt vier getrennter Listen (eine für Kennzeichen, eine für Zeiten und so weiter) gibt es
eine Liste von Karten. So können die Werte gar nicht durcheinandergeraten.

##### Das Kennzeichen als Zeichen-Array

```cpp
char plate[PLATE_MAX_LENGTH + 2];
```

Das ist ein **Array** von Zeichen: 12 `char`-Plätze hintereinander (10 + 2). Ein Array ist
eine Reihe gleichartiger Werte mit fester Länge. Man spricht die Plätze mit einer Nummer in
eckigen Klammern an, ab 0: `plate[0]` ist das erste Zeichen.

Ein Text in einem solchen Array endet immer mit einem besonderen Zeichen: der **Null**,
geschrieben `'\0'`. Sie markiert das Ende. So liegt `"LU 12345"` darin:

```text
Stelle:   0    1    2    3    4    5    6    7    8     9    10   11
Inhalt:  'L'  'U'  ' '  '1'  '2'  '3'  '4'  '5'  '\0'   ?    ?    ?
                                                   └── hier endet der Text
```

Deshalb ist das Array 2 länger als die Höchstlänge: ein Platz für die Null, einer als
Reserve.

Warum kein `String` wie sonst überall? Ein `String` holt sich bei jeder Änderung neuen
Speicher aus einem gemeinsamen Vorrat (dem **Heap**). Passiert das sehr oft, zerfällt der
freie Speicher in viele kleine Stücke, und irgendwann passt nichts Grösseres mehr hinein.
Das nennt man **Fragmentierung**. Ein Array hat dagegen eine feste Grösse, die sich nie
ändert. Für Daten, die lange leben, ist das auf einem Mikrocontroller sicherer.

##### Die anderen Typen der Speisekarte

```cpp
enum class EntryResult : uint8_t {
  ACCEPTED, INVALID_PLATE, GARAGE_FULL, ALREADY_PARKED, MEMORY_FULL
};

enum class PaymentState : uint8_t {
  PLATE_UNKNOWN,   // zu diesem Kennzeichen gibt es keine Einfahrt
  NOT_PAID_YET,    // Auto ist da, hat aber nicht bezahlt
  PAID             // Auto ist da und hat bezahlt
};

struct ParkingInfo {
  PaymentState state;
  String       plate;
  int64_t      entryUptimeSecond;
  uint32_t     parkedSeconds;
  uint32_t     feeInCents;
};
```

`EntryResult` sind die fünf möglichen Ausgänge einer Einfahrt. `PaymentState` sind die
drei Antworten auf „hat dieses Auto bezahlt?". Genau diese drei fragt Pico 2 vor jeder
Ausfahrt ab.

`ParkingInfo` ist ein `struct`, mit dem eine Funktion **mehrere Werte auf einmal**
zurückgeben kann. Eine Funktion hat nur einen Rückgabewert. Packt man fünf Werte in ein
`struct`, ist dieser eine Rückgabewert eben ein ganzes Bündel.

Dann folgen die Funktionen. Die meisten kennst du schon vom Namen her aus der Geschichte
in [Abschnitt 1.2](#12-eine-geschichte-ein-auto-im-parkhaus). Eine sieht seltsam aus:

```cpp
const ParkingRecord* getParkingRecord(uint8_t index);
```

Was das Sternchen bedeutet, kommt gleich bei den Zeigern.

#### Die Küche: parking_garage.cpp

Datei: [src/cashdesk/parking_garage.cpp](../src/cashdesk/parking_garage.cpp)

##### Die Liste aller Parkvorgänge

```cpp
ParkingRecord g_records[MAX_PARKING_RECORDS];
```

Ein Array aus 24 Karteikarten. Es entsteht beim Start und wächst nie:

```text
g_records[0]   g_records[1]   g_records[2]   ...   g_records[23]
┌──────────┐   ┌──────────┐   ┌──────────┐         ┌──────────┐
│ LU 12345 │   │ BE 55511 │   │          │         │          │
│ PAID     │   │ FINISHED │   │ UNUSED   │   ...   │ UNUSED   │
└──────────┘   └──────────┘   └──────────┘         └──────────┘
```

Auf einem Mikrocontroller ist eine feste Grösse ein Vorteil: Wir wissen von Anfang an, wie
viel Speicher die Kasse braucht. Er kann unterwegs nicht ausgehen.

##### Steht ein Auto gerade im Parkhaus?

```cpp
bool isCurrentlyParked(const ParkingRecord& record) {
  return record.status == RecordStatus::PARKED_UNPAID
      || record.status == RecordStatus::PARKED_PAID;
}
```

Eine kleine Hilfsfunktion, damit diese Frage überall gleich beantwortet wird. Die Karte
kommt als `const ParkingRecord&`, also ohne Kopie und nur zum Anschauen.

##### Ein Auto finden, und was ein Zeiger ist

[parking_garage.cpp:30](../src/cashdesk/parking_garage.cpp#L30)

```cpp
ParkingRecord* findActiveRecord(const String& plate) {
  for (uint8_t i = 0; i < MAX_PARKING_RECORDS; i++) {
    ParkingRecord& record = g_records[i];

    if (isCurrentlyParked(record) && plate.equals(record.plate)) {
      return &record;
    }
  }
  return nullptr;
}
```

Die Funktion geht alle 24 Karten durch und sucht ein Auto mit diesem Kennzeichen, das
gerade im Parkhaus steht.

**`ParkingRecord& record = g_records[i];`** legt eine Referenz an: `record` ist jetzt
einfach ein **zweiter Name** für die Karte Nummer `i`. Keine Kopie. Ändert man etwas an
`record`, ändert man die Karte in der Liste.

**`plate.equals(record.plate)`** vergleicht das gesuchte Kennzeichen mit dem auf der Karte.

Jetzt das Wichtigste: Der Rückgabetyp ist **`ParkingRecord*`**, mit Sternchen. Das ist ein
**Zeiger**.

Ein Zeiger ist eine **Adresse**. Er sagt nicht „hier ist die Karte", sondern „die Karte
liegt dort". Wie ein Zettel, auf dem steht „Fach 3 im Karteikasten". Mit dem Zettel kann
man zur Karte gehen und sie ansehen oder ändern.

- **`&record`** heisst „die Adresse von `record`". Das gibt die Funktion zurück, wenn sie
  das Auto gefunden hat.
- **`nullptr`** ist ein Zeiger, der **auf nichts** zeigt. Ein leerer Zettel. Den gibt die
  Funktion zurück, wenn sie nichts gefunden hat.

Warum gibt die Funktion nicht einfach die Karte zurück? Weil sie dann eine **Kopie**
zurückgäbe. Wer die Kopie bezahlt markiert, ändert nichts an der echten Karte in der
Liste. Mit dem Zeiger arbeitet der Aufrufer direkt an der echten Karte. Und mit `nullptr`
kann man sauber sagen „gibt es nicht".

So benutzt man das Ergebnis:

```cpp
ParkingRecord* record = findActiveRecord("LU 12345");

if (record == nullptr) {
  // nicht gefunden
} else {
  record->status = RecordStatus::PARKED_PAID;   // die echte Karte ändern
}
```

Hat man einen **Zeiger**, kommt man mit **`->`** an die Felder. Hat man die Karte selbst
oder eine Referenz, nimmt man den **Punkt**:

| Was man hat | Zugriff auf ein Feld |
|---|---|
| eine Karte oder eine Referenz | `record.status` |
| einen Zeiger | `record->status` |

**Wichtigste Regel bei Zeigern:** Vor dem Benutzen immer prüfen, ob er `nullptr` ist.
Greift man über einen leeren Zettel auf eine Karte zu, stürzt der Pico ab. Im ganzen
Projekt steht deshalb nach jedem Suchen ein `if (... == nullptr)`.

##### Einen Platz für ein neues Auto besorgen

[parking_garage.cpp:54](../src/cashdesk/parking_garage.cpp#L54)

```cpp
ParkingRecord* findSlotForNewRecord() {
  for (uint8_t i = 0; i < MAX_PARKING_RECORDS; i++) {
    if (g_records[i].status == RecordStatus::UNUSED) {
      return &g_records[i];
    }
  }

  ParkingRecord* oldest = nullptr;

  for (uint8_t i = 0; i < MAX_PARKING_RECORDS; i++) {
    ParkingRecord& record = g_records[i];

    if (record.status != RecordStatus::FINISHED) {
      continue;
    }
    if (oldest == nullptr || record.exitUptimeSecond < oldest->exitUptimeSecond) {
      oldest = &record;
    }
  }

  return oldest;
}
```

Zwei Schleifen nacheinander:

1. Gibt es eine **unbenutzte** Karte? Dann die nehmen.
2. Sonst: die **älteste abgeschlossene** Karte suchen und wiederverwenden. Dabei geht der
   älteste Verlaufseintrag verloren, was nicht schlimm ist, weil das Auto längst weg ist.

**`continue`** springt sofort zur nächsten Runde der Schleife. Hier: „Diese Karte ist nicht
abgeschlossen, also nicht in Frage, weiter zur nächsten."

So findet man das Kleinste: `oldest` merkt sich den bisher ältesten Fund. Für jede Karte
wird geprüft: „Ist noch gar nichts gemerkt, **oder** ist diese älter als die gemerkte?"
Dann wird sie die neue älteste.

**Karten von Autos, die noch im Parkhaus stehen, werden nie überschrieben.** Sonst würde
jemand seinen Parkvorgang verlieren und käme nicht mehr raus.

##### Wie lange steht ein Auto schon da?

```cpp
uint32_t secondsSinceEntry(int64_t entryUptimeSecond) {
  int64_t elapsed = (int64_t)uptimeInSeconds() - entryUptimeSecond;

  if (elapsed < 0) {
    elapsed = 0;
  }
  return (uint32_t)elapsed;
}
```

Jetzt minus Einfahrt. Gerechnet wird mit `int64_t`, also **mit** Vorzeichen, damit ein
negatives Ergebnis überhaupt erkennbar ist. Eigentlich kann das nicht vorkommen. Falls doch,
etwa weil ein Testwerkzeug die Zeit verstellt hat, wird es auf 0 gesetzt.

Warum ist das wichtig? Würde man eine negative Zahl wie -5 in `uint32_t` umwandeln, käme
nicht 0 heraus, sondern **4 294 967 291**, eine riesige Zahl. Das wären über 136 Jahre
Parkdauer. Zahlen ohne Vorzeichen können nicht negativ werden, sie laufen über.

##### Das Kennzeichen auf die Karte schreiben

```cpp
void storePlate(ParkingRecord& record, const String& plate) {
  strncpy(record.plate, plate.c_str(), sizeof(record.plate) - 1);
  record.plate[sizeof(record.plate) - 1] = '\0';
}
```

**`strncpy(Ziel, Quelle, Anzahl)`** kopiert Zeichen von der Quelle ins Ziel, aber höchstens
so viele, wie angegeben. `plate.c_str()` gibt den Inhalt des `String` in der alten
C-Schreibweise heraus, die `strncpy` versteht. Wir kopieren höchstens 11 Zeichen (12 minus
1), damit nie über das Ende des Arrays geschrieben wird.

Die zweite Zeile setzt die End-Null auf den letzten Platz. Das ist nötig, weil `strncpy`
sie bei einem zu langen Text **nicht** selbst setzt. Ohne End-Null würde jede Ausgabe
einfach weiterlesen, bis sie irgendwo im Speicher zufällig auf eine Null trifft. Unser
Kennzeichen ist durch `checkPlate` zwar nie zu lang, aber diese Zeile ist eine
Sicherung für den Fall, dass sich das einmal ändert.

##### Alles auf Anfang

```cpp
void resetAllParkingRecords() {
  for (uint8_t i = 0; i < MAX_PARKING_RECORDS; i++) {
    g_records[i].plate[0]          = '\0';
    g_records[i].entryUptimeSecond = 0;
    g_records[i].exitUptimeSecond  = 0;
    g_records[i].paidCents         = 0;
    g_records[i].status            = RecordStatus::UNUSED;
  }
}
```

Setzt alle 24 Karten zurück. `plate[0] = '\0'` macht das Kennzeichen zu einem leeren Text,
weil die End-Null gleich an erster Stelle steht. Wird einmal in `setup()` aufgerufen.

##### Die Gebühr berechnen

[parking_garage.cpp:134](../src/cashdesk/parking_garage.cpp#L134)

```cpp
uint32_t calculateFeeInCents(uint32_t parkedSeconds) {
  uint32_t hours = parkedSeconds / 3600;

  if (parkedSeconds % 3600 != 0) {
    hours++;
  }
  if (hours == 0) {
    hours = 1;
  }

  return hours * CENTS_PER_HOUR;
}
```

Hier steckt die Tarifregel „pro **angefangene** Stunde" in drei Schritten:

1. `parkedSeconds / 3600` ergibt die **vollen** Stunden (der Rest wird abgeschnitten).
2. Ist ein **Rest** da (`% 3600` nicht 0), läuft gerade eine weitere Stunde, und die zählt
   ganz: eine Stunde dazu.
3. Steht jemand exakt 0 Sekunden, käme 0 heraus. Es gibt keine Gratiszeit, also mindestens 1.

| Parkdauer | Sekunden | `/ 3600` | Rest? | Stunden | Gebühr |
|---|---|---|---|---|---|
| 0 Minuten | 0 | 0 | nein | 0, dann 1 | 200 = CHF 2.00 |
| 1 Minute | 60 | 0 | ja | 1 | 200 = CHF 2.00 |
| 60 Minuten | 3600 | 1 | nein | 1 | 200 = CHF 2.00 |
| 61 Minuten | 3660 | 1 | ja | 2 | 400 = CHF 4.00 |
| 2 h 30 min | 9000 | 2 | ja | 3 | 600 = CHF 6.00 |

##### Plätze zählen

```cpp
uint8_t countOccupiedSpots() {
  uint8_t count = 0;
  for (uint8_t i = 0; i < MAX_PARKING_RECORDS; i++) {
    if (isCurrentlyParked(g_records[i])) {
      count++;
    }
  }
  return count;
}

uint8_t countFreeSpots() {
  uint8_t occupied = countOccupiedSpots();
  if (occupied >= MAX_PARKING_SPOTS) {
    return 0;
  }
  return (uint8_t)(MAX_PARKING_SPOTS - occupied);
}
```

Die Belegung wird nicht in einer eigenen Variablen mitgezählt, sondern jedes Mal frisch aus
der Liste ausgezählt. So kann sie nie falsch sein. Würde man bei jeder Einfahrt +1 und bei
jeder Ausfahrt -1 rechnen, reichte ein einziger vergessener Fall, und die Zahl stimmte für
immer nicht mehr.

In `countFreeSpots` schützt die Prüfung wieder vor dem Überlauf: 3 minus 4 ergäbe bei
`uint8_t` nicht -1, sondern 255.

##### Die Einfahrt

[parking_garage.cpp:184](../src/cashdesk/parking_garage.cpp#L184)

```cpp
EntryResult registerEntry(const String& userInput,
                          String& plateOut,
                          String& errorMessageOut) {
  errorMessageOut = "";

  // Schritt 1: Ist das Kennzeichen gültig?
  PlateCheckResult checkResult = checkPlate(userInput, plateOut);
  if (checkResult != PlateCheckResult::VALID) {
    errorMessageOut = plateErrorMessage(checkResult);
    return EntryResult::INVALID_PLATE;
  }

  // Schritt 2: Steht dieses Auto schon drin?
  if (findActiveRecord(plateOut) != nullptr) {
    errorMessageOut = F("Dieses Fahrzeug steht bereits im Parkhaus.");
    return EntryResult::ALREADY_PARKED;
  }

  // Schritt 3: Ist noch ein Platz frei?
  if (countOccupiedSpots() >= MAX_PARKING_SPOTS) {
    errorMessageOut = F("Das Parkhaus ist voll.");
    return EntryResult::GARAGE_FULL;
  }

  // Schritt 4: Eine Karte besorgen.
  ParkingRecord* record = findSlotForNewRecord();
  if (record == nullptr) {
    errorMessageOut = F("Der Speicher der Kasse ist voll.");
    return EntryResult::MEMORY_FULL;
  }

  // Schritt 5: Erst jetzt speichern.
  storePlate(*record, plateOut);
  record->entryUptimeSecond = (int64_t)uptimeInSeconds();
  record->exitUptimeSecond  = 0;
  record->paidCents         = 0;
  record->status            = RecordStatus::PARKED_UNPAID;

  logWithOccupancy(F("Einfahrt: "), *record);
  return EntryResult::ACCEPTED;
}
```

Die Funktion hat drei Ausgaben: den Rückgabewert (wie ist es ausgegangen?), `plateOut`
(das bereinigte Kennzeichen) und `errorMessageOut` (falls nicht, warum?).

Das Muster ist dir schon bekannt: vier Prüfungen mit frühem `return`, und erst ganz am Ende
wird etwas verändert. **Zuerst alles prüfen, dann speichern.** Würde man schon nach
Schritt 1 eine Karte belegen und dann in Schritt 3 merken, dass das Parkhaus voll ist,
müsste man das wieder rückgängig machen. Vergisst man das, bleibt Datenmüll liegen.

**`*record`** mit Sternchen **vor** einem Zeiger heisst: „die Karte, auf die der Zeiger
zeigt". `storePlate` will eine Referenz auf eine Karte, keinen Zeiger. Mit `*record` geben
wir ihr die Karte selbst. Das Sternchen hat also zwei Bedeutungen: Beim Typ (`ParkingRecord*`)
heisst es „Zeiger auf", vor einer Variablen (`*record`) heisst es „das, worauf gezeigt
wird".

##### Auskunft geben

[parking_garage.cpp:227](../src/cashdesk/parking_garage.cpp#L227)

```cpp
ParkingInfo lookupParkingInfo(const String& userInput) {
  ParkingInfo info;

  info.state             = PaymentState::PLATE_UNKNOWN;
  info.plate             = normalizePlate(userInput);
  info.entryUptimeSecond = 0;
  info.parkedSeconds     = 0;
  info.feeInCents        = 0;

  ParkingRecord* record = findActiveRecord(info.plate);
  if (record == nullptr) {
    return info;
  }

  info.state = (record->status == RecordStatus::PARKED_PAID)
             ? PaymentState::PAID
             : PaymentState::NOT_PAID_YET;

  info.entryUptimeSecond = record->entryUptimeSecond;
  info.parkedSeconds     = secondsSinceEntry(record->entryUptimeSecond);

  if (record->status == RecordStatus::PARKED_PAID) {
    info.feeInCents = record->paidCents;
  } else {
    info.feeInCents = calculateFeeInCents(info.parkedSeconds);
  }

  return info;
}
```

Zuerst wird das Antwort-Bündel komplett mit dem Fall „unbekannt" gefüllt. Wird das Auto
nicht gefunden, geht genau das zurück. So hat die Antwort nie ein vergessenes Feld mit
zufälligem Inhalt.

Wird es gefunden, kommt eine wichtige Regel: **Bei einem bezahlten Auto wird der bezahlte
Betrag angezeigt und nicht neu gerechnet.** Warum? Anna bezahlt nach 59 Minuten CHF 2.00.
Sie trinkt noch einen Kaffee. Nach 61 Minuten würde eine neue Rechnung CHF 4.00 ergeben.
Die Kasse würde ihr plötzlich wieder eine Schuld anzeigen, obwohl sie bezahlt hat. Deshalb
ist der Betrag nach der Bezahlung **eingefroren**.

##### Bezahlen

[parking_garage.cpp:267](../src/cashdesk/parking_garage.cpp#L267)

```cpp
PaymentState payParkingFee(const String& userInput,
                           uint32_t& paidCentsOut,
                           bool& wasAlreadyPaidOut) {
  paidCentsOut      = 0;
  wasAlreadyPaidOut = false;

  String plate = normalizePlate(userInput);

  ParkingRecord* record = findActiveRecord(plate);
  if (record == nullptr) {
    return PaymentState::PLATE_UNKNOWN;
  }

  if (record->status == RecordStatus::PARKED_PAID) {
    wasAlreadyPaidOut = true;
    paidCentsOut      = record->paidCents;
    return PaymentState::PAID;
  }

  paidCentsOut      = calculateFeeInCents(secondsSinceEntry(record->entryUptimeSecond));
  record->paidCents = paidCentsOut;
  record->status    = RecordStatus::PARKED_PAID;

  // ... Ausgabe im Monitor ...
  return PaymentState::PAID;
}
```

Drei Fälle: unbekanntes Kennzeichen, schon bezahlt, oder jetzt bezahlen. Beim **doppelten
Bezahlen** wird nichts neu gebucht. Der Aufrufer erfährt über `wasAlreadyPaidOut`, dass
schon früher bezahlt wurde, und kann eine passende Meldung zeigen.

Beim echten Bezahlen wird der Betrag auf der Karte festgehalten (das ist das „Einfrieren")
und der Zustand auf `PARKED_PAID` gesetzt.

##### Ausfahren

[parking_garage.cpp:301](../src/cashdesk/parking_garage.cpp#L301)

```cpp
bool finishParking(const String& userInput) {
  String plate = normalizePlate(userInput);

  ParkingRecord* record = findActiveRecord(plate);

  if (record == nullptr || record->status != RecordStatus::PARKED_PAID) {
    return false;
  }

  record->exitUptimeSecond = (int64_t)uptimeInSeconds();
  record->status           = RecordStatus::FINISHED;

  // ... Ausgabe im Monitor ...
  return true;
}
```

Abschliessen geht **nur** bei einem Auto, das da ist **und** bezahlt hat. Diese Prüfung ist
die letzte Sicherung, bevor ein Platz frei wird. Selbst wenn Pico 2 einen Fehler hätte und
ein unbezahltes Auto rauslassen wollte, würde die Kasse hier Nein sagen.

Beachte die Reihenfolge in der Bedingung: Zuerst wird geprüft, ob `record` `nullptr` ist.
Nur wenn nicht, wird `record->status` angeschaut. Bei `||` hört C++ auf, sobald der erste
Teil stimmt. Wäre `record` leer, käme der gefährliche Zugriff also nie zustande.

##### Von aussen nur lesen

```cpp
const ParkingRecord* getParkingRecord(uint8_t index) {
  if (index >= MAX_PARKING_RECORDS) {
    return nullptr;
  }
  return &g_records[index];
}
```

Mit dieser Funktion kann das Hauptprogramm alle Karten durchgehen, um die Fahrzeugliste zu
bauen. Das **`const`** vor dem Zeiger heisst: Über diesen Zeiger darf man die Karte nur
**lesen**, nicht ändern. Ohne `const` könnte jede Stelle im Programm zum Beispiel einen
Betrag auf null setzen und dabei alle Regeln dieser Datei umgehen. So bleibt jede Änderung
an den Daten in dieser einen Datei, wo die Regeln stehen.

Ist die Nummer zu gross, kommt `nullptr` zurück. Ohne diese Prüfung würde `g_records[30]`
in fremden Speicher greifen.

##### Testwerkzeuge, die nur im Test-Build existieren

```cpp
#if TEST_BUILD

bool debugMoveEntryTimeBack(const String& userInput, uint32_t minutes) {
  // ... Auto suchen ...
  record->entryUptimeSecond = (int64_t)uptimeInSeconds() - (int64_t)minutes * 60;

  if (record->status == RecordStatus::PARKED_PAID) {
    record->status    = RecordStatus::PARKED_UNPAID;
    record->paidCents = 0;
  }
  // ...
}

void debugDeleteAllRecords() {
  resetAllParkingRecords();
}

#endif
```

**`#if TEST_BUILD` ... `#endif`** ist eine Anweisung an den Präprozessor, nicht an den
Compiler. Ist `TEST_BUILD` 1 (so steht es in `platformio.ini`), bleibt der Code stehen. Ist
es 0, **löscht der Präprozessor diese Zeilen**, bevor der Compiler sie überhaupt sieht.
Sie sind dann nicht nur abgeschaltet, sie existieren in der Firmware gar nicht.

Das ist wichtig, denn `debugMoveEntryTimeBack` ist mächtig: Es verschiebt die Einfahrtszeit
eines Autos in die Vergangenheit. Damit testet man in Sekunden, ob nach 61 Minuten wirklich
CHF 4.00 verlangt werden. In einer echten Anlage wäre so eine Funktion eine Einladung zum
Betrug.

Ein bezahltes Auto wird dabei wieder auf „offen" gesetzt. Sonst bliebe der eingefrorene
alte Betrag stehen, und man könnte die neue Gebühr gar nicht prüfen.

**Zusammenfassung:** Die Kasse speichert 24 Karteikarten in einem festen Array. Jede Karte
wandert durch vier Zustände. Neu waren `struct`, Arrays, Zeichen-Arrays mit End-Null,
Zeiger mit `*`, `&`, `->` und `nullptr`, `continue`, `strncpy` und `#if`. Das wichtigste
Prinzip: zuerst alles prüfen, dann ändern.

---

### 3.8 web_page_cashdesk.h: Die Webseite der Kasse

**Wozu:** Das ist die Seite, die man im Browser unter `http://192.168.89.211` sieht.
**Wer benutzt sie:** das Hauptprogramm der Kasse schickt sie an den Browser.

Datei: [src/cashdesk/web_page_cashdesk.h](../src/cashdesk/web_page_cashdesk.h)

#### Eine Webseite in einer C++-Datei

Auf einem PC liegt eine Webseite als Datei `index.html` in einem Ordner. Der Pico hat aber
kein Dateisystem mit Ordnern. Deshalb wird die ganze Seite **ein einziger langer Text im
Programm**:

```cpp
static const char CASHDESK_WEB_PAGE[] PROGMEM = R"HTML(<!doctype html>
<html lang="de">
...
</html>)HTML";
```

- `const char CASHDESK_WEB_PAGE[]` ist ein Zeichen-Array, wie beim Kennzeichen. Die Länge
  steht nicht in den eckigen Klammern, weil der Compiler sie selbst aus dem Text
  ausrechnet.
- **`PROGMEM`** sagt: Dieser Text bleibt im grossen Programmspeicher und wird nicht in den
  knappen Arbeitsspeicher kopiert. Dasselbe Prinzip wie `F()`.
- **`R"HTML( ... )HTML"`** ist ein **Rohtext**. Alles zwischen `R"HTML(` und `)HTML"`
  wird genau so übernommen, wie es dasteht. Normalerweise müsste man in C++ jedes `"` als
  `\"` schreiben (siehe [3.5](#35-json_texth-text-sicher-verpacken)). Eine Webseite hat
  hunderte Anführungszeichen. Ohne Rohtext wäre die Datei unlesbar. Das Wort `HTML` ist
  frei gewählt, es markiert nur Anfang und Ende.

#### Drei Sprachen in einer Seite

Eine Webseite besteht aus drei Sprachen, jede mit einer eigenen Aufgabe:

| Sprache | Aufgabe | Steht in |
|---|---|---|
| **HTML** | Was auf der Seite ist: Überschriften, Felder, Knöpfe, Tabellen | `<body>` |
| **CSS** | Wie es aussieht: Farben, Abstände, Schriftgrösse | `<style>` |
| **JavaScript** | Was passiert: auf Klicks reagieren, den Pico fragen, Werte eintragen | `<script>` |

Wichtig: **JavaScript läuft im Browser auf dem Laptop, nicht auf dem Pico.** Der Pico
liefert die Seite nur einmal aus. Danach holt sich die Seite selbständig Daten vom Pico und
baut sie ein.

#### HTML: der Aufbau

```html
<div class="card">
  <h2>Kennzeichen suchen und bezahlen</h2>
  <div class="row">
    <input id="searchInput" placeholder="z.B. LU 12345" autocomplete="off">
    <button onclick="searchPlate()">Suchen</button>
    <button class="secondary" id="payButton" onclick="payFee()" disabled>Bezahlen</button>
  </div>
  <div id="searchMessage" class="message"></div>
  ...
</div>
```

HTML besteht aus **Elementen** in spitzen Klammern. Die meisten haben einen Anfang
`<div>` und ein Ende `</div>`, dazwischen steht der Inhalt.

| Element | Bedeutung |
|---|---|
| `<div>` | ein Kasten, der andere Elemente zusammenfasst |
| `<h1>`, `<h2>` | Überschriften |
| `<input>` | ein Eingabefeld |
| `<button>` | ein Knopf |
| `<table>`, `<tr>`, `<td>` | Tabelle, Zeile, Zelle |
| `<b>`, `<span>` | ein Stück Text, fett oder normal |

In den spitzen Klammern stehen **Attribute**:

- **`id="searchInput"`** gibt einem Element einen eindeutigen Namen. Über diesen Namen
  findet JavaScript das Element später wieder.
- **`class="card"`** ordnet einer Stil-Gruppe aus dem CSS zu.
- **`onclick="searchPlate()"`** heisst: Bei einem Klick die JavaScript-Funktion
  `searchPlate` aufrufen.
- **`disabled`** macht einen Knopf grau und unklickbar. Der Bezahlen-Knopf ist am Anfang
  gesperrt, bis ein Auto gefunden wurde.

Die Seite hat vier Kästen: Suchen und Bezahlen, Fahrzeuge im Parkhaus, abgeschlossene
Vorgänge und (nur im Test-Build) die Testwerkzeuge. Oben drüber steht eine Leiste mit
Belegung, Tarif und Uhrzeit.

#### CSS: das Aussehen

```css
.card { background:#fff; border:1px solid var(--border); border-radius:10px; padding:18px; }
.message.ok    { display:block; background:#eafaf0; color:var(--green); }
.message.error { display:block; background:#fdeeee; color:var(--red); }
```

Eine CSS-Regel sagt: „Alle Elemente mit dieser Klasse sehen so aus." `.card` sind die
weissen Kästen mit runden Ecken. Ein Meldungsfeld mit den Klassen `message` und `ok` wird
grün, mit `message` und `error` rot. Farben stehen als Code wie `#fff` (weiss).
`var(--green)` greift auf eine Farbe zu, die oben einmal festgelegt wurde.

Für das Verständnis des Projekts musst du CSS nicht im Detail kennen. Es macht die Seite
nur hübsch und ändert nichts am Verhalten.

#### JavaScript: das Verhalten

JavaScript ist eine andere Sprache als C++, sieht aber ähnlich aus: Funktionen, Variablen,
`if`, geschweifte Klammern. Gehen wir die Funktionen der Reihe nach durch.

##### Eine Variable für das gesuchte Kennzeichen

```javascript
let selectedPlate = "";
```

`let` legt eine Variable an. Typen schreibt man in JavaScript nicht hin. Hier merkt sich
die Seite, welches Kennzeichen zuletzt gesucht wurde, damit der Bezahlen-Knopf weiss, für
welches Auto er zahlen soll.

##### Eine Meldung anzeigen

```javascript
function showMessage(elementId, text, kind) {
  const element = document.getElementById(elementId);
  element.textContent = text;
  element.className = "message " + kind;
}
```

- `document.getElementById("...")` sucht das HTML-Element mit diesem `id`.
- `element.textContent = text` setzt seinen Text.
- `element.className = "message ok"` gibt ihm die Klassen `message` und `ok`, wodurch es
  laut CSS grün wird. Mit `"message error"` wird es rot, mit `"message warn"` orange.

`const` ist wie `let`, nur kann die Variable danach nicht neu zugewiesen werden.

##### Die Uhrzeit an den Pico schicken

```javascript
async function sendBrowserClockToPico() {
  const localUnixTime = Math.floor(Date.now() / 1000) - new Date().getTimezoneOffset() * 60;
  try {
    await fetch("/api/setclock?epoch=" + localUnixTime);
  } catch (error) {
  }
}
```

`Date.now()` liefert die aktuelle Zeit des Laptops in Millisekunden seit 1970. Geteilt
durch 1000 und abgerundet (`Math.floor`) ergibt das die Unix-Zeit in Sekunden.

`getTimezoneOffset()` liefert den Unterschied zur Weltzeit in Minuten. In der Schweiz im
Sommer ist das -120. Mal 60 sind das Sekunden. Abgezogen ergibt das die **lokale** Zeit.
So muss der Pico selbst nichts über Zeitzonen wissen.

**`fetch(adresse)`** schickt eine HTTP-Anfrage an den Pico. Das ist der Moment, in dem die
Webseite mit dem Pico redet. Die Adresse beginnt mit `/`, ohne IP. Dann geht die Anfrage
automatisch an den Server, von dem die Seite gekommen ist, also an die Kasse.

**`async` und `await`:** Eine Anfrage übers Netz dauert einen Moment. `await` heisst:
„Warte hier, bis die Antwort da ist, aber friere dabei den Browser nicht ein." Eine Funktion,
die `await` benutzt, muss mit `async` markiert sein.

**`try { ... } catch (error) { ... }`:** Geht im `try`-Block etwas schief (zum Beispiel ist
der Pico nicht erreichbar), springt JavaScript in den `catch`-Block statt abzustürzen. Hier
ist der `catch`-Block leer: Klappt das Stellen der Uhr nicht, bleibt sie eben ungestellt.

##### Ein Kennzeichen suchen

```javascript
async function searchPlate() {
  const plate = document.getElementById("searchInput").value;

  if (!plate.trim()) {
    showMessage("searchMessage", "Bitte ein Kennzeichen eingeben.", "error");
    return;
  }

  const reply = await (await fetch("/api/status?plate=" + encodeURIComponent(plate))).json();
  displayParkingInfo(reply);
}
```

`.value` holt den Text aus dem Eingabefeld. Ist er leer (nach Entfernen der Leerzeichen),
gibt es sofort eine Meldung.

**`encodeURIComponent(plate)`** macht das Kennzeichen sicher für die Adresszeile. Aus dem
Leerzeichen wird `%20`.

Die Zeile mit den zwei `await` sieht kompliziert aus. Von innen nach aussen gelesen:

1. `fetch(...)` schickt die Frage und wartet auf die Antwort.
2. `.json()` liest den JSON-Text der Antwort und macht daraus ein JavaScript-Objekt.
3. Danach kann man mit Punkt auf die Felder zugreifen: `reply.status`, `reply.fee`.

##### Die Auskunft anzeigen

```javascript
function displayParkingInfo(reply) {
  const grid = document.getElementById("searchGrid");
  const payButton = document.getElementById("payButton");

  if (reply.status === "unknown") {
    selectedPlate = "";
    grid.style.display = "none";
    payButton.disabled = true;
    showMessage("searchMessage", "Das Kennzeichen " + reply.plate + " wurde im System nicht gefunden. ...", "error");
    return;
  }

  selectedPlate = reply.plate;
  grid.style.display = "grid";
  document.getElementById("infoPlate").textContent    = reply.plate;
  document.getElementById("infoEntry").textContent    = reply.entry;
  document.getElementById("infoDuration").textContent = reply.duration;
  document.getElementById("infoFee").textContent      = reply.fee;
  document.getElementById("infoStatus").textContent   = (reply.status === "paid") ? "bezahlt" : "offen";

  if (reply.status === "paid") {
    payButton.disabled = true;
    showMessage("searchMessage", "Für dieses Kennzeichen wurde bereits bezahlt. ...", "ok");
  } else {
    payButton.disabled = false;
    showMessage("searchMessage", "Offener Betrag: " + reply.fee, "warn");
  }
}
```

`===` ist in JavaScript der Vergleich „ist genau gleich" (dasselbe wie `==` in C++).

Drei Fälle: Auto unbekannt (Raster verstecken, Knopf sperren, rote Meldung), schon bezahlt
(Knopf sperren, grüne Meldung) oder noch offen (Knopf freigeben, orange Meldung mit Betrag).
`grid.style.display = "none"` versteckt ein Element, `"grid"` zeigt es wieder.

##### Bezahlen

```javascript
async function payFee() {
  if (!selectedPlate) {
    return;
  }

  const reply = await (await fetch("/api/pay?plate=" + encodeURIComponent(selectedPlate))).json();
  showMessage("searchMessage", reply.message, reply.ok ? "ok" : "error");

  if (reply.ok) {
    document.getElementById("payButton").disabled     = true;
    document.getElementById("infoStatus").textContent = "bezahlt";
    document.getElementById("infoFee").textContent    = reply.amount;
  }

  refreshPage();
}
```

Schickt `/api/pay` an die Kasse und zeigt die Antwort an. Der Text der Meldung kommt fertig
vom Pico (`reply.message`), die Seite muss ihn nur anzeigen.

##### Die Seite auffrischen

```javascript
async function refreshPage() {
  let data;
  try {
    data = await (await fetch("/api/vehicles")).json();
  } catch (error) {
    return;
  }

  document.getElementById("occupancy").textContent = data.occupied + " / " + data.totalSpots;
  document.getElementById("clockText").textContent = data.clockIsSet ? data.now : "nicht gestellt";
  document.getElementById("debugCard").style.display = data.testBuild ? "block" : "none";

  document.getElementById("activeList").innerHTML =
    data.vehicles.map(vehicle => buildTableRow(vehicle, false)).join("");
  document.getElementById("historyList").innerHTML =
    data.history.map(vehicle => buildTableRow(vehicle, true)).join("");

  // ... "keine Fahrzeuge"-Hinweise ein- oder ausblenden ...

  if (selectedPlate) {
    // die offene Suche ebenfalls auffrischen, damit Dauer und Gebühr mitlaufen
  }
}
```

Diese Funktion läuft alle 2 Sekunden. Sie holt `/api/vehicles` und trägt alles ein.
Antwortet der Pico gerade nicht, bleibt die alte Anzeige einfach stehen.

Die Zeile mit `map` und `join` baut die Tabelle:

- `data.vehicles` ist eine Liste von Autos.
- `.map(vehicle => buildTableRow(vehicle, false))` macht aus **jedem** Auto eine
  Tabellenzeile als HTML-Text. `vehicle => ...` ist eine Kurzschreibweise für eine kleine
  Funktion ohne Namen: „Nimm ein `vehicle` und gib `buildTableRow(vehicle, false)` zurück."
- `.join("")` klebt alle Zeilen zu einem Text zusammen.
- `innerHTML = ...` setzt diesen HTML-Text in die Tabelle ein.

`buildTableRow` baut eine einzelne Zeile aus `<tr>` und `<td>`, mit einem farbigen Etikett
für den Status.

Die Testwerkzeug-Karte wird nur angezeigt, wenn der Pico `"testBuild":true` meldet.

##### Der Start der Seite

```javascript
document.getElementById("searchInput").addEventListener("keydown", event => {
  if (event.key === "Enter") {
    searchPlate();
  }
});

sendBrowserClockToPico().then(refreshPage);
setInterval(refreshPage, 2000);
```

Diese Zeilen stehen in keiner Funktion. Sie laufen einmal, sobald die Seite geladen ist:

1. `addEventListener("keydown", ...)` hängt eine Funktion an das Suchfeld, die bei jedem
   Tastendruck läuft. Ist es die Eingabetaste, wird gesucht.
2. Zuerst die Uhr stellen, **danach** (`.then`) die Seite zum ersten Mal füllen.
3. **`setInterval(refreshPage, 2000)`** ruft `refreshPage` ab jetzt alle 2000 Millisekunden
   auf. So bleibt die Anzeige von selbst aktuell.

Die beiden Debug-Funktionen `debugMoveEntryTime` und `debugDeleteAll` funktionieren genauso:
Werte aus den Feldern holen, Anfrage schicken, Antwort anzeigen, Seite auffrischen.

**Zusammenfassung:** Die Webseite ist ein Rohtext im Programmspeicher. HTML ist der Aufbau,
CSS das Aussehen, JavaScript das Verhalten. JavaScript läuft im Browser und holt sich die
Daten mit `fetch` vom Pico.

---

### 3.9 cashdesk/main.cpp: Das Hauptprogramm der Kasse

**Wozu:** Hier läuft alles zusammen. Diese Datei startet den Pico, verbindet ihn mit dem
WLAN, startet den Webserver und beantwortet jede Anfrage, indem sie die Funktionen aus
`parking_garage` aufruft. **Sie enthält `setup()` und `loop()` der Kasse.**

Datei: [src/cashdesk/main.cpp](../src/cashdesk/main.cpp)

Wir lesen diese Datei **von unten nach oben**: zuerst `setup()` und `loop()`, damit du das
grosse Bild hast, dann die einzelnen Endpunkte.

#### Die Einbindungen und der Webserver

```cpp
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#include "../common/config.h"
#include "../common/json_text.h"
#include "../common/license_plate.h"
#include "../common/network.h"
#include "../common/clock_time.h"
#include "parking_garage.h"
#include "web_page_cashdesk.h"

static WebServer server(WEB_SERVER_PORT);
```

`../common/config.h` heisst: „eine Ordnerebene hoch (`..`), dann in `common`". Die Datei
liegt ja in `cashdesk/`.

**`static WebServer server(WEB_SERVER_PORT);`** erzeugt das Webserver-Objekt. `WebServer`
ist der Typ aus der Bibliothek, `server` der Name, und in Klammern steht der Wert, mit dem
es eingerichtet wird: Port 80. Ein **Port** ist eine Art Türnummer an einer IP-Adresse.
Webseiten benutzen standardmässig Port 80, deshalb muss man ihn im Browser nicht
hinschreiben.

**`static`** vor einer Variablen ausserhalb von Funktionen heisst: „nur in dieser Datei
sichtbar". Es hat dieselbe Wirkung wie der anonyme Namensraum.

#### setup(): einmal beim Start

[main.cpp:410](../src/cashdesk/main.cpp#L410)

```cpp
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  delay(1500);

  Serial.println();
  Serial.println(F("=== Parksystem M450 - Pico 1 (Kasse) ==="));

  resetAllParkingRecords();
  startWifi(IP_CASHDESK, "parkhaus-kasse");

  server.on("/",             handleHomePage);
  server.on("/api/vehicles", handleVehicleList);
  server.on("/api/status",   handleStatusRequest);
  server.on("/api/entry",    handleEntryRequest);
  server.on("/api/pay",      handlePaymentRequest);
  server.on("/api/exit",     handleExitRequest);
  server.on("/api/setclock", handleSetClock);
#if TEST_BUILD
  server.on("/api/debug/time",  handleDebugMoveTime);
  server.on("/api/debug/reset", handleDebugReset);
#endif
  server.onNotFound(handleUnknownEndpoint);

  server.begin();

  Serial.print(F("[KASSE] Webserver laeuft. Plaetze: "));
  Serial.println(MAX_PARKING_SPOTS);
}
```

Der Reihe nach:

1. **`Serial.begin(115200)`** startet die serielle Ausgabe mit der Geschwindigkeit aus
   `platformio.ini`.
2. **`pinMode(LED_BUILTIN, OUTPUT)`** stellt die kleine LED auf dem Pico als Ausgang ein.
3. **`delay(1500)`** wartet anderthalb Sekunden. Der serielle Monitor am Laptop braucht nach
   dem Einschalten einen Moment, um sich zu verbinden. Ohne diese Pause gingen die ersten
   Zeilen verloren. In `setup()` ist das Warten in Ordnung.
4. Alle 24 Karteikarten leeren, dann ins WLAN.
5. **Die Endpunkte anmelden.** Das ist der Kern.
6. **`server.begin()`** startet den Webserver. Ab jetzt nimmt er Anfragen an.

#### Die Endpunkte anmelden: ein Telefonbuch

```cpp
server.on("/api/status", handleStatusRequest);
```

Diese Zeile **ruft `handleStatusRequest` nicht auf**. Beachte: Hinter dem Namen stehen keine
Klammern. Sie trägt nur in ein Telefonbuch ein: „Wenn jemand `/api/status` anruft, verbinde
mit der Funktion `handleStatusRequest`." Man übergibt die Funktion selbst als Wert.

Der Webserver merkt sich alle diese Einträge. `server.onNotFound(...)` legt fest, welche
Funktion drankommt, wenn ein Pfad in keinem Eintrag steht.

#### loop(): immer wieder

[main.cpp:452](../src/cashdesk/main.cpp#L452)

```cpp
void loop() {
  server.handleClient();
  keepWifiAlive();
  uptimeInSeconds();
  digitalWrite(LED_BUILTIN, isWifiConnected() ? HIGH : LOW);
}
```

Vier Zeilen, die viele tausend Mal pro Sekunde laufen:

1. **`server.handleClient()`** schaut nach, ob eine Anfrage angekommen ist. Wenn ja, sucht es
   den Pfad im Telefonbuch und ruft die passende Funktion auf. Wenn nein, kehrt es sofort
   zurück.
2. **`keepWifiAlive()`** kümmert sich um WLAN-Ausfälle (siehe [3.6](#36-network-ins-wlan-kommen-und-drin-bleiben)).
3. **`uptimeInSeconds()`** hält den Sekundenzähler über die 49-Tage-Grenze aktuell (siehe
   [3.4](#34-clock_time-zeit-messen-und-anzeigen)). Das Ergebnis wird hier nicht gebraucht,
   der Aufruf selbst ist der Zweck.
4. **Die LED** leuchtet (`HIGH`), wenn der Pico im WLAN ist, sonst ist sie aus (`LOW`). So
   sieht man den Zustand ohne Bildschirm.

**Keine dieser Zeilen wartet.** Deshalb kommt `handleClient()` ständig dran, und die
Webseite reagiert sofort.

#### Zwei kleine Helfer

[main.cpp:51](../src/cashdesk/main.cpp#L51)

```cpp
static void sendJsonResponse(int httpStatusCode, const String& jsonBody) {
  server.sendHeader(F("Cache-Control"), F("no-store"));
  server.send(httpStatusCode, "application/json; charset=utf-8", jsonBody);
}
```

Schickt eine Antwort. `sendHeader` fügt die Zeile `Cache-Control: no-store` hinzu (siehe
[2.2](#22-fragen-und-antworten-http)). `send` schickt Statuscode, Art des Inhalts und den
Inhalt selbst. `application/json` sagt dem Browser „das ist JSON", `charset=utf-8` sorgt
dafür, dass Umlaute richtig ankommen.

```cpp
static String paymentStateToText(PaymentState state) {
  switch (state) {
    case PaymentState::PAID:         return F("paid");
    case PaymentState::NOT_PAID_YET: return F("unpaid");
    default:                         return F("unknown");
  }
}
```

Übersetzt den Bezahlzustand in ein Wort. **Genau diese drei Wörter wertet Pico 2 aus.** Sie
sind eine Abmachung zwischen den beiden Geräten. Ändert man hier eines, muss man es auch bei
Pico 2 ändern, sonst verstehen sie sich nicht mehr. Eine solche Abmachung nennt man
**Schnittstelle**.

`entryResultToCode` macht dasselbe für das Ergebnis einer Einfahrt: `ok`, `full`, `already`,
`memory` oder `invalid`.

#### JSON von Hand zusammensetzen

Die Antworten werden Stück für Stück als Text gebaut. Das sieht so aus:

```cpp
String response = F("{\"plate\":\"");
response += escapeForJson(info.plate);
response += F("\",\"status\":\"");
response += paymentStateToText(info.state);
response += F("\"}");
```

Wieder die Rückwärtsstriche: `\"` ist im C++-Text ein Anführungszeichen. Lesen wir es Stück
für Stück, mit dem Auto `LU 12345`:

| C++-Code | landet im Text |
|---|---|
| `F("{\"plate\":\"")` | `{"plate":"` |
| `escapeForJson(info.plate)` | `LU 12345` |
| `F("\",\"status\":\"")` | `","status":"` |
| `paymentStateToText(...)` | `paid` |
| `F("\"}")` | `"}` |
| **zusammen** | `{"plate":"LU 12345","status":"paid"}` |

Jedes Kennzeichen und jede Meldung geht durch `escapeForJson`, damit nie kaputtes JSON
entsteht. Zahlen wie `countOccupiedSpots()` werden ohne Anführungszeichen angehängt, weil
sie in JSON auch ohne stehen.

#### /api/vehicles: die Fahrzeugliste

[main.cpp:143](../src/cashdesk/main.cpp#L143)

```cpp
static void handleVehicleList() {
  String activeVehicles;
  String history;

  for (uint8_t i = 0; i < MAX_PARKING_RECORDS; i++) {
    const ParkingRecord* record = getParkingRecord(i);

    if (record == nullptr || record->status == RecordStatus::UNUSED) {
      continue;
    }

    if (record->status == RecordStatus::FINISHED) {
      if (history.length() > 0) {
        history += ',';
      }
      history += buildFinishedVehicleJson(*record);
    } else {
      if (activeVehicles.length() > 0) {
        activeVehicles += ',';
      }
      activeVehicles += buildActiveVehicleJson(*record);
    }
  }

  String response = F("{\"occupied\":");
  response += countOccupiedSpots();
  // ... totalSpots, free, clockIsSet, now, testBuild ...
  response += F(",\"vehicles\":[");
  response += activeVehicles;
  response += F("],\"history\":[");
  response += history;
  response += F("]}");

  sendJsonResponse(200, response);
}
```

Die Funktion geht alle 24 Karten durch und sortiert sie in zwei Listen: Autos, die gerade
drin sind, und ausgefahrene Autos. Unbenutzte Karten werden mit `continue` übersprungen.

**Das Komma:** In einer JSON-Liste stehen Kommas **zwischen** den Einträgen, nicht davor und
nicht danach. `[A,B,C]` ist richtig, `[,A,B,C]` ist kaputt. Deshalb kommt vor jeden neuen
Eintrag ein Komma, aber nur, wenn schon einer drinsteht.

Die beiden Helfer `buildFinishedVehicleJson` und `buildActiveVehicleJson` bauen den Text für
ein einzelnes Auto. Bei einem Auto im Parkhaus wird die Dauer bis jetzt gerechnet, bei einem
ausgefahrenen die Dauer von Einfahrt bis Ausfahrt. Bei einem bezahlten Auto steht wieder der
eingefrorene Betrag statt einer neuen Rechnung.

Ein Teil der Antwort hängt vom Test-Build ab:

```cpp
#if TEST_BUILD
  response += F("true");
#else
  response += F("false");
#endif
```

`#else` ist der „sonst"-Teil. Je nach Build bleibt genau eine der beiden Zeilen übrig. So
erfährt die Webseite, ob sie die Testwerkzeuge anzeigen soll.

Eine Antwort sieht dann so aus:

```json
{"occupied":1,"totalSpots":3,"free":2,"clockIsSet":true,"now":"22.10.2026 14:30:00",
 "testBuild":true,
 "vehicles":[{"plate":"LU 12345","entry":"22.10.2026 13:10:00","status":"unpaid",
              "exit":"-","duration":"1 h 20 min","fee":"CHF 4.00"}],
 "history":[]}
```

#### /api/status: Auskunft zu einem Auto

[main.cpp:203](../src/cashdesk/main.cpp#L203)

```cpp
static void handleStatusRequest() {
  ParkingInfo info = lookupParkingInfo(server.arg("plate"));

  bool plateIsUnknown = (info.state == PaymentState::PLATE_UNKNOWN);

  String response = F("{\"plate\":\"");
  response += escapeForJson(info.plate);
  response += F("\",\"status\":\"");
  response += paymentStateToText(info.state);
  response += F("\",\"entry\":\"");
  response += plateIsUnknown ? String(F("-")) : formatDateAndTime(info.entryUptimeSecond);
  // ... duration und fee genauso ...
  response += F("\",\"feeInCents\":");
  response += info.feeInCents;
  response += F("}");

  sendJsonResponse(200, response);
}
```

**`server.arg("plate")`** holt den Wert des Parameters `plate` aus der Adresse. Bei
`/api/status?plate=LU%2012345` liefert es `"LU 12345"` (der Webserver wandelt `%20` schon
zurück in ein Leerzeichen).

Bei einem unbekannten Kennzeichen gibt es keine sinnvollen Zeiten, also steht dort ein Strich
statt irreführender Nullen. `String(F("-"))` ist nötig, weil beide Seiten des `? :` vom
selben Typ sein müssen, und `formatDateAndTime` liefert einen `String`.

Diesen Endpunkt fragt **auch Pico 2** vor jeder Ausfahrt. Für ihn zählt nur das Feld
`status`.

#### /api/entry: die Einfahrt

[main.cpp:230](../src/cashdesk/main.cpp#L230)

```cpp
static void handleEntryRequest() {
  String plate;
  String errorMessage;

  EntryResult result = registerEntry(server.arg("plate"), plate, errorMessage);

  bool accepted = (result == EntryResult::ACCEPTED);

  String message;
  if (accepted) {
    message = String(F("Einfahrt registriert für ")) + plate + F(".");
  } else if (result == EntryResult::GARAGE_FULL) {
    message = F("Das Parkhaus ist voll. Keine weitere Einfahrt möglich.");
  } else {
    message = errorMessage;
  }

  // Antwort: ok, code, plate, message, occupied, totalSpots
  ...
}
```

Die eigentliche Arbeit macht `registerEntry` aus [3.7](#37-parking_garage-das-gedächtnis-der-kasse).
Diese Funktion übersetzt nur das Ergebnis in eine Antwort. `else if` ist eine weitere
Bedingung, die nur geprüft wird, wenn die vorherige nicht stimmte.

Die Antwort enthält die aktuelle Belegung. So kann Pico 2 seine Anzeige sofort
aktualisieren, ohne extra nachzufragen.

#### /api/pay und /api/exit

`handlePaymentRequest` ([main.cpp:268](../src/cashdesk/main.cpp#L268)) ruft `payParkingFee`
auf und wählt eine von drei Meldungen: unbekannt, bereits bezahlt (mit Betrag) oder Bezahlung
erfolgt (mit Betrag).

`handleExitRequest` ([main.cpp:305](../src/cashdesk/main.cpp#L305)) ruft `finishParking` auf
und meldet, ob der Platz freigegeben wurde. Pico 2 ruft diesen Endpunkt erst auf, wenn er
vorher den Status `paid` gesehen hat.

#### /api/setclock: die Uhr stellen

[main.cpp:325](../src/cashdesk/main.cpp#L325)

```cpp
static void handleSetClock() {
  if (!server.hasArg("epoch")) {
    sendJsonResponse(400, F("{\"ok\":false,\"message\":\"Parameter epoch fehlt.\"}"));
    return;
  }

  uint32_t epochSeconds = (uint32_t)strtoul(server.arg("epoch").c_str(), nullptr, 10);

  if (epochSeconds < 1600000000UL) {
    sendJsonResponse(400, F("{\"ok\":false,\"message\":\"Unplausible Zeitangabe.\"}"));
    return;
  }

  if (!isWallClockSet()) {
    setWallClock(epochSeconds);
    // ... Ausgabe im Monitor ...
  }

  sendJsonResponse(200, F("{\"ok\":true}"));
}
```

- `server.hasArg("epoch")` prüft, ob der Parameter überhaupt mitgeschickt wurde. Wenn nicht:
  Statuscode **400**, „deine Anfrage ist kaputt".
- **`strtoul`** (string to unsigned long) macht aus dem Text `"1792710000"` eine Zahl. Was
  in einer Adresse steht, ist immer Text, auch wenn es wie eine Zahl aussieht. Die `10` heisst
  „im Zehnersystem".
- **Plausibilitätsprüfung:** 1 600 000 000 ist der 13. September 2020. Jeder vernünftige
  Zeitstempel liegt danach. Ein kleinerer Wert heisst, dass etwas schiefging. Ohne diese
  Prüfung stünden alle Zeiten im Jahr 1970.
- **Nur einmal stellen.** Würde jeder Seitenaufruf die Uhr neu setzen, könnte ein zweiter
  Laptop mit falsch gestellter Uhr alle Anzeigen verschieben.

#### Die Testwerkzeuge und der Rest

`handleDebugMoveTime` und `handleDebugReset` stehen zwischen `#if TEST_BUILD` und `#endif`
und rufen die gleichnamigen Funktionen aus `parking_garage` auf.

`handleUnknownEndpoint` antwortet auf jeden unbekannten Pfad mit Statuscode **404**. Ohne sie
bekäme der Browser bei einem Tippfehler eine leere Antwort, und man wüsste nicht, was los ist.

**Zusammenfassung:** `setup()` richtet alles ein und meldet die Endpunkte im Telefonbuch des
Webservers an. `loop()` nimmt ständig Anfragen entgegen, ohne je zu warten. Jeder Endpunkt
holt seine Parameter mit `server.arg`, ruft eine Funktion aus `parking_garage` auf und
schickt das Ergebnis als JSON zurück.

---

### 3.10 barrier: Die Schranke als Objekt

**Wozu:** Eine Schranke öffnen und nach 5 Sekunden von selbst schliessen, **ohne dabei zu
warten**. **Wer benutzt sie:** nur Pico 2.

Hier kommt das letzte grosse Sprachmittel von C++: die **Klasse**.

#### Was eine Klasse ist

Bisher waren Daten und Funktionen getrennt. Die Karteikarten lagen in `g_records`, die
Funktionen, die mit ihnen arbeiten, standen daneben. Eine **Klasse** packt beides zusammen:
die Daten **und** die Funktionen, die dazu gehören.

Eine Klasse ist ein **Bauplan**. Nach dem Bauplan kann man beliebig viele **Objekte** bauen.
Jedes Objekt hat seine eigenen Daten. Der Bauplan „Schranke" sagt: Jede Schranke hat einen
Servo, eine Pin-Nummer, einen Zustand (offen oder zu) und einen Zeitpunkt, seit dem sie
offen ist. Und jede Schranke kann `open()` und `update()`. Baut man zwei Schranken, hat
jede ihren eigenen Zustand.

Du hast schon Objekte benutzt, ohne es zu wissen: `String`, `WiFi`, `server` und `Serial`
sind alles Objekte von Klassen aus Bibliotheken.

#### Der Bauplan: barrier.h

Datei: [src/barriers/barrier.h](../src/barriers/barrier.h)

```cpp
class Barrier {
public:
  Barrier(uint8_t servoPin, const char* name);

  void begin();
  void open();
  void update();
  bool isOpen() const;
  uint32_t millisUntilClose() const;

private:
  void closeNow();
  bool openTimeIsOver() const;

  Servo       _servo;
  uint8_t     _servoPin;
  const char* _name;
  bool        _isOpen;
  uint32_t    _openedAtMs;
};
```

**`public:`** und **`private:`** teilen die Klasse in zwei Bereiche:

- **public** (öffentlich): Das darf jeder benutzen. Hier stehen die Methoden, mit denen man
  die Schranke bedient.
- **private** (privat): Das darf nur die Klasse selbst benutzen. Hier stehen die inneren
  Daten und zwei Hilfsmethoden.

Warum verstecken? Damit niemand von aussen zum Beispiel `_isOpen = false` setzt, ohne dass
der Servo tatsächlich zufährt. Dann würden Anzeige und Wirklichkeit auseinanderlaufen. Wer
die Schranke bedienen will, muss die öffentlichen Methoden benutzen, und die halten Zustand
und Servo immer zusammen. Das nennt man **Kapselung**.

**Die Daten der Schranke** (man nennt sie **Member-Variablen**):

| Variable | Bedeutung |
|---|---|
| `_servo` | ein Objekt der Servo-Bibliothek, das den Motor steuert |
| `_servoPin` | GPIO-Nummer des Signalkabels |
| `_name` | Name für die Ausgabe im Monitor, z. B. `"Schranke"` |
| `_isOpen` | steht der Arm gerade oben? |
| `_openedAtMs` | wann wurde er geöffnet, als `millis()`-Wert |

Der **Unterstrich am Anfang** ist eine Konvention in diesem Projekt. Er zeigt beim Lesen
einer Methode sofort: Dieser Wert gehört zum Objekt und lebt so lange wie die Schranke.

Ein Objekt in einem Objekt: `_servo` ist selbst ein Objekt der Klasse `Servo`. Unsere
Schranke **hat** einen Servo.

**`Barrier(uint8_t servoPin, const char* name);`** ist der **Konstruktor**. Er heisst genau
wie die Klasse und hat keinen Rückgabetyp, nicht einmal `void`. Er läuft automatisch, wenn
ein Objekt gebaut wird, und richtet es ein.

**`const` hinter einer Methode**, wie bei `bool isOpen() const;`, ist ein Versprechen: „Diese
Methode verändert nichts am Objekt, sie schaut nur." Der Compiler prüft das.

#### Die Umsetzung: barrier.cpp

Datei: [src/barriers/barrier.cpp](../src/barriers/barrier.cpp)

##### Der Konstruktor

[barrier.cpp:15](../src/barriers/barrier.cpp#L15)

```cpp
Barrier::Barrier(uint8_t servoPin, const char* name)
  : _servoPin(servoPin),
    _name(name),
    _isOpen(false),
    _openedAtMs(0) {
}
```

**`Barrier::`** vor dem Namen heisst „gehört zur Klasse `Barrier`". So weiss der Compiler,
dass hier die Methode der Klasse gemeint ist und keine lose Funktion.

Was nach dem **Doppelpunkt** steht, heisst **Initialisierungsliste**. Die Member-Variablen
bekommen ihre Startwerte direkt beim Entstehen des Objekts. Das ist besser, als sie im Rumpf
zuzuweisen: Dort hätten sie für einen kurzen Moment einen zufälligen Wert. Der Rumpf in den
geschweiften Klammern ist leer, weil alles schon erledigt ist.

Gebaut wird eine Schranke so (in `barriers/main.cpp`):

```cpp
static Barrier entryBarrier(SERVO_PIN_ENTRY, "Schranke");
```

Typ `Barrier`, Name `entryBarrier`, und in Klammern die Werte für den Konstruktor.

Der Servo wird im Konstruktor **noch nicht** angesprochen. Solche Objekte ausserhalb von
Funktionen entstehen nämlich schon, bevor `setup()` läuft. Zu diesem Zeitpunkt ist die
Hardware des Pico noch nicht bereit. Deshalb gibt es eine eigene Methode `begin()`.

##### begin: den Servo anmelden

[barrier.cpp:24](../src/barriers/barrier.cpp#L24)

```cpp
void Barrier::begin() {
  _servo.attach(_servoPin);
  _servo.write(SERVO_ANGLE_CLOSED);
  _isOpen = false;
  // ... Ausgabe im Monitor ...
}
```

- **`_servo.attach(8)`** verbindet das Servo-Objekt mit GP8 (die 8 kommt aus
  `SERVO_PIN_ENTRY` in `config.h`). Ab jetzt schickt der Pico
  auf diesem Pin alle 20 Millisekunden den Steuerimpuls (siehe [1.3](#13-die-geräte)).
- **`_servo.write(SERVO_ANGLE_CLOSED)`** fährt den Arm in die geschlossene Stellung, bei
  uns 90 Grad. Beim Einschalten weiss der Servo
  nicht, wo er steht. Ohne diese Zeile bliebe der Arm dort, wo ihn jemand zuletzt von Hand
  hingedreht hat.

##### open: öffnen, ohne zu warten

[barrier.cpp:42](../src/barriers/barrier.cpp#L42)

```cpp
void Barrier::open() {
  _servo.write(SERVO_ANGLE_OPEN);
  _openedAtMs = millis();
  _isOpen     = true;
  // ... Ausgabe im Monitor ...
}
```

Arm in die offene Stellung (`SERVO_ANGLE_OPEN`, bei uns 0 Grad), **die Uhrzeit merken**,
Zustand auf offen. Und dann kehrt die Methode
**sofort** zurück. Sie wartet nicht die 5 Sekunden ab.

##### update: nachschauen, ob es Zeit ist

[barrier.cpp:55](../src/barriers/barrier.cpp#L55)

```cpp
void Barrier::update() {
  if (!_isOpen) {
    return;
  }

  if (openTimeIsOver()) {
    closeNow();
  }
}

bool Barrier::openTimeIsOver() const {
  return (millis() - _openedAtMs) >= BARRIER_OPEN_TIME_MS;
}

void Barrier::closeNow() {
  _servo.write(SERVO_ANGLE_CLOSED);
  _isOpen = false;
  // ... Ausgabe im Monitor ...
}
```

`update()` wird in **jedem** Durchlauf von `loop()` aufgerufen. Es schaut nur kurz nach:

1. Ist die Schranke zu? Dann gibt es nichts zu tun.
2. Sind seit dem Öffnen 5000 Millisekunden vergangen? Dann zufahren.

Das ist wieder das Muster aus [3.6](#36-network-ins-wlan-kommen-und-drin-bleiben): merken,
wann etwas passiert ist, und bei jedem Durchlauf nachschauen. So sieht das über die Zeit aus:

```text
Zeit in ms:   0          1         2   ...   4999      5000
              │          │         │          │          │
              open()     update()  update()   update()   update()
              Arm hoch   zu früh   zu früh    zu früh    Zeit um: closeNow()
                                                         Arm runter

Dazwischen läuft loop() tausende Male und beantwortet Webanfragen,
prüft das WLAN, fragt die Kasse. Nichts davon muss warten.
```

Zum Vergleich, so hätte man es **nicht** machen dürfen:

```cpp
servo.write(90);
delay(5000);       // 5 Sekunden lang ist der Pico taub und stumm
servo.write(0);
```

Kürzer, aber 5 Sekunden lang würde der Pico keine Webanfrage beantworten. Die Seite der
Schranke würde einfrieren, als wäre das Gerät abgestürzt.

Eine Sache, die sich Zustand und Uhrzeit merkt und bei jedem Aufruf entscheidet, was als
Nächstes passiert, nennt man **Zustandsmaschine**. Unsere ist sehr einfach: Sie hat nur die
Zustände „offen" und „zu".

##### Wie lange noch offen?

[barrier.cpp:73](../src/barriers/barrier.cpp#L73)

```cpp
uint32_t Barrier::millisUntilClose() const {
  if (!_isOpen) {
    return 0;
  }

  uint32_t elapsed = millis() - _openedAtMs;

  if (elapsed >= BARRIER_OPEN_TIME_MS) {
    return 0;
  }

  return BARRIER_OPEN_TIME_MS - elapsed;
}
```

Daraus zeichnet die Webseite den ablaufenden grünen Balken. Die mittlere Prüfung ist wieder
der Schutz vor dem Überlauf: Wäre `elapsed` schon 5100, ergäbe `5000 - 5100` bei einer
vorzeichenlosen Zahl nicht -100, sondern rund 4,29 Milliarden. Der Balken wäre
kilometerlang. (Das kann kurz vorkommen, wenn `update()` den Arm noch nicht geschlossen hat.)

`isOpen()` gibt einfach `_isOpen` zurück.

**Zusammenfassung:** Eine Klasse ist ein Bauplan für Objekte, die Daten und Methoden
zusammenpacken. Neu waren `class`, `public`/`private`, Konstruktor, Initialisierungsliste,
`Klasse::Methode` und `const`-Methoden. Die Schranke ist eine Zustandsmaschine, die nie wartet.

---

### 3.11 cashdesk_client: Pico 2 ruft die Kasse an

**Wozu:** Alles, was Pico 2 braucht, um die Kasse etwas zu fragen. **Wer benutzt sie:** nur
das Hauptprogramm von Pico 2.

#### Die Speisekarte: cashdesk_client.h

Datei: [src/barriers/cashdesk_client.h](../src/barriers/cashdesk_client.h)

Für jede Art von Frage gibt es ein `struct` für die Antwort:

```cpp
struct EntryReply {
  bool   cashDeskReachable;   // hat die Kasse überhaupt geantwortet?
  bool   accepted;            // hat sie die Einfahrt erlaubt?
  String code;                // ok, full, already, invalid oder memory
  String message;             // fertiger Text für die Webseite
  int    occupied;            // belegte Plätze laut Kasse
  int    totalSpots;          // Plätze insgesamt laut Kasse
};

struct StatusReply {
  bool   cashDeskReachable;
  String paymentState;   // unknown, unpaid oder paid
  String plate;
  String fee;            // zum Beispiel "CHF 4.00"
};

struct OccupancyReply {
  bool cashDeskReachable;
  int  occupied;
  int  totalSpots;
};
```

Jede Antwort hat als erstes Feld **`cashDeskReachable`**. Damit lassen sich zwei völlig
verschiedene Fälle auseinanderhalten:

| Fall | `cashDeskReachable` | `accepted` | Was wir wissen |
|---|---|---|---|
| Kasse aus oder WLAN weg | `false` | (egal) | **gar nichts** |
| Kasse sagt Nein, z. B. voll | `true` | `false` | **genau Bescheid** |
| Kasse sagt Ja | `true` | `true` | alles gut |

In beiden ersten Fällen bleibt die Schranke zu, aber aus verschiedenen Gründen. Der Benutzer
bekommt deshalb auch verschiedene Meldungen: „Kasse nicht erreichbar" ist etwas anderes als
„Parkhaus voll".

Dazu vier Funktionen, eine pro Frage:

```cpp
EntryReply     askCashDeskToRegisterEntry(const String& plate);   // darf es rein?
StatusReply    askCashDeskForStatus(const String& plate);         // hat es bezahlt?
bool           askCashDeskToFinishParking(const String& plate);   // es fährt jetzt raus
OccupancyReply askCashDeskForOccupancy();                         // wie voll ist es?
```

#### Die Küche: cashdesk_client.cpp

Datei: [src/barriers/cashdesk_client.cpp](../src/barriers/cashdesk_client.cpp)

##### Text sicher für eine Webadresse machen

[cashdesk_client.cpp:20](../src/barriers/cashdesk_client.cpp#L20)

```cpp
String encodeForUrl(const String& text) {
  String result;
  result.reserve(text.length() * 3);

  for (unsigned int i = 0; i < text.length(); i++) {
    char currentChar = text.charAt(i);

    bool isSafeCharacter = isAlphaNumeric(currentChar)
                        || currentChar == '-'
                        || currentChar == '_'
                        || currentChar == '.'
                        || currentChar == '~';

    if (isSafeCharacter) {
      result += currentChar;
    } else {
      char buffer[4];
      snprintf(buffer, sizeof(buffer), "%%%02X", (uint8_t)currentChar);
      result += buffer;
    }
  }

  return result;
}
```

In einer Webadresse haben manche Zeichen eine eigene Bedeutung: `?` leitet die Parameter
ein, `&` trennt sie, und Leerzeichen sind gar nicht erlaubt. Diese Funktion lässt Buchstaben,
Ziffern und vier harmlose Zeichen durch. Alles andere wird zu einem **Prozentzeichen mit der
Zeichennummer in Hexadezimal**.

Jedes Zeichen hat eine Nummer. Das Leerzeichen hat die Nummer 32, das ist hexadezimal `20`.
So entsteht aus dem Leerzeichen `%20`. Der Platzhalter `"%%%02X"` zerlegt:

- `%%` schreibt ein einzelnes Prozentzeichen,
- `%02X` schreibt die Zahl hexadezimal mit zwei Stellen in Grossbuchstaben.

Aus `"LU 12345"` wird `"LU%2012345"`. Diese Arbeit macht in der Webseite
`encodeURIComponent`, hier im C++-Code müssen wir es selbst tun.

`text.length() * 3` reserviert genug Platz für den schlimmsten Fall, in dem jedes Zeichen zu
drei Zeichen wird.

##### Eine Anfrage an die Kasse schicken

[cashdesk_client.cpp:67](../src/barriers/cashdesk_client.cpp#L67)

```cpp
String sendRequestToCashDesk(const String& path, bool& reachableOut) {
  reachableOut = false;

  if (!isWifiConnected()) {
    Serial.println(F("[KASSE-CLIENT] Kein WLAN, Aufruf uebersprungen."));
    return String();
  }

  WiFiClient client;
  client.setTimeout(HTTP_TIMEOUT_MS);

  HTTPClient http;
  http.setTimeout(HTTP_TIMEOUT_MS);
  http.setReuse(false);

  String url = buildCashDeskBaseUrl() + path;

  if (!http.begin(client, url)) {
    Serial.println(F("[KASSE-CLIENT] Aufruf konnte nicht gestartet werden."));
    return String();
  }

  int statusCode = http.GET();

  String responseBody;

  if (statusCode > 0) {
    responseBody = http.getString();
    reachableOut = true;
  } else {
    Serial.print(F("[KASSE-CLIENT] Kasse nicht erreichbar, Fehler "));
    Serial.println(statusCode);
  }

  http.end();

  return responseBody;
}
```

Das ist der Moment, in dem Pico 2 zum **Client** wird. Schritt für Schritt:

1. **Ohne WLAN gar nicht erst versuchen.** Das spart im Fehlerfall die vollen 2 Sekunden
   Wartezeit.
2. **`WiFiClient client;`** ist die Netzwerkverbindung selbst, eine Leitung zur Kasse.
   `setTimeout(2000)` begrenzt auch den Verbindungsaufbau auf 2 Sekunden. Ohne diese Grenze
   könnte ein Aufruf bei ausgeschaltetem Router viel länger hängen.
3. **`HTTPClient http;`** spricht HTTP über diese Leitung. Es baut die Anfrage
   `GET /api/... HTTP/1.1` zusammen und zerlegt die Antwort.
4. **`setReuse(false)`**: Nach jeder Anfrage die Verbindung schliessen, statt sie für die
   nächste offen zu halten. Auf einem Gerät mit so wenig Speicher ist das sicherer.
5. **`http.begin(client, url)`** bereitet die Anfrage vor, zum Beispiel an
   `http://192.168.89.211/api/status?plate=LU%2012345`.
6. **`http.GET()`** schickt sie ab und wartet auf die Antwort, höchstens 2 Sekunden. Es
   liefert den **Statuscode** zurück, zum Beispiel 200. Bei einem Fehler (keine Antwort,
   Zeit abgelaufen) eine **negative** Zahl.
7. Bei Erfolg: den Inhalt mit **`http.getString()`** holen und `reachableOut` auf `true`
   setzen.
8. **`http.end()`** gibt die Verbindung frei.

Zum letzten Punkt: `http.end()` steht absichtlich **nach** dem `if`/`else` und nicht darin.
So wird es in **jedem** Fall erreicht. Würde es im Fehlerfall übersprungen, bliebe bei jedem
Fehlversuch eine Verbindung offen und belegte Speicher. Nach ein paar Dutzend Fehlversuchen
ginge dem Pico der Speicher aus. Dieser Fehler heisst **Ressourcenleck**.

`buildCashDeskBaseUrl()` ist eine kleine Hilfsfunktion, die `"http://"` und die IP-Adresse
der Kasse aus `config.h` zusammensetzt.

##### Einen Wert aus der Antwort lesen

[cashdesk_client.cpp:159](../src/barriers/cashdesk_client.cpp#L159)

```cpp
String readJsonField(const String& json, const String& fieldName) {
  String searchPattern = String('"') + fieldName + String("\":");

  int patternPosition = json.indexOf(searchPattern);
  if (patternPosition < 0) {
    return String();
  }

  int valueStart = patternPosition + searchPattern.length();

  if (valueStart < (int)json.length() && json.charAt(valueStart) == '"') {
    return readQuotedValue(json, valueStart);
  }

  return readPlainValue(json, valueStart);
}
```

Pico 2 bekommt die Antwort der Kasse als Text. Er braucht daraus einzelne Werte, zum Beispiel
das Feld `status`. Diese Funktion sucht es heraus. Spielen wir das durch mit der Antwort:

```text
{"ok":true,"code":"full","message":"Das Parkhaus ist voll."}
```

**Gesucht: `code`.**

1. Das Suchmuster wird gebaut: `"code":` (Name in Anführungszeichen plus Doppelpunkt).
2. `indexOf` findet es an Stelle 11.
3. Dahinter beginnt der Wert, an Stelle 11 + 7 = 18.
4. Dort steht ein `"`, also ist der Wert ein Text. `readQuotedValue` liest bis zum nächsten
   `"` und liefert **`full`**.

**Gesucht: `ok`.**

1. Suchmuster `"ok":`, gefunden an Stelle 1, Wert beginnt an Stelle 6.
2. Dort steht ein `t`, kein Anführungszeichen. `readPlainValue` liest bis zum nächsten
   Komma, `}` oder `]` und liefert **`true`**.

Das ist kein vollwertiger JSON-Leser. Er würde zum Beispiel durcheinanderkommen, wenn ein
Text selbst ein Anführungszeichen enthielte. Er funktioniert hier nur deshalb zuverlässig,
weil wir **beide Seiten selbst geschrieben haben** und genau wissen, wie die Antworten
aussehen. Für fremde Daten würde man eine fertige JSON-Bibliothek nehmen.

##### Die vier Fragen an die Kasse

[cashdesk_client.cpp:180](../src/barriers/cashdesk_client.cpp#L180)

```cpp
EntryReply askCashDeskToRegisterEntry(const String& plate) {
  EntryReply reply;

  reply.cashDeskReachable = false;
  reply.accepted          = false;
  reply.occupied          = -1;
  reply.totalSpots        = MAX_PARKING_SPOTS;

  String path = String(F("/api/entry?plate=")) + encodeForUrl(plate);
  String json = sendRequestToCashDesk(path, reply.cashDeskReachable);

  if (!reply.cashDeskReachable) {
    reply.message = F("Die Kasse ist zurzeit nicht erreichbar. Die Einfahrt kann nicht registriert werden.");
    return reply;
  }

  reply.accepted   = (readJsonField(json, "ok") == "true");
  reply.code       = readJsonField(json, "code");
  reply.message    = readJsonField(json, "message");
  reply.occupied   = readJsonField(json, "occupied").toInt();
  reply.totalSpots = readJsonField(json, "totalSpots").toInt();

  return reply;
}
```

Alle vier Funktionen folgen demselben Muster:

1. Antwort-Bündel mit sicheren Startwerten füllen („nichts gehört").
2. Den Pfad zusammenbauen, das Kennzeichen dabei mit `encodeForUrl` sichern.
3. Anfrage schicken. `reply.cashDeskReachable` wird dabei als Ausgabeparameter direkt befüllt.
4. Keine Antwort? Dann mit dem „nichts gehört"-Bündel zurück.
5. Sonst die Felder aus dem JSON lesen.

`readJsonField(json, "ok") == "true"` macht aus dem Text `"true"` einen echten `bool`.
**`.toInt()`** macht aus einem Text wie `"2"` die Zahl 2.

`askCashDeskForStatus` holt `status`, `plate` und `fee`. `askCashDeskToFinishParking` gibt
nur `true` oder `false` zurück. `askCashDeskForOccupancy` holt `occupied` und `totalSpots`
aus `/api/vehicles`.

**Zusammenfassung:** Pico 2 wird hier zum Client. Jede Anfrage hat eine Obergrenze von
2 Sekunden und gibt die Verbindung immer wieder frei. Die Antworten unterscheiden sauber
zwischen „keine Antwort" und „Antwort ist Nein".

---

### 3.12 web_page_barriers.h: Die Webseite der Schranke

**Wozu:** Die Seite unter `http://192.168.89.212`, mit einem Feld für die Einfahrt und einem
für die Ausfahrt. **Diese beiden Felder ersetzen die Kamera.**

Datei: [src/barriers/web_page_barriers.h](../src/barriers/web_page_barriers.h)

Der Aufbau ist genau wie bei der Kassenseite aus [3.8](#38-web_page_cashdeskh-die-webseite-der-kasse):
ein Rohtext mit HTML, CSS und JavaScript. Ich zeige nur, was hier neu ist.

#### Knöpfe sperren, solange eine Anfrage läuft

```javascript
function setButtonsDisabled(disabled) {
  document.getElementById("entryButton").disabled = disabled;
  document.getElementById("exitButton").disabled = disabled;
}
```

Eine Ausfahrt dauert einen Moment, weil Pico 2 erst die Kasse fragen muss. Ungeduldige
Menschen klicken dann gern mehrmals. Damit nicht mehrere Anfragen gleichzeitig losgehen,
werden beide Knöpfe gesperrt, bis die Antwort da ist.

#### Einfahrt und Ausfahrt

```javascript
async function requestEntry() {
  const plate = document.getElementById("entryPlate").value;

  showMessage("entryMessage", "Anfrage läuft ...", "warn");
  setButtonsDisabled(true);

  try {
    const reply = await (await fetch("/api/entry?plate=" + encodeURIComponent(plate))).json();
    showMessage("entryMessage", reply.message, reply.ok ? "ok" : "error");

    if (reply.ok) {
      document.getElementById("entryPlate").value = "";
    }
  } catch (error) {
    showMessage("entryMessage", "Pico 2 antwortet nicht.", "error");
  }

  setButtonsDisabled(false);
  refreshPage();
}
```

1. Sofort eine orange Meldung „Anfrage läuft", damit man sieht, dass etwas passiert.
2. Knöpfe sperren.
3. Pico 2 fragen. Die Meldung, die zurückkommt, wird angezeigt: grün bei Erfolg, rot sonst.
4. Bei Erfolg das Feld leeren, damit beim nächsten Auto nicht das alte Kennzeichen stehen
   bleibt.
5. Antwortet **Pico 2 selbst** nicht, springt JavaScript in den `catch`-Block. Das ist ein
   anderer Fall als eine Absage der Kasse. Eine Absage der Kasse kommt ja als normale Antwort
   von Pico 2 mit `"ok":false`.
6. Knöpfe wieder freigeben, Seite auffrischen.

`requestExit` ist genauso aufgebaut, nur mit `/api/exit`.

#### Der ablaufende Balken

```javascript
const BARRIER_OPEN_TIME_MS = 5000;

function showBarrierState(prefix, isOpen, msLeft) {
  const stateElement = document.getElementById(prefix + "State");
  stateElement.textContent = isOpen ? "offen" : "geschlossen";
  stateElement.className = "state " + (isOpen ? "open" : "closed");

  const percentLeft = Math.round(msLeft / BARRIER_OPEN_TIME_MS * 100);
  document.getElementById(prefix + "Bar").style.width = isOpen ? percentLeft + "%" : "0%";
}
```

`prefix` ist `"entry"` oder `"exit"`. Aus `"entry" + "State"` wird der Elementname
`"entryState"`. So bedient eine Funktion beide Schranken.

Der Balken zeigt, wie viel der 5 Sekunden noch übrig ist. Bei 2500 Millisekunden Rest sind
das 2500 / 5000 × 100 = 50 Prozent. Die Breite des grünen Balkens wird auf `"50%"` gesetzt.

#### Die Beschriftung mit dem richtigen Pin

```javascript
function labelBarriers(data) {
  if (data.singleServo) {
    const label = "Schrankenarm (GP" + data.entryPin + ", gemeinsam)";
    document.getElementById("entryLabel").textContent = label;
    document.getElementById("exitLabel").textContent  = label;
    document.getElementById("testEntryButton").textContent = "Servo testen";
    document.getElementById("testExitButton").style.display = "none";
  } else {
    document.getElementById("entryLabel").textContent = "Einfahrtsschranke (GP" + data.entryPin + ")";
    document.getElementById("exitLabel").textContent  = "Ausfahrtsschranke (GP" + data.exitPin + ")";
  }
}
```

Die Pin-Nummern stehen nicht fest im HTML. Pico 2 schickt sie in `/api/state` als
`entryPin` und `exitPin` mit, und die stammen aus `config.h`. So gibt es die Nummer nur an
einer einzigen Stelle im ganzen Projekt. Steckt man den Servo um und ändert `config.h`,
zeigt die Seite automatisch den neuen Pin.

Meldet Pico 2 `"singleServo":true`, beschriftet die Seite beide Anzeigen als denselben Arm.
Sie täuscht keine zwei Schranken vor, die es nicht gibt.

#### Auffrischen jede Sekunde

```javascript
async function refreshPage() {
  // ... /api/state holen ...
  document.getElementById("occupancy").textContent =
    data.cashDeskOnline ? (data.occupied + " / " + data.totalSpots) : "unbekannt";
  // ... grüner oder roter Punkt für die Kasse, Zustand beider Schranken ...
}

refreshPage();
setInterval(refreshPage, 1000);
```

Ist die Kasse weg, steht bei der Belegung **„unbekannt"** und nicht etwa `0 / 3`. Eine Null
würde „das Parkhaus ist leer" bedeuten. Das wissen wir in diesem Moment aber gar nicht.

Die Seite fragt jede Sekunde (die Kasse nur alle 2), damit der Balken flüssig abläuft.

---

### 3.13 barriers/main.cpp: Das Hauptprogramm der Schranke

**Wozu:** Das Gegenstück zum Hauptprogramm der Kasse. Es startet Pico 2, beantwortet die
Anfragen seiner Webseite, fragt bei Bedarf die Kasse und bewegt den Servo. **Enthält
`setup()` und `loop()` der Schranke.**

Datei: [src/barriers/main.cpp](../src/barriers/main.cpp)

Die wichtigste Regel dieser Datei steht schon oben im Kommentar: **Die Schranke geht nur auf,
wenn alle Bedingungen erfüllt sind. Im Zweifel bleibt sie zu.**

#### Eine Schranke, zwei Namen

[main.cpp:52](../src/barriers/main.cpp#L52)

```cpp
#if SERVO_COUNT == 1
static Barrier  entryBarrier(SERVO_PIN_ENTRY, "Schranke");
static Barrier& exitBarrier = entryBarrier;
#else
static Barrier  entryBarrier(SERVO_PIN_ENTRY, "Einfahrt");
static Barrier  exitBarrier(SERVO_PIN_EXIT, "Ausfahrt");
#endif
```

Je nach `SERVO_COUNT` aus `config.h` bleibt einer der beiden Teile übrig.

**Mit einem Servo** (unser Fall) wird nur **ein** Objekt gebaut: `entryBarrier`. Die Zeile
`static Barrier& exitBarrier = entryBarrier;` legt eine **Referenz** an. `exitBarrier` ist
kein zweites Objekt, sondern **ein zweiter Name für dasselbe**. Ruft der Code
`exitBarrier.open()` auf, öffnet sich derselbe Arm wie bei `entryBarrier.open()`.

So kann der übrige Code ganz normal von Einfahrt und Ausfahrt reden. Er muss nicht wissen, ob
dahinter ein oder zwei Servos stecken.

**Warum nicht einfach zwei Objekte mit demselben Pin?** Das würde gebaut, wäre aber doppelt
kaputt:

1. Zwei `Servo`-Objekte würden sich um denselben Anschluss streiten.
2. Jedes hätte seine **eigene Fünf-Sekunden-Uhr**. Öffnet die Einfahrt und zwei Sekunden
   später die Ausfahrt, liefe die Uhr der Einfahrt zuerst ab und würde den Arm fallen lassen,
   obwohl die Ausfahrt ihn noch oben braucht.

Mit der Referenz gibt es genau einen Zustand und genau eine Uhr, wie in der Wirklichkeit
auch, denn es gibt ja nur einen Arm.

#### Der Zwischenspeicher für die Kasse

[main.cpp:72](../src/barriers/main.cpp#L72)

```cpp
static bool     g_cashDeskIsOnline = false;
static int      g_occupiedSpots    = 0;
static int      g_totalSpots       = MAX_PARKING_SPOTS;
static uint32_t g_lastPollMs       = 0;

constexpr uint32_t POLL_INTERVAL_NORMAL_MS  = 3000;
constexpr uint32_t POLL_INTERVAL_OFFLINE_MS = 10000;
```

Hier merkt sich Pico 2 den letzten bekannten Stand der Kasse: Ist sie erreichbar, wie viele
Plätze sind belegt? Warum, steht in [2.5 e](#e-was-im-hintergrund-ständig-passiert).

Antwortet die Kasse, fragt Pico 2 alle 3 Sekunden nach. Antwortet sie nicht, nur noch alle
10 Sekunden. Das nennt man **Zurückweichen** (englisch backoff). Jeder Fehlversuch kostet
2 Sekunden Wartezeit. Bei einer ausgefallenen Kasse alle 3 Sekunden 2 Sekunden zu verlieren,
wäre zu viel.

#### Kleine Helfer

```cpp
static void sendSimpleReply(bool wasSuccessful, const String& message) {
  String response = F("{\"ok\":");
  response += wasSuccessful ? F("true") : F("false");
  response += F(",\"message\":\"");
  response += escapeForJson(message);
  response += F("\"}");

  sendJsonResponse(200, response);
}
```

Fast alle Antworten von Pico 2 haben nur zwei Felder: `ok` und `message`. Diese Funktion baut
sie. `sendJsonResponse` ist dieselbe wie bei der Kasse.

[main.cpp:126](../src/barriers/main.cpp#L126)

```cpp
static bool readAndCheckPlate(String& plateOut) {
  PlateCheckResult checkResult = checkPlate(server.arg("plate"), plateOut);

  if (checkResult == PlateCheckResult::VALID) {
    return true;
  }

  Serial.print(F("[EINGABE] Abgelehnt, ungueltiges Kennzeichen. Laenge: "));
  Serial.println(server.arg("plate").length());

  sendSimpleReply(false, plateErrorMessage(checkResult));
  return false;
}
```

Holt das Kennzeichen aus der Anfrage und prüft es. Ist es gültig: `true`, und in `plateOut`
steht die bereinigte Schreibweise. Ist es ungültig, **schickt die Funktion die
Fehlerantwort gleich selbst** und gibt `false` zurück. Der Aufrufer muss dann nur noch
aussteigen.

Diese Funktion gibt es, weil Einfahrt und Ausfahrt genau dieselbe Prüfung am Anfang brauchen.
Einmal geschrieben statt zweimal heisst: Eine spätere Änderung kann nicht an einer der beiden
Stellen vergessen werden.

Im Monitor wird nur die **Länge** der Eingabe ausgegeben, nicht die Eingabe selbst. Bei
seltsamen Eingaben mit Steuerzeichen bliebe der Monitor so lesbar.

[main.cpp:149](../src/barriers/main.cpp#L149)

```cpp
static void refreshOccupancyFromCashDesk(bool forceNow) {
  uint32_t interval = g_cashDeskIsOnline ? POLL_INTERVAL_NORMAL_MS
                                         : POLL_INTERVAL_OFFLINE_MS;

  if (!forceNow && (millis() - g_lastPollMs) < interval) {
    return;
  }

  g_lastPollMs = millis();

  OccupancyReply reply = askCashDeskForOccupancy();
  g_cashDeskIsOnline = reply.cashDeskReachable;

  if (reply.cashDeskReachable) {
    g_occupiedSpots = reply.occupied;
    g_totalSpots    = reply.totalSpots;
  }
}
```

Frischt den Zwischenspeicher auf. Mit `forceNow` gleich `true` sofort, sonst nur, wenn das
Intervall abgelaufen ist. Die Zahlen werden **nur** übernommen, wenn die Kasse wirklich
geantwortet hat. Sonst würden wir die letzten bekannten Werte mit Nullen überschreiben.

#### Die Einfahrt

[main.cpp:187](../src/barriers/main.cpp#L187)

```cpp
static void handleEntryRequest() {
  String plate;

  // Grund 1: Das Kennzeichen ist ungültig.
  if (!readAndCheckPlate(plate)) {
    return;
  }

  EntryReply reply = askCashDeskToRegisterEntry(plate);

  // Grund 2: Die Kasse antwortet nicht.
  if (!reply.cashDeskReachable) {
    sendSimpleReply(false, reply.message);
    return;
  }

  g_cashDeskIsOnline = true;
  g_occupiedSpots    = reply.occupied;
  g_totalSpots       = reply.totalSpots;

  // Grund 3: Die Kasse sagt Nein.
  if (!reply.accepted) {
    sendSimpleReply(false, reply.message);
    return;
  }

  entryBarrier.open();

  String message = plate;
  message += F(" ist registriert. Die Einfahrtsschranke öffnet und schliesst nach 5 Sekunden wieder.");
  sendSimpleReply(true, message);
}
```

**Der Servo wird erst in der vorletzten Zeile angesprochen.** Davor stehen drei Gründe, aus
denen die Schranke zubleibt, jeder mit eigenem `return`. Wer die Funktion liest, sieht auf
einen Blick alle Fälle.

Zu Grund 2: Ohne Kasse kein Parkvorgang, und ohne Parkvorgang darf niemand hinein. Sonst
stünde ein Auto im Parkhaus, von dem niemand weiss. Es könnte nie bezahlen und käme nie
wieder raus.

Ist die Kasse erreichbar, werden die Werte im Zwischenspeicher gleich aktualisiert. Die
Antwort der Kasse enthält ja die neue Belegung.

#### Die Ausfahrt: die heikelste Funktion

[main.cpp:231](../src/barriers/main.cpp#L231)

Hier wird über Geld entschieden. Die Funktion prüft deshalb vier Dinge nacheinander und
bricht bei jedem Zweifel ab:

```cpp
static void handleExitRequest() {
  String plate;

  // Prüfung 1: Ist das Kennzeichen gültig?
  if (!readAndCheckPlate(plate)) {
    return;
  }

  StatusReply reply = askCashDeskForStatus(plate);

  // Prüfung 2: Hat die Kasse geantwortet?
  if (!reply.cashDeskReachable) {
    g_cashDeskIsOnline = false;
    sendSimpleReply(false, F("Die Kasse ist zurzeit nicht erreichbar. ..."));
    return;
  }

  g_cashDeskIsOnline = true;

  // Prüfung 3: Kennt die Kasse dieses Kennzeichen?
  if (reply.paymentState == "unknown") {
    // Meldung: "Zu dem Kennzeichen ... ist keine Einfahrt bekannt."
    return;
  }

  // Prüfung 4: Ist wirklich bezahlt?
  if (reply.paymentState != "paid") {
    // Meldung: "Die Parkgebühr für ... ist noch offen (CHF ...)."
    return;
  }

  // Erst den Vorgang bei der Kasse abschliessen ...
  if (!askCashDeskToFinishParking(plate)) {
    sendSimpleReply(false, F("Die Kasse konnte den Parkvorgang nicht abschliessen. ..."));
    return;
  }

  // ... und dann öffnen.
  exitBarrier.open();
  refreshOccupancyFromCashDesk(true);

  // Meldung: "Bezahlung bestätigt. ..."
}
```

**Prüfung 3** fängt den Fall ab, in dem sich jemand vertippt. Oder bei einer echten Kamera:
in dem sie falsch liest. Das Auto hat als `BE 55511` bezahlt, an der Ausfahrt kommt aber
`BE 55512` an. Das Format ist gültig, die Kasse kennt das Kennzeichen aber nicht. Die
Schranke bleibt zu, und die Meldung sagt genau, woran es liegt. Mit dem richtigen
Kennzeichen geht sie dann auf. Das ist einer der Grenzfälle aus dem Testkonzept.

**Prüfung 4** ist bewusst so geschrieben: `!= "paid"` und nicht `== "unpaid"`. Geprüft wird
also nicht, ob etwas schiefging, sondern ob **der eine erlaubte Fall** vorliegt. Käme eines
Tages ein neuer Zustand dazu, etwa `"refunded"`, würde er automatisch abgewiesen statt
versehentlich durchgelassen. Diese Denkweise heisst **Positivliste**: Erlaubt ist nur, was
ausdrücklich erlaubt ist.

**Die Reihenfolge am Ende** ist das Wichtigste der ganzen Funktion: **Erst abschliessen,
dann öffnen.** Warum?

- Andersherum: Schranke auf, Auto fährt raus, dann soll die Kasse abschliessen, antwortet
  aber gerade nicht. Das Auto ist weg, der Platz bleibt aber für immer als belegt gebucht.
  Nach drei solchen Fällen wäre das Parkhaus dauerhaft „voll", obwohl es leer ist.
- So herum: Die Kasse schliesst ab, dann geht die Schranke auf. Antwortet die Kasse nicht,
  bleibt die Schranke zu. Das Auto muss es nochmals versuchen. Unangenehm, aber harmlos und
  von selbst behoben.

Wieder das Prinzip **im Zweifel sicher**: Von zwei möglichen Fehlern wählt man den, der sich
von selbst behebt.

Nach dem Öffnen wird die Belegung sofort aufgefrischt (`true`), damit die Anzeige nicht
3 Sekunden hinterherhinkt.

#### Der Zustand für die Webseite

[main.cpp:322](../src/barriers/main.cpp#L322)

```cpp
static void handleStateRequest() {
  String response = F("{\"cashDeskOnline\":");
  response += g_cashDeskIsOnline ? F("true") : F("false");
  // ... wifi, occupied, totalSpots aus dem Zwischenspeicher ...
  response += F(",\"entryBarrierOpen\":");
  response += entryBarrier.isOpen() ? F("true") : F("false");
  response += F(",\"entryBarrierMsLeft\":");
  response += entryBarrier.millisUntilClose();
  // ... dasselbe für exitBarrier, dann singleServo und testBuild ...
  sendJsonResponse(200, response);
}
```

Diese Funktion **ruft die Kasse nie an**. Sie gibt nur die gemerkten Werte und den Zustand
der Schranke heraus. Deshalb ist sie immer sofort fertig, und die Seite der Schranke bleibt
bedienbar, selbst wenn die Kasse ausgefallen ist.

#### Der Servo-Test

```cpp
#if TEST_BUILD
static void handleServoTest() {
  String whichBarrier = server.arg("barrier");

#if SERVO_COUNT == 1
  (void)whichBarrier;
  entryBarrier.open();

  String message = F("Servo an GP");
  message += SERVO_PIN_ENTRY;
  message += F(" öffnet die Schranke und schliesst sie nach 5 Sekunden wieder.");
  sendSimpleReply(true, message);
#else
  // je nach whichBarrier die Einfahrt oder die Ausfahrt öffnen
#endif
}
#endif
```

Öffnet die Schranke ohne Parkvorgang. Damit prüft man vor dem ersten Test, ob die
Verkabelung stimmt. Bei einem Servo gibt es nichts auszuwählen. **`(void)whichBarrier;`**
sagt dem Compiler: „Ich benutze diese Variable absichtlich nicht." Sonst gäbe es eine Warnung.

#### setup und loop

[main.cpp:393](../src/barriers/main.cpp#L393)

```cpp
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  delay(1500);

  Serial.println();
  Serial.println(F("=== Parksystem M450 - Pico 2 (Schranken) ==="));

  entryBarrier.begin();
#if SERVO_COUNT == 2
  exitBarrier.begin();
#else
  Serial.print(F("[SCHRANKE] Ein-Servo-Betrieb: Ein- und Ausfahrt teilen GP"));
  Serial.println(SERVO_PIN_ENTRY);
#endif

  startWifi(IP_BARRIERS, "parkhaus-schranken");

  server.on("/",          handleHomePage);
  server.on("/api/entry", handleEntryRequest);
  server.on("/api/exit",  handleExitRequest);
  server.on("/api/state", handleStateRequest);
#if TEST_BUILD
  server.on("/api/servotest", handleServoTest);
#endif
  server.onNotFound(handleUnknownEndpoint);

  server.begin();

  // ... Ausgabe ...
  refreshOccupancyFromCashDesk(true);
}
```

Wie bei der Kasse, mit zwei Besonderheiten:

- `begin()` wird bei einem Servo **nur einmal** aufgerufen. Ein zweites `attach()` auf
  denselben Pin wäre genau das Problem, das die Referenz vermeiden soll.
- Am Ende wird die Kasse einmal sofort gefragt, damit die Seite nicht die ersten 3 Sekunden
  „unbekannt" anzeigt.

[main.cpp:434](../src/barriers/main.cpp#L434)

```cpp
void loop() {
  server.handleClient();
  keepWifiAlive();

  entryBarrier.update();
#if SERVO_COUNT == 2
  exitBarrier.update();
#endif

  refreshOccupancyFromCashDesk(false);

  digitalWrite(LED_BUILTIN, isWifiConnected() ? HIGH : LOW);
}
```

Fünf Dinge, die ständig im Wechsel laufen, ohne dass eines auf das andere wartet:

1. Webanfragen beantworten.
2. WLAN überwachen.
3. **Die Schranke schliessen, wenn ihre Zeit um ist.** Hier zeigt sich, warum `open()` nicht
   wartet: Diese eine Zeile erledigt das Schliessen nebenbei.
4. Die Belegung bei der Kasse auffrischen, wenn das Intervall abgelaufen ist.
5. Die LED auf den WLAN-Zustand setzen.

Einziger Haken: Wenn Pico 2 die Kasse fragt und sie nicht antwortet, wartet `http.GET()` bis
zu 2 Sekunden. In diesen 2 Sekunden steht auch `loop()`. Deshalb ist die Grenze so klein
gewählt, und deshalb fragt Pico 2 eine ausgefallene Kasse nur alle 10 Sekunden.

**Zusammenfassung:** Die Schranke öffnet nur, wenn jede Prüfung bestanden ist. Sie fragt die
Kasse nur, wenn es nötig ist, und merkt sich den Rest. Bei der Ausfahrt wird zuerst
abgeschlossen und dann geöffnet, damit kein Platz für immer blockiert bleibt.

---

## Teil 4: Eine Fahrt durch den ganzen Code

Jetzt kennst du jede Datei einzeln. In diesem Teil verfolgen wir die Geschichte aus
[Abschnitt 1.2](#12-eine-geschichte-ein-auto-im-parkhaus) noch einmal, diesmal durch den
Code. Jeder Schritt nennt die Funktion, die gerade läuft. Halte die Dateien daneben offen und
spring mit, dann siehst du, wie alles zusammenspielt.

### Phase 0: Strom an

**Pico 1 (Kasse)** startet:

1. Das Arduino-Framework ruft `setup()` auf ([cashdesk/main.cpp:410](../src/cashdesk/main.cpp#L410)).
2. `resetAllParkingRecords()` leert alle 24 Karteikarten.
3. `startWifi(IP_CASHDESK, ...)` sucht die Netze, verbindet sich und gibt sich die Adresse
   `192.168.89.211`.
4. `server.on(...)` trägt alle Endpunkte ins Telefonbuch ein, `server.begin()` startet den
   Webserver.
5. Ab jetzt läuft `loop()` endlos: `handleClient()`, `keepWifiAlive()`, `uptimeInSeconds()`,
   LED.

**Pico 2 (Schranke)** startet:

1. `setup()` ([barriers/main.cpp:393](../src/barriers/main.cpp#L393)).
2. `entryBarrier.begin()`: `_servo.attach(8)` und `_servo.write(SERVO_ANGLE_CLOSED)`. Der
   Arm fährt zu.
3. `startWifi(IP_BARRIERS, ...)`: Adresse `192.168.89.212`.
4. Endpunkte eintragen, Webserver starten.
5. `refreshOccupancyFromCashDesk(true)` fragt die Kasse sofort nach der Belegung:
   `askCashDeskForOccupancy()` → `sendRequestToCashDesk("/api/vehicles")` → `http.GET()`.
   Auf Pico 1 landet die Anfrage über `handleClient()` bei `handleVehicleList()`. Die Antwort
   `{"occupied":0,"totalSpots":3,...}` wird mit `readJsonField` gelesen und im
   Zwischenspeicher abgelegt.
6. Ab jetzt läuft `loop()`.

### Phase 1: Die Kassenseite wird geöffnet

1. Der Browser ruft `http://192.168.89.211/` auf. Pico 1 findet `/` im Telefonbuch und ruft
   `handleHomePage()` auf. Die schickt den Rohtext `CASHDESK_WEB_PAGE`.
2. Im Browser läuft das JavaScript los: `sendBrowserClockToPico()` schickt
   `/api/setclock?epoch=...`.
3. Pico 1: `handleSetClock()` prüft den Wert und ruft `setWallClock()` auf. Ab jetzt kann
   `formatDateAndTime()` echte Uhrzeiten liefern.
4. Danach ruft der Browser alle 2 Sekunden `refreshPage()` auf, also `/api/vehicles`.

### Phase 2: Anna fährt ein

Auf der Seite der Schranke wird `lu12345` eingetippt und „Einfahrt bestätigen" gedrückt.

1. **Browser:** `requestEntry()` zeigt „Anfrage läuft", sperrt die Knöpfe und schickt
   `fetch("/api/entry?plate=lu12345")`.
2. **Pico 2:** Im nächsten `loop()`-Durchlauf merkt `server.handleClient()` die Anfrage und
   ruft `handleEntryRequest()` auf.
3. **Pico 2:** `readAndCheckPlate()` ruft `checkPlate("lu12345", plate)` auf.
   - `normalizePlate()` → `toUpperCaseWithSingleSpaces()` macht `"LU12345"`.
   - `looksLikePlateWithMissingSpace()` sagt ja → `insertSpaceAfterCantonCode()` macht
     `"LU 12345"`.
   - Länge 8, nicht zu lang. `hasValidCantonCode()` ja, `hasValidNumberPart()` ja.
   - Ergebnis `VALID`, in `plate` steht `"LU 12345"`.
4. **Pico 2:** `askCashDeskToRegisterEntry("LU 12345")`.
   - `encodeForUrl()` macht `"LU%2012345"`.
   - `sendRequestToCashDesk("/api/entry?plate=LU%2012345")` → `http.GET()`. Pico 2 wartet
     jetzt auf die Antwort, höchstens 2 Sekunden.
5. **Pico 1:** `handleClient()` → `handleEntryRequest()` → `registerEntry()`.
   - `checkPlate()` noch einmal: gültig.
   - `findActiveRecord("LU 12345")` geht 24 Karten durch, findet nichts → `nullptr`. Gut,
     das Auto ist nicht schon drin.
   - `countOccupiedSpots()` zählt 0. Kleiner als 3, also Platz frei.
   - `findSlotForNewRecord()` findet `g_records[0]` unbenutzt und gibt seine Adresse zurück.
   - `storePlate()` kopiert `"LU 12345"` auf die Karte. `entryUptimeSecond` wird auf
     `uptimeInSeconds()` gesetzt, sagen wir 120. Status `PARKED_UNPAID`.
   - Im Monitor erscheint `[KASSE] Einfahrt: LU 12345  Belegung: 1/3`.
   - Zurück kommt `ACCEPTED`.
6. **Pico 1:** `handleEntryRequest()` baut
   `{"ok":true,"code":"ok","plate":"LU 12345","message":"Einfahrt registriert für LU 12345.","occupied":1,"totalSpots":3}`
   und schickt es mit `sendJsonResponse()`.
7. **Pico 2:** `http.GET()` liefert 200, `http.getString()` den Text, `http.end()` gibt die
   Verbindung frei. `readJsonField()` liest `ok`, `code`, `message`, `occupied`,
   `totalSpots`.
8. **Pico 2:** Zurück in `handleEntryRequest()`: Kasse erreichbar, Zwischenspeicher auf
   1 von 3 setzen, Einfahrt akzeptiert. **Jetzt erst** `entryBarrier.open()`:
   `_servo.write(SERVO_ANGLE_OPEN)`, `_openedAtMs = millis()`, `_isOpen = true`. **Der Arm
   geht hoch.**
9. **Pico 2:** `sendSimpleReply(true, "LU 12345 ist registriert. ...")`.
10. **Browser:** grüne Meldung, Feld leeren, Knöpfe freigeben, `refreshPage()`. Das holt
    `/api/state`, und `handleStateRequest()` meldet `entryBarrierOpen: true` und etwa
    5000 Millisekunden Rest. Der grüne Balken ist voll.
11. **Die nächsten 5 Sekunden:** Pico 2 läuft tausende Male durch `loop()`.
    `entryBarrier.update()` fragt jedes Mal `openTimeIsOver()`, bekommt „nein" und kehrt
    zurück. Der Browser fragt jede Sekunde `/api/state` und der Balken schrumpft.
12. **Nach 5000 Millisekunden:** `openTimeIsOver()` sagt ja → `closeNow()` →
    `_servo.write(SERVO_ANGLE_CLOSED)`. **Der Arm geht runter.**

### Phase 3: Die Zeit vergeht

- Die Kassenseite ruft alle 2 Sekunden `/api/vehicles` ab. `handleVehicleList()` findet
  `g_records[0]` im Zustand `PARKED_UNPAID` und baut mit `buildActiveVehicleJson()` eine
  Zeile. Darin rechnet `calculateFeeInCents()` die aktuelle Gebühr. Die Tabelle zeigt die
  Parkdauer wachsen.
- Pico 2 fragt alle 3 Sekunden mit `refreshOccupancyFromCashDesk(false)` die Belegung ab.
- Beide `loop()` rufen ständig `keepWifiAlive()` auf. Bricht das WLAN kurz ab, verbinden sie
  sich von selbst wieder.

### Phase 4: Anna bezahlt nach 80 Minuten

1. **Browser (Kasse):** `searchPlate()` schickt `/api/status?plate=LU%2012345`.
2. **Pico 1:** `handleStatusRequest()` → `lookupParkingInfo()`.
   - `findActiveRecord()` findet `g_records[0]`.
   - `secondsSinceEntry(120)` ergibt 4800 Sekunden (80 Minuten).
   - `calculateFeeInCents(4800)`: 4800 / 3600 = 1, Rest 1200, also 2 Stunden, also
     **400 Rappen**.
   - `formatMoney(400)` macht `"CHF 4.00"`, `formatDuration(4800)` macht `"1 h 20 min"`.
3. **Browser:** `displayParkingInfo()` zeigt alles an, orange Meldung „Offener Betrag:
   CHF 4.00", der Bezahlen-Knopf wird freigegeben.
4. **Browser:** Klick auf Bezahlen → `payFee()` → `/api/pay?plate=LU%2012345`.
5. **Pico 1:** `handlePaymentRequest()` → `payParkingFee()`. Nicht schon bezahlt, also
   400 Rappen auf die Karte schreiben, Status `PARKED_PAID`. Der Betrag ist jetzt
   eingefroren.
6. **Browser:** grüne Meldung „Bezahlung erfolgt. Betrag: CHF 4.00."

### Phase 5: Anna fährt aus

1. **Browser (Schranke):** `requestExit()` → `/api/exit?plate=LU%2012345`.
2. **Pico 2:** `handleExitRequest()`.
   - **Prüfung 1:** `readAndCheckPlate()` → gültig.
   - `askCashDeskForStatus("LU 12345")` → Pico 1 `handleStatusRequest()` →
     `lookupParkingInfo()` → Status `PARKED_PAID`, also `"paid"`, Gebühr die eingefrorenen
     400 Rappen.
   - **Prüfung 2:** Kasse hat geantwortet.
   - **Prüfung 3:** Status ist nicht `"unknown"`.
   - **Prüfung 4:** Status ist `"paid"`.
3. **Pico 2:** `askCashDeskToFinishParking("LU 12345")` → Pico 1 `handleExitRequest()` →
   `finishParking()`. Die Karte bekommt die Ausfahrtszeit und den Zustand `FINISHED`. Der
   Platz ist frei. Antwort `{"ok":true,...}`.
4. **Pico 2:** **Erst jetzt** `exitBarrier.open()`. Weil `exitBarrier` nur ein zweiter Name
   für `entryBarrier` ist, fährt derselbe Arm hoch.
5. **Pico 2:** `refreshOccupancyFromCashDesk(true)` holt sofort die neue Belegung: 0 von 3.
6. **Browser:** grüne Meldung „Bezahlung bestätigt. Die Ausfahrtsschranke öffnet für
   LU 12345."
7. Nach 5 Sekunden schliesst `update()` den Arm wieder.
8. **Kassenseite:** Beim nächsten Auffrischen steht `LU 12345` unter „Abgeschlossene
   Vorgänge", gebaut von `buildFinishedVehicleJson()`.

Und wenn Anna **nicht** bezahlt hätte? Dann wäre der Status bei Prüfung 4 `"unpaid"`
gewesen. `handleExitRequest()` hätte die Meldung mit dem offenen Betrag geschickt und mit
`return` aufgehört. `exitBarrier.open()` wäre nie erreicht worden.

### Die vier Ideen, die das ganze Projekt tragen

Wenn du dir aus diesem Dokument nur vier Dinge merkst, dann diese:

1. **Eine einzige Quelle der Wahrheit.** Nur die Kasse speichert Parkvorgänge. Die Schranke
   fragt immer nach. So können nie zwei verschiedene Listen entstehen.
2. **Merken statt warten.** In `loop()` wartet nie etwas. Statt `delay()` merkt man sich mit
   `millis()`, wann etwas passiert ist, und schaut bei jedem Durchlauf nach, ob es wieder Zeit
   ist. So bleibt der Pico immer ansprechbar.
3. **Erst prüfen, dann handeln.** Jede wichtige Funktion prüft zuerst alle Bedingungen mit
   frühem `return`. Gespeichert oder geöffnet wird erst ganz am Ende.
4. **Im Zweifel sicher.** Fehlt eine Antwort, bleibt die Schranke zu. Und bei der Ausfahrt
   wird zuerst abgeschlossen, dann geöffnet, damit ein Fehler nie einen Platz für immer
   blockiert.

---

## Anhang A: Begriffe von A bis Z

| Begriff | Erklärung | Erklärt in |
|---|---|---|
| `&` (Adresse) | vor einer Variablen: „wo liegt sie im Speicher" | 3.4, 3.7 |
| `&` (Referenz) | in einem Typ: keine Kopie, sondern das Original | 3.3 |
| Array | Reihe gleichartiger Werte mit fester Länge, Zugriff mit `[0]`, `[1]` | 3.7 |
| `async` / `await` | JavaScript: auf eine Netzwerkantwort warten, ohne den Browser einzufrieren | 3.8 |
| Ausgabeparameter | Parameter, in den eine Funktion ein Ergebnis schreibt, endet bei uns auf `Out` | 3.3 |
| Bedingungsoperator | `bedingung ? A : B`, kurzes `if`/`else` in einer Zeile | 3.6 |
| Bibliothek | fertiger Code von anderen, den man einbindet | 3.0 |
| BSSID | Hardware-Adresse des Routers, mit dem ein Gerät verbunden ist | 3.6 |
| `break` | beendet einen `case` in einem `switch` | 3.5 |
| Cache, Zwischenspeicher | gemerkte Antwort, damit man nicht jedes Mal neu fragen muss | 2.5, 3.13 |
| Cast | Umwandlung in einen anderen Typ, z. B. `(uint32_t)wert` | 3.3 |
| `char` | ein einzelnes Zeichen, in einfachen Anführungszeichen: `'A'` | 3.3 |
| Client | wer fragt | 2.2 |
| Compiler | übersetzt Quellcode in Maschinenbefehle | 3.1 |
| `const` | „wird nicht verändert", vom Compiler überwacht | 3.3, 3.10 |
| `constexpr` | Konstante, die beim Bauen feststeht | 3.2 |
| `continue` | springt zur nächsten Runde einer Schleife | 3.7 |
| Deklaration | Ankündigung einer Funktion, ohne Rumpf (in der `.h`) | 3.3 |
| Definition | die Funktion mit ihrem Rumpf (in der `.cpp`) | 3.3 |
| `#define` | Textersetzung vor dem Kompilieren | 3.2 |
| Endpunkt | ein Pfad wie `/api/status`, den ein Server bedient | 2.2 |
| `enum class` | Liste benannter Möglichkeiten | 3.3 |
| Escaping | besondere Zeichen harmlos umschreiben, z. B. `"` zu `\"` | 3.5 |
| `F("...")` | Text bleibt im Programmspeicher statt im Arbeitsspeicher | 3.3 |
| `fetch` | JavaScript: eine HTTP-Anfrage schicken | 3.8 |
| Firmware | das fest auf einem Gerät gespeicherte Programm | 1.6 |
| Flashen | Firmware auf den Pico übertragen | 1.6 |
| `for` | Schleife mit Zähler | 3.3 |
| Fragmentierung | freier Speicher zerfällt in kleine Stücke | 3.7 |
| Framework (Arduino) | fertiger Rahmen, der `setup()` und `loop()` aufruft | 3.0 |
| Funktion | benannter Block Code, der aufgerufen werden kann | 3.0, 3.3 |
| Gateway | Adresse des Routers | 2.1 |
| GPIO | Allzweck-Anschluss am Pico, im Code mit seiner GPIO-Nummer | 1.3 |
| Header (`.h`) | die „Speisekarte": was es gibt | 3.3 |
| Heap | gemeinsamer Speichervorrat, aus dem sich `String` bedient | 3.7 |
| HTTP | Technik für Fragen und Antworten im Netz | 2.2 |
| `if` / `else` | Bedingung | 3.3 |
| `#if` / `#endif` | Code nur in bestimmten Builds behalten | 3.7 |
| `#include` | den Inhalt einer anderen Datei einfügen | 3.2 |
| Initialisierungsliste | Startwerte im Konstruktor nach dem Doppelpunkt | 3.10 |
| Injection | Angriff, bei dem Eingaben die Struktur einer Antwort verändern | 3.5 |
| `inline` | Funktion darf mehrfach im Programm vorkommen | 3.5 |
| IP-Adresse | „Hausnummer" eines Geräts im Netz | 2.1 |
| JSON | Textformat für Daten: `{"name":"wert"}` | 2.3 |
| Kapselung | Innenleben einer Klasse verstecken (`private`) | 3.10 |
| Klasse | Bauplan für Objekte mit Daten und Methoden | 3.10 |
| Konstruktor | Methode, die beim Bauen eines Objekts läuft | 3.10 |
| `loop()` | läuft nach dem Start immer wieder | 3.0 |
| Member-Variable | Variable, die zu einem Objekt gehört, bei uns mit `_` am Anfang | 3.10 |
| Methode | Funktion, die zu einem Objekt gehört, Aufruf mit Punkt | 3.3 |
| `millis()` | Millisekunden seit dem Einschalten | 3.4 |
| Modulo `%` | Rest einer Division | 3.4 |
| Namensraum, anonymer | `namespace { }`: nur in dieser Datei sichtbar | 3.3 |
| `nullptr` | Zeiger, der auf nichts zeigt | 3.7 |
| Objekt | ein nach einem Bauplan gebautes Ding | 3.6, 3.10 |
| Parameter | Wert, den man einer Funktion beim Aufruf mitgibt | 3.3 |
| Port | „Türnummer" an einer IP-Adresse, Webseiten nutzen 80 | 3.9 |
| Positivliste | nur ausdrücklich Erlaubtes durchlassen | 3.13 |
| `#pragma once` | Header-Datei höchstens einmal einfügen | 3.2 |
| Präprozessor | bearbeitet `#include`, `#define`, `#if` vor dem Kompilieren | 3.2 |
| `PROGMEM` | Daten bleiben im Programmspeicher | 3.8 |
| `public` / `private` | für alle sichtbar / nur für die Klasse selbst | 3.10 |
| Referenz | zweiter Name für ein bestehendes Ding, keine Kopie | 3.3, 3.13 |
| Ressourcenleck | etwas belegen und nie freigeben | 3.11 |
| Rohtext | `R"HTML( ... )HTML"`, Text ohne Escape-Zeichen | 3.8 |
| Rückgabewert | Ergebnis einer Funktion, mit `return` | 3.3 |
| Schnittstelle | Abmachung zwischen zwei Programmteilen oder Geräten | 3.9 |
| Server | wer antwortet | 2.2 |
| `Serial` | Textausgabe über das USB-Kabel an den Laptop | 2.7, 3.6 |
| `setup()` | läuft einmal beim Start | 3.0 |
| `snprintf` | formatierten Text in einen Puffer schreiben | 3.4 |
| SSID | Name eines WLANs | 3.2 |
| `static` | ausserhalb von Funktionen: nur in dieser Datei sichtbar | 3.9 |
| Statuscode | Zahl in einer HTTP-Antwort: 200, 400, 404 | 2.2 |
| `String` | Text beliebiger Länge | 3.3 |
| `struct` | Bündel mehrerer zusammengehöriger Werte | 3.7 |
| Subnetzmaske | sagt, welche Adressen zum eigenen Netz gehören | 2.1 |
| `switch` / `case` | einen von mehreren Wegen anhand eines Wertes wählen | 3.3 |
| Typ | welche Art von Wert: `bool`, `uint8_t`, `String` ... | 3.2 |
| Überlauf | Zahl springt beim Überschreiten des Höchstwerts auf 0 zurück | 3.4 |
| `uint8_t` und Co. | ganze Zahlen ohne Vorzeichen mit 8, 16, 32 oder 64 Bit | 3.2 |
| Variable | benannter Speicherplatz für einen Wert | 3.3 |
| `void` | „gibt nichts zurück" | 3.4 |
| `while` | Schleife, solange eine Bedingung stimmt | 3.6 |
| Zeiger `*` | Adresse eines Dings, Zugriff auf Felder mit `->` | 3.7 |
| Zurückweichen (backoff) | bei Fehlern seltener nachfragen | 3.13 |
| Zustandsmaschine | merkt sich Zustand und Zeit, entscheidet bei jedem Aufruf neu | 3.10 |

---

## Anhang B: Wo ändere ich was?

| Ich will ... | Datei | Was ändern |
|---|---|---|
| ein anderes WLAN benutzen | `src/common/config.h` | `WIFI_SSID` und `WIFI_PASSWORD`. **Das Netz muss 2,4 GHz haben.** |
| andere IP-Adressen | `src/common/config.h` | `IP_CASHDESK`, `IP_BARRIERS`, `IP_GATEWAY`, `IP_SUBNET_MASK` |
| den Tarif ändern | `src/common/config.h` | `CENTS_PER_HOUR`. Dazu in `web_page_cashdesk.h` den Text „CHF 2.00" in der oberen Leiste anpassen, der steht fest im HTML. |
| eine Gratiszeit oder ein Tagesmaximum | `src/cashdesk/parking_garage.cpp` | in `calculateFeeInCents()` |
| mehr oder weniger Parkplätze | `src/common/config.h` | `MAX_PARKING_SPOTS`. Muss deutlich kleiner bleiben als `MAX_PARKING_RECORDS` (24) in `parking_garage.h`. |
| die Schranke länger offen lassen | `src/common/config.h` | `BARRIER_OPEN_TIME_MS`. Dazu in `web_page_barriers.h` die gleichnamige Konstante im JavaScript anpassen, sonst stimmt der Balken nicht. |
| andere Winkel für offen und zu | `src/common/config.h` | `SERVO_ANGLE_CLOSED`, `SERVO_ANGLE_OPEN` |
| einen zweiten Servo anschliessen | `src/common/config.h` | `SERVO_COUNT` auf `2`, Signal des zweiten Servos an GP9 (Pin 12) |
| die Kennzeichen-Regeln ändern | `src/common/license_plate.cpp` | `hasValidCantonCode()`, `hasValidNumberPart()`, dazu `PLATE_MAX_LENGTH` in `config.h` |
| eine Fehlermeldung umformulieren | `license_plate.cpp`, `cashdesk/main.cpp`, `barriers/main.cpp` | die Texte in `F("...")` |
| die Testwerkzeuge abschalten | `platformio.ini` | `-DTEST_BUILD=0` in beiden Umgebungen |
| länger auf die Kasse warten | `src/common/config.h` | `HTTP_TIMEOUT_MS` (Vorsicht: friert Pico 2 bei Ausfall länger ein) |
| Pico 2 öfter oder seltener nachfragen lassen | `src/barriers/main.cpp` | `POLL_INTERVAL_NORMAL_MS`, `POLL_INTERVAL_OFFLINE_MS` |
| das Aussehen der Seiten ändern | `web_page_cashdesk.h`, `web_page_barriers.h` | den CSS-Teil im `<style>` |

Nach jeder Änderung beide Firmwares neu bauen und auf beide Picos spielen. Ändert sich nur
etwas in `cashdesk/`, reicht die Kasse, bei `barriers/` die Schranke. Bei `common/` immer
beide.
