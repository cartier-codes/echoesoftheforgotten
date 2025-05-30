#include "headers/add.h"
#include "headers/struct.h"

void addCFToLookup(CFCollection *cfList, CaseFile *cf){
    if(cfList->size == cfList->capacity){
        cfList->capacity *=2;
        cfList->case_files = realloc(cfList->case_files, cfList->capacity * sizeof(CaseFile));
    }
    cfList->case_files[cfList->size] = *cf;
    cfList->size++;
}
void addItemToContainer(Item *container, Item *item) {
    if (container->is_container) { // Ensure we're adding to a container
        if (container->size == container->capacity) {
            container->capacity *= 2; // Double the capacity when full
            container->items = (Item **)realloc(container->items, container->capacity * sizeof(Item *)); // Reallocate space
        }
        container->items[container->size] = item; // Add item pointer to container
        container->size++;
    } else {
        printf("Error: %s is not a container.\n", container->name);
    }
}

void addInnerToRoom(Location *room, Location *inner){
    if(room->loc_cap == room->loc_size){
        room->loc_cap *=2;
        room->inner_locations = (Location **)realloc(room->inner_locations, room->loc_cap * sizeof(Location *)); // Reallocate space
    }
    room->inner_locations[room->loc_size] = inner;
    inner->outer_location = room;
    room->loc_size++;
}

void addIRToEvent(Item *item_required, EMSEvent *event){
    event->item_required = item_required;
}
void addCFToEvent(CaseFile *cf_req, EMSEvent *event){
    event->cf_required= cf_req;
}
void addEventToEMS(EMSEvent *event, EMS *ems){
    if(ems->size == ems->capacity){
        ems->capacity *=2;
        ems->event_queue = realloc(ems->event_queue, ems->capacity * sizeof(EMSEvent));
    }
    ems->event_queue[ems->size] = *event;
    ems->size++;
}
void addItemToEvent(EMSEvent *event, Item *item, CaseFile *case_file) {
    // Check if the event pointer is NULL
    if (event == NULL) {
        fprintf(stderr, "Error: Event pointer is NULL.\n");
        return;
    }

    // Handle case where item is NULL
    if (item == NULL) {
        // Check if case_file is NULL
        if (case_file == NULL) {
            fprintf(stderr, "Error: CaseFile pointer is NULL when adding NULL item.\n");
            return;
        }

        // Resize cf_gained if needed
        if (event->cf_size == event->cf_cap) {
            size_t new_cap = event->cf_cap == 0 ? 1 : event->cf_cap * 2;
            CaseFile *temp = realloc(event->cf_gained, new_cap * sizeof(CaseFile));
            if (temp == NULL) {
                fprintf(stderr, "Error: Failed to resize cf_gained.\n");
                return;
            }
            event->cf_gained = temp;
            event->cf_cap = new_cap;
        }

        // Add case_file to cf_gained
        event->cf_gained[event->cf_size] = *case_file;
        event->cf_size++;
        return;
    }

    // Handle case where item is not NULL
    if (event->size == event->capacity) {
        size_t new_capacity = event->capacity == 0 ? 1 : event->capacity * 2;
        Item *temp = realloc(event->items_gained, new_capacity * sizeof(Item));
        if (temp == NULL) {
            fprintf(stderr, "Error: Failed to resize items_gained.\n");
            return;
        }
        event->items_gained[event->size] = temp;
        event->size++;
        event->capacity = new_capacity;
    }

    // Add item to items_gained
    event->items_gained[event->size] = item;
    event->size++;
}
void addToInventory(Item *item, Inventory *inventory)
{
    if(!item->is_collectable && inventory->size > 3){
        printf("You cannot take this item");
        return;
    }
    if(inventory->size == inventory->capacity){
            inventory->capacity*=2;
            inventory->items =realloc(inventory->items,inventory->capacity * sizeof(Item));
        }
    if (inventory->size < inventory->capacity)
    {
        inventory->items[inventory->size] = item;
        inventory->size++;
    }
    else
    {
        printf("Inventory is full!\n");
    }
}
void addCaseToInventory(CaseFile *file, Inventory *inventory)
{
    if(inventory->cf_size == inventory->cf_cap){
        inventory->cf_cap *=2;
        inventory->case_files =realloc(inventory->case_files,inventory->cf_cap * sizeof(CaseFile));
    }
    if(inventory->cf_cap == 32){
        printf("Inventory is full!\n");
        return;
    }
    inventory->case_files[inventory->cf_size] = *file;
    inventory->cf_size++;

}
void addConnection(Location *room1, Location *room2, char *direction)
{
    if (strcmp(direction, "north") == 0)
    {
        room1->north = room2;
        room2->south = room1;
    }
    else if (strcmp(direction, "east") == 0)
    {
        room1->east = room2;
        room2->west = room1;
    }
    else if (strcmp(direction, "west") == 0)
    {
        room1->west = room2;
        room2->east = room1;
    }
    else if (strcmp(direction, "south") == 0)
    {
        room1->south = room2;
        room2->north = room1;
    }
}
void addItemToRoom(Item *item, Location *room){
if(room->size == room->capacity){
    if(room->capacity == 0)
        room->capacity = 1;
    else
        room->capacity *= 2;
    room->items = (Item **)realloc(room->items, room->capacity * sizeof(Item *));
    if(room->items == NULL) {
        // Handle realloc failure
        perror("realloc failed");
        exit(1);
    }
}

    room->items[room->size] = item;
    room->size++;
}
void addCaseToRoom(CaseFile *case_file, Location *room){
    if(room->case_file_num == room->case_file_cap){
        room->case_file_cap*=2;
        room->case_files =realloc(room->case_files,room->capacity * sizeof(CaseFile));
    }
    room->case_files[room->case_file_num] = *case_file;
    room->case_file_num++;
}