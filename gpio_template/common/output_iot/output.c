#include <stdio.h>
#include <esp_log.h>
#include <driver/gpio.h>
#include "output.h"

void esp_gpio_output_create(gpio_num_t gpio_num)
{
    gpio_pad_select_gpio(gpio_num); 
    gpio_set_direction(gpio_num, GPIO_MODE_INPUT_OUTPUT); 
}
void esp_gpio_writepin(gpio_num_t gpio_num, int level)
{
    gpio_set_level(gpio_num, level); 
}
void esp_gpio_togglepin(gpio_num_t gpio_num)
{
    int level = gpio_get_level(gpio_num); 
    gpio_set_level(gpio_num, !level); 
}