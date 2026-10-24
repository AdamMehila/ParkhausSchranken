// ===========================================================================
// Pico 2, die Schranke
//
// Dieser Pico bedient die Hardware und die Benutzerführung. Er speichert
// selbst nichts. Für jede Entscheidung fragt er bei der Kasse auf Pico 1 nach.
//
// Seine Aufgaben:
//   Weboberfläche mit je einem Eingabefeld für Einfahrt und Ausfahrt
//   den Servo ansteuern
//   vor jeder Ausfahrt den Bezahlstatus bei der Kasse abfragen
//
// Im Browser erreichbar unter http://192.168.4.12
//
// Die wichtigste Regel in dieser Datei: Die Schranke geht nur auf, wenn alle
// Bedingungen erfüllt sind. Im Zweifel bleibt sie zu. Deshalb steht in jeder
// Prüfung ein return, bevor der Servo überhaupt angesprochen wird.
// ===========================================================================

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#include "../common/config.h"
#include "../common/json_text.h"
#include "../common/license_plate.h"
#include "../common/network.h"
#include "cashdesk_client.h"
#include "barrier.h"
#include "web_page_barriers.h"


static WebServer server(WEB_SERVER_PORT);


// ---------------------------------------------------------------------------
// Die Schranken
//
// Bei nur einem angeschlossenen Servo gibt es auch nur ein Objekt. exitBarrier
// ist dann eine Referenz, also ein zweiter Name für dasselbe Objekt. Jeder
// Aufruf über diesen Namen bewegt denselben Arm.
//
// Warum nicht einfach zwei Objekte mit demselben Pin? Das wäre gleich doppelt
// falsch. Erstens würden zwei Servo-Objekte um denselben Anschluss streiten.
// Zweitens hätte jedes seine eigene Fünf-Sekunden-Uhr. Öffnet die Einfahrt
// und zwei Sekunden später die Ausfahrt, dann liefe die Uhr der Einfahrt
// zuerst ab und würde den Arm fallen lassen, obwohl die Ausfahrt ihn noch
// oben haben will.
//
// Mit der Referenz gibt es genau einen Zustand und genau eine Uhr. Genau das
// entspricht der Wirklichkeit, denn es gibt ja auch nur einen Arm.
// ---------------------------------------------------------------------------
#if SERVO_COUNT == 1
static Barrier  entryBarrier(SERVO_PIN_ENTRY, "Schranke");
static Barrier& exitBarrier = entryBarrier;
#else
static Barrier  entryBarrier(SERVO_PIN_ENTRY, "Einfahrt");
static Barrier  exitBarrier(SERVO_PIN_EXIT, "Ausfahrt");
#endif


// ---------------------------------------------------------------------------
// Der zwischengespeicherte Zustand der Kasse
//
// Die Weboberfläche fragt bei Pico 2 jede Sekunde nach dem aktuellen Stand.
// Würde Pico 2 daraufhin jedes Mal die Kasse anrufen, hätten wir zwei
// Probleme: unnötigen Netzverkehr, und vor allem bei ausgefallener Kasse
// jede Sekunde zwei Sekunden Blockade.
//
// Deshalb merkt sich Pico 2 den letzten bekannten Stand und frischt ihn nur
// alle paar Sekunden im Hintergrund auf.
// ---------------------------------------------------------------------------
static bool     g_cashDeskIsOnline = false;
static int      g_occupiedSpots    = 0;
static int      g_totalSpots       = MAX_PARKING_SPOTS;
static uint32_t g_lastPollMs       = 0;

// Solange die Kasse antwortet, fragen wir alle drei Sekunden nach.
constexpr uint32_t POLL_INTERVAL_NORMAL_MS = 3000;

// Antwortet sie nicht, gehen wir auf zehn Sekunden hoch.
//
// Das nennt man Zurückweichen. Es bringt gleich zwei Vorteile: Eine
// überlastete Kasse wird nicht noch zusätzlich bombardiert, und Pico 2
// verbringt weniger Zeit mit dem Warten auf Antworten, die sowieso nicht
// kommen.
constexpr uint32_t POLL_INTERVAL_OFFLINE_MS = 10000;


// ---------------------------------------------------------------------------
// Kleine Helfer
// ---------------------------------------------------------------------------

// Schickt eine JSON-Antwort an den Browser.
static void sendJsonResponse(int httpStatusCode, const String& jsonBody) {
  server.sendHeader(F("Cache-Control"), F("no-store"));
  server.send(httpStatusCode, "application/json; charset=utf-8", jsonBody);
}


// Schickt eine kurze Antwort mit nur zwei Feldern: hat es geklappt, und der
// Text dazu. Fast alle Endpunkte dieser Datei antworten in dieser Form.
static void sendSimpleReply(bool wasSuccessful, const String& message) {
  String response = F("{\"ok\":");
  response += wasSuccessful ? F("true") : F("false");
  response += F(",\"message\":\"");
  response += escapeForJson(message);
  response += F("\"}");

  sendJsonResponse(200, response);
}


// Holt das Kennzeichen aus der Anfrage und prüft es.
//
// Gibt true zurück, wenn das Kennzeichen in Ordnung ist. In plateOut steht
// dann die vereinheitlichte Schreibweise.
//
// Ist es nicht in Ordnung, schickt die Funktion selbst schon die
// Fehlerantwort und gibt false zurück. Der Aufrufer muss dann nur noch
// aussteigen.
//
// Diese Funktion gibt es, weil Einfahrt und Ausfahrt genau dieselbe Prüfung
// am Anfang brauchen. Einmal geschrieben statt zweimal bedeutet auch: Eine
// spätere Änderung an der Prüfung kann nicht an einer der beiden Stellen
// vergessen werden.
static bool readAndCheckPlate(String& plateOut) {
  PlateCheckResult checkResult = checkPlate(server.arg("plate"), plateOut);

  if (checkResult == PlateCheckResult::VALID) {
    return true;
  }

  // Eine ungültige Eingabe erreicht die Kasse gar nicht erst, und der Servo
  // bewegt sich nicht. Die Prüfung findet hier statt, und die Kasse prüft
  // zur Sicherheit noch einmal.
  Serial.print(F("[EINGABE] Abgelehnt, ungueltiges Kennzeichen. Laenge: "));
  Serial.println(server.arg("plate").length());

  sendSimpleReply(false, plateErrorMessage(checkResult));
  return false;
}


// Frischt die bekannte Belegung bei der Kasse auf.
//
// forceNow true bedeutet: sofort nachfragen, ohne auf das nächste Intervall
// zu warten. Das wird nach jeder Einfahrt und Ausfahrt benutzt, damit die
// Anzeige nicht mehrere Sekunden hinterherhinkt.
static void refreshOccupancyFromCashDesk(bool forceNow) {
  uint32_t interval = g_cashDeskIsOnline ? POLL_INTERVAL_NORMAL_MS
                                         : POLL_INTERVAL_OFFLINE_MS;

  if (!forceNow && (millis() - g_lastPollMs) < interval) {
    return;   // es ist noch nicht so weit
  }

  g_lastPollMs = millis();

  OccupancyReply reply = askCashDeskForOccupancy();
  g_cashDeskIsOnline = reply.cashDeskReachable;

  // Die Zahlen werden nur übernommen, wenn die Kasse wirklich geantwortet
  // hat. Sonst würden wir die letzten bekannten Werte mit Nullen
  // überschreiben und das Parkhaus sähe plötzlich leer aus.
  if (reply.cashDeskReachable) {
    g_occupiedSpots = reply.occupied;
    g_totalSpots    = reply.totalSpots;
  }
}


// ---------------------------------------------------------------------------
// Die Endpunkte
// ---------------------------------------------------------------------------

// Liefert die Weboberfläche aus.
static void handleHomePage() {
  server.send(200, "text/html; charset=utf-8", BARRIERS_WEB_PAGE);
}


// Einfahrt: Kennzeichen prüfen, bei der Kasse anmelden, Schranke öffnen.
//
// Der Servo wird erst in der letzten Zeile angesprochen. Davor steht jede
// Prüfung mit ihrem eigenen return. Wer die Funktion liest, sieht dadurch
// auf einen Blick alle Gründe, aus denen die Schranke zubleibt.
static void handleEntryRequest() {
  String plate;

  // Grund 1: Das Kennzeichen ist ungültig.
  if (!readAndCheckPlate(plate)) {
    return;
  }

  EntryReply reply = askCashDeskToRegisterEntry(plate);

  // Grund 2: Die Kasse antwortet nicht. Ohne Kasse kein Parkvorgang, und
  // ohne Parkvorgang darf niemand hinein. Sonst stünde ein Fahrzeug im
  // Parkhaus, von dem niemand etwas weiss, und es käme nie wieder heraus.
  if (!reply.cashDeskReachable) {
    sendSimpleReply(false, reply.message);
    return;
  }

  // Ab hier wissen wir sicher, dass die Kasse erreichbar ist.
  g_cashDeskIsOnline = true;
  g_occupiedSpots    = reply.occupied;
  g_totalSpots       = reply.totalSpots;

  // Grund 3: Die Kasse sagt Nein. Zum Beispiel weil das Parkhaus voll ist
  // oder dieses Fahrzeug schon drinsteht.
  if (!reply.accepted) {
    sendSimpleReply(false, reply.message);
    return;
  }

  // Alles geprüft, jetzt darf der Arm hoch.
  entryBarrier.open();

  String message = plate;
  message += F(" ist registriert. Die Einfahrtsschranke öffnet und schliesst nach 5 Sekunden wieder.");
  sendSimpleReply(true, message);
}


// Ausfahrt: Bezahlstatus bei der Kasse abfragen und nur bei "bezahlt" öffnen.
//
// Dies ist die heikelste Funktion des ganzen Projekts, denn hier wird über
// Geld entschieden. Sie prüft deshalb vier Dinge nacheinander und bricht bei
// jedem Zweifel ab.
static void handleExitRequest() {
  String plate;

  // Prüfung 1: Ist das Kennzeichen gültig.
  if (!readAndCheckPlate(plate)) {
    return;
  }

  StatusReply reply = askCashDeskForStatus(plate);

  // Prüfung 2: Hat die Kasse geantwortet.
  if (!reply.cashDeskReachable) {
    g_cashDeskIsOnline = false;
    Serial.println(F("[AUSFAHRT] Kasse nicht erreichbar, Schranke bleibt zu."));
    sendSimpleReply(false, F("Die Kasse ist zurzeit nicht erreichbar. Der Bezahlstatus kann nicht geprüft werden, die Schranke bleibt geschlossen."));
    return;
  }

  g_cashDeskIsOnline = true;

  // Prüfung 3: Kennt die Kasse dieses Kennzeichen überhaupt.
  //
  // Das ist der Fall, in dem sich jemand vertippt. Er hat vielleicht unter
  // BE 55511 bezahlt und tippt an der Ausfahrt BE 55512. Das Format ist
  // gültig, die Kasse kennt das Kennzeichen aber nicht. Die Schranke bleibt
  // zu, und die Meldung sagt genau, woran es liegt.
  if (reply.paymentState == "unknown") {
    Serial.print(F("[AUSFAHRT] Unbekanntes Kennzeichen: "));
    Serial.println(plate);

    String message = F("Zu dem Kennzeichen ");
    message += plate;
    message += F(" ist keine Einfahrt bekannt. Die Schranke bleibt geschlossen.");
    sendSimpleReply(false, message);
    return;
  }

  // Prüfung 4: Ist wirklich bezahlt worden.
  //
  // Beachte die Form der Bedingung. Geprüft wird nicht, ob irgendetwas
  // schiefging, sondern ob der eine erlaubte Fall vorliegt. Käme eines
  // Tages ein neuer Zustand dazu, würde er automatisch abgewiesen statt
  // versehentlich durchgelassen.
  if (reply.paymentState != "paid") {
    Serial.print(F("[AUSFAHRT] Noch nicht bezahlt: "));
    Serial.println(plate);

    String message = F("Die Parkgebühr für ");
    message += plate;
    message += F(" ist noch offen (");
    message += reply.fee;
    message += F("). Bitte zuerst an der Kasse bezahlen. Die Schranke bleibt geschlossen.");
    sendSimpleReply(false, message);
    return;
  }

  // Jetzt wird der Vorgang bei der Kasse abgeschlossen, und zwar BEVOR die
  // Schranke aufgeht.
  //
  // Die Reihenfolge ist Absicht. Ginge zuerst die Schranke auf und die Kasse
  // würde danach nicht antworten, wäre das Fahrzeug weg, der Platz aber
  // weiterhin als belegt gebucht. Dieser Platz liesse sich nie wieder
  // freigeben.
  //
  // So herum ist der schlimmste Fall, dass ein bezahltes Fahrzeug einmal
  // nicht rauskommt und es nochmal versuchen muss. Das ist unangenehm, aber
  // reparierbar. Ein für immer blockierter Platz wäre es nicht.
  if (!askCashDeskToFinishParking(plate)) {
    sendSimpleReply(false, F("Die Kasse konnte den Parkvorgang nicht abschliessen. Die Schranke bleibt geschlossen."));
    return;
  }

  exitBarrier.open();

  // Die Belegung hat sich gerade geändert, also sofort nachfragen statt bis
  // zum nächsten Intervall zu warten.
  refreshOccupancyFromCashDesk(true);

  String message = F("Bezahlung bestätigt. Die Ausfahrtsschranke öffnet für ");
  message += plate;
  message += F(".");
  sendSimpleReply(true, message);
}


// Liefert den aktuellen Zustand für die Weboberfläche.
//
// Diese Funktion ruft die Kasse NICHT an. Sie gibt nur die
// zwischengespeicherten Werte heraus und ist deshalb immer sofort fertig.
// Genau darum bleibt die Oberfläche auch dann bedienbar, wenn die Kasse
// ausgefallen ist.
static void handleStateRequest() {
  String response = F("{\"cashDeskOnline\":");
  response += g_cashDeskIsOnline ? F("true") : F("false");
  response += F(",\"wifi\":");
  response += isWifiConnected() ? F("true") : F("false");
  response += F(",\"occupied\":");
  response += g_occupiedSpots;
  response += F(",\"totalSpots\":");
  response += g_totalSpots;
  response += F(",\"entryBarrierOpen\":");
  response += entryBarrier.isOpen() ? F("true") : F("false");
  response += F(",\"entryBarrierMsLeft\":");
  response += entryBarrier.millisUntilClose();
  response += F(",\"exitBarrierOpen\":");
  response += exitBarrier.isOpen() ? F("true") : F("false");
  response += F(",\"exitBarrierMsLeft\":");
  response += exitBarrier.millisUntilClose();
  response += F(",\"singleServo\":");
  response += (SERVO_COUNT == 1) ? F("true") : F("false");

  // Die GPIO-Nummern der Servos, damit die Webseite sie anzeigen kann, ohne
  // dass sie dort ein zweites Mal fest eingetragen werden müssen. Bei nur
  // einem Servo hängt die Ausfahrt am selben Pin wie die Einfahrt.
  response += F(",\"entryPin\":");
  response += SERVO_PIN_ENTRY;
  response += F(",\"exitPin\":");
  response += (SERVO_COUNT == 1) ? SERVO_PIN_ENTRY : SERVO_PIN_EXIT;

  response += F(",\"testBuild\":");
#if TEST_BUILD
  response += F("true");
#else
  response += F("false");
#endif
  response += F("}");

  sendJsonResponse(200, response);
}


#if TEST_BUILD
// Fährt einen Servo einmal auf und zu, ohne dass ein Parkvorgang entsteht.
//
// Damit lässt sich vor dem ersten Testfall nachweisen, dass die Verkabelung
// stimmt. Findet man einen Fehler erst mitten in der Prüfung, weiss man
// nicht, ob die Software oder das Kabel schuld war.
static void handleServoTest() {
  String whichBarrier = server.arg("barrier");

#if SERVO_COUNT == 1
  // Bei einem einzigen Servo gibt es nichts auszuwählen. Die Umwandlung
  // nach void sagt dem Compiler, dass wir die Variable absichtlich nicht
  // benutzen, und unterdrückt so eine Warnung.
  (void)whichBarrier;

  entryBarrier.open();

  // Der Pin wird aus config.h übernommen und nicht fest hingeschrieben. So
  // stimmt die Meldung auch dann noch, wenn der Servo umgesteckt wird.
  String message = F("Servo an GP");
  message += SERVO_PIN_ENTRY;
  message += F(" öffnet die Schranke und schliesst sie nach 5 Sekunden wieder.");
  sendSimpleReply(true, message);
#else
  if (whichBarrier == "exit") {
    exitBarrier.open();
    sendSimpleReply(true, F("Ausfahrtsschranke öffnet und schliesst nach 5 Sekunden wieder."));
  } else {
    entryBarrier.open();
    sendSimpleReply(true, F("Einfahrtsschranke öffnet und schliesst nach 5 Sekunden wieder."));
  }
#endif
}
#endif


// Antwortet auf jede Adresse, für die es keinen Endpunkt gibt.
static void handleUnknownEndpoint() {
  sendJsonResponse(404, F("{\"ok\":false,\"message\":\"Unbekannter Endpunkt.\"}"));
}


// ---------------------------------------------------------------------------
// setup und loop
// ---------------------------------------------------------------------------

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  delay(1500);   // dem seriellen Monitor Zeit zum Verbinden geben

  Serial.println();
  Serial.println(F("=== Parksystem M450 - Pico 2 (Schranken) ==="));

  entryBarrier.begin();

  // begin() darf bei einem geteilten Servo nur einmal laufen. Ein zweites
  // attach() auf denselben Pin wäre genau das Problem, das die Referenz
  // weiter oben vermeiden soll.
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

  Serial.print(F("[SCHRANKEN] Webserver laeuft. Kasse erwartet unter "));
  Serial.println(IPAddress(IP_CASHDESK));

  // Einmal sofort nachfragen, damit die Oberfläche nicht zuerst drei
  // Sekunden lang "unbekannt" anzeigt.
  refreshOccupancyFromCashDesk(true);
}


void loop() {
  // Browseranfragen beantworten.
  server.handleClient();

  // WLAN nach einem Ausfall wieder aufbauen.
  keepWifiAlive();

  // Die Schranke schliessen, sobald ihre fünf Sekunden um sind.
  //
  // Hier zeigt sich, warum open() nicht wartet: Diese eine Zeile erledigt
  // das Schliessen nebenbei, während alles andere weiterläuft.
  entryBarrier.update();
#if SERVO_COUNT == 2
  exitBarrier.update();
#endif

  // Die Belegung im Hintergrund auffrischen. Das false bedeutet: nur, wenn
  // das Intervall abgelaufen ist.
  refreshOccupancyFromCashDesk(false);

  // Die eingebaute LED zeigt den WLAN-Zustand.
  digitalWrite(LED_BUILTIN, isWifiConnected() ? HIGH : LOW);
}
