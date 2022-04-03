#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

typedef enum
{
    PROVISION_ACCESSPOINT, 
    PROVISION_SMARTCONFIG
} provision_type_t; 

void app_config(void); 

#endif