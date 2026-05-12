#include "Buzzer.h"

Buzzer::Buzzer(int pin) : _pin(pin) {}

void Buzzer::begin() {
    pinMode(_pin, OUTPUT);
    ledcAttach(_pin, 2000, BUZZER_RESOLUTION);
    ledcWrite(_pin, 0);
}

void Buzzer::playAlert() {
    unsigned long now = millis();
    if (now < _endTime) {
        _endTime += 20000;
    } else {
        _endTime = now + 20000;
        _playing = true;
        _step = 0;
        _stepStart = now;
        playStep();
    }
}

void Buzzer::addTime(unsigned long t) {
    unsigned long now = millis();
    if (now < _endTime) {
        _endTime += t;
    } else {
        _endTime = now + t;
        _playing = true;
        _step = 0;
        _stepStart = now;
        playStep();
    }
}

void Buzzer::stop() {
    _playing = false;
    _endTime = 0;
    ledcWrite(_pin, 0); 
}

void Buzzer::playStep() {
    int freq = _freqs[_step];
    if (freq == 0) {
        ledcWrite(_pin, 0);
    } else {
        ledcAttach(_pin, freq, BUZZER_RESOLUTION);
        ledcWrite(_pin, BUZZER_DUTY);
    }
}

void Buzzer::update() {
    if (!_playing) return;

    unsigned long now = millis();

    if (now >= _endTime) {
        stop();
        return;
    }

    if (now - _stepStart >= _durations[_step]) {
        _step = (_step + 1) % MELODY_LENGTH;
        _stepStart = now;
        playStep();
    }
}
