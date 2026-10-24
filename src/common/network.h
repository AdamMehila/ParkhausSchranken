#pragma once
#include <Arduino.h>
#include <IPAddress.h>

// ===========================================================================
// WLAN-Verbindung. Beide Picos benutzen diese Datei.
//
// Die IP-Adresse wird fest gesetzt und nicht vom Router zugeteilt. Nur so
// findet Pico 2 die Kasse nach jedem Neustart zuverlässig an derselben
// Adresse.
// ===========================================================================


// Verbindet den Pico mit dem Test-WLAN.
//
// ownIp     ist die feste Adresse, die dieser Pico bekommen soll
// hostname  ist ein Name, unter dem das Gerät im Netz erscheint
//
// Die Funktion wartet höchstens 20 Sekunden auf die Verbindung. Danach läuft
// das Programm auf jeden Fall weiter. Ein Pico, der ewig auf das WLAN wartet,
// wirkt für den Benutzer wie abgestürzt. Deshalb gibt es diese Obergrenze.
void startWifi(IPAddress ownIp, const char* hostname);


// Muss in jedem Durchlauf von loop() aufgerufen werden.
//
// Die Funktion prüft, ob die Verbindung noch steht, und baut sie nach einem
// Ausfall selbständig wieder auf. Sie wartet dabei nie, sie schaut nur kurz
// nach und kehrt sofort zurück.
void keepWifiAlive();


// Sagt, ob gerade eine WLAN-Verbindung besteht.
bool isWifiConnected();
