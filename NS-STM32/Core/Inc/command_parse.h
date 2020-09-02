#ifndef command_parse_h
#define command_parse_h
#include "main.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

#define STIM_HSEM_ID (10U)
#define STIM_HSEM_PROCESS_ID 24U

void parse_command(char *command);
char **split(const char *command, char splitter);

#endif
