#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_intr_alloc.h"
#include "soc/uart_struct.h"
#include "soc/uart_reg.h"
#include "esp32_uart.h"
#include "esp_err.h"

#define BIT_3 (1 << 3) 

EventGroupHandle_t xEvent; 
TaskHandle_t xTask; 

void eventTask(void * arg); 

#define BUF_SIZE 1024
#define EX_UART_NUM UART_NUM_0

void uart_0_callback(uart_port_t uart_num)
{
	BaseType_t xBase; 
	xEventGroupSetBitsFromISR(xEvent, BIT_3, &xBase); 
	esp32_uart_log(EX_UART_NUM, "Tran Thanh Test Interrupt: %d\n", 1);
}

void app_main()
{
	esp32_uart_config(EX_UART_NUM, 115200, BUF_SIZE); 
	esp32_uart_set_rx_callback(uart_0_callback);
	xEvent = xEventGroupCreate(); 
	xTaskCreate(eventTask, "Task01", 2048, NULL, 6, xTask); 
	while(1)
	{
		static int i = 0; 
		esp32_uart_log(EX_UART_NUM, "Tran Thanh Test: %d\n", i++);
		vTaskDelay(5000 / portTICK_PERIOD_MS); 
	}
}

void eventTask(void * arg)
{
	char task_str[100]; 
	while (1)
	{
		xEventGroupWaitBits(xEvent, BIT_3, pdTRUE, pdFAIL, portMAX_DELAY); 
		esp32_uart_read_string(task_str); 
		esp32_uart_log(EX_UART_NUM, "Task Hande: %s\n", task_str); 
	}
}
