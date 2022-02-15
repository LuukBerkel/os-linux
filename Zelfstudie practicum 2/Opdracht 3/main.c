#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

void task(void *parameter) {
    while (1) {
        printf("Task %d\n", (int *) parameter);
        vTaskDelay(5);
    }
}

void app_main() {
    for (int i = 0; i < 20; i++) {
        xTaskCreatePinnedToCore(task, ("Task " + i), 10000, (void *) i, i, NULL, 0);
    }

    while (1) {
        vTaskDelay(10);
    }
}
