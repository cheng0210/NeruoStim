#ifndef utils_h
#define utils_h
#include "main.h"
#include "nvs_flash.h"
#include "nvs.h"

int parse_command(char *command);
char **split(const char *command, char splitter);
void clear_wifi_config();


#endif