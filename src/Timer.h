#pragma once
#include <Arduino.h>

class Timer
{
private:
    unsigned long _clk = 0;
    unsigned int _delay = 0;

public:
    Timer(int delay);
    bool click();
    void reset();
};