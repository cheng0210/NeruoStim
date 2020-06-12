#include "main.h"
void app_main(){ // runs in cpu0
    CHANNEL_NUM = 1;
    MAX_FREQ = 100000;//10KHZ
    BATTERY_LEVEL = 0;
    BATTERY_UPDATE_TIME_INTERVAL = 120000; //update battery level every 2 mins
    PHASE_ONE_TIME = 100;// default 100us
    PHASE_TWO_TIME = 100;// default 100us
    STIM_AMP = 0;// default 0uA
    INTER_PHASE_GAP = 0;//default 0us
    INTER_STIM_DELAY = 0;//default 0us
    ANODIC_CATHODIC = 1;//default cathodic
    STIM_TYPE = 0;//default uniform stim
    STIM_DURATION = 0xffffffff;//default is forever in ms
    BURST_TIME = 0;
    INTER_BURST_DELAY = 0;

    i2c_connection_status = battery_init();
    
    ble_init();//ble host stack is running on cpu0 which will not affect cpu1
    xTaskCreatePinnedToCore(delay_test, "delay test", 2048, NULL, 2, NULL, 1); // run delay test on cpu1
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
    int x = 0;
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
    } 
    /*gpio_pad_select_gpio(16);
    gpio_set_direction(16, GPIO_MODE_OUTPUT);
    int isOn = 0;
    while (true)
    {
        isOn = !isOn;
        gpio_set_level(16, isOn);
    }*/
}