/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "output_iot.h"
#include "input_iot.h"

TimerHandle_t xTimers[2]; 
EventGroupHandle_t EventGroup;

#define BIT_EVENT_BUTTON_PRESS (1 << 0)
#define BIT_EVENT_UART__RECV (1 << 1)

void vTask1 (void * arg)
{
    for(;;)
    {
        EventBits_t uxBits = xEventGroupWaitBits(EventGroup, BIT_EVENT_BUTTON_PRESS | BIT_EVENT_UART__RECV, pdTRUE, pdFALSE, portMAX_DELAY);
        if(uxBits & BIT_EVENT_BUTTON_PRESS)
        {
            printf("Button press\n"); 
        }
        if(uxBits & BIT_EVENT_UART__RECV)
        {
            
        }
    }
}
void vTimerCallback(TimerHandle_t xTimer)
 {
    uint32_t ulCount;
    configASSERT(xTimer);
    ulCount = ( uint32_t ) pvTimerGetTimerID( xTimer );
    if(ulCount == 0)
    {
        static int x = 0;   
        output_io_set_level(GPIO_NUM_2, x); 
        x = !x; 
    }
    else if(ulCount == 1)
    {
        printf("Hello timer print\n"); 
    }
 }
 void button_callback(int pin)
 {
     if(pin == GPIO_NUM_0)
     {
         BaseType_t pxHigherPriorityTaskWoken; 
         xEventGroupSetBitsFromISR(EventGroup, BIT_EVENT_BUTTON_PRESS, &pxHigherPriorityTaskWoken); 
     }
 } 
void app_main(void)
{   
    output_io_create(GPIO_NUM_2); 
    input_io_create(GPIO_NUM_0, HI_TO_LO); 
    input_set_callback(button_callback); 
    xTimers[0] = xTimerCreate("Timer01",pdMS_TO_TICKS(500), pdTRUE,(void *)0,vTimerCallback);
    xTimers[1] = xTimerCreate("Timer02",pdMS_TO_TICKS(1000), pdTRUE,(void *)1,vTimerCallback);

    // xTimerStart(xTimers[0], 100); 
    // xTimerStart(xTimers[1], 100);

    EventGroup = xEventGroupCreate(); 

    xTaskCreate(vTask1, "Taskc", 1024, NULL, 6, NULL);
}
