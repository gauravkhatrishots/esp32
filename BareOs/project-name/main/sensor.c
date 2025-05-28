#include "sensor.h"

void fillDummyData( int temp, int humid) {
    // Fill the newData structure with dummy values
    newData.temperature = temp; // Example temperature value
    newData.humidity = humid;    // Example humidity value
}
void SensorTimeoutHandler(TimerHandle_t xTimer) {
        static int callCount = 0; // Static variable to keep track of the number of calls
        fillDummyData(callCount, (callCount+1)); // Fill with dummy data{
        callCount++;
        ESP_LOGI("SENSOR", "Sensor Timeout Handler called. Call count: %d", callCount);

}
void registerTimer()
{
    // Create Timer 1 with a period of 1000 ms (1 second)
    sensorTimerObj = xTimerCreate("ReadOutTimer", pdMS_TO_TICKS(1000), pdTRUE, (void *)0, SensorTimeoutHandler);
    if (sensorTimerObj != NULL) {
        xTimerStart(sensorTimerObj, 0); // Start the timer
    } else {
        ESP_LOGE("SENSOR", "Failed to create Timer 1");
    }
}


void vSensorTask(void *pvParameters) {
    ESP_LOGE("SENSOR","SENSOR Task Started.");
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));  // 1000ms = 1s
        xQueueSend(dataQueue, &newData, portMAX_DELAY);
        ESP_LOGI("SENSOR", "sensor data added to the queue.");
        // Task code goes here
    }
}
void setSensorData(QueueHandle_t inputQueue){
dataQueue = inputQueue;
}

void runSensorTask()
{
    xTaskCreate(
    vSensorTask,            // Pointer to the task function
    "SENSORAppTask",          // Name of the task (for debugging)
    2048,                      // Stack size in words (not bytes!)
    NULL,                      // Parameters to pass to the task
    5,                         // Priority of the task
    NULL                       // Pointer to return the task handle (can be NULL)   
);
}
