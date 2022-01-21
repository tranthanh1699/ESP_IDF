#ifndef _OUTPUT_H 
#define _OUTPUT_H
#include "esp_err.h"
#include "hal/gpio_types.h"

void esp_gpio_output_create(gpio_num_t gpio_num); 
void esp_gpio_writepin(gpio_num_t gpio_num, int level); 
void esp_gpio_togglepin(gpio_num_t gpio_num); 

#endif 
