#include "esp32_ledc.h"
#include "driver/gpio.h"
#include "driver/ledc.h"

long esp32_map(long x, long in_min, long in_max, long out_min, long out_max) 
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void esp32_ledc_init(uint32_t frequency, gpio_num_t gpio)
{
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_HIGH_SPEED_MODE, 
        .timer_num = LEDC_TIMER_1, 
        .duty_resolution = LEDC_TIMER_13_BIT, 
        .freq_hz = frequency, 
        .clk_cfg = LEDC_AUTO_CLK, 
    }; 
    ledc_timer_config(&ledc_timer); 

    ledc_channel_config_t ledc_channel = 
    {
        .speed_mode = LEDC_HIGH_SPEED_MODE, 
        .channel = LEDC_CHANNEL_1, 
        .timer_sel = LEDC_TIMER_1, 
        .intr_type = LEDC_INTR_DISABLE, 
        .gpio_num = gpio, 
        .duty = 0, 
        .hpoint = 0, 
    }; 
    ledc_channel_config(&ledc_channel);
}

void esp32_ledc_set_duty(uint32_t duty)
{
    long duty_parse = esp32_map(duty, 0, 100, 0, 8191); 
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, duty_parse); 
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1); 
}

void esp32_ledc_set_frequency(uint32_t frequency)
{
    ledc_set_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_1, frequency); 
}