#ifndef ESP32_UART_H
#define ESP32_UART_H
#include <hal/uart_types.h>
#include <esp_err.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include <freertos/queue.h>

typedef void (*uart_callback_t)(uart_port_t uart_num); 

void esp32_uart_config(uart_port_t uart_num, int baud, int buffer_size); 
void esp32_uart_log(uart_port_t uart_num, const char * str,...);
void esp32_uart_set_rx_callback(void * callback); 
void esp32_uart_read_string(char * str); 
uint32_t esp32_uart_available(); 
void esp32_clear_buffer(); 
#endif