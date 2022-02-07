#include "esp32_uart.h"
#include <driver/uart.h>
#include "string.h"
#include "esp_err.h"
#include "stdarg.h"
#include "soc/uart_reg.h"
#include "soc/uart_struct.h"
#include "freertos/queue.h"

#define USING_CALLBACK 1
#define RING_BUFFER_SIZE 1024

static intr_handle_t handle_console;


uart_callback_t uart_callback = NULL; 

RINGBUF uart_ring; 
uint8_t uart_buffer[RING_BUFFER_SIZE]; 
uint8_t rxBuffer[124]; 

static QueueHandle_t rxQueueHandle;

static void IRAM_ATTR uart_intr_handle(void * arg)
{
    uart_port_t  port = (uart_port_t)arg;

	uint16_t rx_fifo_len, status;
    status = UART0.int_st.val; // read UART interrupt Status
    rx_fifo_len = UART0.status.rxfifo_cnt; // read number of bytes in UART buffer
    uint16_t i = 0; 
    while(rx_fifo_len){
        uint8_t data = UART0.fifo.rw_byte; // read all bytes
		rxBuffer[i] = data; 
		RINGBUF_Put(&uart_ring, data); 
		i++; 
        rx_fifo_len--;
    }
	rxBuffer[i] = '\0'; 
	uart_clear_intr_status(port, UART_RXFIFO_FULL_INT_CLR|UART_RXFIFO_TOUT_INT_CLR);
	uart_callback(port); 
	esp32_uart_log(port ,"Test from uart: %s\n", rxBuffer);  
}

void esp32_uart_config(uart_port_t uart_num, int baud, int buffer_size)
{
    uart_config_t uart_config = {
		.baud_rate = 115200,
		.data_bits = UART_DATA_8_BITS,
		.parity = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_1,
		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE
	};
    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(uart_num, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
	//Install UART driver, and get the queue.
	ESP_ERROR_CHECK(uart_driver_install(uart_num, buffer_size * 2, 0, 0, NULL, 0));
	// release the pre registered UART handler/subroutine
	ESP_ERROR_CHECK(uart_isr_free(uart_num));
	// register new UART subroutine
	ESP_ERROR_CHECK(uart_isr_register(uart_num, uart_intr_handle, (void*)uart_num, ESP_INTR_FLAG_IRAM, &handle_console));
	// enable RX interrupt
	ESP_ERROR_CHECK(uart_enable_rx_intr(uart_num));

	rxQueueHandle = xQueueCreate(1024, sizeof(char)); 
	RINGBUF_Init(&uart_ring, (uint8_t *)uart_buffer, RING_BUFFER_SIZE); 
	
}   

void esp32_uart_log(uart_port_t uart_num, const char * str,...)
{
    char rx_str[500]; 
	va_list args; 
	va_start(args, str); 
	vsprintf(rx_str, str, args); 
	va_end(args); 
	uart_write_bytes(uart_num, (const char *)rx_str, strlen(rx_str)); 
}

void esp32_uart_set_rx_callback(void * callback)
{
    uart_callback = callback; 
}

uint32_t esp32_uart_available()
{
	return RINGBUF_GetFill(&uart_ring); 
}

void esp32_clear_buffer()
{
	
}

void esp32_uart_read_string(char * str)
{
	while(esp32_uart_available() > 0)
	{
		uint8_t data; 
		RINGBUF_Get(&uart_ring, &data); 
		*str = data; 
		str++; 
	}
}

void esp32_send_char(uint8_t * rxChar)
{
	xQueueSend(rxQueueHandle, rxChar, 100 / portTICK_PERIOD_MS); 
}