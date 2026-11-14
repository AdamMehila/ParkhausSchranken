// Steuert Chrome über das DevTools-Protokoll für die Testdurchführung.
//
// Chrome muss mit eigenem Profil und Debug-Port laufen, zum Beispiel:
//   chrome.exe --remote-debugging-port=9222 --user-data-dir=<Ordner> http://192.168.137.211 http://192.168.137.212
//
// Aufruf:  node tools/browser_test.mjs <Aktion> [Argumente]
//
//   kasse-read                     Belegung, Uhr, Listen, Suchresultat und Meldungen der Kasse-Seite
//   kasse-search <Kennzeichen>     Suchfeld füllen, "Suchen" klicken
//   kasse-pay [trotzdem]           "Bezahlen" klicken. Ohne "trotzdem" nur, wenn der Knopf frei ist.
//                                  Mit "trotzdem" auch auf den gesperrten Knopf klicken und melden,
//                                  ob die Seite eine Anfrage an /api/pay geschickt hat
//   kasse-debugtime <Kz> <Min>     Karte Testwerkzeuge: "Einfahrt zurücksetzen"
//   kasse-reset                    Karte Testwerkzeuge: "Alle Vorgänge löschen"
//   schranke-read                  Belegung, Kasse erreichbar, Arm-Anzeigen, Meldungen, ganzer Seitentext
//   schranke-entry <Kennzeichen>   Feld "Einfahrt" füllen, "Einfahrt bestätigen" klicken
//   schranke-exit <Kennzeichen>    Feld "Ausfahrt" füllen, "Ausfahrt bestätigen" klicken
//   schranke-servo                 "Servo testen" klicken
//   reload <kasse|schranke>        Seite neu laden
//   shot <kasse|schranke|api> <Datei.png>   Screenshot der ganzen Seite
//   open-url <Adresse>             Adresse in einem eigenen Tab "api" öffnen und den Seitentext liefern
//
// Ein leeres Kennzeichen wird als "" übergeben. Eingaben werden als Tastatureingabe
// (Input.insertText) und Klicks als Mausereignisse an der Mitte des Knopfs geschickt.
// Jede Aktion wird mit Uhrzeit an die Logdatei angehängt (Umgebungsvariable BROWSER_LOG,
// Standard docs/nachweise/Browser_Z1.log).

import { appendFileSync, writeFileSync } from "node:fs";
import { spawn } from "node:child_process";

const PORT = 9222;
const HOSTS = { kasse: "192.168.137.211", schranke: "192.168.137.212" };
const LOG = process.env.BROWSER_LOG ?? "docs/nachweise/Browser_Z1.log";

function now() {
  const d = new Date();
  const p = (n, w = 2) => String(n).padStart(w, "0");
  return `${p(d.getHours())}:${p(d.getMinutes())}:${p(d.getSeconds())}.${p(d.getMilliseconds(), 3)}`;
}

async function listTargets() {
  return (await fetch(`http://127.0.0.1:${PORT}/json/list`)).json();
}

async function findTab(name) {
  const targets = (await listTargets()).filter(t => t.type === "page");
  if (name === "api") {
    return targets.find(t => t.title === "api" || t.url.includes("/api/")) ?? null;
  }
  const host = HOSTS[name];
  return targets.find(t => t.url.startsWith(`http://${host}/`) && !t.url.includes("/api/")) ?? null;
}

class Cdp {
  constructor(wsUrl) {
    this.ws = new WebSocket(wsUrl);
    this.id = 0;
    this.waiting = new Map();
    this.ws.onmessage = ev => {
      const msg = JSON.parse(ev.data);
      if (msg.id && this.waiting.has(msg.id)) {
        const { resolve, reject } = this.waiting.get(msg.id);
        this.waiting.delete(msg.id);
        msg.error ? reject(new Error(msg.error.message)) : resolve(msg.result);
      }
    };
  }
  open() {
    return new Promise((resolve, reject) => {
      this.ws.onopen = resolve;
      this.ws.onerror = reject;
    });
  }
  send(method, params = {}) {
    const id = ++this.id;
    this.ws.send(JSON.stringify({ id, method, params }));
    return new Promise((resolve, reject) => this.waiting.set(id, { resolve, reject }));
  }
  async eval(expression) {
    const r = await this.send("Runtime.evaluate", { expression, awaitPromise: true, returnByValue: true });
    if (r.exceptionDetails) throw new Error(r.exceptionDetails.exception?.description ?? "Fehler im Skript");
    return r.result.value;
  }
  close() { this.ws.close(); }
}

async function connect(name) {
  const tab = await findTab(name);
  if (!tab) throw new Error(`Kein Tab für ${name} gefunden`);
  const cdp = new Cdp(tab.webSocketDebuggerUrl);
  await cdp.open();
  // Ein verdeckter Tab bekommt keine Mausklicks und Tastatureingaben zugestellt.
  // Darum den Tab vor jeder Aktion in den Vordergrund holen und warten, bis er sichtbar ist.
  // Ist das Chrome-Fenster von einem anderen Fenster verdeckt, bleibt visibilityState "hidden",
  // obwohl der Tab vorne ist und Klicks ankommen. Darum nur kurz warten statt auf "visible".
  await cdp.send("Page.bringToFront");
  await sleep(150);
  await cdp.eval(INSTALL_TRACKER);
  return cdp;
}

// Zählt in der Seite die laufenden und fertigen Anfragen je Pfad. Ändert am Verhalten der Seite nichts.
const INSTALL_TRACKER = `(() => {
  if (window.__t && window.__t.v === 2) return true;
  window.__t = { v: 2, pending: 0, started: [], done: [] };
  const original = window.fetch.bind(window);
  window.fetch = async (url, opts) => {
    const path = String(url).split("?")[0];
    window.__t.pending++;
    window.__t.started.push({ path, t: Date.now() });
    try { return await original(url, opts); }
    finally { window.__t.pending--; window.__t.done.push({ path, t: Date.now() }); }
  };
  return true;
})()`;

const sleep = ms => new Promise(r => setTimeout(r, ms));

async function pageNow(cdp) { return cdp.eval("Date.now()"); }

async function waitForRequest(cdp, path, since, timeoutMs = 12000) {
  const start = Date.now();
  while (Date.now() - start < timeoutMs) {
    const done = await cdp.eval(`window.__t.done.some(d => d.path === ${JSON.stringify(path)} && d.t >= ${since})`);
    if (done) {
      await sleep(400);   // Zeit für json() und das Schreiben in die Seite
      return true;
    }
    await sleep(25);
  }
  return false;
}

async function typeInto(cdp, selector, text) {
  await cdp.eval(`(() => { const e = document.querySelector(${JSON.stringify(selector)});
    e.scrollIntoView({ block: "center" }); e.focus(); e.select(); return true; })()`);
  if (text === "") {
    await cdp.eval(`(() => { const e = document.querySelector(${JSON.stringify(selector)});
      e.value = ""; e.dispatchEvent(new Event("input", { bubbles: true })); return true; })()`);
  } else {
    await cdp.send("Input.insertText", { text });
  }
  return cdp.eval(`document.querySelector(${JSON.stringify(selector)}).value`);
}

async function click(cdp, selector, force = false) {
  const box = await cdp.eval(`(() => { const e = document.querySelector(${JSON.stringify(selector)});
    e.scrollIntoView({ block: "center" }); const r = e.getBoundingClientRect();
    return { x: r.x + r.width / 2, y: r.y + r.height / 2, disabled: !!e.disabled }; })()`);
  if (box.disabled && !force) return { clicked: false, reason: "Knopf gesperrt (disabled)" };
  // Mit BEEP=1 piept der Laptop zweimal kurz vor dem Klick (Zeichen für den Beobachter des Arms).
  if (process.env.BEEP === "1") {
    spawn("powershell.exe", ["-NoProfile", "-Command", "[console]::beep(1000,200);[console]::beep(1000,200)"], { stdio: "ignore" });
    await sleep(900);
  }
  const t = await pageNow(cdp);
  for (const type of ["mouseMoved", "mousePressed", "mouseReleased"]) {
    await cdp.send("Input.dispatchMouseEvent", { type, x: box.x, y: box.y, button: "left", clickCount: 1 });
  }
  return { clicked: true, t, clickTime: now(), disabled: box.disabled };
}

const READ_MESSAGE = id => `(() => { const e = document.getElementById("${id}");
  return { text: e.textContent, kind: e.className.replace("message", "").trim() || "keine", visible: getComputedStyle(e).display !== "none" }; })()`;

const REFRESH = `(async () => { if (typeof refreshPage === "function") { try { await refreshPage(); } catch (e) {} } return true; })()`;

async function readKasse(cdp) {
  // Ein Tab im Hintergrund frischt sich nur verzögert auf. Vor dem Ablesen dieselbe
  // Funktion aufrufen, die die Seite sonst alle 2 s selbst aufruft.
  await cdp.eval(REFRESH);
  return cdp.eval(`(() => {
    const t = id => document.getElementById(id)?.textContent ?? null;
    const rows = id => [...document.querySelectorAll("#" + id + " tr")].map(r => [...r.cells].map(c => c.textContent.trim()).join(" | "));
    const msg = id => { const e = document.getElementById(id); return { text: e.textContent, kind: e.className.replace("message", "").trim() || "keine", visible: getComputedStyle(e).display !== "none" }; };
    const grid = document.getElementById("searchGrid");
    return {
      belegung: t("occupancy"), uhr: t("clockText"),
      suchMeldung: msg("searchMessage"),
      suchresultatSichtbar: getComputedStyle(grid).display !== "none",
      suchresultat: { kennzeichen: t("infoPlate"), einfahrt: t("infoEntry"), parkdauer: t("infoDuration"), gebuehr: t("infoFee"), status: t("infoStatus"), bezahltAm: t("infoPaidAt") },
      bezahlenGesperrt: document.getElementById("payButton").disabled,
      fahrzeugeImParkhaus: rows("activeList"), abgeschlosseneVorgaenge: rows("historyList"),
      testwerkzeugeSichtbar: getComputedStyle(document.getElementById("debugCard")).display !== "none",
      testwerkzeugeMeldung: msg("debugMessage")
    };
  })()`);
}

async function readSchranke(cdp) {
  await cdp.eval(REFRESH);
  return cdp.eval(`(() => {
    const t = id => document.getElementById(id)?.textContent ?? null;
    const msg = id => { const e = document.getElementById(id); return { text: e.textContent, kind: e.className.replace("message", "").trim() || "keine", visible: getComputedStyle(e).display !== "none" }; };
    return {
      belegung: t("occupancy"), kasse: t("cashDeskText"),
      einfahrtArm: t("entryState"), ausfahrtArm: t("exitState"),
      einfahrtMeldung: msg("entryMessage"), ausfahrtMeldung: msg("exitMessage"),
      einfahrtFeld: document.getElementById("entryPlate").value, ausfahrtFeld: document.getElementById("exitPlate").value,
      seitentext: document.body.innerText
    };
  })()`);
}

async function screenshot(cdp, file) {
  const m = await cdp.send("Page.getLayoutMetrics");
  const size = m.cssContentSize ?? m.contentSize;
  await sleep(300);   // Zeit für das Zeichnen nach dem Wechsel in den Vordergrund
  const r = await cdp.send("Page.captureScreenshot", {
    format: "png", captureBeyondViewport: true,
    clip: { x: 0, y: 0, width: size.width, height: size.height, scale: 1 }
  });
  writeFileSync(file, Buffer.from(r.data, "base64"));
  return { datei: file, breite: Math.round(size.width), hoehe: Math.round(size.height) };
}

async function act(cdp, { fill, button, path, force = false }) {
  const result = {};
  for (const [selector, text] of fill ?? []) result[`eingabe ${selector}`] = await typeInto(cdp, selector, text);
  let c = await click(cdp, button, force);
  // Kommt ein Klick nicht bei der Seite an (verdeckter Tab), startet keine Anfrage.
  // Dann bis zu zweimal neu klicken. Das ist sicher, weil der Pico noch nichts erhalten hat.
  if (c.clicked && path && !c.disabled) {
    for (let attempt = 1; attempt <= 2; attempt++) {
      let started = false;
      for (let i = 0; i < 30 && !started; i++) {
        started = await cdp.eval(`window.__t.started.some(d => d.path === ${JSON.stringify(path)} && d.t >= ${c.t})`);
        if (!started) await sleep(50);
      }
      if (started) break;
      result[`klickOhneWirkung${attempt}`] = c.clickTime;
      await cdp.send("Page.bringToFront");
      await sleep(300);
      c = await click(cdp, button, force);
    }
  }
  result.klick = c.clicked ? c.clickTime : c.reason;
  if (c.clicked) result.knopfGesperrtBeimKlick = c.disabled;
  // Bei einem Klick auf einen gesperrten Knopf 3 s warten. Bleibt die Anfrage aus, ist anfrageFertig false.
  if (c.clicked && path) result.anfrageFertig = await waitForRequest(cdp, path, c.t, c.disabled ? 3000 : 12000);
  else await sleep(400);
  return result;
}

async function main() {
  const [action, ...args] = process.argv.slice(2);
  const start = now();
  let out;
  let cdp;
  try {
    switch (action) {
      case "kasse-read": cdp = await connect("kasse"); out = await readKasse(cdp); break;
      case "kasse-search":
        cdp = await connect("kasse");
        out = await act(cdp, { fill: [["#searchInput", args[0] ?? ""]], button: 'button[onclick="searchPlate()"]', path: (args[0] ?? "").trim() ? "/api/status" : null });
        out.seite = await readKasse(cdp); break;
      case "kasse-pay":
        cdp = await connect("kasse");
        out = await act(cdp, { button: "#payButton", path: "/api/pay", force: args[0] === "trotzdem" });
        out.seite = await readKasse(cdp); break;
      case "kasse-debugtime":
        cdp = await connect("kasse");
        out = await act(cdp, { fill: [["#debugPlate", args[0]], ["#debugMinutes", args[1]]], button: 'button[onclick="debugMoveEntryTime()"]', path: "/api/debug/time" });
        out.seite = await readKasse(cdp); break;
      case "kasse-reset":
        cdp = await connect("kasse");
        out = await act(cdp, { button: 'button[onclick="debugDeleteAll()"]', path: "/api/debug/reset" });
        out.seite = await readKasse(cdp); break;
      case "schranke-read": cdp = await connect("schranke"); out = await readSchranke(cdp); break;
      case "schranke-entry":
        cdp = await connect("schranke");
        out = await act(cdp, { fill: [["#entryPlate", args[0] ?? ""]], button: "#entryButton", path: "/api/entry" });
        out.seite = await readSchranke(cdp); delete out.seite.seitentext; break;
      case "schranke-exit":
        cdp = await connect("schranke");
        out = await act(cdp, { fill: [["#exitPlate", args[0] ?? ""]], button: "#exitButton", path: "/api/exit" });
        out.seite = await readSchranke(cdp); delete out.seite.seitentext; break;
      case "schranke-servo":
        cdp = await connect("schranke");
        out = await act(cdp, { button: "#testEntryButton", path: "/api/servotest" });
        out.seite = await readSchranke(cdp); delete out.seite.seitentext; break;
      case "reload": {
        cdp = await connect(args[0]);
        await cdp.send("Page.reload", { ignoreCache: true });
        await sleep(3000);
        cdp.close();
        cdp = await connect(args[0]);
        out = args[0] === "kasse" ? await readKasse(cdp) : await readSchranke(cdp);
        break;
      }
      case "shot": cdp = await connect(args[0]); out = await screenshot(cdp, args[1]); break;
      case "open-url": {
        let tab = await findTab("api");
        if (!tab) tab = await (await fetch(`http://127.0.0.1:${PORT}/json/new?about:blank`, { method: "PUT" })).json();
        cdp = new Cdp(tab.webSocketDebuggerUrl); await cdp.open();
        await cdp.send("Page.navigate", { url: args[0] });
        await sleep(1500);
        out = { adresse: args[0], seitentext: await cdp.eval("document.body ? document.body.innerText : ''") };
        break;
      }
      default:
        throw new Error(`Unbekannte Aktion: ${action}`);
    }
  } catch (e) {
    out = { fehler: String(e.message ?? e) };
  } finally {
    cdp?.close();
  }
  const line = `${start} ${action} ${JSON.stringify(args)} -> ${JSON.stringify(out)}`;
  appendFileSync(LOG, line + "\n", "utf8");
  console.log(JSON.stringify(out, null, 2));
}

main();
