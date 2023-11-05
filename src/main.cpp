#include <Arduino.h>
#include "wifihttps.h"

void setup()
{
    Serial.begin(9600);
    pinMode(13, INPUT); // pin 7

    if (digitalRead(13) == 1)
    {
        Serial.println("Accses point mode");
        WiFi.scanNetworksAsync(wifishowhtml);
        Wifisetup();
        delay(5000); // hard coded dealy scan takes time returns -1 if not compleet
    }
    else
    {
        Serial.println("Working mode");
        Wifistart();
        ac.begin();
        ahtstart();
    }

    Serial.println(readWIFI());
    Serial.println(readpass());
}

unsigned long previousMillis = 0;
const long interval = 10000;

void loop()
{

    if ((WiFiMulti.run() == WL_CONNECTED))
    { // checks for wifi connection

        unsigned long currentMillis = millis();

        if (currentMillis - previousMillis >= interval)
        {
            previousMillis = currentMillis;
            ahtdata();

            emonCMSConnection();

            ApiConnection();
        }
    }

    server.handleClient();
}
