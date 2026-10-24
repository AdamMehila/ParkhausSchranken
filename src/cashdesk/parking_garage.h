#pragma once
#include <Arduino.h>
#include "../common/config.h"
#include "../common/license_plate.h"

// ===========================================================================
// Die Datenhaltung der Kasse. Hier liegt das Wissen darüber, welches
// Fahrzeug wann eingefahren ist und ob es bezahlt hat.
//
// Nur Pico 1 benutzt diese Datei. Pico 2 fragt diese Daten über das Netz ab
// und speichert selbst nichts. So gibt es im ganzen System genau eine
// Stelle, an der die Wahrheit steht. Zwei Geräte, die beide buchhalten,
// wären früher oder später verschieden.
//
// Alles liegt im Arbeitsspeicher. Ein Stromausfall löscht also alle Daten.
// Das ist für dieses Schulprojekt so gewollt und im README vermerkt.
// ===========================================================================


// So viele Parkvorgänge kann die Kasse gleichzeitig behalten.
//
// Die Zahl ist absichtlich viel grösser als MAX_PARKING_SPOTS, weil auch
// bereits ausgefahrene Fahrzeuge als Verlauf stehen bleiben sollen. Nur so
// kann ein Fahrzeug später erneut einfahren, ohne dass sein alter Eintrag
// verschwindet.
constexpr uint8_t MAX_PARKING_RECORDS = 24;


// Der Zustand eines einzelnen Eintrags.
enum class RecordStatus : uint8_t {
  UNUSED,          // dieser Platz in der Liste ist noch nie benutzt worden
  PARKED_UNPAID,   // Fahrzeug steht im Parkhaus, Gebühr noch offen
  PARKED_PAID,     // Fahrzeug steht im Parkhaus, Gebühr ist bezahlt
  FINISHED         // Fahrzeug ist ausgefahren, Eintrag bleibt als Verlauf
};


// Ein einzelner Parkvorgang.
//
// struct bündelt mehrere Werte, die zusammengehören, unter einem Namen.
// Statt vier einzelner Listen für Kennzeichen, Einfahrt, Ausfahrt und Betrag
// gibt es eine Liste von ParkingRecord. Dadurch können die Werte gar nicht
// durcheinandergeraten.
struct ParkingRecord {
  // Das Kennzeichen als klassisches Zeichen-Array statt als String.
  //
  // Der Grund ist der Speicher. Ein String holt sich bei jeder Änderung neu
  // Platz vom sogenannten Heap. Macht man das tausende Male, zerfällt der
  // freie Speicher in kleine Stücke und irgendwann passt nichts Grosses mehr
  // hinein. Das Array hier hat dagegen eine feste Grösse, die sich nie ändert.
  //
  // Die Grösse ist PLATE_MAX_LENGTH plus 2. Eines der beiden Zusatzzeichen
  // ist das abschliessende Nullzeichen, das jedes C-Zeichenarray braucht,
  // das andere ist Reserve.
  char plate[PLATE_MAX_LENGTH + 2];

  int64_t  entryUptimeSecond;   // Laufzeitsekunde der Einfahrt
  int64_t  exitUptimeSecond;    // Laufzeitsekunde der Ausfahrt
  uint32_t paidCents;           // tatsächlich bezahlter Betrag in Rappen
  RecordStatus status;
};


// Was beim Versuch einer Einfahrt herauskommen kann.
enum class EntryResult : uint8_t {
  ACCEPTED,        // Einfahrt wurde registriert
  INVALID_PLATE,   // Kennzeichen ist fehlerhaft
  GARAGE_FULL,     // kein freier Parkplatz mehr
  ALREADY_PARKED,  // dieses Kennzeichen steht schon im Parkhaus
  MEMORY_FULL      // die Liste der Vorgänge ist voll
};


// Der Bezahlzustand zu einem Kennzeichen. Genau diese drei Fälle fragt
// Pico 2 vor jeder Ausfahrt ab.
enum class PaymentState : uint8_t {
  PLATE_UNKNOWN,   // zu diesem Kennzeichen gibt es keine Einfahrt
  NOT_PAID_YET,    // Fahrzeug ist da, hat aber noch nicht bezahlt
  PAID             // Fahrzeug ist da und hat bezahlt
};


// Alle Angaben zu einem Fahrzeug auf einen Blick, zusammengefasst für die
// Weboberfläche und für Pico 2.
struct ParkingInfo {
  PaymentState state;
  String       plate;
  int64_t      entryUptimeSecond;
  uint32_t     parkedSeconds;
  uint32_t     feeInCents;
};


// Löscht alle Einträge und setzt die Liste auf den Anfangszustand.
// Wird einmal beim Start aufgerufen.
void resetAllParkingRecords();


// Registriert eine Einfahrt.
//
// userInput          das vom Benutzer eingegebene Kennzeichen
// plateOut           enthält danach die vereinheitlichte Schreibweise
// errorMessageOut    enthält bei einem Fehler den Text für die Oberfläche
EntryResult registerEntry(const String& userInput,
                          String& plateOut,
                          String& errorMessageOut);


// Sucht ein Fahrzeug, das gerade im Parkhaus steht, und liefert alle
// Angaben dazu. Ist das Kennzeichen unbekannt, steht im Ergebnis
// PaymentState::PLATE_UNKNOWN.
ParkingInfo lookupParkingInfo(const String& userInput);


// Verbucht die Bezahlung.
//
// paidCentsOut        enthält danach den bezahlten Betrag
// wasAlreadyPaidOut   wird true, wenn vorher schon bezahlt worden war
//
// Eine zweite Bezahlung wird bewusst nicht ausgeführt. Der Betrag bleibt
// derselbe, und der Aufrufer erfährt über wasAlreadyPaidOut, dass nichts
// Neues gebucht wurde.
PaymentState payParkingFee(const String& userInput,
                           uint32_t& paidCentsOut,
                           bool& wasAlreadyPaidOut);


// Schliesst einen Parkvorgang ab und gibt den Platz wieder frei.
// Das geht nur bei einem Fahrzeug, das bezahlt hat.
// Gibt true zurück, wenn es geklappt hat.
bool finishParking(const String& userInput);


// Wie viele Fahrzeuge stehen gerade im Parkhaus.
uint8_t countOccupiedSpots();

// Wie viele Plätze sind noch frei.
uint8_t countFreeSpots();


// Berechnet die Gebühr für eine Parkdauer.
// Tarif: 2 Franken pro angefangene Stunde, mindestens eine Stunde.
uint32_t calculateFeeInCents(uint32_t parkedSeconds);


// Gibt einen einzelnen Eintrag zum Anschauen heraus.
//
// Der Rückgabetyp ist ein Zeiger auf const. Das const bedeutet: Lesen ja,
// Ändern nein. Ohne dieses const könnte jede beliebige Stelle im Programm
// einen Eintrag verändern, zum Beispiel einen Betrag auf null setzen, und
// dabei die Regeln dieser Datei umgehen.
//
// Bei einem zu grossen index kommt nullptr zurück, also ein Zeiger, der auf
// nichts zeigt. Der Aufrufer muss das prüfen.
const ParkingRecord* getParkingRecord(uint8_t index);


#if TEST_BUILD
// ---------------------------------------------------------------------------
// Die folgenden zwei Funktionen gibt es nur im Test-Build.
//
// Das #if ist eine Anweisung an den Präprozessor. Ist TEST_BUILD nicht
// gesetzt, verschwinden diese Zeilen vollständig, bevor der Compiler sie
// überhaupt zu sehen bekommt. In einer echten Anlage wäre eine Funktion, die
// Einfahrtszeiten verschiebt, nämlich eine offene Tür für Betrug.
// ---------------------------------------------------------------------------

// Schiebt die Einfahrtszeit eines Fahrzeugs künstlich in die Vergangenheit.
// Damit lässt sich eine Parkdauer von drei Stunden in drei Sekunden testen.
bool debugMoveEntryTimeBack(const String& userInput, uint32_t minutes);

// Löscht alle Parkvorgänge, damit ein Testfall sauber von vorne beginnt.
void debugDeleteAllRecords();
#endif
