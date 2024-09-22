#ifndef EMS_H
#define EMS_H

#include "dialogue.h"
#include "add.h"
#include <stdio.h>
#include "struct.h"


void updateEMS(EMS *ems,Detective *detective, enum Command command, char *token, CFCollection *CFCO);
#endif