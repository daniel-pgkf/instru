#include "WebServerManager.h"

WebServerManager::WebServerManager(
    TiltSensor& tilt,
    PIRSensor& pir,
    FotoSensor& foto,
    MQSensor& mq,
    RFReceiver& rf,
    bool useCoolUI
)
    : server(80),
      tiltSensor(tilt),
      pirSensor(pir),
      fotoSensor(foto),
      mqSensor(mq),
      rfReceiver(rf),
      _useCoolUI(useCoolUI) {}

void WebServerManager::begin() {
    server.on("/", [this]() { handleRoot(); });
    server.on("/data", [this]() { handleData(); });
    server.begin();
}

void WebServerManager::handleClient() {
    server.handleClient();
}

void WebServerManager::handleRoot() {
    String htmlSencillo = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head><title>Equipo9</title></head>
    <body>
        <h2>Equipo9</h2>
        <h3>Log RF</h3>
        <ul id="rf_log"></ul>
        <h3>Sensor Tilt</h3>
        <p>Inclinacion detectada: <span id="tilt_d"></span></p>
        <p>Contador inclinaciones: <span id="tilt_c"></span></p>
        <p>Alerta tilt: <span id="tilt_a"></span></p>
        <h3>Sensor PIR</h3>
        <p>Movimiento detectado: <span id="pir_m"></span></p>
        <h3>FotoSensor</h3>
        <p>Lectura celda: <span id="foto_v"></span> V</p>
        <p>Estado luz: <span id="foto_s"></span></p>
        <h3>Sensor MQ</h3>
        <p>ADC MQ: <span id="mq_adc"></span></p>
        <p>Estado contaminacion: <span id="mq_state"></span></p>
        <script>
        setInterval(() => {
            fetch('/data').then(r => r.json()).then(data => {
                document.getElementById('tilt_d').innerText = data.tilt_detected;
                document.getElementById('tilt_c').innerText = data.tilt_count;
                document.getElementById('tilt_a').innerText = data.tilt_alert;
                document.getElementById('pir_m').innerText = data.pir_motion;
                document.getElementById('foto_v').innerText = data.foto_reading.toFixed(2);
                document.getElementById('foto_s').innerText = data.foto_state;
                document.getElementById('mq_adc').innerText = data.mq_adc;
                document.getElementById('mq_state').innerText = data.mq_state;
                const log = document.getElementById('rf_log');
                log.innerHTML = '';
                (data.rf_log || []).forEach(msg => {
                    const li = document.createElement('li');
                    li.innerText = msg;
                    log.appendChild(li);
                });
            });
        }, 500);
        </script>
    </body>
    </html>
    )rawliteral";

    String htmlAtractivo = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Panel Equipo 9</title>
    <style>
        body { font-family: sans-serif; background: #1a1a1a; color: #e0e0e0; margin: 0; padding: 20px; }
        .container { max-width: 860px; margin: auto; }
        .header { text-align: center; padding: 10px; border-bottom: 2px solid #333; margin-bottom: 20px; }
        .grid { display: grid; grid-template-columns: 1fr 1fr; gap: 15px; }
        .card { background: #2d2d2d; padding: 15px; border-radius: 8px; box-shadow: 0 4px 10px rgba(0,0,0,0.3); }
        .full-width { grid-column: 1 / -1; }
        h3 { margin-top: 0; color: #4dabf7; font-size: 0.9rem; text-transform: uppercase; }
        .val { font-size: 1.8rem; font-weight: bold; display: block; margin: 10px 0; }
        .status { padding: 4px 8px; border-radius: 4px; font-size: 0.8rem; font-weight: bold; }
        .bg-off { background: #444; }
        .bg-on { background: #ff4757; color: white; }
        .bg-warn { background: #ffa502; color: black; }
        #alert-box { display: none; background: #ff4757; color: white; text-align: center; padding: 15px; font-weight: bold; border-radius: 8px; margin-bottom: 15px; animation: pulse 1s infinite; }
        @keyframes pulse { 0%{opacity:1}50%{opacity:0.6}100%{opacity:1} }
        .bar-container { background: #444; border-radius: 5px; height: 12px; width: 100%; overflow: hidden; }
        .bar-fill { background: #4dabf7; height: 100%; width: 0%; transition: width 0.3s; }
        canvas { display: block; width: 100%; background: #1e1e1e; border-radius: 6px; }
        .chart-legend { display: flex; flex-wrap: wrap; gap: 14px; margin-top: 8px; font-size: 0.75rem; color: #aaa; }
        .legend-item { display: flex; align-items: center; gap: 5px; }
        .legend-dot { width: 10px; height: 3px; border-radius: 2px; }
        #rf_log { margin: 0; padding-left: 18px; font-size: 0.85rem; font-family: monospace; color: #ccc; list-style: disc; max-height: 180px; overflow-y: auto; }
        #rf_log li { padding: 2px 0; border-bottom: 1px solid #3a3a3a; }
    </style>
</head>
<body>
<div class="container">
    <div class="header"><h2>EQUIPO 9 | MONITOREO LOCAL</h2></div>
    <div id="alert-box">ALERTA CRITICA: SEGURIDAD COMPROMETIDA</div>
    <div class="grid">

        <div class="card full-width">
            <h3>Log RF</h3>
            <ul id="rf_log"></ul>
        </div>

        <div class="card full-width">
            <h3>Retransmisión RF</h3>
            <span class="val" id="rf_tx">SIN DATOS</span>
        </div>

        <div class="card">
            <h3>Sensor Tilt</h3>
            <span id="tilt_d" class="status bg-off">ESTABLE</span>
            <span class="val" id="tilt_c">0</span>
            <small>Inclinaciones registradas</small>
        </div>
        <div class="card">
            <h3>Sensor PIR</h3>
            <span id="pir_m" class="status bg-off">SIN MOVIMIENTO</span>
            <span class="val" id="pir_val">CALMA</span>
        </div>
        <div class="card full-width">
            <h3>Fotoresistencia (Luminosidad)</h3>
            <div style="display:flex;justify-content:space-between;align-items:flex-end;">
                <span class="val" id="foto_v">0.00 V</span>
                <span id="foto_s" style="margin-bottom:15px;">Detectando...</span>
            </div>
            <div class="bar-container"><div id="bar_foto" class="bar-fill"></div></div>
        </div>
        <div class="card full-width">
            <h3>MQ-135 (Calidad de Aire)</h3>
            <div style="display:flex;justify-content:space-between;align-items:flex-end;">
                <span class="val" id="mq_adc">0</span>
                <span id="mq_state" style="margin-bottom:15px;">Analizando...</span>
            </div>
            <div class="bar-container"><div id="bar_mq" class="bar-fill" style="background:#ffa502;"></div></div>
        </div>
        <div class="card full-width">
            <h3>Vout Fotocelda vs Tiempo</h3>
            <canvas id="cFoto" height="160"></canvas>
            <div class="chart-legend">
                <div class="legend-item"><div class="legend-dot" style="background:#ffffff;"></div>Dato (Vout)</div>
                <div class="legend-item"><div class="legend-dot" style="background:#555;border-top:2px dashed #555;height:0;"></div>Oscuridad &le;0.3V</div>
                <div class="legend-item"><div class="legend-dot" style="background:#ffd43b;border-top:2px dashed #ffd43b;height:0;"></div>Tenue &le;1.2V</div>
                <div class="legend-item"><div class="legend-dot" style="background:#69db7c;border-top:2px dashed #69db7c;height:0;"></div>Media &le;3.0V</div>
                <div class="legend-item"><div class="legend-dot" style="background:#ff922b;border-top:2px dashed #ff922b;height:0;"></div>Alta &gt;3.0V</div>
            </div>
        </div>
        <div class="card full-width">
            <h3>ADC MQ vs Tiempo</h3>
            <canvas id="cMQ" height="160"></canvas>
            <div class="chart-legend">
                <div class="legend-item"><div class="legend-dot" style="background:#ffffff;"></div>Dato (ADC)</div>
                <div class="legend-item"><div class="legend-dot" style="background:#69db7c;border-top:2px dashed #69db7c;height:0;"></div>Bajo &lt;200</div>
                <div class="legend-item"><div class="legend-dot" style="background:#ffd43b;border-top:2px dashed #ffd43b;height:0;"></div>Medio &lt;400</div>
                <div class="legend-item"><div class="legend-dot" style="background:#ffa502;border-top:2px dashed #ffa502;height:0;"></div>Med-Alto &lt;600</div>
                <div class="legend-item"><div class="legend-dot" style="background:#ff6b6b;border-top:2px dashed #ff6b6b;height:0;"></div>Alto &ge;600</div>
            </div>
        </div>
    </div>
</div>
<script>
const MAX_POINTS = 60;
const fotoHist = [];
const mqHist = [];

function drawChart(canvasId, data, yMin, yMax, thresholds) {
    const canvas = document.getElementById(canvasId);
    const ctx = canvas.getContext('2d');
    const dpr = window.devicePixelRatio || 1;
    const cssW = canvas.parentElement.clientWidth - 30;
    const cssH = 170;
    canvas.style.width = cssW + 'px';
    canvas.style.height = cssH + 'px';
    canvas.width = cssW * dpr;
    canvas.height = cssH * dpr;
    ctx.scale(dpr, dpr);
    const W = cssW, H = cssH;
    const PAD = { top: 8, right: 12, bottom: 28, left: 48 };
    const cW = W - PAD.left - PAD.right;
    const cH = H - PAD.top - PAD.bottom;
    ctx.clearRect(0, 0, W, H);
    function xPos(i) { return PAD.left + (i / (MAX_POINTS - 1)) * cW; }
    function yPos(v) { return PAD.top + cH - ((Math.min(Math.max(v, yMin), yMax) - yMin) / (yMax - yMin)) * cH; }
    const bands = [];
    let prev = yMin;
    for (let i = 0; i < thresholds.length; i++) {
        bands.push({ from: prev, to: thresholds[i].value, color: thresholds[i].color });
        prev = thresholds[i].value;
    }
    bands.push({ from: prev, to: yMax, color: thresholds[thresholds.length - 1].colorTop || thresholds[thresholds.length - 1].color });
    bands.forEach(b => {
        const y1 = yPos(b.to);
        const y2 = yPos(b.from);
        ctx.fillStyle = b.color + '40';
        ctx.fillRect(PAD.left, y1, cW, y2 - y1);
        ctx.strokeStyle = b.color + '80';
        ctx.lineWidth = 1;
        ctx.setLineDash([]);
        ctx.beginPath();
        ctx.moveTo(PAD.left, y1);
        ctx.lineTo(PAD.left + cW, y1);
        ctx.stroke();
    });
    ctx.setLineDash([]);
    ctx.strokeStyle = '#444';
    ctx.lineWidth = 1;
    const dec = yMax <= 4 ? 1 : 0;
    const yTicks = 5;
    for (let i = 0; i <= yTicks; i++) {
        const v = yMin + (i / yTicks) * (yMax - yMin);
        const y = yPos(v);
        ctx.beginPath();
        ctx.moveTo(PAD.left - 3, y);
        ctx.lineTo(PAD.left, y);
        ctx.stroke();
        ctx.fillStyle = '#999';
        ctx.font = '11px sans-serif';
        ctx.textAlign = 'right';
        ctx.fillText(v.toFixed(dec), PAD.left - 5, y + 4);
    }
    ctx.beginPath();
    ctx.moveTo(PAD.left, PAD.top);
    ctx.lineTo(PAD.left, PAD.top + cH);
    ctx.stroke();
    ctx.beginPath();
    ctx.moveTo(PAD.left, PAD.top + cH);
    ctx.lineTo(PAD.left + cW, PAD.top + cH);
    ctx.stroke();
    ctx.fillStyle = '#777';
    ctx.font = '11px sans-serif';
    ctx.textAlign = 'left';
    ctx.fillText('\u2190 ' + (MAX_POINTS * 0.5) + 's', PAD.left + 2, PAD.top + cH + 18);
    ctx.textAlign = 'right';
    ctx.fillText('ahora', PAD.left + cW, PAD.top + cH + 18);
    if (data.length < 2) return;
    ctx.beginPath();
    ctx.strokeStyle = 'rgba(255,255,255,0.85)';
    ctx.lineWidth = 2;
    ctx.lineJoin = 'round';
    ctx.setLineDash([]);
    ctx.moveTo(xPos(0), yPos(data[0]));
    for (let i = 1; i < data.length; i++) ctx.lineTo(xPos(i), yPos(data[i]));
    ctx.stroke();
    const last = data[data.length - 1];
    const px = xPos(data.length - 1), py = yPos(last);
    ctx.shadowBlur = 8;
    ctx.shadowColor = '#ffffff';
    ctx.beginPath();
    ctx.arc(px, py, 4, 0, Math.PI * 2);
    ctx.fillStyle = '#ffffff';
    ctx.fill();
    ctx.shadowBlur = 0;
}

function update() {
    fetch('/data').then(r => r.json()).then(data => {
        document.getElementById('tilt_c').innerText = data.tilt_count;
        const tiltD = document.getElementById('tilt_d');
        if (data.tilt_detected) { tiltD.innerText = "INCLINADO"; tiltD.className = "status bg-on"; }
        else { tiltD.innerText = "ESTABLE"; tiltD.className = "status bg-off"; }
        document.getElementById('alert-box').style.display = data.tilt_alert ? "block" : "none";
        const pirM = document.getElementById('pir_m');
        document.getElementById('pir_val').innerText = data.pir_motion ? "ACTIVO" : "CALMA";
        pirM.className = data.pir_motion ? "status bg-warn" : "status bg-off";
        pirM.innerText = data.pir_motion ? "MOVIMIENTO" : "CALMA";
        document.getElementById('foto_v').innerText = data.foto_reading.toFixed(2) + " V";
        document.getElementById('foto_s').innerText = data.foto_state;
        document.getElementById('bar_foto').style.width = ((data.foto_reading / 3.3) * 100) + "%";
        document.getElementById('mq_adc').innerText = data.mq_adc;
        document.getElementById('mq_state').innerText = data.mq_state;
        document.getElementById('bar_mq').style.width = ((data.mq_adc / 4095) * 100) + "%";
        document.getElementById('rf_tx').innerText = data.rf_tx;

        const log = document.getElementById('rf_log');
        log.innerHTML = '';
        (data.rf_log || []).forEach(msg => {
            const li = document.createElement('li');
            li.innerText = msg;
            log.appendChild(li);
        });

        fotoHist.push(data.foto_reading);
        if (fotoHist.length > MAX_POINTS) fotoHist.shift();
        mqHist.push(data.mq_adc);
        if (mqHist.length > MAX_POINTS) mqHist.shift();
        drawChart('cFoto', fotoHist, 0, 3.3, [
            { value: 0.3,  color: '#555555' },
            { value: 1.2,  color: '#ffd43b' },
            { value: 3.0,  color: '#69db7c', colorTop: '#ff922b' }
        ]);
        drawChart('cMQ', mqHist, 0, 1000, [
            { value: 200,  color: '#69db7c' },
            { value: 400,  color: '#ffd43b' },
            { value: 600,  color: '#ffa502', colorTop: '#ff6b6b' }
        ]);
    });
}

setInterval(update, 500);
update();
</script>
</body>
</html>
    )rawliteral";

    if (_useCoolUI) {
        server.send(200, "text/html", htmlAtractivo);
    } else {
        server.send(200, "text/html", htmlSencillo);
    }
}

void WebServerManager::handleData() {
    String json = "{";
    json += "\"tilt_detected\":" + String(tiltSensor.isTiltDetected() ? "true" : "false") + ",";
    json += "\"tilt_count\":" + String(tiltSensor.getCount()) + ",";
    json += "\"tilt_alert\":" + String(tiltSensor.isAlertActive() ? "true" : "false") + ",";
    json += "\"pir_motion\":" + String(pirSensor.isMotionDetected() ? "true" : "false") + ",";
    json += "\"foto_reading\":" + String(fotoSensor.getCellReading(), 3) + ",";
    json += "\"foto_state\":\"" + fotoSensor.getLightState() + "\",";
    json += "\"mq_adc\":" + String(mqSensor.getADC()) + ",";
    json += "\"rf_tx\":\"" + rfReceiver.getLastRetransmission() + "\",";
    json += "\"mq_state\":\"" + mqSensor.getPollutionState() + "\",";
    json += "\"rf_log\":[";
    int count = rfReceiver.getLogCount();
    for (int i = 0; i < count; i++) {
        json += "\"" + rfReceiver.getLog(i) + "\"";
        if (i < count - 1) json += ",";
    }
    json += "]}";
    server.send(200, "application/json", json);
}