#pragma once

#include <WebServer.h>
#include "TiltSensor.h"
#include "PIRSensor.h"
#include "FotoSensor.h"
#include "MQSensor.h"

class WebServerManager {
public:
    WebServerManager(TiltSensor& tilt, PIRSensor& pir, FotoSensor& foto, MQSensor& mq);
    void begin();
    void handleClient();

private:
    WebServer server;
    TiltSensor& tiltSensor;
    PIRSensor& pirSensor;
    FotoSensor& fotoSensor;
    MQSensor& mqSensor;
    
    void handleRoot();
    void handleData();
};