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
    case 1:
        wait_green();
        break;
    case 2:
        wait_indigo();
        break;
    case 3:
        wait_fire();
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

void Led::setEffect(void (*effect)())
{
    _current_tick = effect;
}

void Led::setPixelColor(int i, CRGB crgb)
{
    _leds[i] = crgb;
}

void Led::show()
{
    FastLED.show();
}

Led Strip = Led();