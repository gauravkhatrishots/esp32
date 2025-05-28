#ifndef SENSOR_H
#define SENSOR_H
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "ESP_LOG.h"
#include "esp_system.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "types.h"
struct sensorData newData;

void fillDummyData( int temp, int humid);
QueueHandle_t dataQueue = NULL; // Queue to hold sensor data
// Timer handle
TimerHandle_t sensorTimerObj = NULL;
// ReadOutTimer callback
void SensorTimeoutHandler(TimerHandle_t xTimer); 
void registerTimer();

void vSensorTask(void *pvParameters);
void setSensorData(QueueHandle_t inputQueue);
void runSensorTask();
#endif // SENSOR_H