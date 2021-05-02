#include <SPI.h>
#include <mcp_can.h>
#include "mcp2515_can.h"

const int spiCSPin = 10;
int ledHIGH    = 1;
int ledLOW     = 0;

mcp2515_can CAN(spiCSPin);

void setup()
{
    Serial.begin(115200);

    while (CAN_OK != CAN.begin(CAN_500KBPS))
    {
        Serial.println("CAN BUS init Failed");
        delay(100);
    }
    Serial.println("CAN BUS Shield Init OK!");
}

unsigned char stmp[4] = {1, 1, 2, 3};
int lock = 0;

void loop()
{   
  Serial.println("type 0 to lock and 1 to unlock");
  while (Serial.available() == 0){};
  lock = Serial.read() - '0';
  if (lock) Serial.println("vehicle unlocked");
  else Serial.println("vehicle locked");
  stmp[0] = lock;
  CAN.sendMsgBuf(0x43, 0, 4, stmp);
}
