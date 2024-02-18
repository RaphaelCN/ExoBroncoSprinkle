#include "RFM98.h"

#define DIO0 2
#define LORA_RST 21
#define LORA_CS 10

#define SPI_MOSI 13
#define SPI_MISO 11
#define SPI_SCK 12

int counter = 0;
SPIClass *hspi = NULL;

void setupRFM98()
{
  Serial.print(F("Radio Initializing ... "));

  // Must init custom SPI
  hspi = new SPIClass(HSPI);
  hspi->begin(SPI_SCK, SPI_MISO, SPI_MOSI, LORA_CS);
  pinMode(LORA_CS, OUTPUT);

  // Set LoRa SPI and Pins
  LoRa.setSPI(*hspi);
  LoRa.setPins(LORA_CS, LORA_RST, DIO0);

  while (!LoRa.begin(433E6))
  {
    Serial.print(F("Radio Init Failed!"));
  }
  
  Serial.print(F("Radio Init Success!"));
}

void loraTransmit(DataPacket* pData)
{
  Serial.print("Sending packet: ");
  Serial.println(counter);

  // Send LoRa packet to receiver
  LoRa.beginPacket();
  // Have to add buffer for some reason?
  LoRa.println("    ");
  // Time and Date
  //LoRa.println(data.timestamp);
  //LoRa.println(data.datestamp);

  // Lat Long in degrees
  LoRa.print("LLA: ");
  LoRa.print(pData->latitude); LoRa.print(", "); LoRa.println(pData->longitude);

  // Altitude (meters)
  LoRa.print("Alt: "); LoRa.println(pData->altitude);

  // Temperature (C), Pressure (hPa)
  LoRa.print("Tmp: "); LoRa.println(pData->temperature);
  LoRa.print("Ps: "); LoRa.println(pData->pressure);

  LoRa.endPacket();

  counter++;
}

void loraReceive()
{
  Serial.print("TODO");
}
