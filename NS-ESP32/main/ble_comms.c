#include "ble_comms.h"


static int device_write(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    printf("incoming message: %.*s\n", ctxt->om->om_len, ctxt->om->om_data);
    return 0;
}

static int device_info(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    os_mbuf_append(ctxt->om, "ESPRESSIF SYSTEMS (SHANGHAI) CO., LTD.", strlen("ESPRESSIF SYSTEMS (SHANGHAI) CO., LTD."));
    return 0;
}

static int battry_level(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    os_mbuf_append(ctxt->om, &BATTERY_LEVEL, sizeof(BATTERY_LEVEL));
    return 0;
}

/*
 * Add BLE services/characteristics/descriptors here
 */
static const struct ble_gatt_svc_def gatt_svcs[] = {
    {.type = BLE_GATT_SVC_TYPE_PRIMARY,
     .uuid = BLE_UUID16_DECLARE(DEVICE_INFO_SERVICE),
     .characteristics = (struct ble_gatt_chr_def[]){
         {.uuid = BLE_UUID16_DECLARE(MANUFACTURER_NAME_CHAR),
          .flags = BLE_GATT_CHR_F_READ,
          .access_cb = device_info},
         {.uuid = BLE_UUID128_DECLARE(0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff),
          .flags = BLE_GATT_CHR_F_WRITE,
          .access_cb = device_write},
         {0}
        }
    },
    // {.type = BLE_GATT_SVC_TYPE_PRIMARY,
    //  .uuid = BLE_UUID128_DECLARE(STIMULATION_COMMAND_SERVICE),
    //  .characteristics = (struct ble_gatt_chr_def[]){
    //      {
    //         .uuid = BLE_UUID128_DECLARE(PULSE_ONE),
    //         .flags = BLE_GATT_CHR_F_WRITE,
    //         .access_cb = device_info
    //       },
    //     {
    //         .uuid = BLE_UUID128_DECLARE(PULSE_TWO),
    //         .flags = BLE_GATT_CHR_F_WRITE,
    //         .access_cb = device_info
    //     },
    //      {0}
    //     }
    // },
    // {
    //     .type = BLE_GATT_SVC_TYPE_PRIMARY,
    //     .uuid = BLE_UUID16_DECLARE(BATTRY_SERVICE),
    //     .characteristics = (struct ble_gatt_chr_def[]){
    //         {.uuid = BLE_UUID16_DECLARE(BATTRY_LEVEL_CHAR),
    //         .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY,
    //         .access_cb = battry_level,
    //         .val_handle = &batt_char_attr_hdl},
    //         {0}
    //     }
    // },
    {0}
    };

static int ble_gap_event(struct ble_gap_event *event, void *arg)
{
    switch (event->type)
    {
    case BLE_GAP_EVENT_CONNECT:
        ESP_LOGI("GAP", "BLE_GAP_EVENT_CONNECT %s", event->connect.status == 0 ? "OK" : "Failed");
        if (event->connect.status != 0)
        {
            ble_app_advertise();
        }
        conn_hdl = event->connect.conn_handle;
        break;
    case BLE_GAP_EVENT_DISCONNECT:
        ESP_LOGI("GAP", "BLE_GAP_EVENT_DISCONNECT");
        xTimerStop(battery_update_timer_handler,0);
        ble_app_advertise();
        break;
    case BLE_GAP_EVENT_ADV_COMPLETE:
        ESP_LOGI("GAP", "BLE_GAP_EVENT_ADV_COMPLETE");
        ble_app_advertise();
        break;
    case BLE_GAP_EVENT_SUBSCRIBE:
        ESP_LOGI("GAP", "BLE_GAP_EVENT_SUBSCRIBE");
        if (event->subscribe.attr_handle == batt_char_attr_hdl)
        {
            xTimerStart(battery_update_timer_handler, 0);
        }
        break;
    default:
        break;
    }
    return 0;
}

void ble_app_advertise(void)
{
    struct ble_hs_adv_fields fields;
    memset(&fields, 0, sizeof(fields));

    fields.flags = BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_DISC_LTD;
    fields.tx_pwr_lvl_is_present = 1;
    fields.tx_pwr_lvl = BLE_HS_ADV_TX_PWR_LVL_AUTO;

    fields.name = (uint8_t *)ble_svc_gap_device_name();
    fields.name_len = strlen(ble_svc_gap_device_name());
    fields.name_is_complete = 1;

    ble_gap_adv_set_fields(&fields);

    struct ble_gap_adv_params adv_params;
    memset(&adv_params, 0, sizeof(adv_params));
    adv_params.conn_mode = BLE_GAP_CONN_MODE_UND;
    adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;

    ble_gap_adv_start(ble_addr_type, NULL, BLE_HS_FOREVER, &adv_params, ble_gap_event, NULL);
}

void ble_app_on_sync(void)
{
    ble_hs_id_infer_auto(0, &ble_addr_type);
    ble_app_advertise();
}

void host_task(void *param)
{
    nimble_port_run();
}


void ble_init(void)
{
    nvs_flash_init();

    esp_nimble_hci_and_controller_init();
    nimble_port_init();

    ble_svc_gap_device_name_set(DEVICE_NAME);
    ble_svc_gap_init();
    ble_svc_gatt_init();

    ble_gatts_count_cfg(gatt_svcs);
    ble_gatts_add_svcs(gatt_svcs);

    ble_hs_cfg.sync_cb = ble_app_on_sync;

    battery_update_timer_handler = xTimerCreate("update_battery_timer", pdMS_TO_TICKS(BATTERY_UPDATE_TIME_INTERVAL), pdTRUE, NULL, battery_level_notify);

    nimble_port_freertos_init(host_task);
}
int xxx = 0;
void battery_level_notify()
{
    printf("%d\n",xxx++);
    struct os_mbuf *om = ble_hs_mbuf_from_flat(&BATTERY_LEVEL, sizeof(BATTERY_LEVEL));
    ble_gattc_notify_custom(conn_hdl, batt_char_attr_hdl, om);
}
