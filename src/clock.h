#pragma once
#include<Arduino.h>
#include<time.h>

tm* now()
{
    time_t myTime = time(NULL);
    return localtime(&myTime);
}