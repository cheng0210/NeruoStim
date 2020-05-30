#include "main.h"
#include <rom/ets_sys.h>
void app_main(){
    BATTERY_LEVEL = 100;
    BATTERY_UPDATE_TIME_INTERVAL = 120000;
    ble_init();
}
