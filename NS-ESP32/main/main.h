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
#include <esp32/rom/ets_sys.h>
#include <sys/time.h>
#include "driver/gpio.h"
#include "driver/dac.h"

#define DEVICE_NAME "NeuroStimulator"

uint8_t CHANNEL_NUM;
uint32_t MAX_FREQ;

uint8_t BATTERY_LEVEL;
TickType_t BATTERY_UPDATE_TIME_INTERVAL;//in millisecond

uint16_t STIM_AMP;
uint32_t PHASE_ONE_TIME;
uint32_t PHASE_TWO_TIME;
uint32_t INTER_PHASE_GAP;
uint32_t INTER_STIM_DELAY;
uint32_t STIM_DURATION;
uint8_t ANODIC_CATHODIC;
uint8_t STIM_TYPE;
uint32_t BURST_TIME;
uint32_t INTER_BURST_DELAY;





void delay_test();

#endif
