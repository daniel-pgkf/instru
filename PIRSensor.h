#pragma once
#include <Arduino.h>

class PIRSensor {
public:
    PIRSensor(int pin);

    void begin();
    void update();

    bool isMotionDetected();

private:
    int _pin;
    bool _motionDetected = false;
};