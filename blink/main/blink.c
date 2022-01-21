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
#include "input_iot.h"
#include "output_iot.h"

void gpio_event_callback(int pin)
{
    if(pin == GPIO_NUM_0)
    {   
        output_io_toggle(GPIO_NUM_18);
    }
}

void app_main(void)
{
    // output_io_create(GPIO_NUM_18); 
    // input_io_create(GPIO_NUM_0, HI_TO_LO);
    // input_set_callback(gpio_event_callback);
    gpio_pad_select_gpio(GPIO_NUM_0); 
    gpio_set_direction(GPIO_NUM_0, GPIO_MODE_INPUT); 
    while(1)
    {
        uint32_t gpio_level = gpio_get_level(GPIO_NUM_0); 
        printf("Gia tri la %d\n", gpio_level);
        vTaskDelay(1000/portTICK_PERIOD_MS); 
    }
}
