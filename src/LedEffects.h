#pragma once
#include <Arduino.h>
#include <FastLED.h>
#include <Timer.h>
#include <Led.h>
#include <Utitlity.h>

Timer timerForWaitEffect(50);

void wait()
{
    if (timerForWaitEffect.click())
    {
        int i = random(Strip.getSize());
        bool isBlack = random(5);
        CRGB* leds = Strip.getLeds();
        leds[i] = isBlack ? CRGB::Black : CRGB::Blue;
        Strip.show();
    }
}