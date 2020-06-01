#include "main.h"
void app_main(){ // runs in cpu0
    CHANNEL_NUM = 1;
    MAX_FREQ = 100000;//10KHZ
    BATTERY_LEVEL = 100;
    BATTERY_UPDATE_TIME_INTERVAL = 120000;
    PHASE_ONE_TIME = 100;// default 100us
    PHASE_TWO_TIME = 100;// default 100us
    STIM_AMP = 0;// default 0uA
    
    ble_init();//ble host stack is running on cpu0 which will not affect cpu1
    xTaskCreatePinnedToCore(delay_test, "delay test", 2048, NULL, 2, NULL, 1); // run delay test on cpu1
}
void delay_test()
{
    printf("delay test is running on %d\n", xPortGetCoreID());
    struct timeval tv1;
    struct timeval tv2;
    int i = 0;
    ets_delay_us(1000);
    while (1)
    {
        //there is a ~16us delay in gettimeofday
        gettimeofday(&tv1, NULL);
        ets_delay_us(i);//this delay function is very accurate
        gettimeofday(&tv2, NULL);
        printf("delay %ld us\n", (tv2.tv_usec - tv1.tv_usec));//delay i+16 us
        i++;
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}