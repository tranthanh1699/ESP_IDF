#ifndef INPUT_IOT_H
#define INPUT_IOT_H 
#include "esp_err.h"
#include "hal/gpio_types.h"

typedef enum
{
    LO_TO_HI = 1, 
    HI_TO_LO = 2, 
    ANY_EDGE = 3
}int_edge_type_t; 

typedef void (*input_callback_t)(int pin); 

void input_io_create(gpio_num_t gpio_num, int_edge_type_t type);

int input_io_get_level(gpio_num_t gpio_num);

void input_set_callback(void * callback);

#endif