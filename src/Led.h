#pragma once
#include <Arduino.h>
#include <FastLED.h>

#define LED_PIN 2
#define LED_MATRIX_SIZE 16
#define LED_MAXBRIGHTNESS 100

class Led
{
private:
    void (*_current_tick)();
    CRGB _leds[LED_MATRIX_SIZE * LED_MATRIX_SIZE];
public:
    void setup();
    void tick();
    int getSize();
    void setEffect(void (*effect)());
    void setPixelColor(int i, CRGB crgb);
    void show();
    CRGB* getLeds();
};

extern Led Strip;
