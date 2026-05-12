#pragma once

#include <Arduino.h>
#include "config.h"
#include <RH_ASK.h>

class TiltSensor {
public:
    TiltSensor(int pin, RH_ASK* driver);

    void begin();
    void update();

    bool isTiltDetected();
    bool isAlertActive();
    int getCount();

private:
    int _pin;

    int _samples[TILT_FILTER_WINDOW];
    int _sampleIndex = 0;
    bool _bufferFilled = false;

    int _filteredState = 0;
    int _prevFilteredState = 0;

    bool _tiltDetected = false;
    bool _alert = false;
    unsigned long _alertStart = 0;

    int _count = 0;
    unsigned long _windowStart = 0;

    int computeFilteredState();
    void resetSequence();

    RH_ASK* _rfDriver;
    bool _rfSent;
};