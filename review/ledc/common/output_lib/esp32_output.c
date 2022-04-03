#include "esp32_output.h"
#include "driver/gpio.h"

void esp32_io_config_output_pin(gpio_num_t gpio_num)
{
    gpio_pad_select_gpio(gpio_num); 
    gpio_set_direction(gpio_num, GPIO_MODE_INPUT_OUTPUT); 
}

void esp32_io_set_output(gpio_num_t gpio_num, uint32_t level)
{
    gpio_set_level(gpio_num, level); 
}
void esp32_io_toggle_pin(gpio_num_t gpio_num)
{
    uint32_t old_level = gpio_get_level(gpio_num); 
    gpio_set_level(gpio_num, !old_level); 
}