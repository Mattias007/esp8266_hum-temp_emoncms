#include <Arduino.h>
#include "wifihttps.h"
void setup() {
Serial.begin(9600);


WiFi.scanNetworksAsync(wifishowhtml);

Wifisetup();
ac.begin();

ahtstart();

}

unsigned long previousMillis = 0;
const long interval = 10000; 

void loop() {

unsigned long currentMillis = millis();

if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    ahtdata();

    emonCMSConnection();

    ApiConnection();

}

server.handleClient();  

}

