#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

/* Detective text-adventure game, where our detective is investigating a cold case from the 1990's about a missing child. Eventually, they realise it's themselves. Retracing their entire history, to find the perpetrator.*/ /* Forward declarations */ typedef struct Inventory Inventory; typedef struct Journal Journal; typedef struct Location Location; typedef struct Item Item; typedef struct Event Event; typedef struct Detective Detective; typedef struct ItemCollection ItemCollection; /* Struct definitions */ struct Item { char name[100]; bool is_collectable; char description[1000]; char image[100]; }; struct Weapon { char name[100]; char description[1000]; int damage; }; struct Event { char name[100]; char content[1000]; char timestamp[80]; }; struct Journal { int size; int capacity; Event *events; }; struct Inventory { int size; int capacity; Item *items; }; struct Location { char name[100]; char description[1000]; bool locked; int size; int capacity; Item *items; Location *north; Location *west; Location *east; Location *south; }; struct ItemCollection{ int size; int capacity; Item *items; }; struct Detective { Inventory inventory; Journal journal; Location *current_location; }; void initialiseJournal(Journal *journal) { journal->size = 0; journal->capacity = 1; journal->events = (Event *)malloc(journal->capacity * sizeof(Event)); }
void initialiseItemLookup(ItemCollection *itemList)
{
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

bool itemLookup(ItemCollection *itemList, char *token){
    for(int i = 0; i < itemList->size; i ++){
        if(strcmp(token, itemList->items[i].name) == 0){
            return true;
        }
    }
    return false;
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

void initialiseRoom(Location *room, char *name, char *description, bool locked)
{
    strcpy(room->name, name);
    strcpy(room->description, description);
    room->locked = locked;
    room->size = 0;
    room->capacity = 2;
    room->items = (Item *)malloc(room->capacity * sizeof(Item));
    room->north = NULL;
    room->east = NULL;
    room->west = NULL;
    room->south = NULL;
}

void addToInventory(Item *item, Inventory *inventory)
{
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

void removeFromInventory(char* name, Inventory *inventory, Detective *detective) {
    for (int i = 0; i < inventory->size; i++) {
        if (strcmp(inventory->items[i].name, name) == 0) {
            // Shift remaining items to the left
            addItemToRoom(&inventory->items[i], detective->current_location);
            for (int j = i; j < inventory->size - 1; j++) {
                inventory->items[j] = inventory->items[j + 1];
            }
            inventory->size--;
            printf("\nItem was removed.\n");
        }
    }
    printf("\nItem was not found.\n");
}
void addItemToRoom(Item *item, Location *room){
    if(room->size == room->capacity){
        room->capacity*=2;
        room->items =realloc(room->items,room->capacity * sizeof(Item));
    }
    room->items[room->size] = *item;
    room->size++;
}
void printInventory(Inventory *inventory)
{
    printf("Inventory:\n");
    for (int i = 0; i < inventory->size; i++)
    {
        printf("- %s: %s\n", inventory->items[i].name, inventory->items[i].description);
    }
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
    printf("\nItems in %s:\n", room->name);
    for (int i = 0; i < room->size; i++)
    {
        printf("- %s: %s\n", room->items[i].name, room->items[i].description);
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

void moveCommand(Detective *detective, char* token){
    if(token != NULL){
        if(strcmp(token, "north") == 0 && detective->current_location->north != NULL){
            detective->current_location = detective->current_location->north;
            printf("\nYou have moved North.\n");
            printCurrentRoom(detective->current_location);
            printItems(detective->current_location);
            printNearbyRooms(detective->current_location);
        }
        else if(strcmp(token, "east") == 0 &&detective->current_location->east != NULL){
            detective->current_location = detective->current_location->east;
            printf("\nYou have moved East.\n");
            printCurrentRoom(detective->current_location);
            printItems(detective->current_location);
            printNearbyRooms(detective->current_location);
        }
        else if(strcmp(token, "west") == 0&&detective->current_location->west != NULL){
            detective->current_location = detective->current_location->west;
            printf("\nYou have moved West.\n");
            printCurrentRoom(detective->current_location);
            printItems(detective->current_location);
            printNearbyRooms(detective->current_location);
        }
        else if(strcmp(token, "south") == 0&&detective->current_location->south != NULL){
            detective->current_location = detective->current_location->south;
            printf("\nYou have moved South.\n");
            printCurrentRoom(detective->current_location);
            printItems(detective->current_location);
            printNearbyRooms(detective->current_location);
        }
        else{
            printf("Invalid direction");
        }
    }
    else{
        printf("Invalid direction");
    }
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
                addToInventory(&detective->current_location->items[i], &detective->inventory);
                printf("\nItem added to inventory.\n");
                removeFromRoom(itemName, detective->current_location);
                return; // exit after adding the item to the inventory
            }
    }
        printf("Item '%s' not found.\n", itemName);
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
                removeFromInventory(itemName, &detective->inventory, &detective);
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
    Item casefileno1;
    ItemCollection ItemCO;

    welcomeMessage();
    initialiseRoom(&lobby, "Lobby", "The police station lobby is a bustling, starkly lit room with a sleek front desk, worn-out chairs, and walls adorned with crime prevention posters and a large, imposing emblem of justice.", false);
    initialiseRoom(&office, "Office", "Your office, with its smoke-stained walls and yellowing case files scattered across a cluttered desk, hums with the persistent drone of an old fan and carries the faint scent of stale coffee and cigarettes.", true);
    addConnection(&lobby, &office, "east");
    initialiseItemLookup(&ItemCO);
    initialiseItem(&ID, "ID", false, "Your detective ID, worn and slightly frayed, shows your stern photo, name, badge number, and department logo, all in a weathered leather wallet.", &ItemCO);
    initialiseItem(&office_key, "Office Key",false,"The office key is a small, unassuming piece of metal, its once shiny surface now dulled by years of use. The teeth of the key are worn but still precise, fitting perfectly into the old lock of your office door. Attached to the key is a leather keychain, cracked and aged, with the faint imprint of the department's emblem barely visible.", &ItemCO);
    initialiseInventory(&inventory);
    initialiseJournal(&journal);
    initialisePlayer(&detective, &inventory, &journal, &lobby);
    addToInventory(&ID, &detective.inventory);
    addItemToRoom(&office_key, &lobby);

    Sleep(3000);
    printf("\n\nThe morning sun filters through the frosted glass of the police station's lobby, casting a soft glow on the sleek front desk and the rows of worn-out chairs. The buzz of the early shift hums around you as officers greet each other with tired nods and steaming cups of coffee. You shrug off your coat, the lingering chill of the outside air clinging to it, and make your way past the crime prevention posters that line the walls, heading towards the east wing. As you approach your office, the familiar creak of the door welcomes you back to the cluttered sanctuary of smoke-stained walls, yellowing case files, and the persistent drone of the old ceiling fan. The faint scent of stale coffee and cigarettes is a comfort; another day, another chance to unravel the city's mysteries. You take a deep breath, ready to dive into the day's work,unaware of the revelations that await.\n\n");
    Sleep(1500);
    printInventory(&detective.inventory);
    Sleep(1500);
    printCurrentRoom(detective.current_location);
    printItems(&lobby);
    Sleep(1500);
    printNearbyRooms(detective.current_location);

    printf("type help for available commands\n\n");

    while(loop){
    char res[20];
    char comp[] = "help";
    printf("\n");
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
