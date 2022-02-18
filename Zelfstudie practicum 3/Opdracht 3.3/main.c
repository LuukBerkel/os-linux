#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "freertos/semphr.h"

#define QUEUE_LENGHT 10
#define EMPTY -1

// This is the queue for the application
int data_queue[QUEUE_LENGHT];

// This is the mutex that protects the queue
SemaphoreHandle_t array_acces_mutex;



void ConsumingTask(void *parameter) {
    portTickType xLastWakeTime = xTaskGetTickCount();

    while (true) {
        vTaskDelayUntil(&xLastWakeTime, 1000 / portTICK_RATE_MS);

        xSemaphoreTake(array_acces_mutex, portMAX_DELAY);
        // Consuming and empting queue
        for (int i =  QUEUE_LENGHT-1; i >= 0; i--)
        {
            if (data_queue[i] != EMPTY){
                printf("Consumed number: %d\n\n",  data_queue[i] );
                data_queue[i] = EMPTY;
                break;
            }
        }   
        xSemaphoreGive(array_acces_mutex);
    }
}

void ProducerTask(void *parameter) {
    portTickType xLastWakeTime = xTaskGetTickCount();

    while (true) {
        vTaskDelayUntil(&xLastWakeTime, 800 / portTICK_RATE_MS);
        xSemaphoreTake(array_acces_mutex, portMAX_DELAY);

        // Producing and filling queue
        for (int i = 0; i < QUEUE_LENGHT; i++)
        {
            if (data_queue[i] == EMPTY){
                data_queue[i] = rand();
                printf("Produced number %d\n",  data_queue[i] );
                printf("The size of the array is now %d\n\n", i+1);
                break;
            }
        }
        
        xSemaphoreGive(array_acces_mutex);
    }
}

void app_main() {
    // Cleaning queue so it is empty else there will be random garbage in the queue
    for (int i = 0; i < QUEUE_LENGHT; i++)
    {
        data_queue[i] = EMPTY;
    }
    
    // Creating mutex
    array_acces_mutex = xSemaphoreCreateMutex();

    // Starting program
    xTaskCreatePinnedToCore(ConsumingTask,"",  100000, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(ProducerTask, "",  100000, NULL, 1, NULL, 0);
}
