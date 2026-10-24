#pragma once
#include <Arduino.h>

// ===========================================================================
// Text sicher machen für die Ausgabe in JSON.
//
// JSON ist das Datenformat, in dem die beiden Picos und die Weboberfläche
// miteinander reden. Ein Text steht darin zwischen doppelten Anführungs-
// zeichen:
//
//     {"meldung":"Einfahrt registriert"}
//
// Jetzt stell dir vor, der Benutzer gibt als Kennzeichen " ein. Dann würde
// daraus:
//
//     {"meldung":"Ungueltig: """}
//
// Das ist kaputtes JSON. Der Browser kann es nicht lesen, meldet einen
// Fehler, und die Oberfläche zeigt eine völlig falsche Ursache an. Im
// schlimmeren Fall könnte jemand damit sogar eigene Felder in die Antwort
// schmuggeln.
//
// Die Funktion escapeForJson ersetzt deshalb alle Zeichen, die JSON eine
// besondere Bedeutung gibt, durch eine ungefährliche Schreibweise.
//
// Warum steht die Funktion komplett im Header und nicht in einer
// .cpp-Datei? Weil sie kurz ist und von mehreren Dateien gebraucht wird.
// Das Wort inline erlaubt es, dass sie dadurch mehrfach im Programm landet,
// ohne dass sich der Linker darüber beschwert.
// ===========================================================================

inline String escapeForJson(const String& text) {
  String result;
  result.reserve(text.length() + 8);

  for (unsigned int i = 0; i < text.length(); i++) {
    char currentChar = text.charAt(i);

    switch (currentChar) {
      // Ein Anführungszeichen würde den Text vorzeitig beenden.
      case '"':  result += "\\\"";  break;

      // Der Rückwärtsstrich leitet in JSON selbst eine Ersetzung ein und
      // muss deshalb ebenfalls ersetzt werden.
      case '\\': result += "\\\\";  break;

      // Zeilenumbruch, Wagenrücklauf und Tabulator dürfen in einem
      // JSON-Text nicht direkt vorkommen.
      case '\n': result += "\\n";   break;
      case '\r': result += "\\r";   break;
      case '\t': result += "\\t";   break;

      default:
        // Alle übrigen Steuerzeichen, also alles unterhalb von Code 32,
        // werden als sechsstellige Ersatzschreibweise ausgegeben.
        // Aus Code 1 wird zum Beispiel \u0001.
        if ((uint8_t)currentChar < 0x20) {
          char escapeBuffer[8];
          snprintf(escapeBuffer, sizeof(escapeBuffer), "\\u%04x", (uint8_t)currentChar);
          result += escapeBuffer;
        } else {
          // Ganz normales Zeichen, es wird unverändert übernommen.
          result += currentChar;
        }
    }
  }

  return result;
}
