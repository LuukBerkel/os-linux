#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "freertos/semphr.h"



char* string = "test_string";

SemaphoreHandle_t string_acces_mutex;

TaskHandle_t reader1;
TaskHandle_t reader2;
TaskHandle_t writer1;
TaskHandle_t writer2;
TaskHandle_t writer3;

void readerTask(void *parameter) {
    portTickType xLastWakeTime = xTaskGetTickCount();


    while (true) {
        vTaskDelayUntil(&xLastWakeTime, 1000 / portTICK_RATE_MS);

        //Checking wether the mutex is taken
        if(xQueuePeek((xQueueHandle)string_acces_mutex,(void *)NULL,(portTickType)NULL) == pdTRUE){
            printf("Reading %s: %s\n", (char *) parameter, string);
        }
    }
}

void writingTask(void *parameter) {
    portTickType xLastWakeTime = xTaskGetTickCount();

    while (true) {
        vTaskDelayUntil(&xLastWakeTime, 1000 / portTICK_RATE_MS);
        xSemaphoreTake(string_acces_mutex, portMAX_DELAY);
        string = (char *) parameter;
        printf("Writing: %s\n", string);
        xSemaphoreGive(string_acces_mutex);
    }
}

void app_main() {
    string_acces_mutex = xSemaphoreCreateMutex();

    xTaskCreatePinnedToCore(readerTask, "Reader 1", 10000, "Reader 1", 1, reader1, 0);
    xTaskCreatePinnedToCore(readerTask, "Reader 2", 10000, "Reader 2", 1, reader2, 0);
    xTaskCreatePinnedToCore(writingTask, "Writer 1", 10000, "Writer 1", 2, writer1, 0);
    xTaskCreatePinnedToCore(writingTask, "Writer 2", 10000, "Writer 2", 2, writer2, 0);
    xTaskCreatePinnedToCore(writingTask, "Writer 3", 10000, "Writer 3", 2, writer3, 0);
}
