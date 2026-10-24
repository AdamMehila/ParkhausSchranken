#pragma once
#include <Arduino.h>
#include <Servo.h>

// ===========================================================================
// Eine Schranke, angetrieben von einem SG90-Servo.
//
// Die Winkel für "zu" und "offen" stehen in config.h (SERVO_ANGLE_CLOSED und
// SERVO_ANGLE_OPEN). Nach fünf Sekunden fährt der Arm von selbst wieder zu.
//
// Das Wichtigste an dieser Klasse ist, wie sie NICHT geschrieben ist.
// Naheliegend wäre nämlich:
//
//     servo.write(90);
//     delay(5000);        // so bitte nicht
//     servo.write(0);
//
// Das sähe kürzer aus, hätte aber einen schweren Fehler. delay() hält das
// ganze Programm an. Fünf Sekunden lang würde der Pico keine Webanfrage
// beantworten, das WLAN nicht prüfen und nichts anderes tun. Für den
// Benutzer sähe das aus, als wäre das Gerät abgestürzt.
//
// Stattdessen merkt sich die Klasse zwei Dinge: ob sie offen ist und seit
// wann. Die Methode update() schaut bei jedem Programmdurchlauf nur kurz
// nach, ob die Zeit um ist, und kehrt sofort zurück. Dieses Muster heisst
// Zustandsmaschine und zieht sich durch das ganze Projekt.
// ===========================================================================

class Barrier {
public:
  // Legt eine Schranke an.
  //
  // servoPin  ist die GPIO-Nummer, an der das Signalkabel hängt
  // name      erscheint in den Meldungen im seriellen Monitor
  //
  // Wichtig: Der Servo wird hier noch nicht angesprochen. Das passiert erst
  // in begin(). Der Grund ist, dass dieses Objekt schon vor dem Start des
  // Programms angelegt wird, zu einem Zeitpunkt, an dem die Hardware noch
  // gar nicht bereit ist.
  Barrier(uint8_t servoPin, const char* name);

  // Meldet den Servo an und fährt die Schranke in die geschlossene Stellung.
  // Muss einmal in setup() aufgerufen werden.
  void begin();

  // Öffnet die Schranke und startet die Fünf-Sekunden-Uhr.
  // Die Methode kehrt sofort zurück, sie wartet nicht.
  void open();

  // Muss in jedem Durchlauf von loop() aufgerufen werden.
  // Schliesst die Schranke, sobald die Offenhaltezeit vorbei ist.
  void update();

  // Sagt, ob die Schranke gerade offen ist.
  //
  // Das const hinter der Klammer verspricht, dass diese Methode am Objekt
  // nichts verändert. Der Compiler prüft dieses Versprechen. Wer hier aus
  // Versehen etwas zuweist, bekommt eine Fehlermeldung statt eines Fehlers,
  // den niemand findet.
  bool isOpen() const;

  // Wie viele Millisekunden die Schranke noch offen bleibt.
  // Bei geschlossener Schranke kommt 0 zurück.
  // Die Weboberfläche zeichnet daraus den ablaufenden Balken.
  uint32_t millisUntilClose() const;

private:
  // Fährt den Arm sofort zu und schreibt eine Meldung.
  void closeNow();

  // Prüft, ob die Offenhaltezeit abgelaufen ist.
  bool openTimeIsOver() const;

  Servo       _servo;        // das Objekt der Servo-Bibliothek
  uint8_t     _servoPin;     // GPIO-Nummer des Signalkabels
  const char* _name;         // Name für die Ausgabe im Monitor
  bool        _isOpen;       // steht der Arm gerade oben
  uint32_t    _openedAtMs;   // Zeitpunkt des Öffnens, von millis()
};

// Zur Schreibweise mit dem Unterstrich am Anfang: Alle Variablen, die zu
// einem Objekt gehören, bekommen in diesem Projekt einen Unterstrich
// vorangestellt. So sieht man beim Lesen einer Methode sofort, ob ein Wert
// zum Objekt gehört oder nur lokal in dieser Methode existiert.
