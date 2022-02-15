#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "freertos/semphr.h"

char* string = "test_string";

SemaphoreHandle_t handle;

TaskHandle_t reader1;
TaskHandle_t reader2;
TaskHandle_t writer1;
TaskHandle_t writer2;
TaskHandle_t writer3;

void readerTask(void *parameter) {
    portTickType xLastWakeTime = xTaskGetTickCount();

    while (true) {
        vTaskDelayUntil(&xLastWakeTime, 1000 / portTICK_RATE_MS);
        xSemaphoreTake(handle, portMAX_DELAY);
        printf("Reading %s: %s\n", (char *) parameter, string);
        xSemaphoreGive(handle);
    }
}

void writingTask(void *parameter) {
    portTickType xLastWakeTime = xTaskGetTickCount();

    while (true) {
        vTaskDelayUntil(&xLastWakeTime, 1000 / portTICK_RATE_MS);
        xSemaphoreTake(handle, portMAX_DELAY);
        string = (char *) parameter;
        printf("Writing: %s\n", string);
        xSemaphoreGive(handle);
    }
}

void app_main() {
    handle = xSemaphoreCreateMutex();

    xTaskCreatePinnedToCore(readerTask, "Reader 1", 10000, "Reader 1", 1, reader1, 0);
    xTaskCreatePinnedToCore(readerTask, "Reader 2", 10000, "Reader 2", 1, reader2, 0);
    xTaskCreatePinnedToCore(writingTask, "Writer 1", 10000, "Writer 1", 2, writer1, 0);
    xTaskCreatePinnedToCore(writingTask, "Writer 2", 10000, "Writer 2", 2, writer2, 0);
    xTaskCreatePinnedToCore(writingTask, "Writer 3", 10000, "Writer 3", 2, writer3, 0);
}
