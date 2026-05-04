#pragma once

#include <Arduino.h>
#include "config.h"

class Buzzer {
public:
    Buzzer(int pin);

    void begin();
    void playAlert();
    void update();

private:
    int _pin;

    bool _playing = false;
    int _step = 0;
    unsigned long _stepStart = 0;

    static const int MELODY_LENGTH = 5;

    int _freqs[MELODY_LENGTH] = {2500, 0, 2500, 0, 3200};
    int _durations[MELODY_LENGTH] = {120, 80, 120, 80, 500};

    void playStep();
    void stop();
};