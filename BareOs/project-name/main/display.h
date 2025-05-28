#ifndef DISPLAY_H
#define DISPLAY_H
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "ESP_LOG.h"
#include "esp_system.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "types.h"

struct sensorData receivedValue; // Variable to hold the received sensor data
QueueHandle_t inputQueue; // Queue to hold sensor data

void vDisplayAppTask(void *pvParameters);
void registerInputQueue(QueueHandle_t inputQue);
void runDisplayTask();
#endif // DISPLAY_H