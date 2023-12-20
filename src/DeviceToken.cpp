#include "DeviceToken.h"
#include "EEPROM.h"
#include "EEPROMAddress.h"
#include "Utitlity.h"

void DeviceToken::_init()
{
    EEPROM.get(EEPROM_TOKEN, _status);
    EEPROM.get(EEPROM_TOKEN + sizeof(_status), _token);
    _initialized = true;

    Debug("Device token: status=");
    Debug(_status);
    Debug("; token=");
    DebugLn(_token);
}

DeviceToken::DeviceToken()
{
    
}

void DeviceToken::commit()
{
    EEPROM.put(EEPROM_TOKEN, _status);
    EEPROM.put(EEPROM_TOKEN + sizeof(_status), _token);
    EEPROM.commit();
}

void DeviceToken::reset()
{
    _status = DT_UNREGISTED;
    DeviceToken::commit();
}

bool DeviceToken::isRegisted()
{
    if (!_initialized)
    {
        DeviceToken::_init();
    }
    return _status == DT_REGISTED;
}

void DeviceToken::regist(String* tokenStr)
{
    (*tokenStr).toCharArray(_token, 37);
    _status = DT_REGISTED;
    DeviceToken::commit();
    Debug("Device registed ");  
    DebugLn(_token);
}

const char *DeviceToken::getToken()
{
    return _token;
}

DeviceToken _device_token = DeviceToken();