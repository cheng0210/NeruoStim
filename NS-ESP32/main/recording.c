#include "recording.h"
static const char *TAG = "main";
uint16_t i2s_read_buff[1024];
void example_disp_buf(uint8_t* buf, int length);
void recording_init(){
    i2s_config_t i2s_config = 
    {
      .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN),  // I2S receive mode with ADC
      .sample_rate = 44100,                                                          // set I2S ADC sample rate
      .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,                                 // 16 bit I2S (even though ADC is 12 bit)
      .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,                                 // handle adc data as single channel (right)
      .communication_format = (i2s_comm_format_t)I2S_COMM_FORMAT_I2S,               // I2S format
      .intr_alloc_flags = 0,                                                        // 
      .dma_buf_count = 4,                                                           // number of DMA buffers >=2 for fastness
      .dma_buf_len = 1024,                                                          // number of samples per buffer
      .use_apll = 0,                                                                
    };
    adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_11db);
    adc1_config_width(ADC_WIDTH_12Bit);
    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
	  i2s_set_adc_mode(ADC_UNIT_1, ADC1_CHANNEL_3);
}

void recording(){
    size_t n;
    //i2s_adc_enable(I2S_NUM_0);
    while(1){
        int raw = adc1_get_raw(ADC1_CHANNEL_3);
        printf("%d\n",raw);
        vTaskDelay(500/portTICK_PERIOD_MS);
        /* i2s_read(I2S_NUM_0, &i2s_read_buff, 1024*sizeof(uint16_t), &n, portMAX_DELAY);
        for(uint32_t i = 900; i < (n / sizeof(i2s_read_buff[0])); i++)
        { 
          ESP_LOGI(TAG, "adc dma i2s sample %d: raw %d", i, i2s_read_buff[i]);
        } */
    }
    //i2s_adc_disable(I2S_NUM_0);
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