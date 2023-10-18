#include "MS8607.h"
#include "RFM98.h"
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

// Task 1 - Pressure Sensor
// Uses about 1692 words in stack
void Task1(void *pvParameters) {
  for (;;) {
    // Attempt to take the mutex
    if (xSemaphoreTake(mutex, portMAX_DELAY)) {
      Serial.print("Pressure Sensor running on core: ");
      Serial.println(xPortGetCoreID());

      readMS8607();
      data.temperature = getTemp();
      data.humidity = getHumidity();
      data.pressure = getPressure();

      xSemaphoreGive(mutex); // Release the mutex
      
      vTaskDelay(pdMS_TO_TICKS(1000)); // 1s Delay
    }
  }
}

// Task 2 - Radio Transmission
// Currently using 1536 words in stack
void Task2(void *pvParameters) {
  for (;;) {
    // Attempt to take the mutex
    if (xSemaphoreTake(mutex, portMAX_DELAY)) {
      Serial.print("Radio transmission running on core: ");
      Serial.println(xPortGetCoreID());

      // Timestamp relative to board startup
      unsigned long currentMillis = millis();
      int seconds = currentMillis / 1000 % 60;
      int minutes = (currentMillis / (1000 * 60)) % 60;
      int hours = (currentMillis / (1000 * 60 * 60)) % 24;
      String timestamp = String(hours) + ":" + String(minutes) + ":" + String(seconds);
      data.timestamp = timestamp;
      // TODO: Verify sent messages
      loraTransmit(data);
      // Leave in to observe as peripherals are added
      // Serial.print("# of words in stack currently used: ");
      // Serial.println(uxTaskGetStackHighWaterMark(NULL));

      xSemaphoreGive(mutex); // Release the mutex
      
      vTaskDelay(pdMS_TO_TICKS(1000)); // 1s Delay
    }
  }
}

void setup() {
  Serial.begin(115200);

  setupMS8607();
  setupRFM98();

  mutex = xSemaphoreCreateMutex();
  
  // Create Pressure Sensor (Task 1) on Core 0
  xTaskCreatePinnedToCore(
    Task1,         // Function to execute
    "Task1",       // Name of the task
    2000,          // Stack size (words)
    NULL,          // Task parameters
    1,             // Priority
    &task1Handle,  // Task handle
    0              // Core to run on (Core 0)
  );
   
  // Create Radio (Task 2) on Core 1
  xTaskCreatePinnedToCore(
    Task2,         // Function to execute
    "Task2",       // Name of the task
    3000,          // Stack size (words)
    NULL,          // Task parameters
    1,             // Priority
    &task2Handle,  // Task handle
    1              // Core to run on (Core 1)
  );
}

void loop() {

}

