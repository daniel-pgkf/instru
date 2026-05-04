#include <WiFi.h>
#include "config.h"
#include "TiltSensor.h"
#include "PIRSensor.h"
#include "FotoSensor.h"
#include "WebServerManager.h"
#include "MQSensor.h"
#include "Buzzer.h"

Buzzer buzzer(BUZZER_PIN);
MQSensor mq(MQ_PIN);
TiltSensor tilt(TILT_PIN);
PIRSensor pir(PIR_PIN);
FotoSensor foto(FOTO_PIN_A, FOTO_PIN_B);

WebServerManager web(tilt, pir, foto, mq);

bool prevTiltAlert = false;

void setup() {
    Serial.begin(115200);

    WiFi.softAP(WIFI_SSID, WIFI_PASS);

    tilt.begin();
    pir.begin();
    foto.begin();
    mq.begin();
    
    buzzer.begin();
    
    web.begin();
}

void loop() {
    pir.update();
    foto.update();
    mq.update();
    tilt.update();

    bool currentTiltAlert = tilt.isAlertActive();

    if (currentTiltAlert && !prevTiltAlert) {
        buzzer.playAlert();
    }

    prevTiltAlert = currentTiltAlert;

buzzer.update();

    web.handleClient();
}