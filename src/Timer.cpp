#include <Timer.h>

Timer::Timer(int de)
{
    _clk = millis();
    _delay = de;
}

bool Timer::click()
{
    return (millis() - _clk > _delay);
}

void Timer::reset()
{
    _clk = millis();
}
