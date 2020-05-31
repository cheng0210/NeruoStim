#ifndef ble_comms_h
#define ble_comms_h
#include "main.h"
#include <stdio.h>
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_nimble_hci.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"

/*
 *Define BLE services/characteristics/descriptors here
 */
#define DEVICE_INFO_SERVICE 0x180A
#define MANUFACTURER_NAME_CHAR 0x2A29
#define HARDWARE_REVISION_CHAR 0x2A27
#define FIRMWARE_REVISION_CHAR 0x2A26


#define BATTRY_SERVICE 0x180F
#define BATTRY_LEVEL_CHAR 0x2A19


/*
 * bluetooth connection handler
 */
uint16_t conn_hdl;

/*
 * attribute handler of battery characteristic
 */
uint16_t batt_char_attr_hdl;

/*
 *  best address type configured by ble_hs_id_infer_auto
 */
uint8_t ble_addr_type;

/*
 * a timer handler which is used to update battery level
 */
xTimerHandle battery_update_timer_handler;


/*
 * functions relate to bluetooth comms
 */ 
void battery_level_notify(); //todo 
void ble_app_advertise(void);
void ble_init(void);

#endif