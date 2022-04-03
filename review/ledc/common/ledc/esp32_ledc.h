#ifndef ESP32_LEDC_H
#define ESP32_LEDC_H
#include <hal/ledc_types.h>
#include <hal/gpio_types.h> 

void esp32_ledc_init(uint32_t frequency, gpio_num_t gpio); 
void esp32_ledc_set_duty(uint32_t duty); 
void esp32_ledc_set_frequency(uint32_t frequency); 
#endif