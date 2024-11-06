#ifndef REMOVE_H
#define REMOVE_H

#include <stdlib.h>
#include "struct.h"

void removeFromRoom(char* name, int index, Location *location);
void removeFromContainer(Item *container, Item *item);
void removeFromInventoryInternal(Detective *detective, Item *item);
void removeFromInventory(char* name, Detective *detective, Item *item);

#endif