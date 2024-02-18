#include "MS8607.h"
#include "RFM98.h"
#include "GPS.h"
#include "DataPacket.h"

#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

DataPacket data;

// Define the mutex: remedies resource conflicts when
// multiple tasks try to write to the data packet
SemaphoreHandle_t mutex;

TaskHandle_t task1Handle = NULL;
TaskHandle_t task2Handle = NULL;
TaskHandle_t task3Handle = NULL;

// Task 1 - Pressure Sensor
// Uses about 1692 words in stack
void Task1(void *pvParameters) {
  DataPacket* pData = (DataPacket*)pvParameters;
  for (;;) {
    // Attempt to take the mutex
    if (xSemaphoreTake(mutex, portMAX_DELAY)) {
      Serial.print("Pressure Sensor running on core: ");
      Serial.println(xPortGetCoreID());

      readMS8607(pData);

      Serial.print("Temp: "); Serial.println(pData->temperature);
      Serial.print("Humidity: "); Serial.println(pData->humidity);
      Serial.print("Pressure: "); Serial.println(pData->pressure);

      // Leave in to observe as peripherals are added
      Serial.print("# of words in stack currently used: ");
      Serial.println(uxTaskGetStackHighWaterMark(NULL));

      xSemaphoreGive(mutex); // Release the mutex
      
      vTaskDelay(pdMS_TO_TICKS(10)); // 10ms/100Hz Delay
    }
  }
}

// Task 2 - Radio Transmission
// Currently using 1536 words in stack
void Task2(void *pvParameters) {
  DataPacket* pData = (DataPacket*)pvParameters;
  for (;;) {
    // Attempt to take the mutex
    if (xSemaphoreTake(mutex, portMAX_DELAY)) {
      Serial.print("Radio transmission running on core: ");
      Serial.println(xPortGetCoreID());

      loraTransmit(pData);

      // Leave in to observe as peripherals are added
      Serial.print("# of words in stack currently used: ");
      Serial.println(uxTaskGetStackHighWaterMark(NULL));

      xSemaphoreGive(mutex); // Release the mutex
      
      vTaskDelay(pdMS_TO_TICKS(10)); //  10ms/100Hz Delay
    }
  }
}

// Task 3 - GPS Currently using 1792 words in stack
void Task3(void *pvParameters) {
  DataPacket* pData = (DataPacket*)pvParameters;
  for (;;) {
    // Attempt to take the mutex
    if (xSemaphoreTake(mutex, portMAX_DELAY)) {
      Serial.print("GPS running on core: ");
      Serial.println(xPortGetCoreID());

      readGPS(pData);

      Serial.print("Latitude: "); Serial.println(pData->latitude);
      Serial.print("Longitude: "); Serial.println(pData->longitude);
      Serial.print("Altitude: "); Serial.println(pData->altitude);

      // Leave in to observe as peripherals are added
      Serial.print("# of words in stack currently used: ");
      Serial.println(uxTaskGetStackHighWaterMark(NULL));

      xSemaphoreGive(mutex); // Release the mutex
      
      vTaskDelay(pdMS_TO_TICKS(10)); // 1s Delay
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Beginning Sprinkle");
  setupMS8607();
  setupRFM98();
  setupGPS();

  mutex = xSemaphoreCreateMutex();
  
  // Create Pressure Sensor (Task 1) on Core 0
  xTaskCreatePinnedToCore(
    Task1,         // Function to execute
    "Task1",       // Name of the task
    2000,          // Stack size (words)
    &data,          // Task parameters
    1,             // Priority
    &task1Handle,  // Task handle
    0              // Core to run on (Core 0)
  );
   
  // Create Radio (Task 2) on Core 1
  xTaskCreatePinnedToCore(
    Task2,         // Function to execute
    "Task2",       // Name of the task
    10000,          // Stack size (words)
    &data,          // Task parameters
    1,             // Priority
    &task2Handle,  // Task handle
    1              // Core to run on (Core 1)
  );

  // Create GPS (Task 3) on Core 0
  xTaskCreatePinnedToCore(
    Task3,         // Function to execute
    "Task3",       // Name of the task
    6000,          // Stack size (words)
    &data,          // Task parameters
    1,             // Priority
    &task3Handle,  // Task handle
    0              // Core to run on (Core 0)
  );
}

void loop() {
}
