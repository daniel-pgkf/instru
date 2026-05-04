#pragma once

#include <Arduino.h>
#include "config.h"

class MQSensor {
public:
    MQSensor(int pin);

    void begin();
    void update();

    int getADC();
    String getPollutionState();

private:
    int _pin;

    int _adcValue = 0;
    String _pollutionState = "Nivel bajo";

    void classifyPollution();
};