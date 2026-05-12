#include "PIRSensor.h"

PIRSensor::PIRSensor(int pin) : _pin(pin) {}

void PIRSensor::begin() {
    pinMode(_pin, INPUT);
}

void PIRSensor::update() {
    _motionDetected = digitalRead(_pin);
}

bool PIRSensor::isMotionDetected() {
    return _motionDetected;
}