#ifndef command_parse_h
#define command_parse_h
#include "main.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

void parse_command(char *command);
char **split(const char *command, char splitter);

#endif
