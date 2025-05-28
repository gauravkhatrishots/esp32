#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "esp_system.h"
#include "esp_log.h"
#include "display.h"
#include "sensor.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "types.h"

TimerHandle_t timer2Handle = NULL;
QueueHandle_t Queue = NULL; // Queue to hold sensor data

void app_main() {
    // Create Timer 2 with a period of 2000 ms (2 seconds)
    Queue = xQueueCreate(10, sizeof(struct sensorData)); // Create a queue for sensor data
    registerTimer(); // Register the Sensor timer
    setSensorData(Queue); // Set the sensor data queue
    registerInputQueue(Queue); // Register the input queue for sensor data
    runDisplayTask();
    runSensorTask(); // Register the sensor task
}
