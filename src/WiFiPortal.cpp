#include <WiFiPortal.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include <WiFiConnect.h>
#include <WiFiConfig.h>

ESP8266WebServer _portalserver;

const char *root = "<!doctype html> <html lang=\"ru\"> <head> <meta charset=\"UTF-8\"> <meta name=\"viewport\" content=\"width=device-width, user-scalable=no, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0\"> <meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\"> <title>Document</title> <style> #networks > div { padding: 10px; } .loader { width: 48px; height: 48px; border: 5px solid #FFF; border-bottom-color: indigo; border-radius: 50%; display: inline-block; box-sizing: border-box; animation: rotation 1s linear infinite; } @keyframes rotation { 0% { transform: rotate(0deg); } 100% { transform: rotate(360deg); } } </style> </head> <body style=\"font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', 'Roboto', 'Oxygen', 'Ubuntu', 'Cantarell', 'Fira Sans', 'Droid Sans', 'Helvetica Neue', sans-serif; background: radial-gradient(circle, rgba(255,255,255,1) 52%, rgba(177,174,231,1) 100%, rgba(94,0,255,1) 100%);\"> <form method=\"get\" action=\"/connect\" style=\"display: flex; align-items: center; justify-content: center\"> <div style=\"display: flex; flex-direction: column; justify-content: center; min-height: 100vh; max-width: 600px\"> <h2 style=\"text-align: center; color: black\"> Подключение к сети </h2> <fieldset style=\"max-height: 400px; border-radius: 10px;margin: 10px\"> <legend>WiFi Сети</legend> <div id=\"networks\"> <div id=\"networks-load\" style=\"text-align: center\"> <span class=\"loader\"></span> </div> </div> </fieldset> <fieldset style=\"border-radius: 10px; margin: 10px\"> <legend>Безопасность</legend> <div style=\"padding: 10px;\"> <label for=\"pass\">Пароль</label> <input type=\"password\" name=\"pass\" id=\"pass\" required> </div> </fieldset> <div style=\"text-align: center; margin: 10px\"> <input id=\"submit-btn\" disabled type=\"submit\" value=\"Готово\" style=\"padding: 10px; background-color: indigo; border: none; color: white; border-radius: 10px\"> </div> </div> </form> <script> async function readNetworks() { let response = await fetch('/scan'); let networks = await response.json(); let networksBlock = document.getElementById('networks'); for (let network of networks) { let netBlock = document.createElement('div'); let radio = document.createElement('input'); radio.id = 'radio-' + networks.indexOf(network); radio.name = 'ssid'; radio.value = network.ssid; radio.type = 'radio'; let label = document.createElement('label'); label.innerText = network.ssid; label.htmlFor = radio.id; netBlock.append(radio, label); networksBlock.append(netBlock); } let submitBtn = document.getElementById('submit-btn'); submitBtn.disabled = false; let networkLoading = document.getElementById('networks-load'); networkLoading.style.display = 'none'; } readNetworks(); </script></body></html>";

extern String _wifi_ssid;
extern String _wifi_pass;
extern uint8_t _wifi_mode;

/* Handlers */
void _handle_root()
{
    _portalserver.send(200, CT_TEXT_HTML, root);
}

void _handle_scan()
{
    int8_t n = WiFi.scanNetworks();
    StaticJsonDocument<1024> json;
    char responseBody[1024];
    for (int8_t i = 0; i < n; i++)
    {
        JsonObject obj = json.createNestedObject();
        obj["ssid"] = WiFi.SSID(i);
        obj["rssi"] = WiFi.RSSI(i);
    }
    serializeJson(json, responseBody);
    _portalserver.send(200, CT_APPLICATION_JSON, responseBody);
}

void _handle_connect()
{
    String new_ssid = _portalserver.arg("ssid");
    String new_pass = _portalserver.arg("pass");

    _wifi_ssid = new_ssid;
    _wifi_pass = new_pass;
    _wifi_mode = WIFI_STA_MODE;

    if (!WiFiConnect::connect())
    {
        // reset config
        WiFiConfig::init();
        WiFiConnect::connect();
    } else {
        WiFiConfig::commit();
    }

    _portalserver.sendHeader("Location", "/check", "true");
    _portalserver.send(200, "text/plane", "");
}

void WiFiPortal::tick()
{
    _portalserver.handleClient();
    MDNS.update();
}

void WiFiPortal::start()
{
    _portalserver.on("/", HTTP_GET, _handle_root);
    _portalserver.on("/scan", HTTP_GET, _handle_scan);
    _portalserver.on("/connect", HTTP_GET, _handle_connect);
    _portalserver.begin();
    MDNS.begin("lampochka.local");
}
