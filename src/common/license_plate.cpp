#include "license_plate.h"
#include "config.h"

// Der anonyme Namensraum macht alles darin nur für diese eine Datei sichtbar.
// Andere Dateien können diese Hilfsfunktionen also nicht aufrufen und auch
// nicht versehentlich einen gleichnamigen Namen verwenden.
namespace {

  // Schreibt den Text in Grossbuchstaben und entfernt überflüssige
  // Leerzeichen. Aus "  lu   12345 " wird "LU 12345".
  String toUpperCaseWithSingleSpaces(const String& text) {
    String trimmed = text;
    trimmed.trim();   // entfernt Leerzeichen am Anfang und am Ende

    String result;
    // Wir wissen ungefähr, wie lang das Ergebnis wird. Mit reserve bestellen
    // wir den Speicher im Voraus, statt ihn bei jedem Zeichen neu anzufordern.
    result.reserve(trimmed.length() + 1);

    bool previousCharWasSpace = false;

    for (unsigned int i = 0; i < trimmed.length(); i++) {
      char currentChar = trimmed.charAt(i);
      bool isSpace = (currentChar == ' ' || currentChar == '\t');

      if (isSpace) {
        // Ein Leerzeichen wird nur übernommen, wenn davor kein Leerzeichen
        // stand und schon mindestens ein Zeichen geschrieben wurde.
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


  // Prüft, ob ein Text aussieht wie ein Kennzeichen, bei dem nur das
  // Leerzeichen fehlt. Beispiel: "LU12345"
  bool looksLikePlateWithMissingSpace(const String& text) {
    if (text.indexOf(' ') >= 0) {
      return false;   // es gibt schon ein Leerzeichen, also nichts zu tun
    }
    if (text.length() < 3) {
      return false;   // zu kurz für zwei Buchstaben und mindestens eine Ziffer
    }
    return isAlpha(text.charAt(0))
        && isAlpha(text.charAt(1))
        && isDigit(text.charAt(2));
  }


  // Schiebt nach den ersten zwei Buchstaben ein Leerzeichen ein.
  // Aus "LU12345" wird "LU 12345".
  String insertSpaceAfterCantonCode(const String& text) {
    return text.substring(0, 2) + " " + text.substring(2);
  }


  // Prüft den vorderen Teil: zwei Buchstaben, danach genau an Position 2
  // ein Leerzeichen. Die Zählung beginnt bei 0, Position 2 ist also das
  // dritte Zeichen.
  bool hasValidCantonCode(const String& plate) {
    if (plate.indexOf(' ') != 2) {
      return false;
    }
    return isAlpha(plate.charAt(0)) && isAlpha(plate.charAt(1));
  }


  // Prüft den hinteren Teil: eine bis sechs Ziffern und sonst nichts.
  bool hasValidNumberPart(const String& plate) {
    String numberPart = plate.substring(3);   // alles nach "XY "

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

}   // Ende des anonymen Namensraums


String normalizePlate(const String& userInput) {
  String result = toUpperCaseWithSingleSpaces(userInput);

  // Viele Leute tippen das Kennzeichen ohne Leerzeichen. Wir setzen es
  // nachträglich, damit niemand nur wegen eines fehlenden Trennzeichens
  // eine Fehlermeldung bekommt.
  if (looksLikePlateWithMissingSpace(result)) {
    result = insertSpaceAfterCantonCode(result);
  }

  return result;
}


PlateCheckResult checkPlate(const String& userInput, String& normalizedPlateOut) {
  normalizedPlateOut = normalizePlate(userInput);

  if (normalizedPlateOut.length() == 0) {
    return PlateCheckResult::EMPTY;
  }

  // Die Längenprüfung kommt absichtlich vor der Formatprüfung.
  //
  // Gibt jemand 30 Buchstaben ein, dann ist sowohl die Länge als auch das
  // Format falsch. Der hilfreichere Satz für den Benutzer ist aber "zu lang"
  // und nicht "falsches Format". Darum wird diese Prüfung zuerst gemacht.
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


String plateErrorMessage(PlateCheckResult result) {
  switch (result) {
    case PlateCheckResult::EMPTY:
      return F("Bitte ein Kennzeichen eingeben.");

    case PlateCheckResult::TOO_LONG:
      return F("Das Kennzeichen ist zu lang. Erlaubt sind höchstens 10 Zeichen, zum Beispiel LU 12345.");

    case PlateCheckResult::WRONG_FORMAT:
      return F("Das ist kein gültiges Kennzeichen. Erwartet wird zum Beispiel LU 12345.");

    default:
      // Dieser Fall ist PlateCheckResult::VALID. Dann gibt es keinen Fehler
      // und wir geben einen leeren Text zurück.
      return String();
  }
}
