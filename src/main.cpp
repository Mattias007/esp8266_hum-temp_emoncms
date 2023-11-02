#include <Arduino.h>
#include "wifihttps.h"
void setup() {
Serial.begin(9600);

WiFi.scanNetworksAsync(wifishowhtml);
delay(5000); // hard coded dealy scan takes time returns -1 if not compleet
Wifisetup();
ac.begin();
readpass();
readwifi();
ahtstart();
}

unsigned long previousMillis = 0;
const long interval = 10000; 

void loop() {

if ((WiFiMulti.run() == WL_CONNECTED)) { // checks for wifi connection

unsigned long currentMillis = millis();

if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    ahtdata();

    emonCMSConnection();

    ApiConnection();

}
}

server.handleClient();  

}

