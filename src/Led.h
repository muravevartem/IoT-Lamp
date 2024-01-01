#pragma once
#include <Arduino.h>
#include <FastLED.h>

#define LED_PIN 2
#define LED_MATRIX_SIZE 16
#define LED_MAXBRIGHTNESS 100

class Led
{
private:
    uint8_t _current_effect = 0;
    CRGB _leds[LED_MATRIX_SIZE * LED_MATRIX_SIZE];
public:
    void setup();
    void tick();
    int getSize();
    void show();
    CRGB* getLeds();
};

extern Led Strip;
