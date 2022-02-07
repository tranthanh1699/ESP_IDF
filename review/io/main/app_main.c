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
#include "esp32_output.h"
#include "esp32_input.h"

void gpio_event_callback(int pin)
{
    if(pin == GPIO_NUM_0)
    {   
        esp32_io_toggle_pin(GPIO_NUM_2);
        //printf("Intr pin 0\n"); 
    }
}

void app_main(void)
{
   esp32_io_config_output_pin(GPIO_NUM_2); 
   input_io_create(GPIO_NUM_0, HI_TO_LO);
   input_set_callback(gpio_event_callback);
}
