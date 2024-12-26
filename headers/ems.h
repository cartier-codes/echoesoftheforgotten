#ifndef EMS_H
#define EMS_H

#include "dialogue.h"
#include "add.h"
#include <stdio.h>
#include "struct.h"

void triggerEMS(EMS *ems, int event_index, Detective *detective, enum EventType event);
void updateEMS(EMS *ems,Detective *detective, enum Command command, char *token, CFCollection *CFCO);
#endif