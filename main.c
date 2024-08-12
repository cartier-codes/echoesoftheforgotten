#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

/* Detective text-adventure game, where our detective is investigating a cold case from the 1990's about a missing child. Eventually, they realise it's themselves. Retracing their entire history, to find the perpetrator.*/ /* Forward declarations */ typedef struct Inventory Inventory; typedef struct Journal Journal; typedef struct Location Location; typedef struct Item Item; typedef struct Event Event; typedef struct Detective Detective; typedef struct ItemCollection ItemCollection; typedef struct CaseFile CaseFile;/* Struct definitions */ 
struct Item { char name[100]; bool is_collectable; char description[5000]; bool is_key;};
struct CaseFile{char name[100]; char summary[1000]; char victims[200]; char location[200]; char evidence[500]; char suspects[500];};
struct Weapon { char name[100]; char description[1000]; int damage; }; 
struct Event { char name[100]; char content[1000]; char timestamp[80]; };
struct Journal { int size; int capacity; Event *events; }; 
struct Inventory { int size; int capacity; Item *items; };
struct Location { 
    char name[100]; char description[1000]; char locked_item[100]; int size; int capacity; int case_file_num; int case_file_cap; CaseFile *case_files; Item *items; Location *north; Location *west; Location *east; Location *south; };
struct ItemCollection{ 
    int size; int capacity; Item *items; };

struct Detective { Inventory inventory; Journal journal; Location *current_location; };

void initialiseJournal(Journal *journal) { 
    journal->size = 0; journal->capacity = 1; journal->events = (Event *)malloc(journal->capacity * sizeof(Event)); } 

void initialiseItemLookup(ItemCollection *itemList) {
    itemList->size = 0;
    itemList->capacity = 1;
    itemList->items = (Item *)malloc(itemList->capacity * sizeof(Item));
}

void initialiseItem(Item *item, char *name, bool is_collectable, char *description, ItemCollection *itemList)
{
    strcpy(item->name, name);
    strcpy(item->description, description);
    item->is_collectable = is_collectable;
    itemList->items[itemList->size] = *item;
}

void initialiseInventory(Inventory *inventory)
{
    inventory->size = 0;
    inventory->capacity = 32;
    inventory->items = (Item *)malloc(inventory->capacity * sizeof(Item));
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
    room->items = (Item *)malloc(room->capacity * sizeof(Item));
    room->case_files = (CaseFile *)malloc(room->capacity * sizeof(CaseFile));
    room->north = NULL;
    room->east = NULL;
    room->west = NULL;
    room->south = NULL;
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
        inventory->items[inventory->size] = *item;
        inventory->size++;
    }
    else
    {
        printf("Inventory is full!\n");
    }
}

void addItemToRoom(Item *item, Location *room){
    if(room->size == room->capacity){
        room->capacity*=2;
        room->items =realloc(room->items,room->capacity * sizeof(Item));
    }
    room->items[room->size] = *item;
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
void removeFromInventory(char* name, Inventory *inventory, Detective *detective) {
    for (int i = 0; i < inventory->size; i++) {
        if (strcmp(inventory->items[i].name, name) == 0) {
            // Shift remaining items to the left
            if(!inventory->items[i].is_collectable){
                printf("This item cannot be dropped.\n");
                return;
            }
            addItemToRoom(&inventory->items[i], detective->current_location);
            for (int j = i; j < inventory->size - 1; j++) {
                inventory->items[j] = inventory->items[j + 1];
            }
            inventory->size--;
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

void printInventory(Inventory *inventory)
{
    printf("Inventory:\n");
    for (int i = 0; i < inventory->size; i++)
    {
        printf("- %s: %s\n", inventory->items[i].name, inventory->items[i].description);
    }
    printf("- Journal: The leather-bound journal is a well-worn companion, its cover scuffed from years of use; inside, the pages hold a meticulous record of the detective's thoughts and observations.\n");
}

void printCurrentRoom(Location *room){
    printf("\nCurrent room: %s\n", room->name);
}

void printItems(Location *room)
{
    if(room->size == 0){
        printf("\nNo items in room.\n");
        return;
    }
    printf("\n%s:\n", room->name);
    for (int i = 0; i < room->size; i++)
    {
        printf("- %s: %s\n", room->items[i].name, room->items[i].description);
    }
    for (int i = 0; i < room->case_file_num; i++)
    {
        printf("Case File: %s Summary: %s\n", room->case_files[i].name, room->case_files[i].summary);
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

void openCommand(Detective *detective, char *token){
    if(token != NULL){
        if(strcmp(token, "inventory") == 0){
            printInventory(&detective->inventory);
        }
    }
}
void removeFromRoom(char* name, Location *location) {
    for (int i = 0; i < location->size; i++) {
        if (strcmp(location->items[i].name, name) == 0) {
            // Shift remaining items to the left
            for (int j = i; j < location->size - 1; j++) {
                location->items[j] = location->items[j + 1];
            }
            location->size--;
        }
    }
}

void takeCommand(Detective *detective,char *itemName, ItemCollection *itemlist) {
    
    if(itemName != NULL){
    for (int i = 0; i < detective->current_location->size; i++) {
            if (strcmp(itemName, detective->current_location->items[i].name) == 0) {
                if(!detective->current_location->items[i].is_collectable){
                    printf("This item cannot be taken");
                    return;
                }
                addToInventory(&detective->current_location->items[i], &detective->inventory);
                printf("\nItem added to inventory.\n");
                removeFromRoom(itemName, detective->current_location);
                return; // exit after adding the item to the inventory
            }
    }
        printf("Item '%s' not found.\n", itemName);
    }
}

void moveCommand(Detective *detective, char* direction) {
    if (direction != NULL) {
        if (strcmp(direction, "north") == 0) {
            if (detective->current_location->north != NULL) {
                if (strcmp(detective->current_location->north->locked_item, "") != 0) {
                    bool hasKey = false;
                    for (int i = 0; i < detective->inventory.size; i++) {
                        if (strcmp(detective->inventory.items[i].name, detective->current_location->north->locked_item) == 0) {
                            detective->current_location = detective->current_location->north;
                            printf("\nYou have moved North.\n");
                            printCurrentRoom(detective->current_location);
                            printItems(detective->current_location);
                            printNearbyRooms(detective->current_location);
                            hasKey = true;
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
                }
            } else {
                printf("You can't move North.\n");
            }
        } else if (strcmp(direction, "east") == 0) {
            if (detective->current_location->east != NULL) {
                if (strcmp(detective->current_location->east->locked_item, "") != 0) {
                    bool hasKey = false;
                    for (int i = 0; i < detective->inventory.size; i++) {
                        if (strcmp(detective->inventory.items[i].name, detective->current_location->east->locked_item) == 0) {
                            detective->current_location = detective->current_location->east;
                            printf("\nYou have moved East.\n");
                            printCurrentRoom(detective->current_location);
                            printItems(detective->current_location);
                            printNearbyRooms(detective->current_location);
                            hasKey = true;
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
                }
            } else {
                printf("You can't move East.\n");
            }
        } else if (strcmp(direction, "west") == 0) {
            if (detective->current_location->west != NULL) {
                if (strcmp(detective->current_location->west->locked_item, "") != 0) {
                    bool hasKey = false;
                    for (int i = 0; i < detective->inventory.size; i++) {
                        if (strcmp(detective->inventory.items[i].name, detective->current_location->west->locked_item) == 0) {
                            detective->current_location = detective->current_location->west;
                            printf("\nYou have moved West.\n");
                            printCurrentRoom(detective->current_location);
                            printItems(detective->current_location);
                            printNearbyRooms(detective->current_location);
                            hasKey = true;
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
                }
            } else {
                printf("You can't move West.\n");
            }
        } else if (strcmp(direction, "south") == 0) {
            if (detective->current_location->south != NULL) {
                if (strcmp(detective->current_location->south->locked_item, "") != 0) {
                    bool hasKey = false;
                    for (int i = 0; i < detective->inventory.size; i++) {
                        if (strcmp(detective->inventory.items[i].name, detective->current_location->south->locked_item) == 0) {
                            detective->current_location = detective->current_location->south;
                            printf("\nYou have moved South.\n");
                            printCurrentRoom(detective->current_location);
                            printItems(detective->current_location);
                            printNearbyRooms(detective->current_location);
                            hasKey = true;
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
                }
            } else {
                printf("You can't move South.\n");
            }
        } else {
            printf("Invalid direction.\n");
        }
    }
}

void processCommands(const char *input, Detective *detective, ItemCollection *itemList) {
    char str[256];
    strncpy(str, input, sizeof(str));
    str[sizeof(str) - 1] = '\0'; // Ensure null termination

    const char delimiters[] = " \t\r\n";
    char *token = strtok(str, delimiters);
    if (token != NULL) {
        if(strcmp(token, "move") == 0){
            token = strtok(NULL, delimiters);
            /* check if direction is locked, if locked check inventory if there move.*/
            moveCommand(detective, token);
        }
        else if(strcmp(token, "quit") == 0){
            printf("\nQuitting...\n");
            exit(0);
        }
        else if(strcmp(token, "open")== 0){
            token = strtok(NULL,delimiters);
            openCommand(detective,token);
        }
        else if(strcmp(token, "take") == 0){
            char *itemName = strtok(NULL, ""); // Capture the rest of the input as item name
            if (itemName != NULL) {
                // Remove leading spaces from itemName
                while (*itemName == ' ') itemName++;
                takeCommand(detective, itemName, itemList);

            } else {
                printf("Specify an item to take.\n");
            }
        }
        else if(strcmp(token, "drop") == 0){
            char *itemName = strtok(NULL, ""); // Capture the rest of the input as item name
            if (itemName != NULL) {
                // Remove leading spaces from itemName
                while (*itemName == ' ') itemName++;
                removeFromInventory(itemName, &detective->inventory, detective);
            } else {
                printf("Specify an item to take.\n");
            }
        }
        else{
            printf("Invalid command.");
        }
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
    Inventory inventory;
    Journal journal;
    Location lobby;
    Location office;
    Item ID;
    Item office_key;
    Item eight_ball;
    Item lobby_posters;
    Item reception_desk;
    ItemCollection ItemCO;
    
    welcomeMessage();
    initialiseRoom(&lobby, "Lobby", "The police station lobby is a bustling, starkly lit room with a sleek front desk, worn-out chairs, and walls adorned with crime prevention posters and a large, imposing emblem of justice.", "" );
    initialiseRoom(&office, "Office", "Your office, with its smoke-stained walls and yellowing case files scattered across a cluttered desk, hums with the persistent drone of an old fan and carries the faint scent of stale coffee and cigarettes.", "Office Key");
    addConnection(&lobby, &office, "east");
    initialiseItemLookup(&ItemCO);
    initialiseItem(&ID, "ID", false, "Your detective ID, worn and slightly frayed, shows your stern photo, name, badge number, and department logo, all in a weathered leather wallet.", &ItemCO);
    initialiseItem(&office_key, "Office Key",false,"The office key is a small, unassuming piece of metal, its once shiny surface now dulled by years of use. The teeth of the key are worn but still precise, fitting perfectly into the old lock of your office door. Attached to the key is a leather keychain, cracked and aged, with the faint imprint of the department's emblem barely visible.", &ItemCO);
    initialiseItem(&eight_ball, "8 Ball",false,"The eight-ball is a compact, enigmatic sphere, its black surface scuffed from countless consultations; the white number eight still bold and clear, a silent partner in every case.", &ItemCO);
    initialiseItem(&lobby_posters, "A plethora of lobby posters",false,"The lobby posters are faded and worn, each one a collage of cold cases, public warnings, and gritty mugshots, their edges curling as if the weight of unsolved mysteries clings to every corner of the station.", &ItemCO);
    initialiseItem(&reception_desk,"Reception Desk",false,"The reception desk, with its bulletproof glass and scuffed surface, is cluttered with stacks of paperwork, a buzzing phone, a notepad filled with frantic scribbles, and a coffee cup stained from endless late nights, all under the harsh glare of fluorescent lights.", &ItemCO);
    initialiseInventory(&inventory);
    initialiseJournal(&journal);
    initialisePlayer(&detective, &inventory, &journal, &lobby);
    addToInventory(&ID, &detective.inventory);
    addToInventory(&office_key, &detective.inventory);
    addToInventory(&eight_ball, &detective.inventory);
    addItemToRoom(&lobby_posters, &lobby);
    addItemToRoom(&reception_desk, &lobby);
    Sleep(3000);
    printf("\n\nThe morning sun filters through the frosted glass of the police station's lobby, casting a soft glow on the sleek front desk and the rows of worn-out chairs. The buzz of the early shift hums around you as officers greet each other with tired nods and steaming cups of coffee. You shrug off your coat, the lingering chill of the outside air clinging to it, and make your way past the crime prevention posters that line the walls, heading towards the east wing. As you approach your office, the familiar creak of the door welcomes you back to the cluttered sanctuary of smoke-stained walls, yellowing case files, and the persistent drone of the old ceiling fan. The faint scent of stale coffee and cigarettes is a comfort; another day, another chance to unravel the city's mysteries. You take a deep breath, ready to dive into the day's work,unaware of the revelations that await.\n\n");
    Sleep(1500);
    printInventory(&detective.inventory);
    Sleep(1500);
    printCurrentRoom(detective.current_location);
    printItems(&lobby);
    Sleep(1500);
    printNearbyRooms(detective.current_location);

    printf("type help for available commands\n");

    while(loop){
    char res[20];
    char comp[] = "help";
    printf("\nCommand: ");
    fgets(res,sizeof(res),stdin);
    res[strcspn(res,"\n")] = 0;
    if(strcmp(res,comp) == 0){
        helpCommand();
    }
    else{
        processCommands(res,&detective, &ItemCO);
    }
    }

    return 0;

}
