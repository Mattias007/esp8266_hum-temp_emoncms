#include <ir_Coolix.h>

// USING IREMOTE FORK THE
// Midea AIr condioner
const uint16_t kIrLed = 14; // means PIN D5 !!!!!
IRCoolixAC ac(kIrLed);

void AcOn(int temp)
{
    ac.on();
    ac.setTemp(temp);
    ac.send();
}

void AcOff()
{
    ac.off();
    ac.send();
}