#include <Arduino.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

#define PIN_RED 26
#define PIN_GREEN 33
#define DELAY_RED 1000
#define DELAY_GREEN 200

TaskHandle_t handleRed;
TaskHandle_t handleGreen;
TaskHandle_t handleRandom1;
TaskHandle_t handleRandom2;

void something1(void *parameter) {
    while(true) {
        vTaskDelay(10);
        printf("*");
    }
}

void something2(void *parameter) {
    while(true) {
        vTaskDelay(10);
        printf("*");
    }
}

void green(void *parameter) {
    portTickType xLastWakeTime = xTaskGetTickCount();
    while (1) {
        vTaskDelayUntil(&xLastWakeTime, (DELAY_GREEN / portTICK_RATE_MS));
        digitalWrite(PIN_GREEN, HIGH);
        vTaskDelayUntil(&xLastWakeTime, (DELAY_GREEN / portTICK_RATE_MS));
        digitalWrite(PIN_GREEN, LOW);
    }
}

void red(void *parameter) {
    portTickType xLastWakeTime = xTaskGetTickCount();
    while (1) {
        vTaskDelayUntil(&xLastWakeTime, (DELAY_RED / portTICK_RATE_MS));
        digitalWrite(PIN_RED, HIGH);
        vTaskDelayUntil(&xLastWakeTime, (DELAY_RED / portTICK_RATE_MS));
        digitalWrite(PIN_RED, LOW);
    }
}

void app_main() {
    pinMode(PIN_RED, OUTPUT);
    pinMode(PIN_GREEN, OUTPUT);

    xTaskCreatePinnedToCore(something1,"Random0",1000,NULL,1,&handleRandom1,0);
    xTaskCreatePinnedToCore(something2,"Random1",1000,NULL,1,&handleRandom2,1);

    xTaskCreatePinnedToCore(green,"Green1",1000,NULL,1,&handleGreen,1);
    xTaskCreatePinnedToCore(red,"Red0",1000,NULL,1,&handleRed,0);

    while (1) {
        vTaskDelay(10);
    }
}
