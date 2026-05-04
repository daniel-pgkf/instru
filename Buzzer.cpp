#include "Buzzer.h"

Buzzer::Buzzer(int pin) : _pin(pin) {}

void Buzzer::begin() {
    ledcAttach(_pin, 2000, BUZZER_RESOLUTION); // freq inicial
    ledcWrite(_pin, 0); // apagado
}

void Buzzer::playAlert() {
    _playing = true;
    _step = 0;
    _stepStart = millis();
    playStep();
}

void Buzzer::playStep() {
    int freq = _freqs[_step];

    if (freq == 0) {
        ledcWrite(_pin, 0); // silencio
    } else {
        ledcAttach(_pin, freq, BUZZER_RESOLUTION); // cambiar frecuencia
        ledcWrite(_pin, BUZZER_DUTY);
    }
}

void Buzzer::update() {
    if (!_playing) return;

    if (millis() - _stepStart >= _durations[_step]) {
        _step++;

        if (_step >= MELODY_LENGTH) {
            stop();
            return;
        }

        _stepStart = millis();
        playStep();
    }
}

void Buzzer::stop() {
    _playing = false;
    _step = 0;
    ledcWrite(_pin, 0);
}