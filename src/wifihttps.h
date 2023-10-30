#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <Arduino_JSON.h>

#include "ACcontroll.h"
#include "sensordata.h"

HTTPClient http;
WiFiClient client;
ESP8266WiFiMulti WiFiMulti;

const char *ssid = "MatuLiisu";
const char *password = "matuliisu";
const char *name = "ACTemp0";


void Wifistart(){
    WiFi.mode(WIFI_STA);

    WiFiMulti.addAP(ssid, password);
}


void emonCMSConnection() {

  // wait for WiFi connection


  if ((WiFiMulti.run() == WL_CONNECTED)) {  

    String URL = "http://85.89.32.58/input/post?node=ACT&apikey=17bda09eb30a8f93c375d009a6066c2c&fulljson={\"ACTemp0\":" + String(temp.temperature)  + "}";

    Serial.println("[HTTP] begin...\n");
    if (http.begin(client, URL)) {  // HTTP
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", "Bearer 17bda09eb30a8f93c375d009a6066c2c");
      // start connection and send HTTP header
      int httpCode = http.POST("");

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] POST... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.println(payload);
        }
      } else {
        Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      Serial.println("[HTTP] Unable to connect");
    }
    String URL2 = "http://85.89.32.58/input/post?node=ACH&apikey=17bda09eb30a8f93c375d009a6066c2c&fulljson={\"ACHum0\":" + String(humidity.relative_humidity)  + "}";
    if (http.begin(client, URL2)) {  // HTTP
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", "Bearer 17bda09eb30a8f93c375d009a6066c2c");
      // start connection and send HTTP header
      int httpCode = http.POST("");

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] POST... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.println(payload);
        }
      } else {
        Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      Serial.println("[HTTP] Unable to connect");
    }
  }
}



int lastcommand = 0;
void ApiConnection() {

  if ((WiFiMulti.run() == WL_CONNECTED)) {  

    String URL = "http://acapi.tak21maasik.itmajakas.ee/ControllAC?name=" + String(name) + "";

    if (http.begin(client, URL)) {  // HTTP
    http.addHeader("Content-Type", "application/json");
      Serial.print("[HTTP] API GET...\n");
      // start connection and send HTTP header
      int httpCode = http.POST("");
      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] API GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          JSONVar data = JSON.parse(payload);
          String command = data["command"];
          Serial.println(command);

          if(command == "on" &&  1 != lastcommand){
            lastcommand = 1;
            AcOn(70);
            Serial.println("workingon");
          }
          if(command == "off" && 0 != lastcommand){
            lastcommand = 0;
            AcOff();
            Serial.println("workingoff");
          }
        }
      } else {
        Serial.printf("[HTTP] API GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      Serial.println("[HTTP] Unable to connect");
    }
  }
}
