#pragma once
#include <Arduino.h>

// ===========================================================================
// Zeitrechnung des Parksystems.
//
// Das Grundproblem: Der Pico hat keine Uhr mit Batterie. Nach jedem Neustart
// weiss er nicht, welches Datum ist. Er weiss nur, wie viele Millisekunden
// seit dem Einschalten vergangen sind.
//
// Die Lösung besteht aus zwei getrennten Teilen.
//
// 1. Für das Rechnen benutzen wir nur die Laufzeit seit dem Start, gemessen
//    in Sekunden. Diese Zahl läuft immer gleichmässig weiter und kann nicht
//    falsch sein. Parkdauer und Gebühr stimmen deshalb immer, auch wenn
//    niemand je das Datum gesetzt hat.
//
// 2. Für die Anzeige brauchen wir trotzdem Datum und Uhrzeit. Deshalb
//    schickt die Weboberfläche beim ersten Laden einmal die Uhrzeit des
//    Laptops an die Kasse. Ab dann kann die Kasse jede gespeicherte
//    Laufzeitsekunde in ein richtiges Datum umrechnen.
//
// Der schöne Nebeneffekt: Weil intern Laufzeitsekunden gespeichert sind,
// bekommen auch Parkvorgänge von vor dem Stellen der Uhr nachträglich die
// richtige Uhrzeit angezeigt.
// ===========================================================================


// Laufzeit seit dem Start des Pico, in Sekunden.
//
// Diese Funktion muss regelmässig aufgerufen werden, mindestens einmal alle
// paar Minuten. Der Grund steht ausführlich in der .cpp-Datei. Kurz gesagt
// springt die eingebaute Funktion millis() nach rund 49 Tagen zurück auf
// null, und diese Funktion hier fängt den Sprung ab.
uint32_t uptimeInSeconds();


// Stellt die Uhr. localUnixTime ist die Anzahl Sekunden seit dem 1.1.1970,
// bereits auf die lokale Zeitzone umgerechnet.
// Die Weboberfläche schickt diesen Wert einmal beim Laden der Seite.
void setWallClock(uint32_t localUnixTime);


// Sagt, ob die Uhr schon gestellt wurde. Solange das nicht passiert ist,
// zeigt die Oberfläche bei allen Zeitangaben zwei Striche an.
bool isWallClockSet();


// Rechnet eine gespeicherte Laufzeitsekunde in eine echte Unix-Zeit um.
int64_t uptimeSecondToUnixTime(int64_t uptimeSecond);


// Macht aus einer Laufzeitsekunde einen lesbaren Text wie
// "15.10.2026 14:05:03". Ist die Uhr noch nicht gestellt, kommt "--" zurück.
String formatDateAndTime(int64_t uptimeSecond);


// Macht aus einer Anzahl Sekunden einen Text wie "2 h 05 min".
String formatDuration(uint32_t seconds);


// Macht aus einem Betrag in Rappen einen Text wie "CHF 4.00".
String formatMoney(uint32_t cents);
