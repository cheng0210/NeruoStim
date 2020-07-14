#ifndef wifi_comms_h
#define wifi_comms_h

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "main.h"
#include "esp_event_loop.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "cJSON.h"
#include "esp_http_server.h"
#include "esp_spiffs.h"
#include "nvs.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>
#include "utils.h"


xSemaphoreHandle connectionSemaphore;
xSemaphoreHandle initSemaphore;

uint8_t DISCONNECTED_TIMES;
httpd_handle_t server;
bool SERVER_ON;
uint16_t SOCKET_PORT;

void wifi_init();
void wifi_start(void *params);
void RegisterEndPoints(void);
void resetWifi(void *params);

#endif