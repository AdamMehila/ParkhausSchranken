#pragma once

// ===========================================================================
// Firmware-Version des Parksystems.
//
// Beide Picos geben diese Nummer beim Start im seriellen Monitor aus. So ist
// im Testprotokoll eindeutig, welcher Stand auf den Picos läuft.
//
// Die Nummer steht absichtlich in einer eigenen Datei und nicht in config.h.
// In config.h stehen lokal die echten WLAN-Zugangsdaten. Eine neue Version
// soll config.h deshalb nie berühren.
//
// Jede Fehlerbehebung erhöht die Nummer im selben Commit:
//   1.0  Teststand mit den Testhilfen H-01 bis H-05
//   1.1  und höher: je eine Fehlerbehebung aus der Fehlertabelle
//
// #define statt constexpr, damit die Nummer direkt in F("...")-Texte
// eingesetzt werden kann.
// ===========================================================================

#define FIRMWARE_VERSION "1.4"
