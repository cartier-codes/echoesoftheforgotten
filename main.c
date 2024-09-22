#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <windows.h>
/* Detective text-adventure game, where our detective is investigating a cold case from the 1990's about a missing child. Eventually, they realise it's themselves. Retracing their entire history, to find the perpetrator.*/ /* Forward declarations */ typedef struct Inventory Inventory; typedef struct Journal Journal; typedef struct Location Location; typedef struct Item Item; typedef struct Event Event; typedef struct Detective Detective; typedef struct ItemCollection ItemCollection; typedef struct CaseFile CaseFile; typedef struct DialogueTree DialogueTree;typedef struct EMSEvent EMSEvent;typedef struct Character Character;typedef struct EMS EMS; typedef struct CFCollection CFCollection;
//FIX DROP COMMAND SEG FAULT
struct Item { char name[100]; int size; int capacity; bool is_collectable; char description[5000]; bool is_container; Item **items; int container_index;};
struct CaseFile{char name[100]; char case_no [20]; char date[20]; char lead[50]; char location[200]; char summary[1000]; char victims[500];  char evidence[601]; char suspects[500];};
struct Weapon { char name[100]; char description[1000]; int damage; }; 
struct Event { char name[100]; char content[1000]; char timestamp[80]; };
struct Journal { int size; int capacity; Event *events; }; 
struct Inventory { int size; int cf_size; int cf_cap;int capacity; Item **items; CaseFile *case_files;};
struct Location { char name[100]; char description[1000]; char locked_item[100]; int size; int capacity; int case_file_num; int case_file_cap; CaseFile *case_files; Item **items; Location *north; Location *west; Location *east; Location *south; };
struct ItemCollection{ int size; int capacity; Item **items; };
struct CFCollection{ int size; int capacity; CaseFile *case_files; };
struct Detective { Inventory inventory; Journal journal; Location *current_location; };

#define COLOR_BOLD  "\e[1m"
#define COLOR_OFF   "\e[m"
enum EventType{
    DIALOGUE,
    COMBAT,
    RANDOM,
};
enum Command{
    MOVE,
    TAKE,
    DROP,
    EXAMINE,
    OPEN,
};
struct DialogueTree{
    Character *speaker;
    char text[500];
    DialogueTree *option_1;
    DialogueTree *option_2;
};
struct Character{char name[50]; DialogueTree Dialogue;};
struct EMSEvent{
    int cf_size;
    int cf_cap;
    int size;
    int capacity;
    int dependency_index;
    int event_id;
    bool event_state;
    enum EventType type;
    CaseFile *cf_gained;
    Item **items_gained;
    Item *item_required;
    CaseFile *cf_required;
    DialogueTree Dialogue;
    Location *location;
};
struct EMS{
    EMSEvent *event_queue;
    int size;
    int capacity;
};

void initialiseCharacter(Character *character, char *name, DialogueTree *Dialogue){
    strcpy(character->name, name);
    if(Dialogue != NULL){
    character->Dialogue = *Dialogue;
    }
    else{
        character->Dialogue = (DialogueTree){0};
    }
}

void initialiseDialogue(DialogueTree *root, Character *speaker, char *text, DialogueTree *node_1, DialogueTree *node_2){
    root->speaker = speaker;
    strcpy(root->text, text);
    root->option_1 = node_1;
    root->option_2 = node_2;
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

void addCFToLookup(CFCollection *cfList, CaseFile *cf){
    if(cfList->size == cfList->capacity){
        cfList->capacity *=2;
        cfList->case_files = realloc(cfList->case_files, cfList->capacity * sizeof(CaseFile));
    }
    cfList->case_files[cfList->size] = *cf;
    cfList->size++;
}
void traverseDialogue(DialogueTree *root){
    if(root == NULL){
        printf("Error");
        return;
    }
    if(strcmp(root->speaker->name,"Sr Detective. Kruger") == 0){
        printf("%s: %s\n", root->speaker->name, root->text);
        if(root->option_1 == NULL){
            return;
        }
        else{
            traverseDialogue(root->option_1);
        }
    }
    else{
        printf("%s: %s\n", root->speaker->name, root->text);
        if(root->option_1 == NULL & root->option_2 == NULL){
            return;
        }
        else if(root->option_2 == NULL){
            traverseDialogue(root->option_1);
        }
        else{
        printf("Option 1: %s\n\n", root->option_1->text);
        printf("Option 2: %s\n\n", root->option_2->text);
        printf("\nEnter 1/2: ");
        char res[10];
        fgets(res, sizeof(res), stdin);
        res[strcspn(res,"\n")] = 0;
        if(strcmp(res, "1") == 0){
            traverseDialogue(root->option_1);
        }
        else if(strcmp(res, "2") == 0){
            traverseDialogue(root->option_2);
        }
        }
    }
}
void addItemToLookup(Item *item, ItemCollection *itemCO){
    if(itemCO->size == itemCO->capacity){
        itemCO->capacity *=2;
        itemCO->items = (Item **)realloc(itemCO->items, itemCO->capacity * sizeof(Item *));
    }
    itemCO->items[itemCO->size] = item;
    itemCO->size++;
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

void addIRToEvent(Item *item_required, EMSEvent *event){
    event->item_required = item_required;
}

void addCFToEvent(CaseFile *cf_req, EMSEvent *event){
    event->cf_required= cf_req;
}
void initialiseEMS(EMS *ems){
ems->size = 0;
ems->capacity = 1;
ems->event_queue = (EMSEvent *)malloc(ems->capacity * sizeof(EMSEvent));
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


void updateEMS(EMS *ems,Detective *detective, enum Command command, char *token, CFCollection *CFCO){
    switch(command){
        case MOVE:
            for(int i = 0; i < ems->size; i++){
                switch(ems->event_queue[i].type){
                case DIALOGUE:
                if((ems->event_queue[i].location == detective->current_location && ems->event_queue[i].event_state == false) && (ems->event_queue[i].dependency_index < 0)){
                    traverseDialogue(&ems->event_queue[i].Dialogue);
                if(ems->event_queue[i].cf_size > 0){
                        for(int j =0; j < ems->event_queue[i].cf_size; j++){
                            addCaseToInventory(&ems->event_queue[i].cf_gained[j], &detective->inventory);
                        }
                    }
                    if(ems->event_queue[i].size>0){
                        for(int j= 0; j < ems->event_queue[i].size; j++){
                            addToInventory(ems->event_queue[i].items_gained[j], &detective->inventory);
                        }
                    }
                    printf("\nYou have acquired items.\n");
                    printf("Before setting: event_state = %d\n", ems->event_queue[i].event_state); // Debugging
                    ems->event_queue[i].event_state = true;
                    printf("After setting: event_state = %d\n", ems->event_queue[i].event_state); // Debugging
                };
                break;
                }
            }
        break;
        case EXAMINE:
            for(int i = 0; i < ems->size; i ++){
                switch(ems->event_queue[i].type){
                        case DIALOGUE:
                            if((!ems->event_queue[i].event_state)&&(ems->event_queue[ems->event_queue[i].dependency_index].event_state)){
                                traverseDialogue(&ems->event_queue[i].Dialogue);
                                ems->event_queue[i].event_state = true;
                                }
                            break;
                }
            }
        break;
}
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

void welcomeMessage()
{
    FILE *file = fopen("detective.txt", "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }
    char line[256];
     // Buffer to store each line
     printf("\n\n\n\n");
    while (fgets(line, sizeof(line), file))
    {
        printf("%s", line); // Print each line
    }
    fclose(file);
}

void initialiseRoom(Location *room, char *name, char *description, char* locked_item)
{

    strcpy(room->name, name);
    strcpy(room->description, description);
    if(locked_item != NULL){
    strcpy(room->locked_item, locked_item);
    }
    room->size = 0;
    room->capacity = 1;
    room->case_file_num = 0;
    room->case_file_cap = 1;
    room->items = (Item **)malloc(room->capacity * sizeof(Item *));
    room->case_files = (CaseFile *)malloc(room->capacity * sizeof(CaseFile));
    room->north = NULL;
    room->east = NULL;
    room->west = NULL;
    room->south = NULL;
}

void addItemToRoom(Item *item, Location *room){
    if(room->size == room->capacity){
        room->capacity*=2;
        room->items = (Item **)realloc(room->items, room->capacity * sizeof(Item *));
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


void printInventory(Inventory *inventory)
{
    printf("Inventory:\n");
    for (int i = 0; i < inventory->size; i++)
    {
        printf("- %s: %s\n", inventory->items[i]->name, inventory->items[i]->description);
    }
    for(int i = 0; i < inventory->cf_size; i++){
        printf("- Case File #%d: %s\n", inventory->cf_size, inventory->case_files[i].name);
    }
    printf("- Journal: The leather-bound journal is a well-worn companion, its cover scuffed from years of use; inside, the pages hold a meticulous record of the detective's thoughts and observations.\n");
}

void printCurrentRoom(Location *room){
    printf("\nCurrent room: %s\n", room->name);
}

void printContainerContents(Item *container, int indent_level) {
    for (int i = 0; i < container->size; i++) {
        for (int j = 0; j < indent_level; j++) {
            printf("\t"); // Print indentation
        }
        printf("\n  Item #%d in %s: %s\n", i + 1, container->name, container->items[i]->name);

        if (container->items[i]->is_container && container->items[i]->size > 0) {
            printContainerContents(container->items[i], indent_level + 1); // Recursively print container contents
        }
    }
}
void printItems(Location *room)
{
    printf("Items in %s: \n\n",room->name);
    for(int i = 0; i < room->size; i++){
        printf("%s: %s\n",room->items[i]->name,room->items[i]->description);
        if(room->items[i]->is_container){
            printContainerContents(room->items[i],0);
        }
    }

}

void initialisePlayer(Detective *detective, Inventory *inventory, Journal *journal, Location *current_location)
{
    detective->inventory = *inventory;
    detective->journal = *journal;
    detective->current_location = current_location;
}

void printNearbyRooms(Location *location){
    printf("\nExits:\n\n");
    if (location->north != NULL) {
        printf("- North: %s\n", location->north->name);
    }
    if (location->south != NULL) {
        printf("- South: %s\n", location->south->name);
    }
    if (location->east != NULL) {
        printf("- East: %s\n", location->east->name);
    }
    if (location->west != NULL) {
        printf("- West: %s\n", location->west->name);
    }
    printf("\n");
}

void openCommand(Detective *detective, char *token,ItemCollection *itemCO){
    if(token != NULL){
        if(strcmp(token, "inventory") == 0){
            printInventory(&detective->inventory);
        }
        char *prefix = "Case File";
        size_t prelen = strlen(prefix);
        if(strncmp(token,prefix,prelen) == 0){
            printf("Use command EXAMINE instead.\n");
        }
        for(int i = 0; i < itemCO->size; i++){
            if(strcmp(token, itemCO->items[i]->name) == 0){
                if(itemCO->items[i]->is_container){
                        printContainerContents(itemCO->items[i],0);
                    }
                else{
                    printf("This item cannot be opened.\n");
                    return;
                }
                }
            }
        }
    }

void removeFromRoom(char* name, int index, Location *location) {
    if(index != -1){
        for (int j = index; j < location->size - 1; j++) {
            location->items[j] = location->items[j + 1];
        }
        location->size--;
        printf("Item was removed from room\n");
        return;
    }
    for (int i = 0; i < location->size; i++) {
        if (strcmp(location->items[i]->name, name) == 0) {
            // Shift remaining items to the left
            for (int j = i; j < location->size - 1; j++) {
                location->items[j] = location->items[j + 1];
            }
            location->size--;
        }
    }
    }


void removeFromContainer(Item *container, Item *item){
    for (int i = 0; i < container->size; i++) {
    if (strcmp(container->items[i]->name, item->name) == 0) {
        // Shift remaining items to the left
        for (int j = i; j < container->size - 1; j++) {
            container->items[j] = container->items[j + 1];
            }
        container->size--;
        printf("Item was removed \n");
        }
    }
}
void removeFromInventoryInternal(Detective *detective, Item *item){
    for (int i = 0; i < detective->inventory.size; i++) {
        if(strcmp(item->name,detective->inventory.items[i]->name) == 0){
            addItemToRoom(detective->inventory.items[i], detective->current_location);
            for (int j = i; j < detective->inventory.size - 1; j++) {
                detective->inventory.items[j] = detective->inventory.items[j + 1];
            }
            detective->inventory.size--;
            printf("\nItem was removed.\n");
            return;
        }
    }
}
void removeFromInventory(char* name, Detective *detective, Item *item) {
    if(item != NULL){
    for (int i = 0; i < detective->inventory.size; i++) {
        if(strcmp(item->name,detective->inventory.items[i]->name) == 0){
            addItemToRoom(detective->inventory.items[i], detective->current_location);
            for (int j = i; j < detective->inventory.size - 1; j++) {
                detective->inventory.items[j] = detective->inventory.items[j + 1];
            }
            detective->inventory.size--;
            printf("\nItem was removed.\n");
            return;
        }
    }
    }
    if((strcmp(name,"") != 0)&&(item == NULL)){
    for (int i = 0; i < detective->inventory.size; i++) {
        if (strcmp(detective->inventory.items[i]->name, name) == 0) {
            // Shift remaining items to the left
            if(!detective->inventory.items[i]->is_collectable){
                printf("This item cannot be dropped.\n");
                return;
            }
            addItemToRoom(detective->inventory.items[i], detective->current_location);
            for (int j = i; j < detective->inventory.size - 1; j++) {
                detective->inventory.items[j] = detective->inventory.items[j + 1];
            }
            detective->inventory.size--;
            printf("\nItem was removed.\n");
            return;
        }
    }
    if(strcmp("Journal", name) == 0){
        printf("This item cannot be dropped.\n");
        return;
    }
    printf("\nItem was not found.\n");
    }
}

void moveCommand(EMS *ems,Detective *detective, char* direction, CFCollection *CFCO) {
    if (direction != NULL) {
        if (strcmp(direction, "north") == 0) {
            if (detective->current_location->north != NULL) {
                if (strcmp(detective->current_location->north->locked_item, "") != 0) {
                    bool hasKey = false;
                    for (int i = 0; i < detective->inventory.size; i++) {
                        if (strcmp(detective->inventory.items[i]->name, detective->current_location->north->locked_item) == 0) {
                            detective->current_location = detective->current_location->north;
                            printf("\nYou have moved North.\n");
                            printCurrentRoom(detective->current_location);
                            printItems(detective->current_location);
                            printNearbyRooms(detective->current_location);
                            hasKey = true;
                            updateEMS(ems, detective, MOVE,"",CFCO);
                            break;
                        }
                    }
                    if (!hasKey) {
                        printf("This door is locked.\n");
                    }
                } else {
                    detective->current_location = detective->current_location->north;
                    printf("\nYou have moved North.\n");
                    printCurrentRoom(detective->current_location);
                    printItems(detective->current_location);
                    printNearbyRooms(detective->current_location);
                    updateEMS(ems, detective, MOVE,"",CFCO);
                }
            } else {
                printf("You can't move North.\n");
            }
        } else if (strcmp(direction, "east") == 0) {
            if (detective->current_location->east != NULL) {
                if (strcmp(detective->current_location->east->locked_item, "") != 0) {
                    bool hasKey = false;
                    for (int i = 0; i < detective->inventory.size; i++) {
                        if (strcmp(detective->inventory.items[i]->name, detective->current_location->east->locked_item) == 0) {
                            detective->current_location = detective->current_location->east;
                            printf("\nYou have moved East.\n");
                            printCurrentRoom(detective->current_location);
                            printItems(detective->current_location);
                            printNearbyRooms(detective->current_location);
                            hasKey = true;
                            updateEMS(ems, detective, MOVE,"",CFCO);
                            break;
                        }
                    }
                    if (!hasKey) {
                        printf("This door is locked.\n");
                    }
                } else {
                    detective->current_location = detective->current_location->east;
                    printf("\nYou have moved East.\n");
                    printCurrentRoom(detective->current_location);
                    printItems(detective->current_location);
                    printNearbyRooms(detective->current_location);
                    updateEMS(ems, detective, MOVE,"",CFCO);
                }
            } else {
                printf("You can't move East.\n");
            }
        } else if (strcmp(direction, "west") == 0) {
            if (detective->current_location->west != NULL) {
                if (strcmp(detective->current_location->west->locked_item, "") != 0) {
                    bool hasKey = false;
                    for (int i = 0; i < detective->inventory.size; i++) {
                        if (strcmp(detective->inventory.items[i]->name, detective->current_location->west->locked_item) == 0) {
                            detective->current_location = detective->current_location->west;
                            printf("\nYou have moved West.\n");
                            printCurrentRoom(detective->current_location);
                            printItems(detective->current_location);
                            printNearbyRooms(detective->current_location);
                            hasKey = true;
                            updateEMS(ems, detective, MOVE,"",CFCO);
                            break;
                        }
                    }
                    if (!hasKey) {
                        printf("This door is locked.\n");
                    }
                } else {
                    detective->current_location = detective->current_location->west;
                    printf("\nYou have moved West.\n");
                    printCurrentRoom(detective->current_location);
                    printItems(detective->current_location);
                    printNearbyRooms(detective->current_location);
                    updateEMS(ems, detective, MOVE,"",CFCO);
                }
            } else {
                printf("You can't move West.\n");
            }
        } else if (strcmp(direction, "south") == 0) {
            if (detective->current_location->south != NULL) {
                if (strcmp(detective->current_location->south->locked_item, "") != 0) {
                    bool hasKey = false;
                    for (int i = 0; i < detective->inventory.size; i++) {
                        if (strcmp(detective->inventory.items[i]->name, detective->current_location->south->locked_item) == 0) {
                            detective->current_location = detective->current_location->south;
                            printf("\nYou have moved South.\n");
                            printCurrentRoom(detective->current_location);
                            printItems(detective->current_location);
                            printNearbyRooms(detective->current_location);
                            hasKey = true;
                    updateEMS(ems, detective, MOVE,"",CFCO);
                            break;
                        }
                    }
                    if (!hasKey) {
                        printf("This door is locked.\n");
                    }
                } else {
                    detective->current_location = detective->current_location->south;
                    printf("\nYou have moved South.\n");
                    printCurrentRoom(detective->current_location);
                    printItems(detective->current_location);
                    printNearbyRooms(detective->current_location);
                    updateEMS(ems, detective, MOVE,"",CFCO);
                }
            } else {
                printf("You can't move South.\n");
            }
        } else {
            printf("Invalid direction.\n");
        }
    }
}

void examineCommand(EMS *ems ,Detective *detective, char *token, CFCollection *CFCO)
{
    char *prefix = "Case File #";
    size_t str_len = strlen(token);
    size_t prelen = strlen(prefix);

    if(strncmp(token, prefix, prelen) == 0){
        char num_char = token[str_len - 1];
        if(isdigit(num_char)){
            int cf_num = num_char - '0';
            if(cf_num < 1 || cf_num > detective->inventory.cf_size){
                printf("You do not have this case file.\n");
                return;
            }
            
            strcpy(token,detective->inventory.case_files[cf_num-1].name);
            printf("\n==================================================================================================================================================================");
            printf("\n\nCase File #%d: %s", cf_num, detective->inventory.case_files[cf_num - 1].name);
            printf("\nCase ID: %s\n\n", detective->inventory.case_files[cf_num - 1].case_no);
            printf("Lead: %s\n", detective->inventory.case_files[cf_num - 1].lead);
            printf("%s\n", detective->inventory.case_files[cf_num - 1].location);
            printf("Summary: %s\n\n", detective->inventory.case_files[cf_num - 1].summary);
            printf("%s\n", detective->inventory.case_files[cf_num - 1].victims);
            printf("%s\n\n", detective->inventory.case_files[cf_num - 1].evidence);
            printf("%s\n\n", detective->inventory.case_files[cf_num - 1].suspects);  // Corrected
            printf("==================================================================================================================================================================\n\n");
            Sleep(2000);
            updateEMS(ems, detective, EXAMINE, token, CFCO);
        }
    }
}

void dropCommand(EMS *ems, Detective *detective, char *token){
char *item = strtok(token, " ");
int item_index = -1;  // Initialize to -1 to indicate not found

if (item == NULL) {
    printf("Invalid command format.\n");
    return;
}

for (int i = 0; i < detective->inventory.size; i++) {
    if (strcmp(item, detective->inventory.items[i]->name) == 0) {
        item_index = i;
        break;
    }
}

if (item_index == -1) {
    char *tail_end = strtok(NULL, " ");
    if (tail_end) {
        char two_worded[100];
        snprintf(two_worded, sizeof(two_worded), "%s %s", item, tail_end);
        for (int j = 0; j < detective->inventory.size; j++) {
            if (strcmp(two_worded, detective->inventory.items[j]->name) == 0) {
                item_index = j;
                break;
            }
        }
    }
}

if (item_index == -1) {
    printf("You do not have this item\n");
    return;
}

char *location = strtok(NULL, "");

if (location == NULL) {
        printf("%d: %s",item_index,detective->inventory.items[item_index]);
        removeFromInventory("",detective,detective->inventory.items[item_index]);
    return;
}

int found_location = 0;
for (int i = 0; i < detective->current_location->size; i++) {
    printf(location);
    if (strcmp(location, detective->current_location->items[i]->name) == 0) {
        found_location = 1;
        if (detective->current_location->items[i]->is_container) {
            addItemToContainer(detective->current_location->items[i],detective->inventory.items[item_index]);
            removeFromInventoryInternal(detective,detective->inventory.items[item_index] );
        } else {
            printf("This is not a valid container\n");
        }
        break;
    }
}

if (!found_location) {
    printf("Invalid location or item\n");
}
}
void takeCommand(EMS *ems, Detective *detective, char *token){
char *item = strtok(token, " ");
int item_index = -1;  // Initialize to -1 to indicate not found
int container_index = -1;

printf("Debug: Parsed item: %s\n", item);

if (item == NULL) {
    printf("Invalid command format.\n");
    return;
}

for (int i = 0; i < detective->current_location->size; i++) {
    printf("Debug: Checking item %s at index %d\n", detective->current_location->items[i]->name, i);
    
    if (strcmp(item, detective->current_location->items[i]->name) == 0) {
        item_index = i;
        printf("Debug: Item found in room at index %d\n", i);
        break;
    }
    
    if(detective->current_location->items[i]->is_container && detective->current_location->items[i]->size > 0){
        printf("Debug: Checking container %s at index %d\n", detective->current_location->items[i]->name, i);
        for(int j = 0; j < detective->current_location->items[i]->size; j++){
            printf("Debug: Checking item %s in container at index %d\n", detective->current_location->items[i]->items[j]->name, j);
            if(strcmp(item, detective->current_location->items[i]->items[j]->name) == 0){
                item_index = j;
                container_index = i;
                printf("Debug: Item found in container at index %d, container at index %d\n", j, i);
            }
        }
    }
}

if (item_index == -1) {
    char *tail_end = strtok(NULL, " ");
    if (tail_end) {
        char two_worded[100];
        snprintf(two_worded, sizeof(two_worded), "%s %s", item, tail_end);
        printf("Debug: Parsed two-worded item: %s\n", two_worded);

        for (int i = 0; i < detective->current_location->size; i++) {
            printf("Debug: Checking two-worded item %s at index %d\n", detective->current_location->items[i]->name, i);

            if (strcmp(two_worded, detective->current_location->items[i]->name) == 0) {
                item_index = i;
                printf("Debug: Two-worded item found in room at index %d\n", i);
                break;
            }
            if(detective->current_location->items[i]->is_container && detective->current_location->items[i]->size > 0){
                printf("Debug: Checking two-worded item in container %s at index %d\n", detective->current_location->items[i]->name, i);
                for (int j = 0; j < detective->current_location->items[i]->size; j++) {
                    if (strcmp(two_worded, detective->current_location->items[j]->name) == 0) {
                        container_index = i;
                        item_index = j;
                        printf("Debug: Two-worded item found in container at index %d, container at index %d\n", j, i);
                        break;
                    }
                }
            }
        }
    }
}

if (item_index == -1) {
    printf("You do not have this item\n");
    return;
}

char *location = strtok(NULL, "");

printf("Debug: Parsed location: %s\n", location);

if (location == NULL) {
    if (container_index == -1) {
        printf("Debug: Adding item to inventory from room at index %d\n", item_index);
        addToInventory(detective->current_location->items[item_index], &detective->inventory);
        removeFromRoom("", item_index, detective->current_location);
    } else {
        printf("This item nuh uh\n");
    }
    return;
}

int found_location = 0;
for (int i = 0; i < detective->current_location->size; i++) {
    printf("Debug: Checking location %s against item %s\n", location, detective->current_location->items[i]->name);
    if (strcmp(location, detective->current_location->items[i]->name) == 0) {
        found_location = 1;
        printf("Debug: Location found: %s at index %d\n", location, i);
        
        if (detective->current_location->items[i]->is_container) {
            if(i == container_index){
            printf("Debug: Location is a container, adding item to inventory\n");
            addToInventory(detective->current_location->items[container_index]->items[item_index], &detective->inventory);
            removeFromContainer(detective->current_location->items[container_index], detective->current_location->items[container_index]->items[item_index]);
            }
        } else {
            printf("This is not a valid container\n");
        }
        break;
    }
}

if (!found_location) {
    printf("Invalid location or item\n");
}

}
void processCommand(EMS *ems, char *str, Detective *detective, ItemCollection *itemList, CFCollection *CFCO) {
    // Make a copy of the input string to avoid modifying the original
    char buffer[256];
    strncpy(buffer, str, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0'; // Ensure null termination

    // Find the first space to separate the command
    char *command = strtok(buffer, " ");
    
    if (command != NULL) {
        if (strcmp(command, "move") == 0) {
            // Get the rest of the string as direction
            char *direction = strtok(NULL, "");
            if (direction != NULL) {
                // Remove leading spaces from direction
                while (*direction == ' ') direction++;
                moveCommand(ems, detective, direction, CFCO);
            } else {
                printf("Specify a direction to move.\n");
            }
        }
        else if (strcmp(command, "quit") == 0) {
            printf("\nQuitting...\n");
            exit(0);
        }
        else if (strcmp(command, "examine") == 0) {
            // Get the rest of the string as item name
            char *itemName = strtok(NULL, "");
            if (itemName != NULL) {
                // Remove leading spaces from itemName
                while (*itemName == ' ') itemName++;
                examineCommand(ems,detective,itemName,CFCO);
            } else {
                printf("Specify an item to examine.\n");
            }
        }
        else if(strcmp(command, "open") == 0){
            char *object = strtok(NULL ,"");
            if(object!=NULL){
                while(*object == ' ')object++;
                openCommand(detective, object, itemList);
            }
            else{
                printf("Specify an object to open.\n");
            }
        }
        else if(strcmp(command,"take")== 0){
            char *item = strtok(NULL, "");
            if(item!=NULL){
                while(*item == ' ')item++;
                takeCommand(ems,detective,item);
            }
            else{
                printf("Specify an item to examine.\n");
            }
        }
        else if(strcmp(command, "drop")== 0){
            char *item = strtok(NULL,"");
            if(item != NULL){
                while(*item == ' ' )item++;
                dropCommand(ems,detective,item);
            }
        }
        else {
            printf("Unknown command.\n");
        }
    } else {
        printf("No command provided.\n");
    }
}

void helpCommand(){
    printf("\n\nDetective text-adventure game, where you're investigating a cold case from the 1990's about a missing child. Faced with adversity and plot twists, your objective is to find the perpetrator.\n\n");
    printf("Commands: \n\n");
    printf("move --direction\n");
    printf("take --item (collectable)\n");
    printf("examine --item(non-collectable & collectable)\n");
    printf("use --item --target\n");
    printf("drop --item\n");
    printf("read journal\n");
    printf("write journal\n");
    printf("\n");
}

int main()
{
    bool loop = true;
    Detective detective;
    EMS ems;
    EMSEvent office_scene_1_event;
    EMSEvent examine_scene_1_event;
    Character detective2;
    Character lieutenant;
    Inventory inventory;
    Journal journal;
    Location lobby;
    Location office;
    Item ID;
    Item office_key;
    Item eight_ball;
    Item lobby_posters;
    Item office_desk;
    Item reception_desk;
    Item gun;
    CaseFile casefileno1;
    CFCollection CFCO;
    ItemCollection ItemCO;
    DialogueTree office_scene_1;
    DialogueTree office_scene_o1; 
    DialogueTree office_scene_o2; 
    DialogueTree office_scene_o3; 
    DialogueTree examine_scene_1; 
    DialogueTree examine_scene_1_1;
    DialogueTree examine_scene_1_2;
    DialogueTree examine_scene_1_3;
    DialogueTree examine_scene_1_4;
    welcomeMessage();
    initialiseRoom(&lobby, "Lobby", "The police station lobby is a bustling, starkly lit room with a sleek front desk, worn-out chairs, and walls adorned with crime prevention posters and a large, imposing emblem of justice.", "" );
    initialiseRoom(&office, "Office", "Your office, with its smoke-stained walls and yellowing case files scattered across a cluttered desk, hums with the persistent drone of an old fan and carries the faint scent of stale coffee and cigarettes.", "Office Key");
    addConnection(&lobby, &office, "east");
    initialiseItemLookup(&ItemCO);
    initialiseCFLookup(&CFCO);
    initialiseCase(&casefileno1,"The Campside Kidnapping","037-992", "23/03/1978", "Detective Chris Levins (circa 1993: Max Kruger.)", "In the late night of March 23, 1978, Michael O\'Connell, a four year old toddler, was reported missing by his parents. Michael was last seen in his parents\' tent asleep in Ridgewood National Park, which is known for its clear trails and beautiful scenery. According to his family, they have no recollection of any people of interest. They do not remember the day prior. They woke the next morning, and Michael was nowhere to be found.", "Victim Information:\nName: Michael O\'Connell\nAge: 4\nOccupation: N/A\nPhysical Description:\n\tHeight: 3\"4\n\tWeight: 40lbs\n\tHair: Brown, buzzed\n\tEyes: Green\n\tDistinguishing Features:\n\t\tScar through his right eyebrow\nLast Known Whereabouts:\n\tLocation: 200m north west of Rigewood National Park\'s main entrance\n\tTime: 2200, 23/03/1978", "Location: Ridgewood", "Evidence:\n\tLarge adult sized footprints found leading from the tent toward the rear entrance.\n\tSuspicious Activity:\n\t\tA fisherman, identified as Alfred Zimmerman, was seen lingering around the campsite earlier that evening.\n\t\tZimmerman was detained on child abduction charges, but released under insufficient evidence.\n\t\tWitnesses reported Zimmerman staring at the children from across the lake, acting \"feverish\"\n\tSearch Efforts:\n\t\tSearh teams combed through the park over the span of 3 days.\n\t\tNo physical evidence of Michael was found, no signs of struggle or foul play at the site.", "Primary Suspect: Former felon- Owen Pritchard");
    addCFToLookup(&CFCO,&casefileno1);

    initialiseItem(&ID, "ID", false, "Your detective ID, worn and slightly frayed, shows your stern photo, name, badge number, and department logo, all in a weathered leather wallet.", &ItemCO, false);
    initialiseItem(&office_key, "Office Key",false,"The office key is a small, unassuming piece of metal, its once shiny surface now dulled by years of use. The teeth of the key are worn but still precise, fitting perfectly into the old lock of your office door. Attached to the key is a leather keychain, cracked and aged, with the faint imprint of the department's emblem barely visible.", &ItemCO, false);
    initialiseItem(&eight_ball, "8 Ball",false,"The eight-ball is a compact, enigmatic sphere, its black surface scuffed from countless consultations; the white number eight still bold and clear, a silent partner in every case.", &ItemCO,false);
    initialiseItem(&lobby_posters, "A plethora of lobby posters",false,"The lobby posters are faded and worn, each one a collage of cold cases, public warnings, and gritty mugshots, their edges curling as if the weight of unsolved mysteries clings to every corner of the station.", &ItemCO,false);
    initialiseItem(&reception_desk,"Reception Desk",false,"The reception desk, with its bulletproof glass and scuffed surface, is cluttered with stacks of paperwork, a buzzing phone, a notepad filled with frantic scribbles, and a coffee cup stained from endless late nights, all under the harsh glare of fluorescent lights.", &ItemCO, false);
    initialiseItem(&office_desk,"Office Desk",false, "Your desk, scarred and cluttered, is piled with case files, yellowed notes, and an old typewriter. A rotary phone buzzes occasionally, and a coffee mug, stained from late nights, sits amid the chaos. A flickering desk lamp casts a dim light over the scene, struggling against the hum of outdated overhead fluorescents.",&ItemCO,true);
    initialiseItem(&gun,"CZ-75",true,"On the detective\'s desk, the CZ-75 rests with a matte black finish, its sleek lines and ergonomic grip standing out amid the clutter",&ItemCO,false);

    initialiseInventory(&inventory);
    initialiseJournal(&journal);
    initialisePlayer(&detective, &inventory, &journal, &lobby);

    addToInventory(&ID, &detective.inventory);
    addToInventory(&office_key, &detective.inventory);
    addToInventory(&eight_ball, &detective.inventory);

    addItemToRoom(&lobby_posters, &lobby);
    addItemToRoom(&reception_desk,  &lobby);
    addItemToRoom(&office_desk, &office);

    addItemToContainer(&office_desk,&gun);
    initialiseCharacter(&lieutenant, "Lt. Reynolds", NULL);
    initialiseCharacter(&detective2, "Sr Detective. Kruger", NULL);

    initialiseDialogue(&office_scene_o3, &lieutenant, "I know things have been kind of dry lately, so I have an interesting cold case here for you.", NULL, NULL);
    initialiseDialogue(&office_scene_o1, &detective2, "Doing well, what's up?", &office_scene_o3, NULL);
    initialiseDialogue(&office_scene_o2, &detective2, "Get to the point Reynolds.", &office_scene_o3, NULL);
    initialiseDialogue(&office_scene_1, &lieutenant, "Morning partner, how's life treating you?", &office_scene_o1, &office_scene_o2);

    initialiseDialogue(&examine_scene_1_4, &detective2, "The O\'Connells\' died in a gas leak, so I can't talk to them now. I should start by interviewing this fisherman. That seems like the most ordered approach and God knows I fucking need order.", NULL,NULL);
    initialiseDialogue(&examine_scene_1_3, &detective2, "Kid even has an eyebrow scar like me. Normally I wouldn't bother with a cold case, but this one is intriguing me. And the family doesn\'t remember the day they arrrived? Were they drugged perhaps?",&examine_scene_1_4,NULL);
    initialiseDialogue(&examine_scene_1_2, &detective2, "A young boy, randomly goes missing in the middle of the night, and no one has heard a sliver of information since. Not a whisper, not a sighting, nothing.", &examine_scene_1_3, NULL);
    initialiseDialogue(&examine_scene_1_1, &detective2, "In Ridgewood Park. Damn that's close to home.",&examine_scene_1_2,NULL);
    initialiseDialogue(&examine_scene_1, &detective2, "Interesting. A missing case with an obvious suspect but no arrest.", &examine_scene_1_1, NULL);


    initialiseEMS(&ems);
    initialiseEMSEvent(&office_scene_1_event, &ems, &office, DIALOGUE, &office_scene_1, -1);
    initialiseEMSEvent(&examine_scene_1_event,&ems, NULL, DIALOGUE, &examine_scene_1,0);
    addItemToEvent(&office_scene_1_event, NULL, &casefileno1);
    addCFToEvent(&casefileno1,&examine_scene_1_event);
    addEventToEMS(&office_scene_1_event, &ems);
    addEventToEMS(&examine_scene_1_event,&ems);

    Sleep(1500);
    printInventory(&detective.inventory);
    Sleep(1500);
    printCurrentRoom(detective.current_location);
    printItems(&lobby);
    Sleep(1500);
    printNearbyRooms(detective.current_location);
    printf("type help for available commands\n");
    while(loop){
    char res[300];
    char comp[] = "help";
    printf("\nCommand: ");
    fgets(res,sizeof(res),stdin);
    res[strcspn(res,"\n")] = 0;
    if(strcmp(res,comp) == 0){
        helpCommand();
    }
    else{
        processCommand(&ems, res,&detective, &ItemCO, &CFCO);
    }
    }
    free(inventory.items);
    free(inventory.case_files);
    free(office.items);
    free(office.case_files);
    free(lobby.items);
    free(lobby.case_files);
    free(ems.event_queue);
    free(ItemCO.items);

    return 0;

}
