#include "WebServerManager.h"

WebServerManager::WebServerManager(
    TiltSensor& tilt,
    PIRSensor& pir,
    FotoSensor& foto,
    MQSensor& mq
)
    : server(80),
      tiltSensor(tilt),
      pirSensor(pir),
      fotoSensor(foto),
      mqSensor(mq) {}

void WebServerManager::begin() {
    server.on("/", [this]() { handleRoot(); });
    server.on("/data", [this]() { handleData(); });

    server.begin();
}

void WebServerManager::handleClient() {
    server.handleClient();
}

void WebServerManager::handleRoot() {
    String html = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
        <title>Equipo9</title>
    </head>
    <body>
        <h2>Equipo9</h2>

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
            fetch('/data')
            .then(r => r.json())
            .then(data => {
                document.getElementById('tilt_d').innerText = data.tilt_detected;
                document.getElementById('tilt_c').innerText = data.tilt_count;
                document.getElementById('tilt_a').innerText = data.tilt_alert;

                document.getElementById('pir_m').innerText = data.pir_motion;

                document.getElementById('foto_v').innerText = data.foto_reading.toFixed(2);
                document.getElementById('foto_s').innerText = data.foto_state;

                document.getElementById('mq_adc').innerText = data.mq_adc;
                document.getElementById('mq_state').innerText = data.mq_state;
            });
        }, 500);
        </script>
    </body>
    </html>
    )rawliteral";

    server.send(200, "text/html", html);
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
    json += "\"mq_state\":\"" + mqSensor.getPollutionState() + "\"";

    json += "}";

    server.send(200, "application/json", json);
}