#include "MQSensor.h"

MQSensor::MQSensor(int pin) : _pin(pin) {}

void MQSensor::begin() {
    pinMode(_pin, INPUT);
    analogReadResolution(12);
}

void MQSensor::update() {
    if (millis() - _tiempoAnterior >= _intervaloLectura) {
        int raw = analogRead(_pin);

        _samples[_sampleIndex] = raw;
        _sampleIndex++;

        if (_sampleIndex >= MQ_FILTER_WINDOW) {
            _sampleIndex = 0;
            _bufferFilled = true;
        }

        if (_bufferFilled || _sampleIndex > 0) {
            _adcValue = computeMedian();
            classifyPollution();
        }

        _tiempoAnterior = millis();
    }
}

int MQSensor::computeMedian() {
    int size = _bufferFilled ? MQ_FILTER_WINDOW : _sampleIndex;

    int temp[MQ_FILTER_WINDOW];
    for (int i = 0; i < size; i++) temp[i] = _samples[i];

    // Insertion sort (eficiente para N pequeño)
    for (int i = 1; i < size; i++) {
        int key = temp[i];
        int j = i - 1;
        while (j >= 0 && temp[j] > key) {
            temp[j + 1] = temp[j];
            j--;
        }
        temp[j + 1] = key;
    }

    return temp[size / 2];
}

void MQSensor::classifyPollution() {
    if (_adcValue < MQ_LOW_MAX) {
        _pollutionState = "Nivel bajo";
    } else if (_adcValue < MQ_MEDIUM_MAX) {
        _pollutionState = "Nivel medio";
    } else if (_adcValue >= MQ_HIGH_MIN) {
        _pollutionState = "Nivel alto";
    } else {
        _pollutionState = "Nivel medio-alto";
    }
}

int MQSensor::getADC() {
    return _adcValue;
}

String MQSensor::getPollutionState() {
    return _pollutionState;
}