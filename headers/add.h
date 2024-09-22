#ifndef ADD_H
#define ADD_H

#include "struct.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void addCFToLookup(CFCollection *cfList, CaseFile *cf);
void addItemToContainer(Item *container, Item *item);
void addInnerToRoom(Location *room, Location *inner);
void addIRToEvent(Item *item_required, EMSEvent *event);
void addCFToEvent(CaseFile *cf_req, EMSEvent *event);
void addEventToEMS(EMSEvent *event, EMS *ems);
void addItemToEvent(EMSEvent *event, Item *item, CaseFile *case_file); 
void addToInventory(Item *item, Inventory *inventory);
void addCaseToInventory(CaseFile *file, Inventory *inventory);
void addConnection(Location *room1, Location *room2, char *direction);
void addItemToRoom(Item *item, Location *room);
void addCaseToRoom(CaseFile *case_file, Location *room);
#endif