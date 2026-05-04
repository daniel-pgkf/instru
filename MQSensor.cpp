#include "MQSensor.h"

MQSensor::MQSensor(int pin) : _pin(pin) {}

void MQSensor::begin() {
    pinMode(_pin, INPUT);
    analogReadResolution(12);
}

void MQSensor::update() {
    _adcValue = analogRead(_pin);
    classifyPollution();
}

void MQSensor::classifyPollution() {
    if (_adcValue < MQ_LOW_MAX) {
        _pollutionState = "Nivel bajo";
    } else if (_adcValue >= MQ_LOW_MAX && _adcValue < MQ_MEDIUM_MAX) {
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