#pragma once
#include <Arduino.h>

// ===========================================================================
// Die Weboberfläche der Kasse, also das, was der Benutzer im Browser sieht.
//
// Die ganze Seite steht als ein einziger langer Text in dieser Datei. Das
// sieht ungewohnt aus, hat aber einen guten Grund: Der Pico hat kein
// Dateisystem wie ein PC. Es gibt keinen Ordner, aus dem er eine HTML-Datei
// laden könnte. Also wird die Seite Teil des Programms.
//
// Die Schreibweise R"HTML( ... )HTML" nennt man Rohtext-Literal. Alles
// zwischen den beiden Markierungen wird genau so übernommen, wie es
// dasteht. Besonders praktisch: Anführungszeichen müssen nicht mit einem
// Rückwärtsstrich entwertet werden. Ohne diese Schreibweise müsste jedes
// einzelne " im HTML als \" geschrieben werden und die Datei wäre
// unlesbar.
//
// PROGMEM sagt dem Compiler, dass dieser Text im Flash-Speicher bleiben soll
// und nicht beim Start in den knappen Arbeitsspeicher kopiert wird.
//
// Die Seite lädt sich nie neu. Sie holt ihre Daten alle zwei Sekunden über
// die JSON-Schnittstelle und schreibt sie in die bestehenden Elemente.
// ===========================================================================

static const char CASHDESK_WEB_PAGE[] PROGMEM = R"HTML(<!doctype html>
<html lang="de">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Parkhaus Kasse</title>
<style>
  :root { --border:#d8dee7; --blue:#1d4ed8; --green:#15803d; --red:#b91c1c; --gray:#5b6472; }
  * { box-sizing:border-box; }
  body { margin:0; padding:24px 16px 48px; background:#f3f5f9; color:#111827;
         font-family:system-ui,-apple-system,"Segoe UI",Roboto,sans-serif; }
  .wrapper { max-width:900px; margin:0 auto; }
  h1 { font-size:24px; margin:0 0 4px; }
  .subtitle { color:var(--gray); margin:0 0 20px; font-size:14px; }
  .card { background:#fff; border:1px solid var(--border); border-radius:10px;
          padding:18px; margin-bottom:18px; }
  .card h2 { font-size:16px; margin:0 0 14px; }
  .statusbar { display:flex; gap:12px; flex-wrap:wrap; align-items:center; margin-bottom:18px; }
  .badge { background:#fff; border:1px solid var(--border); border-radius:999px;
           padding:7px 14px; font-size:14px; }
  .badge b { font-variant-numeric:tabular-nums; }
  input { font-size:16px; padding:9px 11px; border:1px solid var(--border);
          border-radius:7px; min-width:170px; text-transform:uppercase; }
  input.narrow { min-width:90px; text-transform:none; }
  button { font-size:15px; padding:9px 16px; border:0; border-radius:7px;
           background:var(--blue); color:#fff; cursor:pointer; }
  button.secondary { background:#4b5563; }
  button:disabled { background:#9aa3b2; cursor:not-allowed; }
  .row { display:flex; gap:10px; flex-wrap:wrap; align-items:center; }
  .message { margin-top:14px; padding:11px 13px; border-radius:7px; font-size:15px; display:none; }
  .message.ok    { display:block; background:#eafaf0; color:var(--green); border:1px solid #b7e6c9; }
  .message.warn  { display:block; background:#fdf1e7; color:#b45309; border:1px solid #f3d3ae; }
  .message.error { display:block; background:#fdeeee; color:var(--red); border:1px solid #f4c2c2; }
  .grid { display:grid; grid-template-columns:repeat(auto-fit,minmax(150px,1fr));
          gap:12px; margin-top:14px; }
  .field { background:#f8fafc; border:1px solid var(--border); border-radius:8px; padding:11px 13px; }
  .field span { display:block; font-size:12px; color:var(--gray); margin-bottom:3px; }
  .field b { font-size:17px; font-variant-numeric:tabular-nums; }
  table { width:100%; border-collapse:collapse; font-size:14px; }
  th, td { text-align:left; padding:9px 8px; border-bottom:1px solid #eef1f5; }
  th { color:var(--gray); font-weight:600; font-size:12px; text-transform:uppercase;
       letter-spacing:.03em; }
  td.number { font-variant-numeric:tabular-nums; }
  .pill { display:inline-block; padding:2px 9px; border-radius:999px; font-size:12px; }
  .pill.unpaid { background:#fdf1e7; color:#b45309; }
  .pill.paid   { background:#eafaf0; color:var(--green); }
  .pill.gone   { background:#eef1f5; color:var(--gray); }
  .empty { color:var(--gray); font-size:14px; padding:10px 0; }
  .debug { border-style:dashed; }
  .debug h2 { color:var(--gray); }
  @media (max-width:560px){ .historyColumn { display:none; } }
</style>
</head>
<body>
<div class="wrapper">
  <h1>Parkhaus &ndash; Kasse</h1>
  <p class="subtitle">Pico 1 &middot; Parkvorg&auml;nge, Geb&uuml;hren und Bezahlung</p>

  <div class="statusbar">
    <div class="badge">Belegung <b id="occupancy">&ndash;</b></div>
    <div class="badge">Tarif <b>CHF 2.00</b> / angefangene Stunde</div>
    <div class="badge">Uhr <b id="clockText">&ndash;</b></div>
  </div>

  <div class="card">
    <h2>Kennzeichen suchen und bezahlen</h2>
    <div class="row">
      <input id="searchInput" placeholder="z.B. LU 12345" autocomplete="off">
      <button onclick="searchPlate()">Suchen</button>
      <button class="secondary" id="payButton" onclick="payFee()" disabled>Bezahlen</button>
    </div>
    <div id="searchMessage" class="message"></div>
    <div class="grid" id="searchGrid" style="display:none">
      <div class="field"><span>Kennzeichen</span><b id="infoPlate">&ndash;</b></div>
      <div class="field"><span>Einfahrt</span><b id="infoEntry">&ndash;</b></div>
      <div class="field"><span>Parkdauer</span><b id="infoDuration">&ndash;</b></div>
      <div class="field"><span>Geb&uuml;hr</span><b id="infoFee">&ndash;</b></div>
      <div class="field"><span>Status</span><b id="infoStatus">&ndash;</b></div>
    </div>
  </div>

  <div class="card">
    <h2>Fahrzeuge im Parkhaus</h2>
    <table>
      <thead><tr><th>Kennzeichen</th><th>Einfahrt</th><th>Parkdauer</th><th>Geb&uuml;hr</th><th>Status</th></tr></thead>
      <tbody id="activeList"></tbody>
    </table>
    <div class="empty" id="activeEmpty">Zurzeit steht kein Fahrzeug im Parkhaus.</div>
  </div>

  <div class="card">
    <h2>Abgeschlossene Vorg&auml;nge</h2>
    <table>
      <thead><tr><th>Kennzeichen</th><th>Einfahrt</th><th class="historyColumn">Ausfahrt</th><th>Bezahlt</th><th>Status</th></tr></thead>
      <tbody id="historyList"></tbody>
    </table>
    <div class="empty" id="historyEmpty">Noch kein Fahrzeug ausgefahren.</div>
  </div>

  <div class="card debug" id="debugCard" style="display:none">
    <h2>Testwerkzeuge (nur im Test-Build)</h2>
    <div class="row">
      <input id="debugPlate" placeholder="z.B. BE 55511" autocomplete="off">
      <input id="debugMinutes" class="narrow" type="number" min="0" value="61">
      <button class="secondary" onclick="debugMoveEntryTime()">Einfahrt zur&uuml;cksetzen</button>
      <button class="secondary" onclick="debugDeleteAll()">Alle Vorg&auml;nge l&ouml;schen</button>
    </div>
    <div id="debugMessage" class="message"></div>
  </div>
</div>

<script>
// Das zuletzt gesuchte Kennzeichen. Es wird gebraucht, damit der
// Bezahlen-Knopf weiss, für welches Fahrzeug er zahlen soll.
let selectedPlate = "";


// Zeigt eine farbige Meldung an. kind ist "ok", "warn" oder "error".
function showMessage(elementId, text, kind) {
  const element = document.getElementById(elementId);
  element.textContent = text;
  element.className = "message " + kind;
}


// Schickt die Uhrzeit dieses Computers einmalig an die Kasse.
//
// Der Pico hat keine eigene Uhr. Ohne diesen Aufruf wüsste er nicht, welches
// Datum ist, und könnte keine Einfahrtszeit anzeigen.
//
// getTimezoneOffset liefert den Unterschied zur Weltzeit in Minuten. Wir
// rechnen ihn hier gleich mit ein, damit der Pico die lokale Zeit bekommt
// und selbst nichts über Zeitzonen wissen muss.
async function sendBrowserClockToPico() {
  const localUnixTime = Math.floor(Date.now() / 1000) - new Date().getTimezoneOffset() * 60;
  try {
    await fetch("/api/setclock?epoch=" + localUnixTime);
  } catch (error) {
    // Schlägt der Aufruf fehl, bleibt die Uhr ungestellt. Die Seite
    // funktioniert trotzdem, sie zeigt dann nur zwei Striche statt Zeiten.
  }
}


// Sucht ein Kennzeichen und zeigt die Angaben dazu an.
async function searchPlate() {
  const plate = document.getElementById("searchInput").value;

  if (!plate.trim()) {
    showMessage("searchMessage", "Bitte ein Kennzeichen eingeben.", "error");
    return;
  }

  // encodeURIComponent macht Sonderzeichen für die Adresszeile sicher.
  // Aus dem Leerzeichen in "LU 12345" wird dabei %20.
  const reply = await (await fetch("/api/status?plate=" + encodeURIComponent(plate))).json();
  displayParkingInfo(reply);
}


// Füllt die Anzeige mit den Angaben zu einem Fahrzeug.
function displayParkingInfo(reply) {
  const grid = document.getElementById("searchGrid");
  const payButton = document.getElementById("payButton");

  if (reply.status === "unknown") {
    selectedPlate = "";
    grid.style.display = "none";
    payButton.disabled = true;
    showMessage("searchMessage",
      "Das Kennzeichen " + reply.plate + " wurde im System nicht gefunden. Es ist keine Einfahrt bekannt.",
      "error");
    return;
  }

  selectedPlate = reply.plate;
  grid.style.display = "grid";
  document.getElementById("infoPlate").textContent    = reply.plate;
  document.getElementById("infoEntry").textContent    = reply.entry;
  document.getElementById("infoDuration").textContent = reply.duration;
  document.getElementById("infoFee").textContent      = reply.fee;
  document.getElementById("infoStatus").textContent   = (reply.status === "paid") ? "bezahlt" : "offen";

  if (reply.status === "paid") {
    payButton.disabled = true;
    showMessage("searchMessage",
      "Für dieses Kennzeichen wurde bereits bezahlt. Eine weitere Zahlung ist nicht nötig.", "ok");
  } else {
    payButton.disabled = false;
    showMessage("searchMessage", "Offener Betrag: " + reply.fee, "warn");
  }
}


// Bezahlt die Gebühr für das zuletzt gesuchte Fahrzeug.
async function payFee() {
  if (!selectedPlate) {
    return;
  }

  const reply = await (await fetch("/api/pay?plate=" + encodeURIComponent(selectedPlate))).json();
  showMessage("searchMessage", reply.message, reply.ok ? "ok" : "error");

  if (reply.ok) {
    document.getElementById("payButton").disabled    = true;
    document.getElementById("infoStatus").textContent = "bezahlt";
    document.getElementById("infoFee").textContent    = reply.amount;
  }

  refreshPage();
}


// Baut eine Tabellenzeile für ein Fahrzeug.
// isHistoryRow entscheidet, ob die Spalte Ausfahrt oder Parkdauer gezeigt wird.
function buildTableRow(vehicle, isHistoryRow) {
  let pill;
  if (vehicle.status === "paid") {
    pill = '<span class="pill paid">bezahlt</span>';
  } else if (vehicle.status === "unpaid") {
    pill = '<span class="pill unpaid">offen</span>';
  } else {
    pill = '<span class="pill gone">ausgefahren</span>';
  }

  if (isHistoryRow) {
    return "<tr><td>" + vehicle.plate +
           "</td><td class='number'>" + vehicle.entry +
           "</td><td class='number historyColumn'>" + vehicle.exit +
           "</td><td class='number'>" + vehicle.fee +
           "</td><td>" + pill + "</td></tr>";
  }

  return "<tr><td>" + vehicle.plate +
         "</td><td class='number'>" + vehicle.entry +
         "</td><td class='number'>" + vehicle.duration +
         "</td><td class='number'>" + vehicle.fee +
         "</td><td>" + pill + "</td></tr>";
}


// Holt den aktuellen Stand vom Pico und schreibt ihn in die Seite.
// Wird alle zwei Sekunden aufgerufen.
async function refreshPage() {
  let data;
  try {
    data = await (await fetch("/api/vehicles")).json();
  } catch (error) {
    // Antwortet der Pico gerade nicht, lassen wir einfach die alten Werte
    // stehen und versuchen es beim nächsten Mal wieder.
    return;
  }

  document.getElementById("occupancy").textContent = data.occupied + " / " + data.totalSpots;
  document.getElementById("clockText").textContent = data.clockIsSet ? data.now : "nicht gestellt";
  document.getElementById("debugCard").style.display = data.testBuild ? "block" : "none";

  document.getElementById("activeList").innerHTML =
    data.vehicles.map(vehicle => buildTableRow(vehicle, false)).join("");
  document.getElementById("historyList").innerHTML =
    data.history.map(vehicle => buildTableRow(vehicle, true)).join("");

  document.getElementById("activeEmpty").style.display  = data.vehicles.length ? "none" : "block";
  document.getElementById("historyEmpty").style.display = data.history.length ? "none" : "block";

  // Ist gerade eine Suche offen, werden Dauer und Gebühr mitgeführt.
  // Sonst würde dort eine veraltete Zahl stehen bleiben.
  if (selectedPlate) {
    const reply = await (await fetch("/api/status?plate=" + encodeURIComponent(selectedPlate))).json();
    if (reply.status !== "unknown") {
      document.getElementById("infoDuration").textContent = reply.duration;
      document.getElementById("infoFee").textContent      = reply.fee;
    }
  }
}


// Verschiebt die Einfahrtszeit eines Fahrzeugs in die Vergangenheit.
// Nur im Test-Build vorhanden.
async function debugMoveEntryTime() {
  const plate = document.getElementById("debugPlate").value;
  const minutes = document.getElementById("debugMinutes").value;

  const reply = await (await fetch(
    "/api/debug/time?plate=" + encodeURIComponent(plate) + "&minutes=" + minutes)).json();

  showMessage("debugMessage", reply.message, reply.ok ? "ok" : "error");
  refreshPage();
}


// Löscht alle Parkvorgänge. Nur im Test-Build vorhanden.
async function debugDeleteAll() {
  const reply = await (await fetch("/api/debug/reset")).json();
  showMessage("debugMessage", reply.message, "ok");

  selectedPlate = "";
  document.getElementById("searchGrid").style.display = "none";
  document.getElementById("payButton").disabled = true;
  refreshPage();
}


// Mit der Eingabetaste im Suchfeld soll die Suche starten.
document.getElementById("searchInput").addEventListener("keydown", event => {
  if (event.key === "Enter") {
    searchPlate();
  }
});

// Beim Laden zuerst die Uhr stellen, danach die Seite füllen.
sendBrowserClockToPico().then(refreshPage);

// Und ab dann alle zwei Sekunden auffrischen.
setInterval(refreshPage, 2000);
</script>
</body>
</html>)HTML";
