#ifndef RFM98_H
#define RFM98_H
#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include "DataPacket.h"

void setupRFM98();
void loraTransmit(DataPacket data);
void loraReceive();

#endif