#include <Arduino.h>
#include <EEPROM.h>  // Long term memory LIMITED CYCELS !!!

void WrirteWIFI(String wifi, String pass){
    Serial.print(wifi);
    Serial.print(pass);

    EEPROM.begin(24);
    for (int i = 0; i < wifi.length(); i++) {
        EEPROM.write(i, wifi[i]);
    }

    for (int i = 0; i < pass.length(); i++) {
        EEPROM.write(i + 200, pass[i]);
    }

    EEPROM.commit();
}

String readWIFI(){

  String dataFromEEPROM = "";
  for (int i = 0; i < 14; i++) { // Length of "Hello, EEPROM!"
    dataFromEEPROM += char(EEPROM.read(i));
  }
  Serial.println("Data from EEPROM: " + dataFromEEPROM);
}

String readpass(){

  String dataFromEEPROM = "";
  for (int i = 0; i < 14; i++) { // Length of "Hello, EEPROM!"
    dataFromEEPROM += char(EEPROM.read(i));
  }
  Serial.println("Data from EEPROM: " + dataFromEEPROM);
  
}