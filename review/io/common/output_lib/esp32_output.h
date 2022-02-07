#ifndef ESP32_OUTPUT_H
#define OUTPUT_H
#include <hal/gpio_types.h>

void esp32_io_config_output_pin(gpio_num_t gpio_num);
void esp32_io_set_output(gpio_num_t gpio_num, uint32_t level); 
void esp32_io_toggle_pin(gpio_num_t gpio_num); 
#endif