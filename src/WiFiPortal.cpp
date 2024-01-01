#include <WiFiPortal.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include <WiFiTool.h>


ESP8266WebServer _portalserver;

// const char *root = "<!doctype html> <html lang=\"ru\"> <head> <meta charset=\"UTF-8\"> <meta name=\"viewport\" content=\"width=device-width, user-scalable=no, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0\"> <meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\"> <title>Document</title> <style> #networks > div { padding: 10px; } .loader { width: 48px; height: 48px; border: 5px solid #FFF; border-bottom-color: indigo; border-radius: 50%; display: inline-block; box-sizing: border-box; animation: rotation 1s linear infinite; } @keyframes rotation { 0% { transform: rotate(0deg); } 100% { transform: rotate(360deg); } } </style> </head> <body style=\"font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', 'Roboto', 'Oxygen', 'Ubuntu', 'Cantarell', 'Fira Sans', 'Droid Sans', 'Helvetica Neue', sans-serif; background: radial-gradient(circle, rgba(255,255,255,1) 52%, rgba(177,174,231,1) 100%, rgba(94,0,255,1) 100%);\"> <form method=\"get\" action=\"/connect\" style=\"display: flex; align-items: center; justify-content: center\"> <div style=\"display: flex; flex-direction: column; justify-content: center; min-height: 100vh; max-width: 600px\"> <h2 style=\"text-align: center; color: black\"> Подключение к сети </h2> <fieldset style=\"max-height: 400px; border-radius: 10px;margin: 10px\"> <legend>WiFi Сети</legend> <div id=\"networks\"> <div id=\"networks-load\" style=\"text-align: center\"> <span class=\"loader\"></span> </div> </div> </fieldset> <fieldset style=\"border-radius: 10px; margin: 10px\"> <legend>Безопасность</legend> <div style=\"padding: 10px;\"> <label for=\"pass\">Пароль</label> <input type=\"password\" name=\"pass\" id=\"pass\" required> </div> </fieldset> <div style=\"text-align: center; margin: 10px\"> <input id=\"submit-btn\" disabled type=\"submit\" value=\"Готово\" style=\"padding: 10px; background-color: indigo; border: none; color: white; border-radius: 10px\"> </div> </div> </form> <script> async function readNetworks() { let response = await fetch('/scan'); let networks = await response.json(); let networksBlock = document.getElementById('networks'); for (let network of networks) { let netBlock = document.createElement('div'); let radio = document.createElement('input'); radio.id = 'radio-' + networks.indexOf(network); radio.name = 'ssid'; radio.value = network.ssid; radio.type = 'radio'; let label = document.createElement('label'); label.innerText = network.ssid; label.htmlFor = radio.id; netBlock.append(radio, label); networksBlock.append(netBlock); } let submitBtn = document.getElementById('submit-btn'); submitBtn.disabled = false; let networkLoading = document.getElementById('networks-load'); networkLoading.style.display = 'none'; } readNetworks(); </script></body></html>";
const char *root = "<!DOCTYPE html><html lang=\"ru\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>IoT-Lamp</title><style>* {font-family: system-ui, -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, 'Open Sans', 'Helvetica Neue', sans-serif;}.form {display: flex;flex-direction: column;min-height: 100vh;width: 100%;justify-content: center;align-items: center;}.form_network {width: 100%;max-width: 300px;border-radius: 15px;margin-bottom: 15px;}.form_radio_btn {margin: 5px;}.form_radio_btn input[type=radio] {display: none;}.form_radio_btn label {display: block;cursor: pointer;padding: 0px 15px;line-height: 34px;border: 1px solid #999;border-radius: 6px;user-select: none;}.form_radio_btn input[type=radio]:checked+label {background-color: lavender;}.form_radio_btn label:hover {background-color: #999;}.form_network_pass>input {margin: 0;padding: 0;}.form_network_pass>input {display: block;border: none;font-size: 1.3em;width: 100%;}.form_submit_btn>input[type=submit] {font-weight: 600;color: #222;font-size: 1em;border: none;padding: 15px;border-radius: 15px;}.form_submit_btn>input:hover {background-color: lavender;}.error {background-color: palevioletred;color: white;font-weight: 500;padding: 5px 20px;border-radius: 15px;}</style></head><body><div class=\"background\"><form action=\"/connect\" method=\"get\" class=\"form\" id=\"form-network\"><div class=\"error\" id=\"error\" style=\"display: none;\">Возникла ошибка</div><h1>Подключение к сети</h1><fieldset class=\"form_network\" id=\"actual-networks\"><legend><b>Доступные сети</b></legend><!-- <div class=\"form_radio_btn\"><input type=\"radio\" id=\"ssid-Network\" name=\"ssid\" value=\"Network\" /><label for=\"ssid-Network\">Network</label></div> --></fieldset><fieldset class=\"form_network form_network_pass\"><legend><b>Пароль</b></legend><input required type=\"password\" name=\"pass\" placeholder=\"******\" /></fieldset><div class=\"form_submit_btn\"><input type=\"submit\" value=\"Подключиться\" /></div></form></div><script>async function scanNetworks() {const response = await fetch(`/scan`);const networks = await response.json();let netContainer = document.getElementById('actual-networks');for (let network of networks) {let divBtn = document.createElement('div');divBtn.className = 'form_radio_btn';let radio = document.createElement('input');radio.type = 'radio';radio.id = 'ssid_' + networks.indexOf(network);radio.name = 'ssid';radio.value = network.ssid;let label = document.createElement('label');label.htmlFor = radio.id;label.innerText = network.ssid;divBtn.append(radio, label);netContainer.append(divBtn);}}scanNetworks().catch(() => {let error = document.getElementById('error');error.style.display = 'block';});let form = document.getElementById('form-network');form.addEventListener('submit', (ev) => {ev.preventDefault();fetch('/connect', {method: 'POST',body: new FormData(form)}).finally(() => document.location.href = '/check')})</script></body></html>";
const char *check = "<!DOCTYPE html><html lang=\"ru\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>IoT-Lamp</title><style>* {font-family: system-ui, -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, 'Open Sans', 'Helvetica Neue', sans-serif;}.success {display: flex;flex-direction: column;justify-content: center;align-items: center;min-height: 100vh;font-size: large;font-weight: 700;color: green;}</style></head><body><div class=\"success\"><p>Подключено :)</p></div></body></html>";

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

   WIFITool.connect(new_ssid.c_str(), new_pass.c_str());

    _portalserver.sendHeader("Location", "/check", "true");
    _portalserver.send(200, "text/plane", "");
}

void _handle_check()
{
    _portalserver.send(200, "text/html", check);
}

void WiFiPortal::tick()
{
    _portalserver.handleClient();
}

void WiFiPortal::start()
{
    _portalserver.on("/", HTTP_GET, _handle_root);
    _portalserver.on("/scan", HTTP_GET, _handle_scan);
    _portalserver.on("/connect", HTTP_POST, _handle_connect);
    _portalserver.on("/check", HTTP_GET, _handle_check);
    _portalserver.begin();
}
