#include "headers/print.h"
#include "headers/struct.h"
#include <stdlib.h>
#include <stdio.h>


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
    printf("\nCurrent room: %s\n\n%s\n\n", room->name, room->description);

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
    }

}
void printNearbyRooms(Location *location){
    printf("\nLocations within this room: \n");
    if(location->inner_locations[0] != NULL){
    for(int i = 0; i < location->loc_size; i++){
        printf("- Location: %s\n", location->inner_locations[i]);
    }}
    printf("\n\nExits:\n\n");
    if (location->north != NULL) {
        printf("- North: %s\n", location->north->name);
    }
    if (location->east != NULL) {
        printf("- East: %s\n", location->east->name);
    }
    if (location->south != NULL) {
        printf("- South: %s\n", location->south->name);
    }
    if (location->west != NULL) {
        printf("- West: %s\n", location->west->name);
    }
    printf("\n");
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