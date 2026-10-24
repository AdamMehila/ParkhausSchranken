// ===========================================================================
// Pico 1, die Kasse
//
// Dieser Pico ist die Buchhaltung des Parkhauses. Er allein weiss, welches
// Fahrzeug drin steht, seit wann, und ob bezahlt wurde.
//
// Seine Aufgaben:
//   Weboberfläche für den Kassierer (suchen, Gebühr sehen, bezahlen)
//   alle Parkvorgänge speichern
//   die Gebühr nach Tarif berechnen
//   Fragen von Pico 2 beantworten
//
// Im Browser erreichbar unter http://192.168.4.11
//
// Zum Aufbau dieser Datei: Ganz unten stehen setup() und loop(). Das sind
// die beiden Funktionen, die der Pico von sich aus aufruft. setup() läuft
// genau einmal beim Einschalten, loop() danach immer wieder. Alles darüber
// sind Hilfsfunktionen, die von dort aus benutzt werden.
// ===========================================================================

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


// Der Webserver, der die Anfragen des Browsers entgegennimmt.
//
// static bedeutet hier: Diese Variable gehört nur zu dieser Datei. Keine
// andere Datei im Projekt kann sie sehen oder verändern.
static WebServer server(WEB_SERVER_PORT);


// ---------------------------------------------------------------------------
// Kleine Helfer
// ---------------------------------------------------------------------------

// Schickt eine JSON-Antwort an den Browser.
//
// Der Kopfzeileneintrag no-store ist wichtig. Ohne ihn merkt sich der
// Browser die Antwort und zeigt beim nächsten Mal die alte Belegung an,
// ohne überhaupt nachzufragen. Der Tester würde dann veraltete Zahlen sehen
// und denken, der Pico sei kaputt.
static void sendJsonResponse(int httpStatusCode, const String& jsonBody) {
  server.sendHeader(F("Cache-Control"), F("no-store"));
  server.send(httpStatusCode, "application/json; charset=utf-8", jsonBody);
}


// Übersetzt den Bezahlzustand in das Wort, das in der JSON-Antwort steht.
// Genau diese drei Wörter wertet Pico 2 aus.
static String paymentStateToText(PaymentState state) {
  switch (state) {
    case PaymentState::PAID:         return F("paid");
    case PaymentState::NOT_PAID_YET: return F("unpaid");
    default:                         return F("unknown");
  }
}


// Übersetzt das Ergebnis einer Einfahrt in ein kurzes Schlüsselwort.
// Pico 2 kann daran erkennen, warum eine Einfahrt abgelehnt wurde, ohne den
// deutschen Meldungstext auswerten zu müssen.
static const __FlashStringHelper* entryResultToCode(EntryResult result) {
  switch (result) {
    case EntryResult::ACCEPTED:       return F("ok");
    case EntryResult::GARAGE_FULL:    return F("full");
    case EntryResult::ALREADY_PARKED: return F("already");
    case EntryResult::MEMORY_FULL:    return F("memory");
    default:                          return F("invalid");
  }
}


// Baut den JSON-Abschnitt für ein Fahrzeug, das bereits ausgefahren ist.
static String buildFinishedVehicleJson(const ParkingRecord& record) {
  String json = F("{\"plate\":\"");
  json += escapeForJson(record.plate);
  json += F("\",\"entry\":\"");
  json += formatDateAndTime(record.entryUptimeSecond);
  json += F("\",\"status\":\"gone\",\"exit\":\"");
  json += formatDateAndTime(record.exitUptimeSecond);
  json += F("\",\"duration\":\"");
  json += formatDuration((uint32_t)(record.exitUptimeSecond - record.entryUptimeSecond));
  json += F("\",\"fee\":\"");
  json += formatMoney(record.paidCents);
  json += F("\"}");
  return json;
}


// Baut den JSON-Abschnitt für ein Fahrzeug, das gerade im Parkhaus steht.
static String buildActiveVehicleJson(const ParkingRecord& record) {
  uint32_t parkedSeconds = (uint32_t)((int64_t)uptimeInSeconds() - record.entryUptimeSecond);

  // Bei einem bezahlten Fahrzeug wird der bezahlte Betrag angezeigt und
  // nicht neu gerechnet. Sonst stünde in der Liste plötzlich wieder eine
  // Schuld, obwohl der Kunde längst bezahlt hat.
  uint32_t feeInCents = (record.status == RecordStatus::PARKED_PAID)
                      ? record.paidCents
                      : calculateFeeInCents(parkedSeconds);

  String json = F("{\"plate\":\"");
  json += escapeForJson(record.plate);
  json += F("\",\"entry\":\"");
  json += formatDateAndTime(record.entryUptimeSecond);
  json += F("\",\"status\":\"");
  json += (record.status == RecordStatus::PARKED_PAID) ? F("paid") : F("unpaid");
  json += F("\",\"exit\":\"-\",\"duration\":\"");
  json += formatDuration(parkedSeconds);
  json += F("\",\"fee\":\"");
  json += formatMoney(feeInCents);
  json += F("\"}");
  return json;
}


// ---------------------------------------------------------------------------
// Die Endpunkte
//
// Ein Endpunkt ist eine Adresse, die der Webserver bedient. Ruft jemand
// http://192.168.4.11/api/status auf, dann läuft die Funktion, die weiter
// unten in setup() für genau diese Adresse angemeldet wurde.
// ---------------------------------------------------------------------------

// Liefert die Weboberfläche aus.
static void handleHomePage() {
  server.send(200, "text/html; charset=utf-8", CASHDESK_WEB_PAGE);
}


// Liefert die Liste aller Fahrzeuge und die aktuelle Belegung.
//
// Die Antwort hat zwei getrennte Listen: vehicles enthält die Fahrzeuge, die
// gerade im Parkhaus stehen, history die bereits ausgefahrenen.
static void handleVehicleList() {
  String activeVehicles;
  String history;

  for (uint8_t i = 0; i < MAX_PARKING_RECORDS; i++) {
    const ParkingRecord* record = getParkingRecord(i);

    // Nie benutzte Plätze in der Liste überspringen.
    if (record == nullptr || record->status == RecordStatus::UNUSED) {
      continue;
    }

    if (record->status == RecordStatus::FINISHED) {
      // In JSON werden die Einträge einer Liste durch Kommas getrennt. Das
      // Komma kommt vor den neuen Eintrag, aber nur wenn schon einer da ist.
      // Sonst stünde am Anfang ein überflüssiges Komma und die Antwort wäre
      // kaputt.
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
  response += F(",\"totalSpots\":");
  response += MAX_PARKING_SPOTS;
  response += F(",\"free\":");
  response += countFreeSpots();
  response += F(",\"clockIsSet\":");
  response += isWallClockSet() ? F("true") : F("false");
  response += F(",\"now\":\"");
  response += formatDateAndTime((int64_t)uptimeInSeconds());
  response += F("\",\"testBuild\":");
#if TEST_BUILD
  response += F("true");
#else
  response += F("false");
#endif
  response += F(",\"vehicles\":[");
  response += activeVehicles;
  response += F("],\"history\":[");
  response += history;
  response += F("]}");

  sendJsonResponse(200, response);
}


// Liefert Auskunft zu einem einzelnen Kennzeichen.
//
// Diesen Endpunkt fragt auch Pico 2 vor jeder Ausfahrt ab. Die Antwort
// enthält im Feld status genau eines von drei Wörtern: unknown, unpaid oder
// paid. Pico 2 öffnet die Schranke nur bei paid.
static void handleStatusRequest() {
  ParkingInfo info = lookupParkingInfo(server.arg("plate"));

  bool plateIsUnknown = (info.state == PaymentState::PLATE_UNKNOWN);

  String response = F("{\"plate\":\"");
  response += escapeForJson(info.plate);
  response += F("\",\"status\":\"");
  response += paymentStateToText(info.state);
  response += F("\",\"entry\":\"");
  // Bei einem unbekannten Kennzeichen gibt es keine sinnvollen Zeiten.
  // Statt irreführender Nullen schicken wir einen Strich.
  response += plateIsUnknown ? String(F("-")) : formatDateAndTime(info.entryUptimeSecond);
  response += F("\",\"duration\":\"");
  response += plateIsUnknown ? String(F("-")) : formatDuration(info.parkedSeconds);
  response += F("\",\"fee\":\"");
  response += plateIsUnknown ? String(F("-")) : formatMoney(info.feeInCents);
  response += F("\",\"feeInCents\":");
  response += info.feeInCents;
  response += F("}");

  sendJsonResponse(200, response);
}


// Registriert eine Einfahrt. Diesen Endpunkt ruft Pico 2 auf, sobald dort
// ein Kennzeichen eingegeben und bestätigt wurde.
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
    // In allen übrigen Fällen hat registerEntry bereits einen passenden
    // Text geliefert.
    message = errorMessage;
  }

  String response = F("{\"ok\":");
  response += accepted ? F("true") : F("false");
  response += F(",\"code\":\"");
  response += entryResultToCode(result);
  response += F("\",\"plate\":\"");
  response += escapeForJson(plate);
  response += F("\",\"message\":\"");
  response += escapeForJson(message);
  response += F("\",\"occupied\":");
  response += countOccupiedSpots();
  response += F(",\"totalSpots\":");
  response += MAX_PARKING_SPOTS;
  response += F("}");

  sendJsonResponse(200, response);
}


// Verbucht eine Bezahlung.
static void handlePaymentRequest() {
  uint32_t paidCents    = 0;
  bool     wasAlreadyPaid = false;

  PaymentState state = payParkingFee(server.arg("plate"), paidCents, wasAlreadyPaid);

  bool paymentOk = (state == PaymentState::PAID);

  String message;
  if (!paymentOk) {
    message = F("Dieses Kennzeichen ist nicht im System. Eine Bezahlung ist nicht möglich.");
  } else if (wasAlreadyPaid) {
    message = String(F("Für dieses Kennzeichen wurde bereits bezahlt (")) + formatMoney(paidCents) + F(").");
  } else {
    message = String(F("Bezahlung erfolgt. Betrag: ")) + formatMoney(paidCents) + F(".");
  }

  String response = F("{\"ok\":");
  response += paymentOk ? F("true") : F("false");
  response += F(",\"status\":\"");
  response += paymentStateToText(state);
  response += F("\",\"alreadyPaid\":");
  response += wasAlreadyPaid ? F("true") : F("false");
  response += F(",\"amount\":\"");
  response += formatMoney(paidCents);
  response += F("\",\"message\":\"");
  response += escapeForJson(message);
  response += F("\"}");

  sendJsonResponse(200, response);
}


// Schliesst einen Parkvorgang ab und gibt den Platz frei.
//
// Pico 2 ruft diesen Endpunkt erst auf, nachdem die Kasse den Status paid
// gemeldet hat, und zwar noch bevor die Schranke aufgeht.
static void handleExitRequest() {
  bool finished = finishParking(server.arg("plate"));

  String response = F("{\"ok\":");
  response += finished ? F("true") : F("false");
  response += F(",\"message\":\"");
  response += finished ? F("Ausfahrt abgeschlossen, der Platz ist wieder frei.")
                       : F("Zu diesem Kennzeichen gibt es keinen bezahlten Parkvorgang.");
  response += F("\",\"occupied\":");
  response += countOccupiedSpots();
  response += F(",\"totalSpots\":");
  response += MAX_PARKING_SPOTS;
  response += F("}");

  sendJsonResponse(200, response);
}


// Stellt die Uhr der Kasse. Die Weboberfläche ruft das einmal beim Laden auf
// und schickt dabei die Uhrzeit des Laptops mit.
static void handleSetClock() {
  if (!server.hasArg("epoch")) {
    sendJsonResponse(400, F("{\"ok\":false,\"message\":\"Parameter epoch fehlt.\"}"));
    return;
  }

  uint32_t epochSeconds = (uint32_t)strtoul(server.arg("epoch").c_str(), nullptr, 10);

  // 1600000000 entspricht dem 13. September 2020. Jeder vernünftige
  // Zeitstempel liegt danach. Ein kleinerer Wert bedeutet, dass etwas
  // schiefgelaufen ist, zum Beispiel weil der Browser gar keine Zahl
  // geschickt hat. Dann würden alle Einfahrtszeiten im Jahr 1970 liegen.
  if (epochSeconds < 1600000000UL) {
    sendJsonResponse(400, F("{\"ok\":false,\"message\":\"Unplausible Zeitangabe.\"}"));
    return;
  }

  // Die Uhr wird nur einmal gestellt. Würde jeder Seitenaufruf sie neu
  // setzen, könnte ein zweiter Laptop mit falsch gestellter Uhr alle
  // bisherigen Anzeigen verschieben.
  if (!isWallClockSet()) {
    setWallClock(epochSeconds);
    Serial.print(F("[KASSE] Uhr gestellt: "));
    Serial.println(formatDateAndTime((int64_t)uptimeInSeconds()));
  }

  sendJsonResponse(200, F("{\"ok\":true}"));
}


#if TEST_BUILD
// ---------------------------------------------------------------------------
// Diese zwei Endpunkte gibt es nur im Test-Build.
//
// Mit ihnen lässt sich eine Parkdauer von mehreren Stunden in Sekunden
// prüfen. In einer echten Anlage wären sie eine offene Einladung zum Betrug,
// deshalb verschwinden sie im Produktivbuild vollständig aus dem Programm.
// ---------------------------------------------------------------------------

// Verschiebt die Einfahrtszeit eines Fahrzeugs in die Vergangenheit.
static void handleDebugMoveTime() {
  uint32_t minutes = (uint32_t)strtoul(server.arg("minutes").c_str(), nullptr, 10);
  String plate = server.arg("plate");

  bool moved = debugMoveEntryTimeBack(plate, minutes);

  String message;
  if (moved) {
    message = String(F("Einfahrtszeit um ")) + minutes + F(" Minuten zurückgesetzt.");
  } else {
    message = F("Kennzeichen steht nicht im Parkhaus.");
  }

  String response = F("{\"ok\":");
  response += moved ? F("true") : F("false");
  response += F(",\"message\":\"");
  response += escapeForJson(message);
  response += F("\"}");

  sendJsonResponse(200, response);
}


// Löscht alle Parkvorgänge.
static void handleDebugReset() {
  debugDeleteAllRecords();
  sendJsonResponse(200, F("{\"ok\":true,\"message\":\"Alle Parkvorgänge gelöscht.\"}"));
}
#endif


// Antwortet auf jede Adresse, für die es keinen Endpunkt gibt.
//
// Ohne diese Funktion bekäme der Browser bei einem Tippfehler eine leere
// Antwort und die Oberfläche würde kommentarlos nichts tun.
static void handleUnknownEndpoint() {
  sendJsonResponse(404, F("{\"ok\":false,\"message\":\"Unbekannter Endpunkt.\"}"));
}


// ---------------------------------------------------------------------------
// setup und loop
// ---------------------------------------------------------------------------

// setup() läuft genau einmal, direkt nach dem Einschalten.
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  // Diese kurze Pause gibt dem seriellen Monitor am PC Zeit, sich zu
  // verbinden. Ohne sie gehen die ersten Zeilen der Ausgabe verloren und man
  // sieht beim Start scheinbar gar nichts.
  delay(1500);

  Serial.println();
  Serial.println(F("=== Parksystem M450 - Pico 1 (Kasse) ==="));

  resetAllParkingRecords();
  startWifi(IP_CASHDESK, "parkhaus-kasse");

  // Jede Adresse bekommt die Funktion zugeordnet, die sie bedient.
  // Der Webserver merkt sich diese Zuordnung und ruft später selbst auf.
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


// loop() läuft danach immer wieder, viele tausend Mal pro Sekunde.
//
// Wichtig: In dieser Funktion darf nichts stehen, das lange wartet. Jede
// Wartezeit hier friert gleichzeitig die Weboberfläche ein, weil dann auch
// server.handleClient() nicht mehr drankommt.
void loop() {
  // Schaut nach, ob eine Browseranfrage da ist, und beantwortet sie.
  server.handleClient();

  // Baut die WLAN-Verbindung nach einem Ausfall wieder auf.
  keepWifiAlive();

  // Hält den überlaufsicheren Sekundenzähler aktuell. Der Rückgabewert wird
  // hier nicht gebraucht, der Aufruf selbst ist der Zweck.
  uptimeInSeconds();

  // Die eingebaute LED leuchtet, solange der Pico im WLAN ist. So sieht man
  // den Netzwerkzustand, ohne einen Bildschirm anzuschliessen.
  digitalWrite(LED_BUILTIN, isWifiConnected() ? HIGH : LOW);
}
