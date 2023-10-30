#include <Arduino.h>
#include "wifihttps.h"


void setup() {
Serial.begin(9600);

Wifistart();

ac.begin();

ahtstart();

}

void loop() {

ahtdata();

emonCMSConnection();

ApiConnection();

delay(10000);
}


