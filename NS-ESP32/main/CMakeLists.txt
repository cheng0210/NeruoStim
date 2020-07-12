idf_component_register(
    SRCS "wifi_comms.c" "battery.c" "recording.c" "utils.c" "main.c"
    INCLUDE_DIRS ""
)
spiffs_create_partition_image(spiffs ../html FLASH_IN_PROJECT)