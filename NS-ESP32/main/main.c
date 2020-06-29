#include "main.h"
#include "sys/queue.h"
#include "soc/sens_reg.h"
#include "soc/rtc_io_reg.h"
portMUX_TYPE rtc_spinlock1 = portMUX_INITIALIZER_UNLOCKED;
void app_main(){ // runs in cpu0
    CHANNEL_NUM = 1;
    MAX_FREQ = 100000;//10KHZ
    PHASE_ONE_TIME = 10;// default 10us
    PHASE_TWO_TIME = 10;// default 10us
    STIM_AMP = 2000;// default 0uA
    INTER_PHASE_GAP = 0;//default 0us
    INTER_STIM_DELAY = 100;//default 0us
    ANODIC_CATHODIC = 1;//default cathodic
    STIM_TYPE = 0;//default uniform stim
    PULSE_NUM = 0;//default 0 is forever in ms
    BURST_NUM = 0;// number of burst
    INTER_BURST_DELAY = 0;
    PULSE_NUM_IN_ONE_BURST = 0;
    RAMP_UP = 0;
    SHORT_ELECTRODE = 1;

    //i2c_connection_status = battery_init();
    dac_output_enable(DAC_CHANNEL_1);
    ble_init();//ble host stack is running on cpu0 which will not affect cpu1
    //xTaskCreatePinnedToCore(delay_test, "gpio test", 2048, NULL, 2, NULL, 1);

    /*  while(1){
        vTaskDelay(10000 / portTICK_PERIOD_MS);
        printf("***********************************************************\n");
        printf("%s and %s\n", ANODIC_CATHODIC ? "CATHODIC" : "ANODIC", STIM_TYPE ? "BURST" : "UNIFORM");
        printf("stim amp : %u   phase one time : %u    phase two time: %u\n", STIM_AMP, PHASE_ONE_TIME, PHASE_TWO_TIME);
        printf("inter phase gap : %u   inter stim delay : %u  pulse num : %u\n", INTER_PHASE_GAP, INTER_STIM_DELAY, PULSE_NUM);
        printf("pulse num in one burst : %u     burst num : %u    inter burst delay : %u\n", PULSE_NUM_IN_ONE_BURST,BURST_NUM, INTER_BURST_DELAY);
        printf("ramp up : %s\n",RAMP_UP?"yes":"no");
        printf("short electrode : %s\n",SHORT_ELECTRODE?"yes":"no");
    }  */
}

void STIM_START(){
    xTaskCreatePinnedToCore(biphasic_loop, "biphasic_loop", 2048, NULL, 2, &STIM_TASK, 1);
    STIM_TASK_STATUS = 1;
    printf("started!\n");
}

void STIM_STOP(){
    vTaskDelete(STIM_TASK);
    dac_output_voltage(DAC_CHANNEL_1,125);
    printf("stopped!\n");
}

void IRAM_ATTR biphasic_loop()
{
    CLEAR_PERI_REG_MASK(SENS_SAR_DAC_CTRL1_REG, SENS_SW_TONE_EN);
    CLEAR_PERI_REG_MASK(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_CW_EN1_M);
    while(1){
    
        SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, 255, RTC_IO_PDAC1_DAC_S);
        ets_delay_us(PHASE_ONE_TIME);
        SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, 127, RTC_IO_PDAC1_DAC_S);
        ets_delay_us(INTER_PHASE_GAP);
        SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, 0, RTC_IO_PDAC1_DAC_S);
        ets_delay_us(PHASE_TWO_TIME);
        SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, 127, RTC_IO_PDAC1_DAC_S);
        ets_delay_us(INTER_STIM_DELAY);

        /* dac_output_voltage(DAC_CHANNEL_1, 255);
        ets_delay_us(PHASE_ONE_TIME);
        dac_output_voltage(DAC_CHANNEL_1, 127);
        ets_delay_us(INTER_PHASE_GAP);
        dac_output_voltage(DAC_CHANNEL_1, 0);
        ets_delay_us(PHASE_TWO_TIME);
        dac_output_voltage(DAC_CHANNEL_1, 127);
        ets_delay_us(INTER_STIM_DELAY); */
    }
}

void delay_test()
{
    dac_output_enable(DAC_CHANNEL_1);
    /* while (1)
    {
        //there is a ~16us delay in gettimeofday
        gettimeofday(&tv1, NULL);
        ets_delay_us(i);//this delay function is very accurate
        gettimeofday(&tv2, NULL);
        printf("delay %ld us\n", (tv2.tv_usec - tv1.tv_usec));//delay i+16 us
        i++;
        vTaskDelay(1000 / portTICK_PERIOD_MS); 
        printf("***********************************************************\n");
        printf("%s and %s\n", ANODIC_CATHODIC ? "CATHODIC" : "ANODIC", STIM_TYPE ? "BURST" : "UNIFORM");
        printf("stim amp : %u   phase one time : %u    phase two time: %u\n", STIM_AMP, PHASE_ONE_TIME, PHASE_TWO_TIME);
        printf("inter phase gap : %u   inter stim delay : %u  stim duration : %u\n", INTER_PHASE_GAP, INTER_STIM_DELAY, STIM_DURATION);
        printf("burst time : %u    inter burst delay : %u\n", BURST_TIME, INTER_BURST_DELAY);
        i = 0;
        while(i<255){
            dac_output_voltage(DAC_CHANNEL_1, i);
            dac_output_voltage(DAC_CHANNEL_2, 0);
            float out = ((float) i)/255 * 3.1 + 0.08;
            printf("dac out is %f\n",out);
            vTaskDelay(5000 / portTICK_PERIOD_MS);
            i = i + 1;
        }
           
    } */
    /* int x = 0;
    while(1){
        x = !x;
        switch(x){
            case 1:
                dac_output_voltage(DAC_CHANNEL_1, 255);
                break;
            case 0:
                dac_output_voltage(DAC_CHANNEL_1, 0);
                break;
            default:
                 break;
        }
        //ets_delay_us(20);
    }  */
    gpio_pad_select_gpio(23);
    gpio_set_direction(23, GPIO_MODE_OUTPUT);
    int isOn = 0;
    while (true)
    {
        isOn = !isOn;
        gpio_set_level(23, isOn);
    }
}