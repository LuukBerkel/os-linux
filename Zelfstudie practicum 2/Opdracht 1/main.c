#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

int priority = 6;
TaskHandle_t handle;

void Task1code(void *parameter) {
    portTickType xLastWakeTime = xTaskGetTickCount();

    while (true) {
        vTaskDelayUntil(&xLastWakeTime, 4000 / portTICK_RATE_MS);
        printf("weer 4 seconden: Prioriteit %d\n", priority);
    }
}

void app_main() {
    portTickType xLastWakeTime = xTaskGetTickCount();
    xTaskCreatePinnedToCore(Task1code,"Task1",10000,(void*) priority,priority,&handle,0);
    while (true) {
        vTaskDelayUntil(&xLastWakeTime, 8000 / portTICK_RATE_MS);
        if (priority == 1) {
            vTaskDelete(handle);
            printf("Klaar");
            break;
        }
        vTaskPrioritySet(&handle, priority);
        priority--;
    }
}
