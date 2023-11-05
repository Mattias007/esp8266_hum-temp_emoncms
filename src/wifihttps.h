#include <Arduino.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <Arduino_JSON.h>
#include <ESP8266WiFi.h>

#include "EEROM.h"
#include "ACcontroll.h"
#include "sensordata.h"


HTTPClient http;
WiFiClient client;
ESP8266WiFiMulti WiFiMulti;
ESP8266WebServer server(80); 

IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);


const char *name = "ACTemp0";



String html = 
"<!DOCTYPE html>"
"<html lang=en'>"
"<head>"
  "<meta charset='UTF-8'>"
  "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
  "<title>Document</title>"
  "<style>"
    "*{" 
      "margin: 0;"
      "padding: 0;"
      "box-sizing: border-box;"
    "}"
    "body{" 
      "display: flex;"
      "flex-direction: column;"
      "align-items: center;"
      "justify-content: space-between;"
      "min-height: 100vh;"
      "min-width: 100vw;"
    "}"
    "nav{" 
      "background-color: aliceblue;"
      "padding-top: 10px;"
      "padding-bottom: 10px;"
      "width: 100vw;"
      "gap: 5px;"
      "display: flex;"
      "justify-content: space-around;"
      "flex-wrap: wrap;"
      "color: #003D8F;"
    "}"
    "nav a{" 
      "font-size: 24px;"
      "text-decoration: none;"
      "color: #003D8F;"
    "}"
    "div form{" 
      "display: grid;"
    "}"
    "footer{" 
      "background-color: aliceblue;"
      "color: #003D8F;"
      "width: 100vw;"
      "padding-top: 10px;"
      "padding-bottom: 10px;"
      "display: flex;"
      "flex-direction: column;"
      "align-items: center;"
    "}"
    "footer div{" 
      "display: grid;"
      "gap: 10px;"
      "grid-template-columns: auto auto;"
    "}"
  "</style>"
"</head>"
"<body>"
    "<nav>"
      "<h1>Teetormaja</h1>"
      "<a href='http://https://www.tt.ee'>www.tt.ee</a>"
    "</nav>"
"<div>"
"<h1>Wifi Connect</h1>";




void wifishowhtml(int networksFound)
{
  html += "<form method='post' action='/wifi'>"
  "<label for='wifi'>Wifi Networks:</label>"
  "<select id='wifi' name='wifi'>";

  for (int i = 0; i < networksFound; i++)
  {
    html += ("<option value='" + WiFi.SSID(i) + "'>" + WiFi.SSID(i) + "</option>");
  }

  html += "</select>"
  "<label for='password'>Wifi password:</label>"
  "<input id='password' name='password' type='text'><input type='submit'>"
  "</div>"
  "<footer>"
  "<h1>Võta Ühendust</h1>"
  "<div>"
    "<p>Aadress:</p>"
    "<p>Rohu 6a, Kuressaare 93819, Eesti Vabariik</p>"
    "<p>Telefon:</p>"
    "<p>45 20 200</p>"
  "</div>"
  "</footer>";

}

void handlePost()
{
  
  if (server.hasArg("wifi") && server.hasArg("password"))
  {
    String ssid = server.arg("wifi");
    String password = server.arg("password");

    WrirteWIFI(ssid, password);

    server.send(200, "text/plain", "POST request received and processed.");
    delay(2000);
    ESP.restart();
  }
  else
  {
    server.send(400, "text/plain", "Bad Request");
  }
}

void handleRoot()
{
  html += "</body></html>";
  server.send(200, "content", html); // Send HTTP status 200 (Ok) and send some text to the browser/client
}

void handleNotFound()
{
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

void Wifisetup()
{

  Serial.println();

  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP("ESPsoftAP_01") ? "Ready" : "Failed!");

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/wifi", HTTP_POST, handlePost); // Call the 'handleRoot' function when a client requests URI "/"
  server.onNotFound(handleNotFound);
  server.begin();
}

void Wifistart()
{
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(readWIFI().c_str(), readpass().c_str());
  while (WiFiMulti.run() != WL_CONNECTED)
  { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(250);
    Serial.print('.');
  }
}

void emonCMSConnection()
{

  String URL = "http://85.89.32.58/input/post?node=ACT&apikey=17bda09eb30a8f93c375d009a6066c2c&fulljson={\"ACTemp0\":" + String(temp.temperature) + "}";

  Serial.println("[HTTP] begin...\n");
  if (http.begin(client, URL))
  { // HTTP
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", "Bearer 17bda09eb30a8f93c375d009a6066c2c");
    // start connection and send HTTP header
    int httpCode = http.POST("");

    // httpCode will be negative on error
    if (httpCode > 0)
    {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
      {
        String payload = http.getString();
        Serial.println(payload);
      }
    }
    else
    {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }
  else
  {
    Serial.println("[HTTP] Unable to connect");
  }
  String URL2 = "http://85.89.32.58/input/post?node=ACH&apikey=17bda09eb30a8f93c375d009a6066c2c&fulljson={\"ACHum0\":" + String(humidity.relative_humidity) + "}";
  if (http.begin(client, URL2))
  { // HTTP
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", "Bearer 17bda09eb30a8f93c375d009a6066c2c");
    // start connection and send HTTP header
    int httpCode = http.POST("");

    // httpCode will be negative on error
    if (httpCode > 0)
    {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
      {
        String payload = http.getString();
        Serial.println(payload);
      }
    }
    else
    {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }
  else
  {
    Serial.println("[HTTP] Unable to connect");
  }
}

int lastcommand = 0;
int lasttemp = 0;
void ApiConnection()
{

  String URL = "http://acapi.tak21maasik.itmajakas.ee/ControllAC?name=" + String(name) + "";

  if (http.begin(client, URL))
  { // HTTP
    http.addHeader("Content-Type", "application/json");
    Serial.print("[HTTP] API GET...\n");
    // start connection and send HTTP header
    int httpCode = http.POST("");
    // httpCode will be negative on error
    if (httpCode > 0)
    {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] API GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
      {
        String payload = http.getString();
        JSONVar data = JSON.parse(payload);
        int command = data["command"];
        int temp = data["targettemp"];
        if ((command == 1 && 1 != lastcommand) || (command == 1 && lasttemp != temp))
        {
          lastcommand = 1;
          lasttemp = temp;
          AcOn(temp);
          Serial.println("workingon");
        }
        if (command == 0 && 0 != lastcommand)
        {
          lastcommand = 0;
          AcOff();
          Serial.println("workingoff");
        }
      }
    }
    else
    {
      Serial.printf("[HTTP] API GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }
  else
  {
    Serial.println("[HTTP] Unable to connect");
  }
}
