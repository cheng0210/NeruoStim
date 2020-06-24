#ifndef main_h
#define main_h
#include "ble_comms.h"
#include <stdio.h>
#include "nvs_flash.h"
#include "esp_log.h"
#include <esp32/rom/ets_sys.h>
#include <sys/time.h>
#include "driver/gpio.h"
#include "driver/dac.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define DEVICE_NAME "NeuroStimulator"

uint8_t CHANNEL_NUM;
uint32_t MAX_FREQ;

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

TaskHandle_t STIM_TASK;
bool STIM_TASK_STATUS; //0 IDEL; 1 WORKING

void STIM_START();
void STIM_STOP();
void delay_test();
void biphasic_loop();
#endif
