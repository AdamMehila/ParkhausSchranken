#pragma once
#include <Arduino.h>

// ===========================================================================
// Der Draht von Pico 2 zur Kasse auf Pico 1.
//
// Pico 2 speichert selbst keine Parkvorgänge. Er fragt für jede Entscheidung
// bei der Kasse nach. Diese Datei enthält alles, was für diese Anfragen
// nötig ist.
//
// Jede Antwort enthält als erstes Feld cashDeskReachable. Damit lassen sich
// zwei grundverschiedene Fälle sauber unterscheiden:
//
//   cashDeskReachable == false
//       Die Kasse hat gar nicht geantwortet. Vielleicht ist sie aus,
//       vielleicht ist das WLAN weg. Wir wissen überhaupt nichts.
//
//   cashDeskReachable == true, accepted == false
//       Die Kasse hat geantwortet und Nein gesagt. Zum Beispiel weil das
//       Parkhaus voll ist. Wir wissen also genau Bescheid.
//
// Im ersten Fall muss die Schranke zubleiben, weil wir nichts wissen. Im
// zweiten Fall bleibt sie zu, weil wir etwas wissen. Für den Benutzer sind
// das zwei völlig verschiedene Meldungen, deshalb werden sie getrennt.
// ===========================================================================


// Antwort auf eine Einfahrtsanfrage.
struct EntryReply {
  bool   cashDeskReachable;   // hat die Kasse überhaupt geantwortet
  bool   accepted;            // hat sie die Einfahrt erlaubt
  String code;                // ok, full, already, invalid oder memory
  String message;             // fertiger Text für die Weboberfläche
  int    occupied;            // belegte Plätze laut Kasse
  int    totalSpots;          // Plätze insgesamt laut Kasse
};


// Antwort auf eine Statusanfrage zu einem Kennzeichen.
struct StatusReply {
  bool   cashDeskReachable;
  String paymentState;   // unknown, unpaid oder paid
  String plate;
  String fee;            // Gebühr als fertiger Text, zum Beispiel "CHF 4.00"
};


// Antwort auf die Frage, wie voll das Parkhaus gerade ist.
struct OccupancyReply {
  bool cashDeskReachable;
  int  occupied;
  int  totalSpots;
};


// Liest einen einzelnen Wert aus einer JSON-Antwort der Kasse.
//
// Das ist ein sehr einfacher Leser und kein vollwertiger JSON-Parser. Er
// funktioniert nur deshalb zuverlässig, weil wir beide Seiten selbst
// geschrieben haben und genau wissen, wie die Antworten aussehen. Für
// fremde JSON-Daten wäre er nicht geeignet.
String readJsonField(const String& json, const String& fieldName);


// Meldet der Kasse eine Einfahrt an.
EntryReply askCashDeskToRegisterEntry(const String& plate);

// Fragt die Kasse, ob für dieses Kennzeichen bezahlt wurde.
StatusReply askCashDeskForStatus(const String& plate);

// Sagt der Kasse, dass das Fahrzeug ausfährt und der Platz frei wird.
// Gibt true zurück, wenn die Kasse den Vorgang abgeschlossen hat.
bool askCashDeskToFinishParking(const String& plate);

// Fragt die aktuelle Belegung ab. Wird im Hintergrund regelmässig aufgerufen,
// damit die Weboberfläche von Pico 2 die Belegung anzeigen kann.
OccupancyReply askCashDeskForOccupancy();
