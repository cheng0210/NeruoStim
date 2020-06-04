#include "main.h"
void app_main(){ // runs in cpu0
    CHANNEL_NUM = 1;
    MAX_FREQ = 100000;//10KHZ
    BATTERY_LEVEL = 100;
    BATTERY_UPDATE_TIME_INTERVAL = 120000;
    PHASE_ONE_TIME = 100;// default 100us
    PHASE_TWO_TIME = 100;// default 100us
    STIM_AMP = 0;// default 0uA
    INTER_PHASE_GAP = 0;//default 0us
    INTER_STIM_DELAY = 0;//default 0us
    ANODIC_CATHODIC = 0;//default anodic
    STIM_TYPE = 0;//default uniform stim
    STIM_DURATION = 0xffffffff;//default is forever
    BURST_TIME = 0;
    INTER_BURST_DELAY = 0;
    
    ble_init();//ble host stack is running on cpu0 which will not affect cpu1
    xTaskCreatePinnedToCore(delay_test, "delay test", 2048, NULL, 2, NULL, 1); // run delay test on cpu1
}
void delay_test()
{
    while (1)
    {
        //there is a ~16us delay in gettimeofday
        /* gettimeofday(&tv1, NULL);
        ets_delay_us(i);//this delay function is very accurate
        gettimeofday(&tv2, NULL);
        printf("delay %ld us\n", (tv2.tv_usec - tv1.tv_usec));//delay i+16 us
        i++;
        vTaskDelay(1000 / portTICK_PERIOD_MS); */
        printf("***********************************************************\n");
        printf("%s and %s\n",ANODIC_CATHODIC?"CATHODIC":"ANODIC",STIM_TYPE?"BURST":"UNIFORM");
        printf("stim amp : %d   phase one time : %d    phase two time: %d\n",STIM_AMP,PHASE_ONE_TIME,PHASE_TWO_TIME);
        printf("inter phase gap : %d   inter stim delay : %d  stim duration : %u\n",INTER_PHASE_GAP,INTER_STIM_DELAY,STIM_DURATION);
        printf("burst time : %d    inter burst delay : %d\n",BURST_TIME,INTER_BURST_DELAY);
        vTaskDelay(30000/ portTICK_PERIOD_MS);
    }
}