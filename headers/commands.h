#ifndef COMMANDS_H
#define COMMANDS_H

#include "struct.h"
#include "print.h"
#include <string.h>
#include "ems.h"
#include <stdbool.h>
#include "remove.h"
#include <ctype.h>
#include <unistd.h>

void openCommand(Detective *detective, char *token,ItemCollection *itemCO);
void moveCommand(EMS *ems,Detective *detective, char* direction, CFCollection *CFCO);
void examineCommand(SNTRPH *sntrph, Detective *detective, char *token, CFCollection *CFCO);
void dropCommand(EMS *ems, Detective *detective, char *token);
void takeCommand(EMS *ems, Detective *detective, char *token);
void processCommand(EMS *ems, char *str, Detective *detective, ItemCollection *itemList, CFCollection *CFCO, SNTRPH *sntrph); 

#endif