#include "ble_comms.h"

static int manufacturer_name(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    os_mbuf_append(ctxt->om, "ESPRESSIF SYSTEMS (SHANGHAI) CO., LTD.", strlen("ESPRESSIF SYSTEMS (SHANGHAI) CO., LTD."));
    return 0;
}

static int firmware_revision(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    os_mbuf_append(ctxt->om, "V0.1", strlen("V0.1"));
    return 0;
}

static int hardware_revision(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    os_mbuf_append(ctxt->om, "ESP32 REV1", strlen("ESP32 REV1"));
    return 0;
}

static int software_revision(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    os_mbuf_append(ctxt->om, "ESP-IDF V4.0.1", strlen("ESP-IDF V4.0.1"));
    return 0;
}

static int channel_number(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    char buffer[8];
    sprintf(buffer, "%d", CHANNEL_NUM);
    os_mbuf_append(ctxt->om,&buffer[0],strlen(buffer));
    return 0;
}

static int maximum_frequency(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    char buffer[8];
    sprintf(buffer, "%d", MAX_FREQ);
    os_mbuf_append(ctxt->om, &buffer[0], strlen(buffer));
    return 0;
}

static int ota_support(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    os_mbuf_append(ctxt->om,"No", strlen("No"));
    return 0;
}

static int battry_level(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    os_mbuf_append(ctxt->om, &BATTERY_LEVEL, sizeof(BATTERY_LEVEL));
    return 0;
}

static int set_phase_one(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    printf("setting phase one time as ");
    PHASE_ONE_TIME = atoi((char *)ctxt->om->om_data);
    printf("%d microseconds\n", PHASE_ONE_TIME);
    return 0;
}

static int read_phase_one(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    char buffer[8];
    sprintf(buffer, "%d", PHASE_ONE_TIME);
    os_mbuf_append(ctxt->om, &buffer[0], strlen(buffer));
    return 0;
}

static int set_phase_two(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    printf("setting phase two time as ");
    PHASE_TWO_TIME = atoi((char *)ctxt->om->om_data);
    printf("%d microseconds\n", PHASE_TWO_TIME);
    return 0;
}

static int read_phase_two(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    char buffer[8];
    sprintf(buffer, "%d", PHASE_TWO_TIME);
    os_mbuf_append(ctxt->om, &buffer[0], strlen(buffer));
    return 0;
}

static int set_stim_amp(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    printf("setting stim amp as ");
    STIM_AMP = atoi((char *)ctxt->om->om_data);
    printf("%d uA\n", STIM_AMP);
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
          .access_cb = manufacturer_name},
         {.uuid = BLE_UUID16_DECLARE(FIRMWARE_REVISION_CHAR),
          .flags = BLE_GATT_CHR_F_READ,
          .access_cb = firmware_revision},
         {.uuid = BLE_UUID16_DECLARE(HARDWARE_REVISION_CHAR),
          .flags = BLE_GATT_CHR_F_READ,
          .access_cb = hardware_revision},
         {.uuid = BLE_UUID16_DECLARE(SOFTWARE_REVISION_CHAR),
          .flags = BLE_GATT_CHR_F_READ,
          .access_cb = software_revision},
         {.uuid = BLE_UUID128_DECLARE(CHANNEL_NUM_CHAR),
          .flags = BLE_GATT_CHR_F_READ,
          .access_cb = channel_number},
         {.uuid = BLE_UUID128_DECLARE(MAX_FREQ_CHAR),
          .flags = BLE_GATT_CHR_F_READ,
          .access_cb = maximum_frequency},
         {.uuid = BLE_UUID128_DECLARE(OTA_SUPPORT_CHAR),
          .flags = BLE_GATT_CHR_F_READ,
          .access_cb = ota_support},
         {0}}},
    {.type = BLE_GATT_SVC_TYPE_PRIMARY, 
     .uuid = BLE_UUID16_DECLARE(BATTRY_SERVICE), 
     .characteristics = (struct ble_gatt_chr_def[]){
         {.uuid = BLE_UUID16_DECLARE(BATTRY_LEVEL_CHAR),
          .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY,
          .access_cb = battry_level, 
          .val_handle = &batt_char_attr_hdl},
         {0}}},
    {.type = BLE_GATT_SVC_TYPE_PRIMARY,
     .uuid = BLE_UUID128_DECLARE(STIMULATION_COMMAND_SERVICE),
     .characteristics = (struct ble_gatt_chr_def[]){
         {.uuid = BLE_UUID128_DECLARE(PHASE_ONE_WRITE_CHAR),
          .flags = BLE_GATT_CHR_F_WRITE,
          .access_cb = set_phase_one},
         {.uuid = BLE_UUID128_DECLARE(PHASE_ONE_READ_CHAR),
          .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY,
          .access_cb = read_phase_one},
         {.uuid = BLE_UUID128_DECLARE(PHASE_TWO_WRITE_CHAR),
          .flags = BLE_GATT_CHR_F_WRITE,
          .access_cb = set_phase_two},
         {.uuid = BLE_UUID128_DECLARE(PHASE_ONE_READ_CHAR),
          .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY,
          .access_cb = read_phase_two},
         {.uuid = BLE_UUID128_DECLARE(STIM_AMP_WRITE_CHAR),
          .flags = BLE_GATT_CHR_F_WRITE,
          .access_cb = set_stim_amp},
         {0}}},
    {0}};

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