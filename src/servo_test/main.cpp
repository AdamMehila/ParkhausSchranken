// ===========================================================================
// Servo-Test: ein eigenes kleines Programm, nur um die Hardware zu prüfen.
//
// Es macht nichts anderes, als den Schrankenarm jede Sekunde zwischen zwei
// Stellungen hin und her zu fahren. Kein WLAN, keine Kasse, keine Webseite.
// Die kleine LED auf dem Pico blinkt im selben Takt, daran sieht man, dass
// das Programm läuft.
//
// Es gibt zwei Varianten. Welche gebaut wird, steht in platformio.ini:
//
//   servo_test            Das Signal geht nur auf den Pin aus config.h
//                         (SERVO_PIN_ENTRY, zurzeit GP8 an Pin 11). Bewegt
//                         sich der Arm hier, ist die Verkabelung richtig.
//
//   servo_test_all_pins   Das Signal geht gleichzeitig auf ALLE Pins.
//                         Bewegt sich der Arm nur bei dieser Variante,
//                         steckt das orange Kabel am falschen Pin.
//
// Nach dem Test muss die richtige Firmware wieder aufgespielt werden, sonst
// läuft auf dem Pico weiterhin nur dieser Test.
// ===========================================================================

#include <Arduino.h>
#include "hardware/gpio.h"
#include "../common/config.h"

// Wird von platformio.ini bei der Variante servo_test_all_pins auf 1 gesetzt.
#ifndef SERVO_TEST_ALL_PINS
  #define SERVO_TEST_ALL_PINS 0
#endif

namespace {

  // Alle wie viele Millisekunden der Arm die Stellung wechselt.
  constexpr uint32_t SWITCH_INTERVAL_MS = 1000;

  // Ein Servo erwartet 50 Mal pro Sekunde einen kurzen Stromstoss. Wie lange
  // der Stoss dauert, bestimmt den Winkel: etwa 1 Millisekunde ist ganz auf der
  // einen Seite, etwa 2 Millisekunden weit auf der anderen.
  constexpr uint32_t PULSE_POSITION_A_US = 1000;   // 1000 Mikrosekunden = 1 ms
  constexpr uint32_t PULSE_POSITION_B_US = 2000;   // 2000 Mikrosekunden = 2 ms

  // Nach jedem Stoss eine Pause, damit zusammen etwa 20 ms pro Durchlauf
  // herauskommen. 20 ms heisst 50 Stösse pro Sekunde.
  constexpr uint32_t PAUSE_BETWEEN_PULSES_MS = 18;

  bool     g_isPositionB    = false;
  uint32_t g_lastSwitchMs   = 0;


  // Liefert die Pins, auf die das Signal gehen soll, als Bitmaske.
  //
  // Eine Bitmaske ist eine Zahl, bei der jedes Bit für einen Pin steht. Ist
  // Bit 8 gesetzt, ist GP8 gemeint. Damit kann man viele Pins mit einem
  // einzigen Befehl gleichzeitig ein- und ausschalten.
  uint32_t buildPinMask() {
#if SERVO_TEST_ALL_PINS
    // Alle Pins, die am Rand des Pico herausgeführt sind: GPIO 0 bis 22 und
    // 26 bis 28. GPIO 23, 24, 25 und 29 fehlen absichtlich, die gehören zum
    // WLAN-Chip.
    return 0x007FFFFFu | (1u << 26) | (1u << 27) | (1u << 28);
#else
    // Nur der Pin aus config.h. Die Schreibweise 1u << 8 heisst: eine 1, um
    // 8 Stellen nach links geschoben, also nur Bit 8 gesetzt, und damit GP8.
    return 1u << SERVO_PIN_ENTRY;
#endif
  }


  // Schickt einen einzelnen Stoss der angegebenen Länge auf alle Pins der Maske.
  void sendOnePulse(uint32_t pinMask, uint32_t pulseLengthUs) {
    gpio_set_mask(pinMask);              // alle gewählten Pins auf "Strom an"
    delayMicroseconds(pulseLengthUs);    // so lange halten
    gpio_clr_mask(pinMask);              // alle wieder auf "Strom aus"
  }


  // Wechselt jede Sekunde die Stellung und lässt die LED mitblinken.
  void switchPositionEverySecond() {
    if (millis() - g_lastSwitchMs < SWITCH_INTERVAL_MS) {
      return;
    }
    g_lastSwitchMs = millis();
    g_isPositionB  = !g_isPositionB;

    digitalWrite(LED_BUILTIN, g_isPositionB ? HIGH : LOW);
    Serial.println(g_isPositionB ? F("[SERVOTEST] Stellung B") : F("[SERVOTEST] Stellung A"));
  }

}


void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  // Die gewählten Pins als Ausgänge einrichten und zuerst ausschalten.
  uint32_t pinMask = buildPinMask();
  gpio_init_mask(pinMask);
  gpio_set_dir_out_masked(pinMask);
  gpio_clr_mask(pinMask);
}


void loop() {
  switchPositionEverySecond();

  uint32_t pulseLengthUs = g_isPositionB ? PULSE_POSITION_B_US : PULSE_POSITION_A_US;
  sendOnePulse(buildPinMask(), pulseLengthUs);

  // Hier ist delay() in Ordnung: Dieses Programm hat ausser dem Servo
  // nichts anderes zu tun, es muss keine Webseite bedienen.
  delay(PAUSE_BETWEEN_PULSES_MS);
}
