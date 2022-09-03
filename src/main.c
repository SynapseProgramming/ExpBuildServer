#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "freertos/task.h"
#include "freertos/queue.h"

TaskHandle_t task_send_Handle = NULL;
TaskHandle_t task_receive_Handle = NULL;
QueueHandle_t queue;

typedef struct
{
    int8_t x;
    int8_t y;
    int8_t z;
    int8_t battery;
    uint8_t ID;

} SensorNodeData;

void task_send(void *arg)
{
    SensorNodeData input;

    input.ID = 1;
    input.x = 2;
    input.y = 5;
    input.z = 45;
    input.battery = 69;

    xQueueSend(queue, &input, pdMS_TO_TICKS(100));
    printf("Sent Data!\n");

    input.ID = 2;
    input.x = 45;
    input.y = 67;
    input.z = 32;
    input.battery = 99;

    xQueueSend(queue, &input, pdMS_TO_TICKS(100));
    printf("Sent Data again!\n");

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void task_receive(void *arg)
{
    // char string_receive[9];

    SensorNodeData receivedData;
    while (1)
    {
        if (xQueueReceive(queue, &(receivedData), pdMS_TO_TICKS(100)))
        {
            ESP_LOGI("ACC SENSOR:", "x: %d y: %d z: %d from sensor: 0x%04x", receivedData.x, receivedData.y, receivedData.z, receivedData.ID);
            ESP_LOGI("BATTERY LEVEL:", "level: %d", receivedData.battery);
        }
        else
        {
            printf("nothing in q!\n");
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void app_main()
{
    // Queue creation
    queue = xQueueCreate(10, sizeof(SensorNodeData));

    // Available
    printf("\nAvailable at the begining: %d \n\n", uxQueueSpacesAvailable(queue));

    // Write data to queue
    xTaskCreate(task_send, "task_send", 4096, NULL, 10, &task_send_Handle);
    vTaskDelay(pdMS_TO_TICKS(1000));

    // Available
    printf("Available after writing: %d \n\n", uxQueueSpacesAvailable(queue));

    // Read data from queue
    xTaskCreate(task_receive, "task_receive", 4096, NULL, 10, &task_receive_Handle);
    vTaskDelay(pdMS_TO_TICKS(1000));

    // Available
    printf("\nAvailable at the end: %d \n\n", uxQueueSpacesAvailable(queue));
}