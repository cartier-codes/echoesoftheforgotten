#ifndef INITIALISE_H
#define INITIALISE_H

#include "struct.h"

void initialiseCharacter(Character *character, char *name, DialogueTree *Dialogue);
void initialiseDialogue(DialogueTree *root, Character *speaker, char *text, DialogueTree *node_1, DialogueTree *node_2);
void initialiseJournal(Journal *journal);
void initialiseCase(CaseFile *case_file,char *name,char *case_no,char *date, char *lead, char *summary, char *victims, char *location, char *evidence, char *suspects);
void initialiseItemLookup(ItemCollection *itemList);
void initialiseCFLookup(CFCollection *cfList);
void initialiseItem(Item *item, const char *name, bool is_collectable, const char *description,ItemCollection *itemCO, bool is_container);
void addItemToLookup(Item *item, ItemCollection *itemCO);
void initialiseInventory(Inventory *inventory);
void initialiseEMSEvent(EMSEvent *event, EMS *ems, Location *location, enum EventType type, DialogueTree *Dialogue, int dep_in);
void initialiseRoom(Location *room, char *name, char *description, char* locked_item);
void initialisePlayer(Detective *detective, Inventory *inventory, Journal *journal, Location *current_location);
void initialiseEMS(EMS *ems);
#endif