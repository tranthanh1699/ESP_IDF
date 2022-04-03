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
#include "esp32_ledc.h"


void app_main(void)
{
   esp32_ledc_init(5000, GPIO_NUM_2); 
   while(1)
   {
       for(int i = 0;  i <= 100; i++)
       {
           esp32_ledc_set_duty(i); 
           vTaskDelay(50 /portTICK_RATE_MS); 
       }
       for(int i = 100; i>= 0; i--)
       {
           esp32_ledc_set_duty(i);
           vTaskDelay(50 / portTICK_RATE_MS); 
       }
   }
}
