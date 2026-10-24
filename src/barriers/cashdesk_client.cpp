#include "cashdesk_client.h"
#include "../common/config.h"
#include "../common/network.h"

#include <WiFi.h>
#include <HTTPClient.h>

namespace {

  // Schreibt Sonderzeichen so um, dass sie in einer Webadresse stehen dürfen.
  //
  // In einer Adresse haben Zeichen wie Leerzeichen, Fragezeichen oder
  // kaufmännisches Und eine eigene Bedeutung. Stünde das Leerzeichen aus
  // "LU 12345" ungeschützt in der Adresse, würde der Server die Anfrage
  // falsch zerlegen.
  //
  // Erlaubt bleiben Buchstaben, Ziffern und die vier Zeichen - _ . ~
  // Alles andere wird zu einem Prozentzeichen mit der Zeichennummer in
  // Hexadezimalschreibweise. Aus dem Leerzeichen wird so %20.
  String encodeForUrl(const String& text) {
    String result;

    // Im schlimmsten Fall wird jedes Zeichen zu drei Zeichen. So viel
    // Speicher bestellen wir gleich vor.
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
        // %%  ergibt ein einzelnes Prozentzeichen
        // %02X schreibt die Zeichennummer mit zwei Stellen in Grossbuchstaben
        snprintf(buffer, sizeof(buffer), "%%%02X", (uint8_t)currentChar);
        result += buffer;
      }
    }

    return result;
  }


  // Baut den Anfang jeder Adresse zur Kasse, also "http://192.168.4.11".
  String buildCashDeskBaseUrl() {
    IPAddress cashDeskIp = IP_CASHDESK;

    String url = F("http://");
    url += cashDeskIp.toString();
    return url;
  }


  // Schickt eine Anfrage an die Kasse und gibt deren Antworttext zurück.
  //
  // path                  der Teil der Adresse nach der IP, zum Beispiel "/api/status?plate=LU%2012345"
  // reachableOut          wird auf true gesetzt, wenn eine Antwort kam
  //
  // Die Funktion wartet höchstens HTTP_TIMEOUT_MS lang. Danach gibt sie auf.
  String sendRequestToCashDesk(const String& path, bool& reachableOut) {
    reachableOut = false;

    // Ohne WLAN brauchen wir es gar nicht erst zu versuchen. Dieser frühe
    // Ausstieg spart im Fehlerfall die vollen zwei Sekunden Wartezeit, und
    // die Weboberfläche kann sofort eine Rückmeldung geben.
    if (!isWifiConnected()) {
      Serial.println(F("[KASSE-CLIENT] Kein WLAN, Aufruf uebersprungen."));
      return String();
    }

    WiFiClient client;

    // Diese Obergrenze gilt auch für den Verbindungsaufbau. Ohne sie würde
    // ein Aufruf bei abgeschaltetem Router viele Sekunden hängen bleiben und
    // dabei den ganzen Pico blockieren.
    client.setTimeout(HTTP_TIMEOUT_MS);

    HTTPClient http;
    http.setTimeout(HTTP_TIMEOUT_MS);

    // Keine Verbindung offen halten. Auf einem Gerät mit so wenig Speicher
    // ist es sicherer, jede Verbindung sauber zu schliessen, als sie für den
    // nächsten Aufruf aufzuheben.
    http.setReuse(false);

    String url = buildCashDeskBaseUrl() + path;

    if (!http.begin(client, url)) {
      Serial.println(F("[KASSE-CLIENT] Aufruf konnte nicht gestartet werden."));
      return String();
    }

    // GET() liefert bei Erfolg den HTTP-Statuscode, also eine positive Zahl
    // wie 200. Bei einem Fehler liefert es eine negative Zahl.
    int statusCode = http.GET();

    String responseBody;

    if (statusCode > 0) {
      responseBody     = http.getString();
      reachableOut     = true;
    } else {
      Serial.print(F("[KASSE-CLIENT] Kasse nicht erreichbar, Fehler "));
      Serial.println(statusCode);
    }

    // end() gibt die Verbindung frei. Diese Zeile steht absichtlich nach dem
    // if und nicht darin, damit sie in jedem Fall erreicht wird. Würde sie
    // im Fehlerfall übersprungen, bliebe jedes Mal eine Verbindung offen und
    // dem Pico ginge nach einigen Fehlversuchen der Speicher aus.
    http.end();

    return responseBody;
  }


  // Liest einen Wert, der in Anführungszeichen steht, zum Beispiel "paid".
  // startIndex zeigt auf das öffnende Anführungszeichen.
  String readQuotedValue(const String& json, int startIndex) {
    int valueStart = startIndex + 1;   // das Anführungszeichen überspringen
    int valueEnd   = json.indexOf('"', valueStart);

    if (valueEnd < 0) {
      return String();   // kein schliessendes Anführungszeichen gefunden
    }

    return json.substring(valueStart, valueEnd);
  }


  // Liest einen Wert ohne Anführungszeichen, also eine Zahl oder true/false.
  // Gelesen wird bis zum nächsten Komma oder bis zur schliessenden Klammer.
  String readPlainValue(const String& json, int startIndex) {
    int valueEnd = startIndex;

    while (valueEnd < (int)json.length()) {
      char currentChar = json.charAt(valueEnd);
      if (currentChar == ',' || currentChar == '}' || currentChar == ']') {
        break;
      }
      valueEnd++;
    }

    String value = json.substring(startIndex, valueEnd);
    value.trim();
    return value;
  }

}   // Ende des anonymen Namensraums


String readJsonField(const String& json, const String& fieldName) {
  // Gesucht wird der Text "feldname": also der Name in Anführungszeichen,
  // gefolgt von einem Doppelpunkt.
  String searchPattern = String('"') + fieldName + String("\":");

  int patternPosition = json.indexOf(searchPattern);
  if (patternPosition < 0) {
    return String();   // dieses Feld kommt in der Antwort nicht vor
  }

  // Hinter das Muster springen, dort beginnt der Wert.
  int valueStart = patternPosition + searchPattern.length();

  if (valueStart < (int)json.length() && json.charAt(valueStart) == '"') {
    return readQuotedValue(json, valueStart);
  }

  return readPlainValue(json, valueStart);
}


EntryReply askCashDeskToRegisterEntry(const String& plate) {
  EntryReply reply;

  // Zuerst alle Felder auf den Fall "nichts gehört" setzen. So ist die
  // Antwort auch dann vollständig, wenn die Funktion gleich wieder
  // zurückkehrt.
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


StatusReply askCashDeskForStatus(const String& plate) {
  StatusReply reply;
  reply.cashDeskReachable = false;

  String path = String(F("/api/status?plate=")) + encodeForUrl(plate);
  String json = sendRequestToCashDesk(path, reply.cashDeskReachable);

  if (!reply.cashDeskReachable) {
    return reply;
  }

  reply.paymentState = readJsonField(json, "status");
  reply.plate        = readJsonField(json, "plate");
  reply.fee          = readJsonField(json, "fee");

  return reply;
}


bool askCashDeskToFinishParking(const String& plate) {
  bool reachable = false;

  String path = String(F("/api/exit?plate=")) + encodeForUrl(plate);
  String json = sendRequestToCashDesk(path, reachable);

  if (!reachable) {
    return false;
  }

  return readJsonField(json, "ok") == "true";
}


OccupancyReply askCashDeskForOccupancy() {
  OccupancyReply reply;
  reply.cashDeskReachable = false;
  reply.occupied          = 0;
  reply.totalSpots        = MAX_PARKING_SPOTS;

  String json = sendRequestToCashDesk(F("/api/vehicles"), reply.cashDeskReachable);

  if (!reply.cashDeskReachable) {
    return reply;
  }

  reply.occupied   = readJsonField(json, "occupied").toInt();
  reply.totalSpots = readJsonField(json, "totalSpots").toInt();

  return reply;
}
