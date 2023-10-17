#include "MS8607.h"
#include "RFM98.h"
#include "DataPacket.h"

DataPacket data;

void setup() {
  Serial.begin (115200);
  setupMS8607();
  setupRFM98();

  /*xTaskCreatePinnedToCore (
    pressureSensor,     // Function to implement the task
    "pressureSensor",   // Name of the task
    1024,      // Stack size in bytes
    NULL,      // Task input parameter
    1,         // Priority of the task
    NULL,      // Task handle.
    0          // Core where the task should run
  );

  vTaskStartScheduler();*/
}

// Core 1: Reserved For Radio
void loop() {
  Serial.print("Arduino loop() running on core: ");
  Serial.println(xPortGetCoreID());

  pressureSensor();

  unsigned long currentMillis = millis();
  int seconds = currentMillis / 1000 % 60;
  int minutes = (currentMillis / (1000 * 60)) % 60;
  int hours = (currentMillis / (1000 * 60 * 60)) % 24;
  String timestamp = "Time: " + String(hours) + ":" + String(minutes) + ":" + String(seconds);
  data.timestamp = timestamp;

  loraTransmit(data);

  delay(1000);
}

// Core 0: Pressure Sensor MS860702
void pressureSensor() {
  Serial.print("Pressure Sensor running on core: ");
  Serial.println(xPortGetCoreID());

  readMS8607();
  data.temperature = getTemp();
  data.humidity = getHumidity();
  data.pressure = getPressure();
}
