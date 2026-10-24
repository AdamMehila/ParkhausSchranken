#include "barrier.h"
#include "../common/config.h"


// Der Konstruktor. Diese Funktion läuft, wenn ein Barrier-Objekt entsteht.
//
// Die Schreibweise mit dem Doppelpunkt heisst Initialisiererliste. Die Werte
// dahinter werden den Variablen direkt beim Anlegen mitgegeben, noch bevor
// der Rumpf in den geschweiften Klammern läuft.
//
// Das ist nicht nur Geschmackssache. Würde man die Zuweisungen in den Rumpf
// schreiben, würde jede Variable zuerst mit einem zufälligen Wert entstehen
// und erst danach ihren richtigen bekommen. Bei grösseren Typen kostet das
// Zeit, und bei Referenzen wäre es gar nicht möglich.
Barrier::Barrier(uint8_t servoPin, const char* name)
  : _servoPin(servoPin),
    _name(name),
    _isOpen(false),
    _openedAtMs(0) {
  // Hier steht mit Absicht nichts. Alles ist schon oben erledigt.
}


void Barrier::begin() {
  // attach verbindet das Servo-Objekt mit einem GPIO-Pin. Ab jetzt schickt
  // der Pico auf diesem Pin alle 20 Millisekunden einen Steuerimpuls.
  _servo.attach(_servoPin);

  // Der Arm fährt in die geschlossene Stellung. Beim Einschalten weiss der
  // Servo nämlich nicht, wo er steht. Ohne diese Zeile bliebe er einfach
  // dort stehen, wo ihn jemand zuletzt von Hand hingedreht hat.
  _servo.write(SERVO_ANGLE_CLOSED);
  _isOpen = false;

  Serial.print(F("[SCHRANKE] "));
  Serial.print(_name);
  Serial.print(F(" bereit an GPIO "));
  Serial.println(_servoPin);
}


void Barrier::open() {
  _servo.write(SERVO_ANGLE_OPEN);

  // Der Zeitpunkt wird gemerkt, nicht gewartet. Das ist der ganze Trick.
  _openedAtMs = millis();
  _isOpen     = true;

  Serial.print(F("[SCHRANKE] "));
  Serial.print(_name);
  Serial.print(F(" oeffnet ("));
  Serial.print(SERVO_ANGLE_OPEN);
  Serial.println(F(" Grad)."));
}


void Barrier::update() {
  // Ist die Schranke zu, gibt es nichts zu tun. Dieser frühe Ausstieg
  // spart die Einrückung für den Rest der Methode.
  if (!_isOpen) {
    return;
  }

  if (openTimeIsOver()) {
    closeNow();
  }
}


bool Barrier::isOpen() const {
  return _isOpen;
}


uint32_t Barrier::millisUntilClose() const {
  if (!_isOpen) {
    return 0;
  }

  uint32_t elapsed = millis() - _openedAtMs;

  // Diese Prüfung ist nötig, weil beide Werte vorzeichenlos sind. Wäre
  // elapsed schon grösser als die Offenhaltezeit, würde die Subtraktion
  // keine negative Zahl ergeben, sondern eine riesige positive. Die
  // Weboberfläche würde dann einen Balken zeichnen, der ins Unendliche geht.
  if (elapsed >= BARRIER_OPEN_TIME_MS) {
    return 0;
  }

  return BARRIER_OPEN_TIME_MS - elapsed;
}


void Barrier::closeNow() {
  _servo.write(SERVO_ANGLE_CLOSED);
  _isOpen = false;

  Serial.print(F("[SCHRANKE] "));
  Serial.print(_name);
  Serial.print(F(" schliesst ("));
  Serial.print(SERVO_ANGLE_CLOSED);
  Serial.println(F(" Grad)."));
}


bool Barrier::openTimeIsOver() const {
  // Die Subtraktion ist auch dann richtig, wenn millis() zwischendurch von
  // seinem Höchstwert zurück auf null gesprungen ist. Ausführlich erklärt
  // ist das in common/clock_time.cpp bei uptimeInSeconds().
  return (millis() - _openedAtMs) >= BARRIER_OPEN_TIME_MS;
}
