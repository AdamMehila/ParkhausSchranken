#include "clock_time.h"
#include <time.h>

// Diese Variablen gehören nur dieser Datei. Der anonyme Namensraum sorgt
// dafür, dass niemand von aussen daran herumschrauben kann.
namespace {

  // Aufsummierte Millisekunden seit dem Start.
  //
  // Der Typ ist uint64_t, also eine sehr grosse ganze Zahl. Damit läuft der
  // Zähler erst nach rund 584 Millionen Jahren über. Das genügt.
  uint64_t g_totalMilliseconds = 0;

  // Der Wert, den millis() beim letzten Aufruf geliefert hat.
  uint32_t g_lastMillisReading = 0;

  bool    g_wallClockIsSet   = false;
  int64_t g_unixTimeAtSync   = 0;   // echte Zeit im Moment des Stellens
  int64_t g_uptimeAtSync     = 0;   // Laufzeitsekunde im Moment des Stellens

}


uint32_t uptimeInSeconds() {
  uint32_t now = millis();

  // Diese eine Zeile ist der wichtigste Trick in dieser Datei.
  //
  // millis() zählt in einer 32-Bit-Zahl und springt nach etwa 49 Tagen
  // zurück auf 0. Eine normale Subtraktion würde dann eine riesige falsche
  // Zahl ergeben. Hier passiert das aber nicht, weil bei vorzeichenlosen
  // Zahlen auch das Rechenergebnis wieder umläuft.
  //
  // Ein kleines Beispiel mit vier statt 32 Stellen, also mit Werten von
  // 0 bis 15. Die Uhr steht bei 14, zwei Schritte später bei 1 (sie ist
  // über die 15 gelaufen). Rechnung: 1 minus 14 ergibt in dieser Arithmetik
  // genau 2. Also das richtige Ergebnis, obwohl die Zahl zwischendurch
  // zurückgesprungen ist.
  //
  // Damit das funktioniert, darf zwischen zwei Aufrufen nicht mehr als eine
  // volle Umlaufzeit vergehen. Deshalb wird die Funktion in loop() bei jedem
  // Durchlauf aufgerufen.
  g_totalMilliseconds += (uint32_t)(now - g_lastMillisReading);
  g_lastMillisReading = now;

  return (uint32_t)(g_totalMilliseconds / 1000ULL);
}


void setWallClock(uint32_t localUnixTime) {
  // Wir merken uns zwei Dinge gleichzeitig: welche echte Zeit jetzt ist und
  // bei welcher Laufzeitsekunde wir gerade stehen. Aus diesem Paar lässt
  // sich später jede andere Laufzeitsekunde umrechnen.
  g_unixTimeAtSync = (int64_t)localUnixTime;
  g_uptimeAtSync   = (int64_t)uptimeInSeconds();
  g_wallClockIsSet = true;
}


bool isWallClockSet() {
  return g_wallClockIsSet;
}


int64_t uptimeSecondToUnixTime(int64_t uptimeSecond) {
  // Beispiel: Die Uhr wurde bei Laufzeitsekunde 100 gestellt und es war
  // 14:00 Uhr. Gefragt ist Laufzeitsekunde 40, also 60 Sekunden früher.
  // Die Rechnung ergibt dann 13:59 Uhr. Auch Werte aus der Zeit vor dem
  // Stellen der Uhr stimmen also.
  return g_unixTimeAtSync + (uptimeSecond - g_uptimeAtSync);
}


String formatDateAndTime(int64_t uptimeSecond) {
  if (!g_wallClockIsSet) {
    return F("--");
  }

  time_t unixTime = (time_t)uptimeSecondToUnixTime(uptimeSecond);

  // gmtime_r zerlegt eine Unix-Zeit in Jahr, Monat, Tag, Stunde und so
  // weiter. Wir nehmen bewusst gmtime und nicht localtime, weil der Browser
  // uns bereits die lokale Zeit geschickt hat. Würden wir hier nochmals eine
  // Zeitzone dazurechnen, wäre die Anzeige um ein oder zwei Stunden falsch.
  struct tm parts;
  gmtime_r(&unixTime, &parts);

  // snprintf schreibt formatierten Text in einen festen Puffer. Der zweite
  // Parameter sagt, wie gross der Puffer ist. Dadurch kann nie über das Ende
  // hinaus geschrieben werden, auch wenn der Text länger würde als gedacht.
  char buffer[40];
  snprintf(buffer, sizeof(buffer), "%02d.%02d.%04d %02d:%02d:%02d",
           parts.tm_mday,
           parts.tm_mon + 1,       // tm_mon zählt ab 0, Januar ist also 0
           parts.tm_year + 1900,   // tm_year zählt ab dem Jahr 1900
           parts.tm_hour,
           parts.tm_min,
           parts.tm_sec);

  return String(buffer);
}


String formatDuration(uint32_t seconds) {
  uint32_t hours = seconds / 3600;            // ganze Stunden
  uint32_t minutes = (seconds % 3600) / 60;   // Rest in ganzen Minuten

  char buffer[24];
  // %02lu bedeutet: mindestens zwei Stellen, vorne mit einer Null auffüllen.
  // Aus 5 Minuten wird damit "05 min" und nicht "5 min".
  snprintf(buffer, sizeof(buffer), "%lu h %02lu min",
           (unsigned long)hours,
           (unsigned long)minutes);

  return String(buffer);
}


String formatMoney(uint32_t cents) {
  char buffer[20];
  // Die Division liefert die ganzen Franken, der Rest die Rappen.
  // 450 Rappen ergeben also 4 Franken und 50 Rappen, angezeigt als CHF 4.50.
  snprintf(buffer, sizeof(buffer), "CHF %lu.%02lu",
           (unsigned long)(cents / 100),
           (unsigned long)(cents % 100));

  return String(buffer);
}
