#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

#include <ledmatrix.h>
#include <wifi.h>



#define HTTP_SERVER_PORT 80

#define JSON_TYPE "application/json"
#define HTML_TYPE "text/html"


ESP8266WebServer apiServer(HTTP_SERVER_PORT);


void handleRoot()
{
    String html = "<!doctype html> <html lang=\"ru\"> <head> <meta charset=\"UTF-8\"> <meta name=\"viewport\" content=\"width=device-width, user-scalable=no, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0\"> <meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\"> <title>Document</title> <style> #networks > div { padding: 10px; } .loader { width: 48px; height: 48px; border: 5px solid #FFF; border-bottom-color: indigo; border-radius: 50%; display: inline-block; box-sizing: border-box; animation: rotation 1s linear infinite; } @keyframes rotation { 0% { transform: rotate(0deg); } 100% { transform: rotate(360deg); } } </style> </head> <body style=\"font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', 'Roboto', 'Oxygen', 'Ubuntu', 'Cantarell', 'Fira Sans', 'Droid Sans', 'Helvetica Neue', sans-serif; background: radial-gradient(circle, rgba(255,255,255,1) 52%, rgba(177,174,231,1) 100%, rgba(94,0,255,1) 100%);\"> <form method=\"get\" action=\"/connect\" style=\"display: flex; align-items: center; justify-content: center\"> <div style=\"display: flex; flex-direction: column; justify-content: center; min-height: 100vh; max-width: 600px\"> <h2 style=\"text-align: center; color: black\"> Подключение к сети </h2> <fieldset style=\"max-height: 400px; border-radius: 10px;margin: 10px\"> <legend>WiFi Сети</legend> <div id=\"networks\"> <div id=\"networks-load\" style=\"text-align: center\"> <span class=\"loader\"></span> </div> </div> </fieldset> <fieldset style=\"border-radius: 10px; margin: 10px\"> <legend>Безопасность</legend> <div style=\"padding: 10px;\"> <label for=\"pass\">Пароль</label> <input type=\"password\" name=\"pass\" id=\"pass\" required> </div> </fieldset> <div style=\"text-align: center; margin: 10px\"> <input id=\"submit-btn\" disabled type=\"submit\" value=\"Готово\" style=\"padding: 10px; background-color: indigo; border: none; color: white; border-radius: 10px\"> </div> </div> </form> <script> async function readNetworks() { let response = await fetch('/scan'); let networks = await response.json(); let networksBlock = document.getElementById('networks'); for (let network of networks) { let netBlock = document.createElement('div'); let radio = document.createElement('input'); radio.id = 'radio-' + networks.indexOf(network); radio.name = 'ssid'; radio.value = network.ssid; radio.type = 'radio'; let label = document.createElement('label'); label.innerText = network.ssid; label.htmlFor = radio.id; netBlock.append(radio, label); networksBlock.append(netBlock); } let submitBtn = document.getElementById('submit-btn'); submitBtn.disabled = false; let networkLoading = document.getElementById('networks-load'); networkLoading.style.display = 'none'; } readNetworks(); </script></body></html>";
    apiServer.send(200, HTML_TYPE, html);
}


void handleScan()
{
  char responseBody[2048];
  int n = WiFi.scanNetworks();
  StaticJsonDocument<300> JSONencoder;
  for (int i = 0; i < n && i < 10; i++)
  {
    JsonObject jsonObj = JSONencoder.createNestedObject();
    jsonObj["ssid"] = WiFi.SSID(i);
    jsonObj["rssi"] = WiFi.RSSI(i);
  }
  serializeJson(JSONencoder, responseBody);
  apiServer.send(200, "application/json", responseBody);
}

void handleConnect()
{
  fullColor(CRGB::AliceBlue);
  tickLed();

  WifiCred newCred;
  apiServer.arg("ssid").toCharArray(newCred.ssid, 16);
  apiServer.arg("pass").toCharArray(newCred.pass, 16);
  newCred.mode = WIFI_CLIENT_MODE;

  if (!String(newCred.ssid).equals(String(wifiCredential.ssid)))
  {
    changeWifiCredential(newCred);
  }

  apiServer.sendHeader("Location", "/check", "true");
  apiServer.send(200, "text/plane", "");

  fullColor(CRGB::Black);
  tickLed();
}

void handleCheck()
{
  apiServer.send(200, "text/html", "<h1>Ok!</h1>");
}

void initRoutes()
{
  apiServer.on("/", HTTP_GET, handleRoot);
  apiServer.on("/scan", HTTP_GET, handleScan);
  apiServer.on("/connect", HTTP_GET, handleConnect);
  apiServer.on("/check", HTTP_GET, handleCheck);
}

void startApi()
{
  apiServer.begin();
}

void tickApi()
{
  apiServer.handleClient();
}