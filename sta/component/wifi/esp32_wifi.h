#ifndef _ESP32_WIFI_H
#define _ESP32_WIFI_H

#include "esp_err.h"
#include "hal/gpio_types.h"

void wifi_init_sta(const char * wifi_ssid, const char * wifi_pass, uint8_t max_retry); 
#endif