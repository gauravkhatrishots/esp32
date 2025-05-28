#include "display.h"

void vDisplayAppTask(void *pvParameters) {
    ESP_LOGE("Display","now the display task is started");
    while (1) {
        if (inputQueue == NULL) {
            ESP_LOGE("Display", "Input queue is not registered. Please register the input queue before running the task.");
            vTaskDelay(pdMS_TO_TICKS(1000)); // Delay to avoid busy waiting
            continue; // Skip the rest of the loop if the queue is not registered
        }
        else {
        if (xQueueReceive(inputQueue, &receivedValue, portMAX_DELAY)) {
            {
                ESP_LOGE ("Display", "Received value: %d", receivedValue.temperature);
                ESP_LOGE ("Display", "Received value: %d", receivedValue.humidity);
            }
        }
            
}
    }
}

void registerInputQueue(QueueHandle_t inputQue) {
    inputQueue = inputQue; // Store the input queue for sensor data
    // This function can be used to register the input queue for sensor data
    // For example, you can store the queue in a global variable or pass it to the task
    // Here, we assume that the queue is already created and passed as an argument
    // You can implement your logic to handle the queue as needed
    ESP_LOGI("Display", "Input queue registered successfully");
}

void runDisplayTask()
{
    xTaskCreate(
    vDisplayAppTask,            // Pointer to the task function
    "DipslyAppTask",          // Name of the task (for debugging)
    2048,                      // Stack size in words (not bytes!)
    NULL,                      // Parameters to pass to the task
    5,                         // Priority of the task
    NULL                       // Pointer to return the task handle (can be NULL)
);
}