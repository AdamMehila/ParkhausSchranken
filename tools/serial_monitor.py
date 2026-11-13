"""Serieller Monitor für die Testdurchführung, läuft ohne Terminal im Hintergrund.

Ersetzt `pio device monitor --filter time --filter log2file`, weil dieser ein
interaktives Terminal verlangt und sich nach einem Neustart des Picos nicht
selbst wieder verbindet. Das Format der Logzeilen ist dasselbe wie beim Filter
`time`: Uhrzeit des Laptops mit Millisekunden, dann " > " und die Zeile.

Zusätzlich vermerkt das Skript jede Verbindung und jede Trennung mit einer
Zeile, die mit "---" beginnt. Eine Trennung mit anschliessendem Banner zeigt
einen Neustart des Picos an.

Aufruf (Python aus PlatformIO, dort ist pyserial enthalten):
    %USERPROFILE%\\.platformio\\penv\\Scripts\\python.exe -I tools/serial_monitor.py COM6 docs/nachweise/Monitor_Pico1_Z1.log

Beenden mit Strg+C oder durch Beenden des Prozesses.
"""
import datetime
import sys
import time

import serial


def now() -> str:
    return datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]


def main() -> None:
    port, log_path = sys.argv[1], sys.argv[2]
    with open(log_path, "a", encoding="utf-8", newline="\n") as log:

        def note(text: str) -> None:
            log.write(f"{now()} --- {text}\n")
            log.flush()

        note(f"Monitor gestartet fuer {port}, 115200 Baud")
        waiting_since_note = False
        while True:
            try:
                ser = serial.Serial(port, 115200, timeout=0.05)
                ser.dtr = True
            except serial.SerialException:
                if not waiting_since_note:
                    note(f"{port} nicht verfuegbar, warte auf den Port")
                    waiting_since_note = True
                time.sleep(0.05)
                continue

            waiting_since_note = False
            note(f"verbunden mit {port}")
            line = b""
            line_start = ""
            try:
                while True:
                    chunk = ser.read(256)
                    for byte in chunk:
                        if not line:
                            line_start = now()   # Zeitpunkt des ersten Zeichens der Zeile
                        if byte == 0x0A:
                            text = line.decode("utf-8", "replace").rstrip("\r")
                            log.write(f"{line_start} > {text}\n")
                            log.flush()
                            line = b""
                        else:
                            line += bytes([byte])
            except (serial.SerialException, OSError) as exc:
                if line:
                    log.write(f"{line_start} > {line.decode('utf-8', 'replace')}\n")
                note(f"Verbindung getrennt ({type(exc).__name__})")
                try:
                    ser.close()
                except Exception:   # noqa: BLE001  Port ist schon weg
                    pass
                time.sleep(0.05)


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        pass
