#include "network.h"
#include "config.h"
#include <WiFi.h>
#include <pico/cyw43_arch.h>

namespace {

  // Nach so vielen Millisekunden gibt startWifi das Warten auf.
  constexpr uint32_t CONNECT_TIMEOUT_MS = 20000;

  // So lange wird nach einem Fehlschlag gewartet, bevor der nächste Versuch
  // startet. Ohne diese Pause würde der Pico ununterbrochen Verbindungs-
  // versuche abfeuern und nichts anderes mehr zustande bringen.
  constexpr uint32_t RETRY_INTERVAL_MS = 15000;

  // Der Wert, den der WLAN-Treiber intern während und nach einer
  // erfolgreichen Anmeldung führt. Er stammt aus cyw43_ctrl.c im Pico-SDK.
  constexpr uint32_t JOIN_STATE_ACTIVE = 0x0001;

  // So oft wird der Funkchip höchstens gefragt, ob er verbunden ist.
  // Jede Frage geht über eine Datenleitung zum Chip und kostet Zeit. Bei
  // tausenden loop()-Durchläufen pro Sekunde wäre das zu viel.
  constexpr uint32_t LINK_CHECK_INTERVAL_MS = 500;

  IPAddress g_ownIp;
  uint32_t  g_lastAttemptMs   = 0;
  bool      g_wasConnected    = false;
  bool      g_lastLinkResult  = false;
  uint32_t  g_lastLinkCheckMs = 0;


  // Fragt direkt beim Funkchip nach, ob er gerade mit einem Router
  // verbunden ist.
  //
  // Warum so umständlich und nicht einfach WiFi.status() == WL_CONNECTED?
  // Weil das bei einer festen IP-Adresse lügt. Die Bibliothek meldet dann
  // schon "verbunden", sobald die Anmeldung BEGINNT. Scheitert sie eine
  // Sekunde später, sieht es aus, als wäre die Verbindung abgerissen,
  // obwohl sie nie bestanden hat. Genau das hat bei der Fehlersuche lange
  // in die falsche Richtung geführt.
  //
  // Diese Prüfung verlangt zwei Dinge gleichzeitig:
  //   1. Der Treiber ist mit der Anmeldung fertig. Während sie läuft, sammelt
  //      er Zwischenschritte als zusätzliche Bits, der Wert ist dann nicht
  //      genau JOIN_STATE_ACTIVE.
  //   2. Der Chip kann die Adresse des Routers nennen, mit dem er verbunden
  //      ist. Ohne Verbindung meldet er hier einen Fehler.
  bool askChipIfConnected() {
    if (cyw43_state.wifi_join_state != JOIN_STATE_ACTIVE) {
      return false;
    }

    uint8_t routerAddress[6];
    return cyw43_wifi_get_bssid(&cyw43_state, routerAddress) == 0;
  }


  // Liefert den Verbindungszustand, fragt den Chip aber höchstens alle
  // 500 Millisekunden. Dazwischen kommt die letzte Antwort zurück.
  bool hasRealWifiLink() {
    if (millis() - g_lastLinkCheckMs >= LINK_CHECK_INTERVAL_MS) {
      g_lastLinkCheckMs = millis();
      g_lastLinkResult  = askChipIfConnected();
    }
    return g_lastLinkResult;
  }


  // Wie hasRealWifiLink, fragt aber auf jeden Fall sofort nach.
  // Gebraucht beim Start, wo es auf jede Viertelsekunde ankommt.
  bool checkWifiLinkNow() {
    g_lastLinkCheckMs = millis();
    g_lastLinkResult  = askChipIfConnected();
    return g_lastLinkResult;
  }


  // Übersetzt den Zustand des WLAN-Chips in einen Satz, der sagt, woran es
  // hängt. Ohne diese Funktion wüsste man nur "geht nicht", aber nicht warum.
  const __FlashStringHelper* describeWifiProblem() {
    switch (cyw43_wifi_link_status(&cyw43_state, CYW43_ITF_STA)) {
      case CYW43_LINK_BADAUTH:
        return F("Anmeldung abgelehnt. Passwort falsch oder der Router verlangt WPA3, das der Pico nicht kann.");
      case CYW43_LINK_NONET:
        return F("Netz nicht gefunden. Funkt der Router nur auf 5 GHz? Der Pico kann nur 2,4 GHz.");
      case CYW43_LINK_FAIL:
        return F("Anmeldung fehlgeschlagen. Der Router hat den Pico abgewiesen.");
      case CYW43_LINK_JOIN:
        return F("Anmeldung laeuft noch.");
      default:
        return F("Keine Verbindung.");
    }
  }


  // Übersetzt die Verschlüsselungsart aus dem Netz-Scan in Klartext.
  const __FlashStringHelper* encryptionToText(uint8_t encryptionType) {
    switch (encryptionType) {
      case ENC_TYPE_NONE: return F("offen");
      case ENC_TYPE_WEP:  return F("WEP");
      case ENC_TYPE_TKIP: return F("WPA");
      case ENC_TYPE_CCMP: return F("WPA2");
      case ENC_TYPE_GCMP: return F("WPA3");
      case ENC_TYPE_AUTO: return F("WPA/WPA2");
      default:            return F("unbekannt");
    }
  }


  // Sucht einmal alle WLANs in der Umgebung und schreibt sie in den Monitor.
  //
  // Das ist eine reine Hilfe für die Fehlersuche. Der Pico sieht nur Netze
  // auf 2,4 GHz. Fehlt das eigene Netz in dieser Liste, dann liegt das
  // Problem beim Router und nicht im Programm.
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

      // Das eigene Netz wird mit einem Pfeil markiert, damit man es in einer
      // langen Liste sofort findet.
      Serial.print(isOwnNetwork ? F("  --> ") : F("      "));
      Serial.print(WiFi.SSID(i));
      Serial.print(F("  Signal "));
      Serial.print(WiFi.RSSI(i));
      Serial.print(F(" dBm  Kanal "));
      Serial.print(WiFi.channel(i));
      Serial.print(F("  "));
      Serial.println(encryptionToText(WiFi.encryptionType(i)));
    }

    if (!ownNetworkFound) {
      Serial.print(F("[WLAN] ACHTUNG: "));
      Serial.print(F(WIFI_SSID));
      Serial.println(F(" ist nicht in der Liste. Der Router funkt vermutlich nur auf 5 GHz."));
    }

    // Die Liste wird nicht mehr gebraucht, also Speicher freigeben.
    WiFi.scanDelete();
  }


  // Startet einen Verbindungsversuch.
  void beginConnecting() {
    // Achtung bei der Reihenfolge der Parameter. Sie lautet in dieser
    // Bibliothek config(ip, dns, gateway, subnet) und nicht, wie man es
    // erwarten würde, config(ip, gateway, subnet, dns).
    // Vertauscht man zwei davon, verbindet sich der Pico scheinbar, kann
    // aber keine andere Adresse erreichen. Als DNS-Server dient hier der
    // Router selbst.
    WiFi.config(g_ownIp, IP_GATEWAY, IP_GATEWAY, IP_SUBNET_MASK);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  }


  // Wartet, bis die Verbindung steht, höchstens aber timeoutMs lang.
  // Gibt true zurück, wenn die Verbindung zustande kam.
  bool waitForConnection(uint32_t timeoutMs) {
    uint32_t startedAt = millis();

    while (!checkWifiLinkNow() && (millis() - startedAt) < timeoutMs) {
      delay(250);
      Serial.print('.');   // ein Punkt pro Viertelsekunde als Lebenszeichen
    }
    Serial.println();

    return checkWifiLinkNow();
  }


  // Schreibt eine Meldung in den seriellen Monitor, aber nur dann, wenn sich
  // der Zustand gerade geändert hat.
  //
  // Ohne diese Prüfung käme bei jedem Durchlauf von loop() eine neue Zeile,
  // also mehrere tausend pro Sekunde. Der Monitor wäre unbrauchbar. So
  // erscheint genau eine Zeile in dem Moment, in dem die Verbindung abbricht
  // oder zurückkommt.
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

}


void startWifi(IPAddress ownIp, const char* hostname) {
  g_ownIp = ownIp;

  WiFi.setHostname(hostname);
  WiFi.mode(WIFI_STA);   // STA heisst Station, der Pico ist also Teilnehmer
                         // in einem fremden Netz und nicht selbst der Router

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


void keepWifiAlive() {
  bool isConnectedNow = hasRealWifiLink();

  logOnlyWhenStateChanges(isConnectedNow);

  if (isConnectedNow) {
    return;   // alles in Ordnung, nichts zu tun
  }

  // Ab hier ist die Verbindung weg. Wir versuchen es erneut, aber nur alle
  // 15 Sekunden. Zwischen den Versuchen bedient der Pico weiterhin seine
  // Weboberfläche, er bleibt also benutzbar.
  if (millis() - g_lastAttemptMs > RETRY_INTERVAL_MS) {
    g_lastAttemptMs = millis();
    Serial.print(F("[WLAN] Neuer Verbindungsversuch. Letzter Stand: "));
    Serial.println(describeWifiProblem());
    beginConnecting();
  }
}


bool isWifiConnected() {
  return hasRealWifiLink();
}
