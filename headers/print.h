#ifndef PRINT_H
#define PRINT_H

#include "struct.h"

void printInventory(Inventory *inventory);
void printCurrentRoom(Location *room);
void printItems(Location *room);
void printContainerContents(Item *container, int indent_level);
void printNearbyRooms(Location *location);
void helpCommand();
void welcomeMessage();
#endif