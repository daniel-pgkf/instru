#pragma once

#include <Arduino.h>
#include "config.h"

class TiltSensor {
public:
    TiltSensor(int pin);

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
};