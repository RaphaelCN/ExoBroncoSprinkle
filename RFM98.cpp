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

void loraTransmit(DataPacket data)
{
  Serial.print("Sending packet: ");
  Serial.println(counter);

  //Send LoRa packet to receiver
  LoRa.beginPacket();
  LoRa.print("    ");
  LoRa.print("Time Since Startup (H:M:S): ");
  LoRa.println(data.timestamp);
  LoRa.print("Temperature (C): ");
  LoRa.println(data.temperature);
  LoRa.print("Humidity (%rH): ");
  LoRa.println(data.humidity);
  LoRa.print("Pressure (hPa): ");
  LoRa.println(data.pressure);
  LoRa.endPacket();

  counter++;
}

void loraReceive()
{
  Serial.print("TODO");
}
