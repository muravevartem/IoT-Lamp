#include <ESP8266mDNS.h>

#define PRINT(x) Serial.print(x)
#define LN() Serial.println()

void initDns()
{
    if (MDNS.begin("lampochka"))
    {
        PRINT("DNS started, available with: ");
        PRINT("http://lampochka.local/");
        LN();
    }
}

void tickDNS()
{
    MDNS.update();
}