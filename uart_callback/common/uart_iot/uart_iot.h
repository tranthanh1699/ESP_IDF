#ifndef UART_IOT_H
#define UART_IOT_H

#include "hal/gpio_types.h"
#include "driver/uart.h"

typedef void (*uart_callback_t)(uint32_t uart_port); 

void uart_create(uart_port_t uart_num); 
void uart_set_callback(void * callback); 

#endif 