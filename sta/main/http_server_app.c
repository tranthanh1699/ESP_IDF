#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <sys/param.h>
#include "esp_netif.h"
#include "esp_eth.h"

#include <esp_http_server.h>
#include "http_server_app.h"

static const char *TAG = "example";
httpd_handle_t server = NULL;



// extern const uint8_t index_html_start[] asm("_binary_hi_gif_start"); 
// extern const uint8_t index_html_end[] asm("_binary_hi_gif_end"); 

extern const uint8_t index_html_start[] asm("_binary_index_html_start"); 
extern const uint8_t index_html_end[] asm("_binary_index_html_end"); 

static server_callback_t callback_dht_data = NULL; 

void server_set_callback(void * callback)
{
    callback_dht_data = callback; 
}

static esp_err_t client_request_handler(httpd_req_t *req)
{
    uint8_t temp, humi; 
    callback_dht_data(&temp, &humi); 
    char buffer[100]; 
    // httpd_resp_set_type(req, "image/gif"); 
    sprintf(buffer, "{\"temp\":\"%d\",\"humi\": \"%d\"}", temp, humi); 
    httpd_resp_send(req, buffer, strlen(buffer));
    return ESP_OK;
}

static const httpd_uri_t client_request_data = {
    .uri       = "/dht11",
    .method    = HTTP_GET,
    .handler   = client_request_handler,
    .user_ctx  = NULL
};


// Index html
static esp_err_t index_handler(httpd_req_t *req)
{

    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, (const char *) index_html_start, index_html_end - index_html_start);
    return ESP_OK;
}

static const httpd_uri_t index_task = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = index_handler,
    .user_ctx  = NULL
};

static esp_err_t led_task_handler(httpd_req_t *req)
{
    char buffer[100]; 
    httpd_req_recv(req, buffer, req->content_len); 
    printf("Data Recevie: %s\n", buffer); 
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

static const httpd_uri_t led_task = {
    .uri       = "/led",
    .method    = HTTP_POST,
    .handler   = led_task_handler,
    .user_ctx  = NULL
};



/* An HTTP POST handler */
// static esp_err_t echo_post_handler(httpd_req_t *req)
// {
//     char buf[100];
//     int ret, remaining = req->content_len;

//     while (remaining > 0) {
//         /* Read the data for the request */
//         if ((ret = httpd_req_recv(req, buf,
//                         MIN(remaining, sizeof(buf)))) <= 0) {
//             if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
//                 /* Retry receiving if timeout occurred */
//                 continue;
//             }
//             return ESP_FAIL;
//         }

//         /* Send back the same data */
//         httpd_resp_send_chunk(req, buf, ret);
//         remaining -= ret;

//         /* Log data received */
//         ESP_LOGI(TAG, "=========== RECEIVED DATA ==========");
//         ESP_LOGI(TAG, "%.*s", ret, buf);
//         ESP_LOGI(TAG, "====================================");
//     }

//     // End response
//     httpd_resp_send_chunk(req, NULL, 0);
//     return ESP_OK;
// }

// static const httpd_uri_t echo = {
//     .uri       = "/echo",
//     .method    = HTTP_POST,
//     .handler   = echo_post_handler,
//     .user_ctx  = NULL
// };


esp_err_t http_404_error_handler(httpd_req_t *req, httpd_err_code_t err)
{
    if (strcmp("/main", req->uri) == 0) {
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "/hello URI is not available");
        /* Return ESP_OK to keep underlying socket open */
        return ESP_OK;
    } else if (strcmp("/echo", req->uri) == 0) {
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "/echo URI is not available");
        /* Return ESP_FAIL to close underlying socket */
        return ESP_FAIL;
    }
    /* For any other URI send 404 and close socket */
    httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "Some 404 error message");
    return ESP_FAIL;
}

void start_webserver(void)
{
    server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        // Set URI handlers
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &client_request_data);
        httpd_register_uri_handler(server, &index_task);
        httpd_register_uri_handler(server, &index_task);
        httpd_register_uri_handler(server, &led_task);
        // httpd_register_uri_handler(server, &echo);
        httpd_register_err_handler(server, HTTPD_404_NOT_FOUND, http_404_error_handler); 
        return server;
    }
    else
    {
        ESP_LOGI(TAG, "Error starting server!");
    }
}

void stop_webserver(void)
{
    httpd_stop(server);
}


