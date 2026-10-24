#pragma once
#include <Arduino.h>

// ===========================================================================
// Die Weboberfläche der Schranke.
//
// Die beiden Eingabefelder ersetzen die Kamera mit automatischer
// Kennzeichenerkennung, die es in diesem Projekt nicht gibt. Fachlich macht
// das keinen Unterschied: Ob das Kennzeichen von einer Kamera kommt oder von
// Hand eingetippt wird, ist für alles Weitere egal.
//
// Zur Schreibweise R"HTML( ... )HTML" steht eine ausführliche Erklärung in
// cashdesk/web_page_cashdesk.h.
// ===========================================================================

static const char BARRIERS_WEB_PAGE[] PROGMEM = R"HTML(<!doctype html>
<html lang="de">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Parkhaus Schranken</title>
<style>
  :root { --border:#d8dee7; --blue:#1d4ed8; --green:#15803d; --red:#b91c1c; --gray:#5b6472; }
  * { box-sizing:border-box; }
  body { margin:0; padding:24px 16px 48px; background:#f3f5f9; color:#111827;
         font-family:system-ui,-apple-system,"Segoe UI",Roboto,sans-serif; }
  .wrapper { max-width:900px; margin:0 auto; }
  h1 { font-size:24px; margin:0 0 4px; }
  .subtitle { color:var(--gray); margin:0 0 20px; font-size:14px; }
  .statusbar { display:flex; gap:12px; flex-wrap:wrap; align-items:center; margin-bottom:18px; }
  .badge { background:#fff; border:1px solid var(--border); border-radius:999px;
           padding:7px 14px; font-size:14px; }
  .badge b { font-variant-numeric:tabular-nums; }
  .dot { display:inline-block; width:9px; height:9px; border-radius:50%;
         background:#9aa3b2; margin-right:7px; vertical-align:middle; }
  .dot.online  { background:var(--green); }
  .dot.offline { background:var(--red); }
  .pair { display:grid; grid-template-columns:1fr 1fr; gap:18px; }
  @media (max-width:720px){ .pair { grid-template-columns:1fr; } }
  .card { background:#fff; border:1px solid var(--border); border-radius:10px; padding:18px; }
  .card h2 { font-size:16px; margin:0 0 4px; }
  .card p.hint { color:var(--gray); font-size:13px; margin:0 0 14px; }
  input { font-size:16px; padding:10px 11px; border:1px solid var(--border);
          border-radius:7px; width:100%; text-transform:uppercase; }
  button { font-size:15px; padding:10px 16px; border:0; border-radius:7px;
           background:var(--blue); color:#fff; cursor:pointer; margin-top:10px; width:100%; }
  button.exit { background:#0f766e; }
  button.secondary { background:#4b5563; width:auto; margin-right:8px; }
  button:disabled { background:#9aa3b2; cursor:not-allowed; }
  .message { margin-top:14px; padding:11px 13px; border-radius:7px; font-size:15px; display:none; }
  .message.ok    { display:block; background:#eafaf0; color:var(--green); border:1px solid #b7e6c9; }
  .message.warn  { display:block; background:#fdf1e7; color:#b45309; border:1px solid #f3d3ae; }
  .message.error { display:block; background:#fdeeee; color:var(--red); border:1px solid #f4c2c2; }
  .barrierBox { margin-top:16px; padding:12px; border:1px solid var(--border);
                border-radius:8px; background:#f8fafc; font-size:14px; }
  .bar { height:8px; border-radius:4px; background:#e2e8f0; margin-top:9px; overflow:hidden; }
  .bar div { height:100%; background:var(--green); width:0%; transition:width .4s linear; }
  .state { font-weight:600; }
  .state.open   { color:var(--green); }
  .state.closed { color:var(--gray); }
  .debug { margin-top:18px; border-style:dashed; }
  .debug h2 { color:var(--gray); }
</style>
</head>
<body>
<div class="wrapper">
  <h1>Parkhaus &ndash; Schranken</h1>
  <p class="subtitle">Pico 2 &middot; Einfahrt und Ausfahrt. Das Eintippen des Kennzeichens ersetzt die Kamera.</p>

  <div class="statusbar">
    <div class="badge">Belegung <b id="occupancy">&ndash;</b></div>
    <div class="badge"><span class="dot" id="cashDeskDot"></span><span id="cashDeskText">Kasse wird gepr&uuml;ft</span></div>
  </div>

  <div class="pair">
    <div class="card">
      <h2>Einfahrt</h2>
      <p class="hint">Kennzeichen erfassen und Schranke &ouml;ffnen.</p>
      <input id="entryPlate" placeholder="z.B. LU 12345" autocomplete="off">
      <button onclick="requestEntry()" id="entryButton">Einfahrt best&auml;tigen</button>
      <div id="entryMessage" class="message"></div>
      <div class="barrierBox">
        <span id="entryLabel">Einfahrtsschranke</span>:
        <span class="state closed" id="entryState">geschlossen</span>
        <div class="bar"><div id="entryBar"></div></div>
      </div>
    </div>

    <div class="card">
      <h2>Ausfahrt</h2>
      <p class="hint">Die Schranke &ouml;ffnet nur, wenn die Kasse die Geb&uuml;hr als bezahlt meldet.</p>
      <input id="exitPlate" placeholder="z.B. LU 12345" autocomplete="off">
      <button class="exit" onclick="requestExit()" id="exitButton">Ausfahrt best&auml;tigen</button>
      <div id="exitMessage" class="message"></div>
      <div class="barrierBox">
        <span id="exitLabel">Ausfahrtsschranke</span>:
        <span class="state closed" id="exitState">geschlossen</span>
        <div class="bar"><div id="exitBar"></div></div>
      </div>
    </div>
  </div>

  <div class="card debug" id="debugCard" style="display:none">
    <h2>Smoke-Test der Servos (nur im Test-Build)</h2>
    <p class="hint">&Ouml;ffnet eine Schranke ohne Parkvorgang, f&uuml;r die Pr&uuml;fung der Eingangskriterien.</p>
    <button class="secondary" onclick="testServo('entry')" id="testEntryButton">Einfahrtsservo testen</button>
    <button class="secondary" onclick="testServo('exit')" id="testExitButton">Ausfahrtsservo testen</button>
  </div>
</div>

<script>
// Die Offenhaltezeit in Millisekunden. Dieselbe Zahl steht in config.h.
// Sie wird hier nur gebraucht, um die Breite des ablaufenden Balkens
// auszurechnen.
const BARRIER_OPEN_TIME_MS = 5000;


// Zeigt eine farbige Meldung an. kind ist "ok", "warn" oder "error".
function showMessage(elementId, text, kind) {
  const element = document.getElementById(elementId);
  element.textContent = text;
  element.className = "message " + kind;
}


// Sperrt oder entsperrt beide Knöpfe.
//
// Während eine Anfrage läuft, werden sie gesperrt. Sonst könnte jemand
// ungeduldig mehrfach klicken und damit mehrere Anfragen gleichzeitig
// losschicken.
function setButtonsDisabled(disabled) {
  document.getElementById("entryButton").disabled = disabled;
  document.getElementById("exitButton").disabled = disabled;
}


// Meldet eine Einfahrt an Pico 2.
async function requestEntry() {
  const plate = document.getElementById("entryPlate").value;

  showMessage("entryMessage", "Anfrage läuft ...", "warn");
  setButtonsDisabled(true);

  try {
    const reply = await (await fetch("/api/entry?plate=" + encodeURIComponent(plate))).json();
    showMessage("entryMessage", reply.message, reply.ok ? "ok" : "error");

    // Nach einer erfolgreichen Einfahrt wird das Feld geleert, damit für
    // das nächste Fahrzeug nicht versehentlich das alte Kennzeichen
    // stehen bleibt.
    if (reply.ok) {
      document.getElementById("entryPlate").value = "";
    }
  } catch (error) {
    // Hier landen wir, wenn Pico 2 selbst nicht antwortet. Das ist etwas
    // anderes als eine Absage der Kasse.
    showMessage("entryMessage", "Pico 2 antwortet nicht.", "error");
  }

  setButtonsDisabled(false);
  refreshPage();
}


// Meldet eine Ausfahrt an Pico 2.
async function requestExit() {
  const plate = document.getElementById("exitPlate").value;

  showMessage("exitMessage", "Bezahlstatus wird bei der Kasse abgefragt ...", "warn");
  setButtonsDisabled(true);

  try {
    const reply = await (await fetch("/api/exit?plate=" + encodeURIComponent(plate))).json();
    showMessage("exitMessage", reply.message, reply.ok ? "ok" : "error");

    if (reply.ok) {
      document.getElementById("exitPlate").value = "";
    }
  } catch (error) {
    showMessage("exitMessage", "Pico 2 antwortet nicht.", "error");
  }

  setButtonsDisabled(false);
  refreshPage();
}


// Fährt einen Servo einmal auf und zu, ohne einen Parkvorgang anzulegen.
// Nur im Test-Build vorhanden.
async function testServo(whichBarrier) {
  const reply = await (await fetch("/api/servotest?barrier=" + whichBarrier)).json();

  const messageId = (whichBarrier === "entry") ? "entryMessage" : "exitMessage";
  showMessage(messageId, reply.message, "ok");

  refreshPage();
}


// Schreibt den Zustand einer Schranke in die Anzeige.
//
// prefix      ist "entry" oder "exit" und wird den Element-Namen vorangestellt
// isOpen      steht der Arm gerade oben
// msLeft      wie lange er noch oben bleibt
function showBarrierState(prefix, isOpen, msLeft) {
  const stateElement = document.getElementById(prefix + "State");
  stateElement.textContent = isOpen ? "offen" : "geschlossen";
  stateElement.className = "state " + (isOpen ? "open" : "closed");

  // Der Balken zeigt die Restzeit als Anteil der vollen Offenhaltezeit.
  const percentLeft = Math.round(msLeft / BARRIER_OPEN_TIME_MS * 100);
  document.getElementById(prefix + "Bar").style.width = isOpen ? percentLeft + "%" : "0%";
}


// Beschriftet die beiden Schranken mit dem Pin, an dem ihr Servo hängt.
//
// Die Pin-Nummern kommen vom Pico (aus config.h) und stehen hier nicht fest
// im Text. So stimmt die Anzeige auch nach einem Umstecken.
//
// Bei nur einem Servo ist der Arm für Einfahrt und Ausfahrt derselbe. Die
// Oberfläche sagt das offen, statt zwei Schranken vorzutäuschen.
function labelBarriers(data) {
  if (data.singleServo) {
    const label = "Schrankenarm (GP" + data.entryPin + ", gemeinsam)";
    document.getElementById("entryLabel").textContent = label;
    document.getElementById("exitLabel").textContent  = label;
    document.getElementById("testEntryButton").textContent = "Servo testen";
    document.getElementById("testExitButton").style.display = "none";
  } else {
    document.getElementById("entryLabel").textContent = "Einfahrtsschranke (GP" + data.entryPin + ")";
    document.getElementById("exitLabel").textContent  = "Ausfahrtsschranke (GP" + data.exitPin + ")";
  }
}


// Holt den aktuellen Zustand von Pico 2 und schreibt ihn in die Seite.
// Wird jede Sekunde aufgerufen.
async function refreshPage() {
  let data;
  try {
    data = await (await fetch("/api/state")).json();
  } catch (error) {
    return;   // Pico 2 antwortet gerade nicht, alte Anzeige stehen lassen
  }

  // Ist die Kasse weg, zeigen wir "unbekannt" und nicht etwa eine Null.
  // Eine Null würde bedeuten "das Parkhaus ist leer", und das wissen wir
  // in diesem Moment gerade nicht.
  document.getElementById("occupancy").textContent =
    data.cashDeskOnline ? (data.occupied + " / " + data.totalSpots) : "unbekannt";

  const dot = document.getElementById("cashDeskDot");
  dot.className = "dot " + (data.cashDeskOnline ? "online" : "offline");
  document.getElementById("cashDeskText").textContent =
    data.cashDeskOnline ? "Kasse erreichbar" : "Kasse nicht erreichbar";

  showBarrierState("entry", data.entryBarrierOpen, data.entryBarrierMsLeft);
  showBarrierState("exit", data.exitBarrierOpen, data.exitBarrierMsLeft);

  labelBarriers(data);

  document.getElementById("debugCard").style.display = data.testBuild ? "block" : "none";
}


// Mit der Eingabetaste soll der jeweilige Vorgang starten.
document.getElementById("entryPlate").addEventListener("keydown", event => {
  if (event.key === "Enter") {
    requestEntry();
  }
});
document.getElementById("exitPlate").addEventListener("keydown", event => {
  if (event.key === "Enter") {
    requestExit();
  }
});

// Einmal sofort füllen, danach jede Sekunde auffrischen. Eine Sekunde ist
// hier nötig, weil der ablaufende Balken flüssig aussehen soll.
refreshPage();
setInterval(refreshPage, 1000);
</script>
</body>
</html>)HTML";
