#include <FastLED.h>

#define LED_PIN 2
#define LED_HEIGHT 16
#define LED_WIDTH 16
#define LED_FULL_SIZE LED_HEIGHT *LED_WIDTH
#define LED_BRIGHTNESS 50

#define for_i(n) for (int i = 0; i < n; i++)


CRGB leds[LED_FULL_SIZE];

void initMatrix()
{
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_FULL_SIZE)
        .setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(LED_BRIGHTNESS);
}

void fullColor(CRGB rgb)
{
    for_i(LED_FULL_SIZE)
    {
        leds[i] = rgb;
    }
}

void tickLed()
{
    FastLED.show();
}