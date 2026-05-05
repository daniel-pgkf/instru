#pragma once
#include <WebServer.h>
#include "TiltSensor.h"
#include "PIRSensor.h"
#include "FotoSensor.h"
#include "MQSensor.h"
#include "RFReceiver.h"

class WebServerManager {
public:
    WebServerManager(TiltSensor& tilt, PIRSensor& pir, FotoSensor& foto, MQSensor& mq, RFReceiver& rf, bool useCoolUI = true);

    void begin();
    void handleClient();

private:
    WebServer server;
    TiltSensor& tiltSensor;
    PIRSensor& pirSensor;
    FotoSensor& fotoSensor;
    MQSensor& mqSensor;
    RFReceiver& rfReceiver;
    bool _useCoolUI;

    void handleRoot();
    void handleData();
};