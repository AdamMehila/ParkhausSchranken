#pragma once
#include <Arduino.h>

// ===========================================================================
// Prüfung und Vereinheitlichung von Autokennzeichen.
//
// Beide Picos benutzen diese Datei. Pico 2 prüft die Eingabe sofort beim
// Tippen, und Pico 1 prüft sie noch einmal, bevor etwas gespeichert wird.
//
// Warum doppelt? Weil jemand die Adresse der Kasse auch direkt im Browser
// aufrufen und Pico 2 damit überspringen könnte. Die Kasse darf sich also
// nicht darauf verlassen, dass vorher schon jemand geprüft hat.
// Dieses Vorgehen nennt man gestaffelte Absicherung.
//
// Gültiges Format: zwei Buchstaben für den Kanton, ein Leerzeichen, dann
// eine bis sechs Ziffern. Beispiel: LU 12345
// ===========================================================================


// Das Ergebnis einer Prüfung. Statt einfach true oder false zurückzugeben
// sagen wir genau, was nicht stimmt. Nur so kann die Weboberfläche dem
// Benutzer einen hilfreichen Satz anzeigen statt nur "Fehler".
enum class PlateCheckResult : uint8_t {
  VALID,         // alles in Ordnung
  EMPTY,         // es wurde gar nichts eingegeben
  TOO_LONG,      // mehr Zeichen als ein Kennzeichen haben kann
  WRONG_FORMAT   // falscher Aufbau, zum Beispiel Sonderzeichen oder nur Ziffern
};


// Bringt eine Eingabe in die einheitliche Schreibweise.
//
// Konkret passiert Folgendes:
//   Leerzeichen am Anfang und Ende fallen weg
//   mehrere Leerzeichen hintereinander werden zu einem einzigen
//   alle Buchstaben werden zu Grossbuchstaben
//   fehlt das Leerzeichen zwischen Kanton und Zahl, wird es eingefügt
//
// "  lu  12345 " und "lu12345" ergeben beide "LU 12345".
//
// Das ist wichtig, damit ein Fahrzeug beim Ausfahren wiedergefunden wird,
// auch wenn der Benutzer es dort anders tippt als beim Einfahren.
String normalizePlate(const String& userInput);


// Prüft eine Eingabe auf Gültigkeit.
//
// userInput          ist der Text, den der Benutzer eingegeben hat
// normalizedPlateOut wird von der Funktion gefüllt und enthält danach die
//                    vereinheitlichte Schreibweise
//
// Der zweite Parameter ist eine Referenz ohne const. Das bedeutet, dass die
// Funktion ihn verändern darf. Auf diese Weise kann eine Funktion zwei Dinge
// zurückgeben: das Prüfergebnis und nebenbei das bereinigte Kennzeichen.
PlateCheckResult checkPlate(const String& userInput, String& normalizedPlateOut);


// Liefert den passenden Fehlertext zu einem Prüfergebnis.
// Bei PlateCheckResult::VALID kommt ein leerer Text zurück, denn dann gibt
// es nichts zu melden.
String plateErrorMessage(PlateCheckResult result);
