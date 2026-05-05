#include <WiFi.h>
#include "config.h"
#include "TiltSensor.h"
#include "PIRSensor.h"
#include "FotoSensor.h"
#include "WebServerManager.h"
#include "MQSensor.h"
#include "Buzzer.h"
#include <RH_ASK.h>
#include "RFReceiver.h"

RH_ASK rf_driver(2000, 18, 2, 0); 
Buzzer buzzer(BUZZER_PIN);
RFReceiver rfReceiver(&rf_driver, &buzzer);
MQSensor mq(MQ_PIN);
TiltSensor tilt(TILT_PIN, &rf_driver);
PIRSensor pir(PIR_PIN);
FotoSensor foto(FOTO_PIN_A, FOTO_PIN_B);
bool CoolUI = true;
WebServerManager web(tilt, pir, foto, mq, rfReceiver, CoolUI);

bool prevTiltAlert = false;

void setup() {
    Serial.begin(115200);
    WiFi.softAP(WIFI_SSID, WIFI_PASS);

    rf_driver.init();
    rfReceiver.begin();
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
    rfReceiver.update();
    tilt.update();

    bool currentTiltAlert = tilt.isAlertActive();

    if (currentTiltAlert && !prevTiltAlert) {
        buzzer.playAlert();
    }
    
    // Si la alerta termina, apagamos el buzzer si no hay RF sonando
    if (!currentTiltAlert && prevTiltAlert) {
        buzzer.stop(); // Opcional si quieres silencio inmediato
    }

    prevTiltAlert = currentTiltAlert;

    buzzer.update();
    web.handleClient();
}