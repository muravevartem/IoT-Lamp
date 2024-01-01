#include "Led.h"
#include "FastLED.h"
#include "Utitlity.h"
#include "Timer.h"
#include "LedEffects.h"
#include "GyverButton.h"

extern GButton mainBtn;

const int led_num = LED_MATRIX_SIZE * LED_MATRIX_SIZE;

void Led::setup()
{
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(_leds, led_num);
    FastLED.setBrightness(LED_MAXBRIGHTNESS);
}

void Led::tick()
{
    static uint8_t currentEffect = 0;

    if (mainBtn.isClick())
    {
        currentEffect++;
        for_i(led_num)
        {
            _leds[i] = CRGB::Black;
        }
        FastLED.show();
    }

    switch (currentEffect)
    {
    case 0:
        wait();
        break;
    default:
        currentEffect = 0;
        tick();
        break;
    }
}

int Led::getSize()
{
    return led_num;
}

void Led::show()
{
    FastLED.show();
}

CRGB* Led::getLeds()
{
    return _leds;
}

Led Strip = Led();