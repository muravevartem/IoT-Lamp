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
        Strip.setPixelColor(i, isBlack ? CRGB::Black : CRGB::Blue);
        Strip.show();
    }
}

void wait_green()
{
    if (timerForWaitEffect.click())
    {
        int i = random(Strip.getSize());
        bool isBlack = random(5);
        Strip.setPixelColor(i, isBlack ? CRGB::Black : CRGB::Green);
        Strip.show();
    }
}

void wait_fire()
{
    if (timerForWaitEffect.click())
    {
        int i = random(Strip.getSize());
        bool isBlack = random(5);
        Strip.setPixelColor(i, isBlack ? CRGB::Red : CRGB::Yellow);
        Strip.show();
    }
}

void wait_indigo()
{
    if (timerForWaitEffect.click())
    {
        int i = random(Strip.getSize());
        bool isBlack = random(5);
        Strip.setPixelColor(i, isBlack ? CRGB::Black : CRGB::Indigo);
        Strip.show();
    }
}