/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "output.h"
#include "input.h"

void gpio_callback_function(int gpio)
{
    esp_gpio_togglepin(GPIO_NUM_2); 
}

void app_main(void)
{
    esp_gpio_output_create(GPIO_NUM_2); 
    esp_gpio_input_create(GPIO_NUM_0, FALLING); 
    esp_gpio_set_callback(gpio_callback_function); 
    while(1)
    {
         vTaskDelay(100); 
    }
}
