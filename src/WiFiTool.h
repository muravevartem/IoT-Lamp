#pragma once
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Timer.h>

// AP settings
#define DEFAULT_WIFI_AP_SSID "Lampo4ka"
#ifdef DEFALT_WIFI_AP_SECURED
#define DEFAULT_WIFI_AP_PASS "lampo4ka"
#endif

//STA settings
#define DEFAULT_WIFI_CONNECTION_ATTEMPS 20
#define DEFAULT_WIFI_CONNECTION_TIMEOUT 1000

class WiFiTool
{
private:
    char _ssid[32]; // Наименование сети
    char _pass[32]; // Пароль от сети
    bool _is_sta = false; // Использовать имеющуюся сеть?
    void (*_unconnected_handler)(); // Обработчик отключения
    void (*_connecting_handler)(); // Обработчик начала подключения
    void (*_connected_handler)(); // Обработчик подключения
    void (*_error_connection_handler)(); // Обработчик ошибки подключения
    bool _wait_connection = false;
    bool _interrupted = false;
    uint8_t _connection_attemps = 0;
    Timer _connection_timer = Timer(DEFAULT_WIFI_CONNECTION_TIMEOUT);
public:
    WiFiTool();
    
    const char* getSsid();
    const char* getPassword();
    bool isSta();
    
    bool staConnected();
    
    void initByEEPROM();
    void connect(const char* ssid, const char* pass); // Подключиться к имеющейся точке
    void wap(); // создать точку доступа
    void commit(); // Сохранить в EEPROM
    
    void tick(); // Обработка статуса подключения

    void unconnectedHandler(void (*handler)()); // указать обработчик отключения
    void connectingHandler(void (*handler)()); // указать обработчик подключения
    void connectedHandler(void (*handler)()); // указать обработчик подключения
    void errorConnectionHandler(void (*handler)()); // указать обработчик ошибки подключения
};

extern WiFiTool WIFITool;

extern WiFiClient WIFIClient;