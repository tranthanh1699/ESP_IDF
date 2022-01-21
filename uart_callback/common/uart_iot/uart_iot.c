#include <stdio.h>
#include <esp_log.h>
#include <driver/gpio.h>
#include "uart_iot.h"

#define BUFF_SIZE 1024

uart_config_t uart_config = 
{
    .baud_rate = 115200, 
    .data_bits = UART_DATA_8_BITS, 
    .parity = UART_PARITY_DISABLE, 
    .stop_bits = UART_STOP_BITS_1, 
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE, 
    .source_clk = UART_SCLK_APB
}

uart_callback_t uart_callback = NULL; 

static void IRAM_ATTR uart_intr_handle(void * arg)
{
    uin32_t port = (uint32_t)arg; 
    uart_callback(port); 
}


void uart_create(uart_port_t uart_num)
{
    uart_driver_install(uart_num, BUFF_SIZE * 2, 0, 0, NULL, 0); 
    uart_param_config(uart_num, &uart_config);
    uart_isr_free(uart_num); 
    uart_isr_register(uart_num, uart_intr_handle, (void*)uart_num, ESP_INTR_FLAG_IRAM, NULL);
    uart_enable_rx_intr(uart_num); 
}
void uart_set_callback(void * callback)
{
    uart_callback = callback; 
}