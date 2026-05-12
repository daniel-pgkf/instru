#pragma once

#define WIFI_SSID "Equipo9"
#define WIFI_PASS "123456789"

//PINES
#define PIR_PIN 22
#define TILT_PIN 26 //14
#define FOTO_PIN_A 34 //ADC
#define FOTO_PIN_B 35 //ADC
#define MQ_PIN 32 //ADC
#define BUZZER_PIN 27 //PWM

 
// pir

// tilt
#define TILT_FILTER_WINDOW 50
#define TILT_FILTER_THRESHOLD_PERCENT 50

#define EVENT_WINDOW_MS 10000
#define EVENT_THRESHOLD 4

#define ALERT_HOLD_MS 3000

// fotoresistencia
#define ADC_MAX 4095.0
#define ADC_VREF 3.3

#define FOTO_OSCURIDAD_MAX 0.3
#define FOTO_TENUE_MAX 1.2
#define FOTO_MEDIA_MAX 3.0

// MQ
#define MQ_LOW_MAX 200
#define MQ_MEDIUM_MAX 400
#define MQ_HIGH_MIN 600

// Buzzer
#define BUZZER_CHANNEL 0
#define BUZZER_RESOLUTION 8
#define BUZZER_DUTY 128
