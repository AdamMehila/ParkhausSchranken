#include "parking_garage.h"
#include "../common/clock_time.h"

namespace {

  // Die eigentliche Liste aller Parkvorgänge.
  //
  // Das ist ein Array mit fester Grösse. Es steht beim Start des Programms
  // fest an einer Stelle im Speicher und wächst nie. Genau das wollen wir
  // auf einem Mikrocontroller: Wir wissen von Anfang an, wie viel Speicher
  // das Programm braucht, und es kann niemals unterwegs ausgehen.
  ParkingRecord g_records[MAX_PARKING_RECORDS];


  // Sagt, ob ein Eintrag zu einem Fahrzeug gehört, das gerade im Parkhaus
  // steht. Bezahlt oder nicht spielt dabei keine Rolle.
  bool isCurrentlyParked(const ParkingRecord& record) {
    return record.status == RecordStatus::PARKED_UNPAID
        || record.status == RecordStatus::PARKED_PAID;
  }


  // Sucht das Fahrzeug mit diesem Kennzeichen, sofern es gerade im Parkhaus
  // steht. Gibt nullptr zurück, wenn es keines gibt.
  //
  // Der Rückgabetyp ist ein Zeiger ohne const, diese Funktion darf das
  // Ergebnis also verändern. Sie ist deshalb absichtlich nur innerhalb
  // dieser Datei sichtbar. Nach aussen gibt es nur getParkingRecord, und
  // das gibt einen Zeiger auf const heraus.
  ParkingRecord* findActiveRecord(const String& plate) {
    for (uint8_t i = 0; i < MAX_PARKING_RECORDS; i++) {
      ParkingRecord& record = g_records[i];

      // Die Klammern sind hier wichtig. Ohne sie würde C++ zuerst das &&
      // auswerten und die Bedingung bekäme eine völlig andere Bedeutung.
      if (isCurrentlyParked(record) && plate.equals(record.plate)) {
        return &record;
      }
    }
    return nullptr;
  }


  // Sucht einen Platz in der Liste für einen neuen Parkvorgang.
  //
  // Zuerst wird ein noch nie benutzter Platz gesucht. Gibt es keinen mehr,
  // wird der älteste bereits abgeschlossene Eintrag wiederverwendet. Dabei
  // geht der älteste Verlaufseintrag verloren, was aber niemanden stört,
  // weil das Fahrzeug längst weg ist.
  //
  // Einträge von Fahrzeugen, die noch im Parkhaus stehen, werden nie
  // überschrieben. Sonst würde jemand seinen Parkvorgang verlieren und
  // könnte nicht mehr ausfahren.
  ParkingRecord* findSlotForNewRecord() {
    for (uint8_t i = 0; i < MAX_PARKING_RECORDS; i++) {
      if (g_records[i].status == RecordStatus::UNUSED) {
        return &g_records[i];
      }
    }

    ParkingRecord* oldest = nullptr;

    for (uint8_t i = 0; i < MAX_PARKING_RECORDS; i++) {
      ParkingRecord& record = g_records[i];

      if (record.status != RecordStatus::FINISHED) {
        continue;   // aktive Fahrzeuge kommen nicht in Frage
      }
      if (oldest == nullptr || record.exitUptimeSecond < oldest->exitUptimeSecond) {
        oldest = &record;
      }
    }

    return oldest;   // kann nullptr sein, wenn wirklich alles belegt ist
  }


  // Wie viele Sekunden sind seit dieser Einfahrt vergangen.
  uint32_t secondsSinceEntry(int64_t entryUptimeSecond) {
    int64_t elapsed = (int64_t)uptimeInSeconds() - entryUptimeSecond;

    // Ein negativer Wert kann eigentlich nicht vorkommen. Falls doch, etwa
    // weil ein Debug-Aufruf die Zeit verstellt hat, liefern wir null statt
    // einer unsinnigen Riesenzahl. Beim Umwandeln nach uint32_t würde aus
    // einer negativen Zahl nämlich eine sehr grosse positive.
    if (elapsed < 0) {
      elapsed = 0;
    }

    return (uint32_t)elapsed;
  }


  // Kopiert ein Kennzeichen in den festen Speicher eines Eintrags.
  void storePlate(ParkingRecord& record, const String& plate) {
    // strncpy kopiert höchstens so viele Zeichen, wie Platz vorhanden ist.
    // Wir lassen dabei bewusst ein Zeichen frei.
    strncpy(record.plate, plate.c_str(), sizeof(record.plate) - 1);

    // Dieses freie Zeichen bekommt jetzt die Null. Sie markiert das Ende des
    // Textes. Ohne sie würde jede Ausgabe einfach weiterlesen, bis sie
    // irgendwo im Speicher zufällig auf eine Null trifft.
    // strncpy setzt diese Null nicht immer selbst, deshalb machen wir es hier.
    record.plate[sizeof(record.plate) - 1] = '\0';
  }


  // Schreibt eine Zeile in den seriellen Monitor, zum Beispiel
  // "[KASSE] Einfahrt: LU 12345  Belegung: 1/3".
  void logWithOccupancy(const __FlashStringHelper* action, const ParkingRecord& record) {
    Serial.print(F("[KASSE] "));
    Serial.print(action);
    Serial.print(record.plate);
    Serial.print(F("  Belegung: "));
    Serial.print(countOccupiedSpots());
    Serial.print('/');
    Serial.println(MAX_PARKING_SPOTS);
  }

}   // Ende des anonymen Namensraums


void resetAllParkingRecords() {
  for (uint8_t i = 0; i < MAX_PARKING_RECORDS; i++) {
    g_records[i].plate[0]          = '\0';
    g_records[i].entryUptimeSecond = 0;
    g_records[i].exitUptimeSecond  = 0;
    g_records[i].paidCents         = 0;
    g_records[i].status            = RecordStatus::UNUSED;
  }
}


uint32_t calculateFeeInCents(uint32_t parkedSeconds) {
  // Eine Stunde hat 3600 Sekunden. Die Division liefert die vollen Stunden.
  uint32_t hours = parkedSeconds / 3600;

  // Der Modulo-Operator % liefert den Rest der Division. Ist er nicht null,
  // dann läuft gerade eine weitere Stunde und die wird vollständig
  // berechnet. 60 Minuten kosten also eine Stunde, 61 Minuten kosten zwei.
  if (parkedSeconds % 3600 != 0) {
    hours++;
  }

  // Wer nur eine Minute steht, hätte nach der Rechnung oben schon eine
  // Stunde. Steht jemand exakt null Sekunden, käme null heraus. Da es keine
  // Gratiszeit gibt, wird daraus trotzdem eine Stunde.
  if (hours == 0) {
    hours = 1;
  }

  return hours * CENTS_PER_HOUR;
}


uint8_t countOccupiedSpots() {
  uint8_t count = 0;

  for (uint8_t i = 0; i < MAX_PARKING_RECORDS; i++) {
    if (isCurrentlyParked(g_records[i])) {
      count++;
    }
  }

  return count;
}


uint8_t countFreeSpots() {
  uint8_t occupied = countOccupiedSpots();

  // Die Prüfung auf grösser oder gleich ist wichtig. Würden wir einfach
  // subtrahieren und wäre occupied grösser als MAX_PARKING_SPOTS, käme bei
  // einer vorzeichenlosen Zahl eine riesige Zahl heraus statt einer
  // negativen.
  if (occupied >= MAX_PARKING_SPOTS) {
    return 0;
  }

  return (uint8_t)(MAX_PARKING_SPOTS - occupied);
}


EntryResult registerEntry(const String& userInput,
                          String& plateOut,
                          String& errorMessageOut) {
  errorMessageOut = "";

  // Schritt 1: Ist das Kennzeichen überhaupt gültig.
  PlateCheckResult checkResult = checkPlate(userInput, plateOut);
  if (checkResult != PlateCheckResult::VALID) {
    errorMessageOut = plateErrorMessage(checkResult);
    return EntryResult::INVALID_PLATE;
  }

  // Schritt 2: Steht dieses Fahrzeug vielleicht schon drin.
  if (findActiveRecord(plateOut) != nullptr) {
    errorMessageOut = F("Dieses Fahrzeug steht bereits im Parkhaus.");
    return EntryResult::ALREADY_PARKED;
  }

  // Schritt 3: Ist überhaupt noch ein Platz frei.
  if (countOccupiedSpots() >= MAX_PARKING_SPOTS) {
    errorMessageOut = F("Das Parkhaus ist voll.");
    return EntryResult::GARAGE_FULL;
  }

  // Schritt 4: Platz in der Liste besorgen.
  ParkingRecord* record = findSlotForNewRecord();
  if (record == nullptr) {
    errorMessageOut = F("Der Speicher der Kasse ist voll.");
    return EntryResult::MEMORY_FULL;
  }

  // Schritt 5: Erst jetzt, wenn alles geprüft ist, wird gespeichert.
  storePlate(*record, plateOut);
  record->entryUptimeSecond = (int64_t)uptimeInSeconds();
  record->exitUptimeSecond  = 0;
  record->paidCents         = 0;
  record->status            = RecordStatus::PARKED_UNPAID;

  logWithOccupancy(F("Einfahrt: "), *record);
  return EntryResult::ACCEPTED;
}


ParkingInfo lookupParkingInfo(const String& userInput) {
  ParkingInfo info;

  // Zuerst wird die Antwort vollständig mit dem Fall "unbekannt" gefüllt.
  // Wer die Funktion später erweitert, kann dann kein Feld mehr vergessen.
  info.state             = PaymentState::PLATE_UNKNOWN;
  info.plate             = normalizePlate(userInput);
  info.entryUptimeSecond = 0;
  info.parkedSeconds     = 0;
  info.feeInCents        = 0;

  ParkingRecord* record = findActiveRecord(info.plate);
  if (record == nullptr) {
    return info;
  }

  info.state = (record->status == RecordStatus::PARKED_PAID)
             ? PaymentState::PAID
             : PaymentState::NOT_PAID_YET;

  info.entryUptimeSecond = record->entryUptimeSecond;
  info.parkedSeconds     = secondsSinceEntry(record->entryUptimeSecond);

  // Hier steckt eine wichtige Regel drin.
  //
  // Bei einem bezahlten Vorgang wird der gespeicherte Betrag angezeigt und
  // nicht neu gerechnet. Sonst würde die Kasse einem Fahrzeug, das bezahlt
  // hat und noch kurz stehen bleibt, beim Überschreiten der nächsten vollen
  // Stunde plötzlich wieder Geld abverlangen. Der bezahlte Betrag ist
  // eingefroren.
  if (record->status == RecordStatus::PARKED_PAID) {
    info.feeInCents = record->paidCents;
  } else {
    info.feeInCents = calculateFeeInCents(info.parkedSeconds);
  }

  return info;
}


PaymentState payParkingFee(const String& userInput,
                           uint32_t& paidCentsOut,
                           bool& wasAlreadyPaidOut) {
  paidCentsOut      = 0;
  wasAlreadyPaidOut = false;

  String plate = normalizePlate(userInput);

  ParkingRecord* record = findActiveRecord(plate);
  if (record == nullptr) {
    return PaymentState::PLATE_UNKNOWN;
  }

  if (record->status == RecordStatus::PARKED_PAID) {
    // Schon bezahlt. Wir buchen nichts noch einmal, sondern melden nur
    // zurück, was damals bezahlt wurde.
    wasAlreadyPaidOut = true;
    paidCentsOut      = record->paidCents;
    return PaymentState::PAID;
  }

  paidCentsOut       = calculateFeeInCents(secondsSinceEntry(record->entryUptimeSecond));
  record->paidCents  = paidCentsOut;
  record->status     = RecordStatus::PARKED_PAID;

  Serial.print(F("[KASSE] Bezahlt: "));
  Serial.print(record->plate);
  Serial.print(' ');
  Serial.println(formatMoney(paidCentsOut));

  return PaymentState::PAID;
}


bool finishParking(const String& userInput) {
  String plate = normalizePlate(userInput);

  ParkingRecord* record = findActiveRecord(plate);

  // Abschliessen geht nur bei einem Fahrzeug, das da ist und bezahlt hat.
  // Diese Prüfung ist die letzte Sicherung, bevor ein Platz frei wird.
  if (record == nullptr || record->status != RecordStatus::PARKED_PAID) {
    return false;
  }

  record->exitUptimeSecond = (int64_t)uptimeInSeconds();
  record->status           = RecordStatus::FINISHED;

  logWithOccupancy(F("Ausfahrt: "), *record);
  return true;
}


const ParkingRecord* getParkingRecord(uint8_t index) {
  if (index >= MAX_PARKING_RECORDS) {
    return nullptr;
  }
  return &g_records[index];
}


#if TEST_BUILD

bool debugMoveEntryTimeBack(const String& userInput, uint32_t minutes) {
  String plate = normalizePlate(userInput);

  ParkingRecord* record = findActiveRecord(plate);
  if (record == nullptr) {
    return false;
  }

  record->entryUptimeSecond = (int64_t)uptimeInSeconds() - (int64_t)minutes * 60;

  // Ein bereits bezahlter Vorgang wird wieder auf offen gesetzt.
  //
  // Sonst würde der eingefrorene alte Betrag stehen bleiben und der Test
  // könnte gar nicht prüfen, ob die neue, längere Parkdauer richtig
  // berechnet wird.
  if (record->status == RecordStatus::PARKED_PAID) {
    record->status    = RecordStatus::PARKED_UNPAID;
    record->paidCents = 0;
  }

  Serial.print(F("[DEBUG] Einfahrtszeit verschoben: "));
  Serial.print(record->plate);
  Serial.print(F(" -"));
  Serial.print(minutes);
  Serial.println(F(" Minuten"));

  return true;
}


void debugDeleteAllRecords() {
  resetAllParkingRecords();
  Serial.println(F("[DEBUG] Alle Parkvorgaenge geloescht."));
}

#endif
