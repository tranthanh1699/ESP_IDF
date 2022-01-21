/* UART Events Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "output_iot.h"
#include "uart_iot.h"


void uart_event_callback(uint32_t uart_port)
{
    if(gpio_nu)
}

void app_main(void)
{
    uart_create(UART_NUM_0); 
    uart_set_callback(uart_event_callback); 
    output_io_create(GPIO_NUM_2); 
}
