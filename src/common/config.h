#pragma once
#include <Arduino.h>
#include <IPAddress.h>

// ===========================================================================
// Zentrale Einstellungen des Parksystems.
//
// Alle Zahlen, die man beim Testen vielleicht ändern möchte, stehen in dieser
// einen Datei. Wer den Tarif oder die Anzahl Plätze anpassen will, muss also
// nicht im Programmcode suchen.
//
// Beide Picos binden diese Datei ein. Dadurch kann es gar nicht passieren,
// dass die Kasse mit anderen Werten rechnet als die Schranke.
//
// Hinweis zur Schreibweise: Namen in GROSSBUCHSTABEN sind Konstanten. Sie
// werden im laufenden Programm nie verändert.
// ===========================================================================


// --- WLAN ------------------------------------------------------------------
// Diese Zugangsdaten gelten für beide Picos. Trage hier dein eigenes WLAN ein.
//
// Wichtig: Der Pico W kann nur 2,4 GHz. Trage also den Namen des 2,4-GHz-Netzes
// ein. Viele Router haben dafür einen eigenen Namen, etwa mit dem Zusatz
// "-2.4GHz".
#define WIFI_SSID     "DEIN-WLAN-NAME"
#define WIFI_PASSWORD "DEIN-WLAN-PASSWORT"


// --- Feste IP-Adressen -----------------------------------------------------
// Jeder Pico bekommt immer dieselbe Adresse. Das ist wichtig, weil Pico 2 die
// Kasse sonst nach jedem Neustart an einer anderen Stelle suchen müsste.
//
// Passe die Werte an dein Netz an. Die Adressen der Picos müssen im selben
// Bereich liegen wie dein Router und dürfen noch von keinem Gerät benutzt
// werden. Gateway und Subnetzmaske zeigt "ipconfig" am Laptop.
#define IP_CASHDESK    IPAddress(192, 168, 1, 201)  // Pico 1, die Kasse
#define IP_BARRIERS    IPAddress(192, 168, 1, 202)  // Pico 2, die Schranke
#define IP_GATEWAY     IPAddress(192, 168, 1, 1)    // der Router
#define IP_SUBNET_MASK IPAddress(255, 255, 255, 0)

// Port 80 ist der Standard für Webseiten. Deshalb genügt im Browser die
// Adresse http://192.168.1.201 ohne Angabe einer Portnummer.
constexpr uint16_t WEB_SERVER_PORT = 80;


// --- Parkhaus --------------------------------------------------------------
// Bewusst sehr klein gewählt. Mit drei Plätzen lässt sich der Fall
// "Parkhaus voll" in einer Minute vorführen statt in einer Stunde.
constexpr uint8_t MAX_PARKING_SPOTS = 3;

// Der Tarif beträgt 2 Franken pro angefangene Stunde.
//
// Der Betrag wird in Rappen gespeichert, also als ganze Zahl. Geld wird nie
// mit Kommazahlen gerechnet, weil ein double den Wert 0.10 nicht genau
// treffen kann. Nach vielen Rechnungen entstehen daraus sichtbare Rundungs-
// fehler. Mit Rappen als ganzer Zahl kann das nicht passieren.
constexpr uint32_t CENTS_PER_HOUR = 200;   // 200 Rappen sind CHF 2.00


// --- Schranke und Servos ---------------------------------------------------
//
// SERVO_COUNT sagt, wie viele Servos tatsächlich angeschlossen sind:
//
//   1 = EIN Servo an GP8 bedient Einfahrt und Ausfahrt gemeinsam.
//       Der Schrankenarm ist dann physisch derselbe. Die Fachlogik bleibt
//       getrennt, es prüft also weiterhin jeder Vorgang seine eigenen
//       Bedingungen. Nur der Antrieb wird geteilt.
//       Das entspricht einem kleinen Parkhaus mit kombinierter Ein- und
//       Ausfahrt. Dies ist die Standardeinstellung.
//
//   2 = ZWEI Servos, Einfahrt an GP8 und Ausfahrt an GP9.
//       Dafür genügt es, diesen Wert zu ändern. Am übrigen Code ändert sich
//       nichts.
//
// Statt die Datei zu ändern kann man den Wert auch beim Bauen mitgeben:
//   build_flags = ... -DSERVO_COUNT=2
#ifndef SERVO_COUNT
  #define SERVO_COUNT 1
#endif

// Die Verkabelung, für die diese Werte gelten:
//   rot    (Strom 5 V)  an Pin 40, VBUS
//   braun  (Masse)      an Pin 8, GND
//   orange (Signal)     an Pin 11, GP8
//
// Achtung: Die GPIO-Nummer ist nicht dieselbe Zahl wie die Pin-Nummer auf der
// Platine. GP8 liegt am physischen Pin 11, GP9 am physischen Pin 12.
// Im Programm zählt immer die GPIO-Nummer. Steckt das orange Kabel woanders,
// muss hier die passende GPIO-Nummer stehen, sonst bewegt sich nichts.
constexpr uint8_t SERVO_PIN_ENTRY = 8;    // GP8, Pin 11
constexpr uint8_t SERVO_PIN_EXIT  = 9;    // GP9, Pin 12, nur bei SERVO_COUNT 2 in Verwendung

// Die beiden Stellungen des Schrankenarms.
//
// Welcher Winkel "zu" und welcher "offen" bedeutet, hängt davon ab, wie der
// rote Arm auf die Achse des Servos gesteckt ist. Bei unserem Aufbau ist der
// Arm bei 90 Grad waagrecht und bei 0 Grad senkrecht. Das Testkonzept nennt es
// umgekehrt (zu bei 0, offen bei 90). Fährt die Schranke falsch herum, tauscht
// man einfach diese beiden Zahlen.
constexpr uint8_t SERVO_ANGLE_CLOSED = 90;   // Arm liegt waagrecht, Durchfahrt gesperrt
constexpr uint8_t SERVO_ANGLE_OPEN   = 0;    // Arm steht senkrecht, Durchfahrt frei

// So lange bleibt die Schranke nach dem Öffnen offen, danach schliesst sie
// von selbst. 5000 Millisekunden sind 5 Sekunden.
constexpr uint32_t BARRIER_OPEN_TIME_MS = 5000;


// --- Verbindung zwischen den beiden Picos ----------------------------------
// Wie lange Pico 2 höchstens auf eine Antwort der Kasse wartet.
//
// Der Wert ist absichtlich klein. Wäre er gross, würde ein Ausfall der Kasse
// die Weboberfläche von Pico 2 für viele Sekunden einfrieren. So merkt der
// Benutzer nach spätestens zwei Sekunden, dass etwas nicht stimmt.
constexpr uint16_t HTTP_TIMEOUT_MS = 2000;


// --- Kennzeichen -----------------------------------------------------------
// Höchstlänge eines Kennzeichens inklusive Leerzeichen, zum Beispiel
// "LU 123456". Längere Eingaben werden abgewiesen.
constexpr uint8_t PLATE_MAX_LENGTH = 10;
