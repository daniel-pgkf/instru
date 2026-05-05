#pragma once

#include <Arduino.h>
#include "config.h"

#define MQ_FILTER_WINDOW 11  // impar para mediana limpia

class MQSensor {
public:
    MQSensor(int pin);

    void begin();
    void update();

    int getADC();
    String getPollutionState();

private:
    int _pin;

    int _samples[MQ_FILTER_WINDOW];
    int _sampleIndex = 0;
    bool _bufferFilled = false;

    int _adcValue = 0;
    String _pollutionState = "Nivel bajo";
    unsigned long _tiempoAnterior = 0;
    const unsigned long _intervaloLectura = 100;

    int computeMedian();
    void classifyPollution();
};