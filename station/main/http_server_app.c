#include "http_server_app.h"
#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <sys/param.h>
#include "esp_netif.h"
#include "esp_eth.h"

#include <esp_http_server.h>
/* A simple example that demonstrates how to create GET and POST
 * handlers for the web server.
 */

static const char *TAG = "example";
httpd_handle_t server = NULL;

// extern const uint8_t index_html_start[] asm("_binary_anh_jpeg_start"); 
// extern const uint8_t index_html_end[] asm("_binary_anh_jpeg_end"); 

extern const uint8_t index_html_start[] asm("_binary_index_html_start"); 
extern const uint8_t index_html_end[] asm("_binary_index_html_end"); 

/* An HTTP GET handler */
static esp_err_t dht_get_handler(httpd_req_t *req)
{
    //const char* resp_str = (const char*) "Hello World";
    httpd_resp_set_type(req, "text/html"); 
    httpd_resp_send(req, (const char *)index_html_start, index_html_end - index_html_start);
    //httpd_resp_set_type(req, "image/jpeg"); 
    //httpd_resp_send(req, (const char *)index_html_start, index_html_end - index_html_start);
    return ESP_OK;
}


static const httpd_uri_t get_data_dht11 = {
    .uri       = "/dht11",
    .method    = HTTP_GET,
    .handler   = dht_get_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = NULL
};

static esp_err_t client_dht_get_handler(httpd_req_t *req)
{
    const char * res_str = (const char *)"{\"temp\":\"27.3\",\"humi\":\"80\"}";
    httpd_resp_send(req, res_str, strlen(res_str));
    return ESP_OK;
}


static const httpd_uri_t client_get_data_dht11 = {
    .uri       = "/getdatadht11",
    .method    = HTTP_GET,
    .handler   = client_dht_get_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = NULL
};


static esp_err_t data_post_handler(httpd_req_t *req)
{
    char buf[100];
    int ret, remaining = req->content_len;
    httpd_req_recv(req, buf, req->content_len); 
    printf("Data: %s\n", buf); 
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

static const httpd_uri_t post_data = {
    .uri       = "/data",
    .method    = HTTP_POST,
    .handler   = data_post_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = NULL
};


esp_err_t http_404_error_handler(httpd_req_t *req, httpd_err_code_t err)
{
    if (strcmp("/dht11", req->uri) == 0) {
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "/hello URI is not available");
        /* Return ESP_OK to keep underlying socket open */
        return ESP_OK;
    }
    /* For any other URI send 404 and close socket */
    httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "Some 404 error message");
    return ESP_FAIL;
}

void start_webserver(void)
{
    
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        // Set URI handlers
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &get_data_dht11);
        httpd_register_uri_handler(server, &post_data);
        httpd_register_uri_handler(server, &client_get_data_dht11);
        httpd_register_err_handler(server, HTTPD_404_NOT_FOUND, http_404_error_handler); 
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




