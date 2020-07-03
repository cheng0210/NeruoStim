#include "recording.h"
static const char *TAG = "main";
void example_disp_buf(uint8_t* buf, int length);
void recording_init(){
    i2s_config_t i2s_config = {
        .mode = I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN,
        .sample_rate = 16000,
        .bits_per_sample = 16,
        .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,                         
        .communication_format = I2S_COMM_FORMAT_I2S_MSB,
        .dma_buf_count = 2,
        .dma_buf_len = 1024,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1                                //Interrupt level 1
    };
    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
	i2s_set_adc_mode(ADC_UNIT_1, ADC1_CHANNEL_5);
}

void recording(){
    esp_adc_cal_characteristics_t adc_chars;
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
    while(1){
        int raw = adc1_get_raw(ADC1_CHANNEL_5);
        ESP_LOGI(TAG, "adc sample: raw %d, voltage %d mV", raw, esp_adc_cal_raw_to_voltage(raw, &adc_chars));
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}
void example_disp_buf(uint8_t* buf, int length)
{
    printf("======\n");
    for (int i = 0; i < length; i++) {
        printf("%02x ", buf[i]);
        if ((i + 1) % 8 == 0) {
            printf("\n");
        }
    }
    printf("======\n");
}
void adc_dma_i2s_read_raw()
{
  esp_adc_cal_characteristics_t adc_chars;
  int16_t buf[10] = {0};
  size_t n;

  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);

  for(int i = 0; i < 10; i++)
  {
    int raw = adc1_get_raw(ADC_CHANNEL_0);
    ESP_LOGI(TAG, "adc sample %d: raw %d, voltage %d mV", i, raw, esp_adc_cal_raw_to_voltage(raw, &adc_chars));
  }
  i2s_config_t i2s_config = {
    .mode = I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN,
    .sample_rate = 16000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
    .communication_format = I2S_COMM_FORMAT_I2S_MSB,
    .intr_alloc_flags = 0,
    .dma_buf_count = 2,
    .dma_buf_len = 1024,
  };
  ESP_ERROR_CHECK(i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL));
  ESP_ERROR_CHECK(i2s_set_adc_mode(ADC_UNIT_1, ADC1_CHANNEL_0));

  i2s_adc_enable(I2S_NUM_0);
  i2s_read(I2S_NUM_0, buf, sizeof(buf), &n, portMAX_DELAY);
  
  i2s_adc_disable(I2S_NUM_0);

  for(uint32_t i = 0; i < (n / sizeof(buf[0])); i++)
  {
    int16_t raw = ~buf[i] & 0xfff;
    ESP_LOGI(TAG, "adc dma i2s sample %d: raw %d, voltage %d mV", i, raw, esp_adc_cal_raw_to_voltage(raw, &adc_chars));
  }
}