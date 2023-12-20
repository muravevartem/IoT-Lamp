#pragma once
#include <Arduino.h>

#define DT_UNREGISTED 0x0
#define DT_REGISTED 0x1


class DeviceToken
{
private:
    char _token[37];
    uint8_t _status;
    bool _initialized = false;
    void _init();
public:
    DeviceToken();
    void commit();
    void reset();
    bool isRegisted();
    void regist(String* tokenStr);
    const char* getToken();
};
