#include "FotoSensor.h"

FotoSensor::FotoSensor(int pinA, int pinB)
    : _pinA(pinA), _pinB(pinB) {}

void FotoSensor::begin() {
    pinMode(_pinA, INPUT);
    pinMode(_pinB, INPUT);

    analogReadResolution(12);
}

float FotoSensor::adcToVoltage(int adc) {
    return (adc * ADC_VREF) / ADC_MAX;
}

void FotoSensor::update() {
    int rawA = analogRead(_pinA);
    int rawB = analogRead(_pinB);

    _voltageA = adcToVoltage(rawA);
    _voltageB = adcToVoltage(rawB);

    _cellReading = _voltageA - _voltageB;

    if (_cellReading < 0) {
        _cellReading = 0;
    }

    classifyLight();
}

void FotoSensor::classifyLight() {
    if (_cellReading <= FOTO_OSCURIDAD_MAX) {
        _lightState = "Oscuridad";
    } else if (_cellReading <= FOTO_TENUE_MAX) {
        _lightState = "Luz tenue";
    } else if (_cellReading <= FOTO_MEDIA_MAX) {
        _lightState = "Intensidad media";
    } else {
        _lightState = "Intensidad alta";
    }
}

float FotoSensor::getVoltageA() {
    return _voltageA;
}

float FotoSensor::getVoltageB() {
    return _voltageB;
}

float FotoSensor::getCellReading() {
    return _cellReading;
}

String FotoSensor::getLightState() {
    return _lightState;
}