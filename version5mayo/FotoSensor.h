#pragma once

#include <Arduino.h>
#include "config.h"

class FotoSensor {
public:
    FotoSensor(int pinA, int pinB);

    void begin();
    void update();

    float getVoltageA();
    float getVoltageB();
    float getCellReading();
    String getLightState();

private:
    int _pinA;
    int _pinB;

    float _voltageA = 0.0;
    float _voltageB = 0.0;
    float _cellReading = 0.0;

    String _lightState = "Oscuridad";

    float adcToVoltage(int adc);
    void classifyLight();
};