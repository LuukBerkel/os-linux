#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include <stdbool.h>
#include "freertos/semphr.h"


/**
 * @brief These are all the adresses of the leds thata are connected.
 */
static const int LEDS[] = {22, 4, 19, 21, 23};

/**
 * @brief This holds 5 forks. If a fork is in use is indicated by the boolean.
 */
static bool state_data[] = {false, false, false, false, false};

/**
 * @brief This is the semaphore that restricts access to the state_data.
 */
SemaphoreHandle_t handle;

/**
 * @brief this functions is for getting a fork.
 * 
 * @return true if a fork is reserved 
 * @return false  if there are no forks left.
 */
bool SeachAndTake(){
    for (int i = 0; i < 5; i++)
    {
        if (!state_data[i]){
            state_data[i] = true;
            return true;
        }
    }

    return false;
}

/**
 * @brief this function is for returning fork
 */
void SearchAndGive(){
    for (int i = 0; i < 5; i++)
    {
        if (state_data[i]){
            state_data[i] = false;
            return;
        }
    }
}


/**
 * @brief This task is a pilosopher which consumes.
 * 
 * @param parameter 
 */
void task(void *parameter) {
    int index = (int) parameter;
     printf("LED_%d\n", index);
    gpio_pad_select_gpio(LEDS[index]);
    gpio_set_direction(LEDS[index], GPIO_MODE_OUTPUT);

    while (1) {

        bool reserved = false;

        xSemaphoreTake(handle, portMAX_DELAY);
        bool fork1 = SeachAndTake();
        bool fork2 = SeachAndTake();
        if (fork1 && !fork2){
            SearchAndGive();
        } else if (fork1 && fork2){
            reserved = true;
        }
        xSemaphoreGive(handle);

        //Eating time
        if (reserved){
             gpio_set_level(LEDS[index], 1);
            vTaskDelay(1000 / portTICK_RATE_MS);
            gpio_set_level(LEDS[index], 0);
             //Putting back 2 forks
            xSemaphoreTake(handle, portMAX_DELAY);
            SearchAndGive();
            SearchAndGive();
            xSemaphoreGive(handle);
        }

    

        //Delaying for other pilosophers so they can procure a fork.
        vTaskDelay(10 / portTICK_RATE_MS);
    }
}


void app_main() {
    handle = xSemaphoreCreateMutex();

    for (int i = 0; i < 5; i++)
    {
        xTaskCreatePinnedToCore(task, ("Task " + i), 10000,(void*) i, 1, NULL, 0);
    }
}
