#include "wifi_comms.h"
void tcp_socket_server(void *pvParameters);
void wifi_init(){
	DISCONNECTED_TIMES = 0;
	esp_log_level_set("WIFI", ESP_LOG_DEBUG);
	initSemaphore = xSemaphoreCreateBinary();
	xTaskCreatePinnedToCore(&wifi_start, "init comms", 1024 * 4, NULL, 15, NULL,0);
	xSemaphoreGive(initSemaphore);
}

static esp_err_t event_handler(void *ctx, system_event_t *event){
	switch (event->event_id)
	{
	case SYSTEM_EVENT_STA_START:
		esp_wifi_connect();
		ESP_LOGI("CONNECTION", "connecting...\n");
		break;

	case SYSTEM_EVENT_STA_CONNECTED:
		ESP_LOGI("CONNECTION", "connected\n");
		break;

	case SYSTEM_EVENT_STA_GOT_IP:
		ESP_LOGI("CONNECTION", "got ip\n");
		xTaskCreatePinnedToCore(tcp_socket_server, "tcp_server", 4096, NULL, 10, NULL,0);
		break;

	case SYSTEM_EVENT_STA_DISCONNECTED:
		ESP_LOGI("CONNECTION", "disconnected\n");
		DISCONNECTED_TIMES++;
		xTaskCreatePinnedToCore(resetWifi, "reset wifi", 2048, NULL, 15, NULL,1);
		break;

	default:
		break;
	}
	return ESP_OK;
}

void connectSTA(char *ssid, char *pass){
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
	wifi_config_t wifi_config;
	memset(&wifi_config, 0, sizeof(wifi_config));
	strcpy((char *)wifi_config.sta.ssid, ssid);
	strcpy((char *)wifi_config.sta.password, pass);
	esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
}

void connectAP()
{
	DISCONNECTED_TIMES = 0;
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
	wifi_config_t wifi_config =
	{
		.ap = {
			.ssid = "NeuroStim",
			.password = "12345678",
			.max_connection = 2,
			.authmode = WIFI_AUTH_WPA_WPA2_PSK}
	};
	esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config);
}

void wifi_start(void *params)
{
	ESP_ERROR_CHECK(nvs_flash_init());
	ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));
	while (true){
		if (xSemaphoreTake(initSemaphore, portMAX_DELAY)){
			
			//need to change to esp_netif_init() when esp-idf update
			tcpip_adapter_init();

			nvs_handle_t nvs;
			nvs_open("wifiCreds", NVS_READWRITE, &nvs);

			size_t ssidLen, passLen;
			char *ssid = NULL, *pass = NULL;

		if (nvs_get_str(nvs, "ssid", NULL, &ssidLen) == ESP_OK){
			if (ssidLen > 0){
				ssid = malloc(ssidLen);
				nvs_get_str(nvs, "ssid", ssid, &ssidLen);
			}
		}

		if (nvs_get_str(nvs, "pass", NULL, &passLen) == ESP_OK){
			if (passLen > 0){
				pass = malloc(passLen);
				nvs_get_str(nvs, "pass", pass, &passLen);
			}
		}

		wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
		ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));

		//disable pwr save to make i2s work
		esp_wifi_set_ps(WIFI_PS_NONE);


		if (ssid != NULL && pass != NULL && DISCONNECTED_TIMES < 3)
		{
			connectSTA(ssid, pass);
			ESP_ERROR_CHECK(esp_wifi_start());
			tcpip_adapter_set_hostname(TCPIP_ADAPTER_IF_STA, DEVICE_NAME);
		}
		else
		{
			connectAP();
			ESP_ERROR_CHECK(esp_wifi_start());
		}
		//xSemaphoreGive(connectionSemaphore);
		if(SERVER_ON == false){
			RegisterEndPoints();
			SERVER_ON = true;
		}
		if (ssid != NULL)
			free(ssid);
		if (pass != NULL)
			free(pass);
		}
	}
}

void resetWifi(void *params)
{
    vTaskDelay(10000 / portTICK_PERIOD_MS);
	//httpd_stop(server);
    esp_wifi_stop();
	xSemaphoreGive(initSemaphore);
    vTaskDelete(NULL);
}

/* void OnConnected(void *para){
	while (true){
		if (xSemaphoreTake(connectionSemaphore, portMAX_DELAY))
		{
			RegisterEndPoints();
			break;
		}
  	}
} */


/**
 * 	modify http server below
 */


static esp_err_t index_url_hit(httpd_req_t *req)
{
    esp_vfs_spiffs_conf_t config = {
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true};
    esp_vfs_spiffs_register(&config);
    char path[520];
    sprintf(path, "/spiffs%s", req->uri);
    if (strcmp(path, "/spiffs/") == 0)
    {
        sprintf(path, "/spiffs/%s", "index.html");
    }
    //style.css
    char *ptr = strrchr(path, '.');
    if (strcmp(ptr, ".css") == 0)
    {
        httpd_resp_set_type(req, "text/css");
    }
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        httpd_resp_send_404(req);
        return ESP_OK;
    }

    char lineRead[256];
    while (fgets(lineRead, sizeof(lineRead), file))
    {
        httpd_resp_sendstr_chunk(req, lineRead);
    }
    httpd_resp_sendstr_chunk(req, NULL);
    esp_vfs_spiffs_unregister(NULL);
    return ESP_OK;
}

static esp_err_t wificonfig_url_hit(httpd_req_t *req)
{
    ESP_LOGI("HTTP", "url %s was hit", req->uri);
    char buf[150];
    memset(&buf, 0, sizeof(buf));
    httpd_req_recv(req, buf, req->content_len);
    char *ssid = strtok(buf, "\r\n");
    char *pass = strtok(NULL, "\r\n");

    ssid = strchr(ssid, '=') + 1;
    pass = strchr(pass, '=') + 1;

    // printf("ssid: %s pass: %s\n", ssid, pass);

    nvs_flash_init();
    nvs_handle_t nvs;
    nvs_open("wifiCreds", NVS_READWRITE, &nvs);
    nvs_set_str(nvs, "ssid", ssid);
    nvs_set_str(nvs, "pass", pass);
    nvs_close(nvs);

    httpd_resp_set_status(req, "303");
    httpd_resp_set_hdr(req, "Location", "/done.html");
    httpd_resp_send(req, NULL, 0);

    xTaskCreatePinnedToCore(resetWifi, "reset wifi", 1024 * 3, NULL, 15, NULL,1);

    return ESP_OK;
}

void RegisterEndPoints(void)
{
    server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.uri_match_fn = httpd_uri_match_wildcard;
    ESP_LOGI("HTTP", "starting server");
    if (httpd_start(&server, &config) != ESP_OK)
    {
        ESP_LOGE("HTTP", "COULD NOT START SERVER");
    }
    httpd_uri_t wificonfig_end_point_config = {
        .uri = "/wificonfig",
        .method = HTTP_POST,
        .handler = wificonfig_url_hit};
    httpd_register_uri_handler(server, &wificonfig_end_point_config);

    httpd_uri_t index_end_point_config = {
        .uri = "/*",
        .method = HTTP_GET,
        .handler = index_url_hit};
    httpd_register_uri_handler(server, &index_end_point_config);
}


/**
 * socket comms modified from example project 'tcp_server' provided by espressif
*/

void tcp_socket_server(void *pvParameters)
{
    char rx_buffer[128];
    char addr_str[128];


	struct sockaddr_in dest_addr;
	dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(SOCKET_PORT);
	inet_ntoa_r(dest_addr.sin_addr, addr_str, sizeof(addr_str) - 1);


	int listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listen_sock < 0) {
		ESP_LOGE("socket", "Unable to create socket: errno %d", errno);
		vTaskDelete(NULL);
	}
	ESP_LOGI("socket", "Socket created");

	int err = bind(listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
	if (err != 0) {
		ESP_LOGE("socket", "Socket unable to bind: errno %d", errno);
		vTaskDelete(NULL);
	}
	ESP_LOGI("socket", "Socket bound, port %d", SOCKET_PORT);

	err = listen(listen_sock, 1);
	if (err != 0) {
		ESP_LOGE("socket", "Error occurred during listen: errno %d", errno);
		vTaskDelete(NULL);
	}
	ESP_LOGI("socket", "Socket listening");

	while (1) {

		struct sockaddr_in6 source_addr; // Large enough for both IPv4 or IPv6
		uint addr_len = sizeof(source_addr);
		int sock = accept(listen_sock, (struct sockaddr *)&source_addr, &addr_len);
		if (sock < 0) {
			ESP_LOGE("socket", "Unable to accept connection: errno %d", errno);
			close(sock);
			continue;
		}
		ESP_LOGI("socket", "Socket accepted");

		int len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
		// Error occurred during receiving
		if (len < 0) {
			ESP_LOGE("socket", "recv failed: errno %d", errno);
			close(sock);
			continue;
		}
		// Connection closed
		else if (len == 0) {
			ESP_LOGI("socket", "Connection closed");
			close(sock);
			continue;
		}
		// Data received
		else {
			rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
			ESP_LOGI("socket", "%s", rx_buffer);

			parse_command(rx_buffer);

			int err = send(sock, rx_buffer, len, 0);
			if (err < 0) {
				ESP_LOGE("socket", "Error occurred during sending: errno %d", errno);
				close(sock);
				continue;
			}
		}
		close(sock);
	}
    vTaskDelete(NULL);
}