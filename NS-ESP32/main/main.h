#ifndef main_h
#define main_h
#include "ble_comms.h"
#include <stdio.h>
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_nimble_hci.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"

#define DEVICE_NAME "NeuroStimulator"

uint8_t BATTERY_LEVEL;
TickType_t BATTERY_UPDATE_TIME_INTERVAL;//in millisecond

#endif
