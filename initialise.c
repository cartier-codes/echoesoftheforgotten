#include "headers/initialise.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


void addItemToLookup(Item *item, ItemCollection *itemCO){
    if(itemCO->size == itemCO->capacity){
        itemCO->capacity *=2;
        itemCO->items = (Item **)realloc(itemCO->items, itemCO->capacity * sizeof(Item *));
    }
    itemCO->items[itemCO->size] = item;
    itemCO->size++;
}

void initialiseEMS(EMS *ems){
ems->size = 0;
ems->capacity = 1;
ems->event_queue = (EMSEvent *)malloc(ems->capacity * sizeof(EMSEvent));
}



void initialiseCharacter(struct Character *character, char *name, struct DialogueTree *Dialogue){
    strcpy(character->name, name);
    if(Dialogue != NULL){
    character->Dialogue = *Dialogue;
    }
    else{
        character->Dialogue = (struct DialogueTree){0};
    }
}
void initialiseDialogue(DialogueTree *root, Character *speaker, char *text, DialogueTree *node_1, DialogueTree *node_2, int event_index){
    root->speaker = speaker;
    strcpy(root->text, text);
    root->option_1 = node_1;
    root->option_2 = node_2;
    root->event_index = event_index;
}

void initialiseJournal(Journal *journal) { 
    journal->size = 0; journal->capacity = 1; journal->events = (Event *)malloc(journal->capacity * sizeof(Event)); } 

void initialiseCase(CaseFile *case_file,char *name,char *case_no,char *date, char *lead, char *summary, char *victims, char *location, char *evidence, char *suspects){

strcpy(case_file->name, name);
strcpy(case_file->case_no, case_no);
strcpy(case_file->date, date);
strcpy(case_file->lead, lead);
strcpy(case_file->location, location);
strcpy(case_file->summary, summary);
strcpy(case_file->victims, victims);
strcpy(case_file->evidence, evidence);
strcpy(case_file->suspects, suspects);
}

void initialiseItemLookup(ItemCollection *itemList) {
    itemList->size = 0;
    itemList->capacity = 1;
    itemList->items = (Item **)malloc(itemList->capacity * sizeof(Item *));
}

void initialiseCFLookup(CFCollection *cfList){
    cfList->size = 0;
    cfList->capacity = 1;
    cfList->case_files = (CaseFile *)malloc(cfList->capacity * sizeof(CaseFile));
}

void initialiseItem(Item *item, const char *name, bool is_collectable, const char *description,ItemCollection *itemCO, bool is_container) {
    item->size = 0;
    item->capacity = 1; // Start with capacity 1 for containers
    strcpy(item->name, name);
    strcpy(item->description, description);
    item->is_collectable = is_collectable;
    item->is_container = is_container;
    item->container_index = -1;

    if (is_container) {
        item->items = (Item **)malloc(item->capacity * sizeof(Item *)); // Allocate space for pointers to items
    } else {
        item->items = NULL; // No need for items array if not a container
    }
    addItemToLookup(item,itemCO);

}

void initialiseInventory(Inventory *inventory)
{
    inventory->size = 0;
    inventory->capacity = 32;
    inventory->cf_size = 0;
    inventory->cf_cap = 1;
    inventory->items = (Item **)malloc(inventory->capacity * sizeof(Item *));
    inventory->case_files = (CaseFile *)malloc(inventory->cf_cap * sizeof(CaseFile));
}

void initialiseEMSEvent(EMSEvent *event, EMS *ems, Location *location, enum EventType type, DialogueTree *Dialogue, int dep_in) {  
    event->size = 0;
    event->capacity = 1;
    event->event_id = ems->size;
    event->dependency_index = dep_in;
    event->items_gained = (Item **)malloc(event->capacity * sizeof(Item));
    event->cf_size = 0;
    event->cf_cap = 1;  // Initialize capacity for CaseFile
    event->cf_gained = (CaseFile *)malloc(event->cf_cap * sizeof(CaseFile));
    event->location = location;
    event->type = type;
    event->Dialogue = *Dialogue;
    event->event_state = false;
    event->cf_required = NULL;
    event->item_required = NULL;
}

void initialiseRoom(Location *room, char *name, char *description, char* locked_item)
{

    strcpy(room->name, name);
    strcpy(room->description, description);
    if(locked_item != NULL){
    strcpy(room->locked_item, locked_item);
    }

    room->size = 0, room->case_file_num = 0, room->loc_size= 0;
    room->capacity = 0, room->case_file_cap = 0, room->loc_cap = 1;

    room->items = (Item **)malloc(room->capacity * sizeof(Item *));
    room->case_files = (CaseFile *)malloc(room->capacity * sizeof(CaseFile));
    room->inner_locations = (Location **)malloc(room->loc_cap * sizeof(Location *));

    room->north = NULL;
    room->east = NULL;
    room->west = NULL;
    room->south = NULL;

}
void initialisePlayer(Detective *detective, Inventory *inventory, Journal *journal, Location *current_location)
{
    detective->inventory = *inventory;
    detective->journal = *journal;
    detective->current_location = current_location;
}

