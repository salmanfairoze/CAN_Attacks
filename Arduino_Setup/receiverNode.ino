#include <SPI.h>
#include <mcp_can.h>
#include "mcp2515_can.h"

const int spiCSPin = 10;
const int ledPin = 2;
boolean ledON = 1;

mcp2515_can CAN(spiCSPin);

void setup()
{
    Serial.begin(115200);
    pinMode(ledPin,OUTPUT);
    pinMode(7, OUTPUT);

    while (CAN_OK != CAN.begin(CAN_500KBPS))
    {
        Serial.println("CAN BUS Init Failed");
        delay(100);
    }
    Serial.println("CAN BUS  Init OK!");
}


void loop()
{
    unsigned char len = 0;
    unsigned char buf[8];
    

    if(CAN_MSGAVAIL == CAN.checkReceive())
    {
        CAN.readMsgBuf(&len, buf);
        unsigned long canId = CAN.getCanId();

        if (canId == 67) // Decimal value of 0x43
        {
          Serial.println(buf[0]);
          digitalWrite(7, buf[0]);
        }
    }
}
