#include <Arduino.h>
#include <EEPROM.h>  // Long term memory LIMITED CYCELS !!!


void WrirteWIFI(String wifi, String pass){
    Serial.println(wifi);
    Serial.println(pass);

    EEPROM.begin(512);
    int wifilen = wifi.length();
        EEPROM.write(500, wifilen);
    for (int i = 0; i < wifilen; i++) {
        EEPROM.write(i, wifi[i]);
    }
    int passlen = pass.length();
    EEPROM.write(501, passlen);
    for (int i = 0; i < passlen; i++) {
        EEPROM.write(i + wifilen, pass[i]);
    }

    EEPROM.commit();
    EEPROM.end();
    Serial.print("commited");
}

String readWIFI(){
    EEPROM.begin(512);
    int len = EEPROM.read(500);
    String dataFromEEPROM = "";
    for (int i = 0; i < len; i++) {
        dataFromEEPROM += char(EEPROM.read(i));
    }
    EEPROM.end();
    return dataFromEEPROM;
}

String readpass(){
    EEPROM.begin(512);
    int wifilen = EEPROM.read(500);
    int len = EEPROM.read(501);
    String dataFromEEPROM = "";
    for (int i = 0; i < len; i++) {

        dataFromEEPROM += char(EEPROM.read(i+wifilen));
    }
    EEPROM.end();
    return dataFromEEPROM;

}
