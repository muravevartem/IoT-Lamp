#include <Timer.h>

Timer::Timer(int de)
{
    _clk = millis();
    _delay = de;
}

bool Timer::click()
{
    bool isClicked = (millis() - _clk > _delay);
    if (isClicked)
    {
        _clk = millis();
    }
    return isClicked;
}

void Timer::reset()
{
    _clk = millis();
}
