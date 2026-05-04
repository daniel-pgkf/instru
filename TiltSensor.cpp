#include "TiltSensor.h"

TiltSensor::TiltSensor(int pin) : _pin(pin) {}

void TiltSensor::begin() {
    pinMode(_pin, INPUT);

    _sampleIndex = 0;
    _bufferFilled = false;

    _filteredState = 0;
    _prevFilteredState = 0;

    _tiltDetected = false;
    _alert = false;
    _alertStart = 0;

    _count = 0;
    _windowStart = 0;
}

void TiltSensor::resetSequence() {
    _sampleIndex = 0;
    _bufferFilled = false;

    _filteredState = 0;
    _prevFilteredState = 0;

    _tiltDetected = false;

    _count = 0;
    _windowStart = 0;
}

int TiltSensor::computeFilteredState() {
    int size = _bufferFilled ? TILT_FILTER_WINDOW : _sampleIndex;

    if (size < TILT_FILTER_WINDOW) {
        return _filteredState;
    }

    int ones = 0;

    for (int i = 0; i < size; i++) {
        if (_samples[i] == 1) ones++;
    }

    int percentOnes = (ones * 100) / size;
    int percentZeros = 100 - percentOnes;

    if (percentOnes >= TILT_FILTER_THRESHOLD_PERCENT) {
        return 1;
    }

    if (percentZeros >= TILT_FILTER_THRESHOLD_PERCENT) {
        return 0;
    }

    return -1;
}

void TiltSensor::update() {
    int raw = digitalRead(_pin);

    _tiltDetected = false;

    if (_alert && millis() - _alertStart > ALERT_HOLD_MS) {
        _alert = false;
    }

    _samples[_sampleIndex] = raw;
    _sampleIndex++;

    if (_sampleIndex >= TILT_FILTER_WINDOW) {
        _sampleIndex = 0;
        _bufferFilled = true;
    }

    int newState = computeFilteredState();

    if (newState == -1) {
        resetSequence();
        return;
    }

    _prevFilteredState = _filteredState;
    _filteredState = newState;

    if (_prevFilteredState == 1 && _filteredState == 0) {
        if (_count == 0) {
            _windowStart = millis();
        }

        _count++;
        _tiltDetected = true;

        if (_count >= EVENT_THRESHOLD) {
            _alert = true;
            _alertStart = millis();

            _count = 0;
            _windowStart = 0;
        }
    }

    if (_count > 0 && millis() - _windowStart > EVENT_WINDOW_MS) {
        resetSequence();
        return;
    }
}

bool TiltSensor::isTiltDetected() {
    return _tiltDetected;
}

bool TiltSensor::isAlertActive() {
    return _alert;
}

int TiltSensor::getCount() {
    return _count;
}