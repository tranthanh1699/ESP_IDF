#ifndef HTTP_SERVER_APP_H
#define HTTP_SERVER_APP_H

typedef void (*server_callback_t)(uint8_t * temp, uint8_t * humi);


void start_webserver(void);
void stop_webserver(void);
void server_set_callback(void * callback);

#endif