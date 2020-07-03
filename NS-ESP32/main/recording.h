#ifndef recording_h
#define recording_h

#include "main.h"
#include "esp_spi_flash.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_partition.h"
#include "driver/i2s.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#define SAMPLE_RATE 3000
#define I2S_BUF_LEN 8192

void recording_init();
void recording();
void adc_dma_i2s_read_raw();



#endif