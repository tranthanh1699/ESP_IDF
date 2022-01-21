#ifndef _INPUT_H_
#define _INPUT_H_
#include "esp_err.h"
#include "hal/gpio_types.h"

typedef void (*esp_gpio_input_callback_t)(int pin);  

 typedef enum
 {
     RISING = 1, 
     FALLING = 2, 
     BOTH = 3
 }edge_typedef_t; 

 void esp_gpio_input_create(gpio_num_t gpio_num, edge_typedef_t type); 
 int esp_gpio_readpin(gpio_num_t gpio_num); 
 void esp_gpio_set_callback(void * callback); 

#endif 
