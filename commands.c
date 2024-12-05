#include "headers/commands.h"

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
        }
        else if (strcmp(direction, "out") == 0)
        {
            if(detective->current_location->outer_location != NULL){ 
                detective->current_location = detective->current_location->outer_location;
                printf("You have moved to the %s. \n", detective->current_location->name);
                printCurrentRoom(detective->current_location);
                printItems(detective->current_location);
                printNearbyRooms(detective->current_location);
                updateEMS(ems, detective, MOVE,"",CFCO);

            }
        }
        
        else {
            for(int i = 0; i < detective->current_location->loc_size; i++){
                if(strcmp(detective->current_location->inner_locations[i]->name, direction) == 0){
                    detective->current_location = detective->current_location->inner_locations[i];
                    printf("\nYou have moved to the %s.\n", detective->current_location->name);
                    printCurrentRoom(detective->current_location);
                    printItems(detective->current_location);
                    printNearbyRooms(detective->current_location);
                    updateEMS(ems, detective, MOVE,"",CFCO);
                    return;
                }
            }
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
            sleep(1);
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
        printf("%d: %s",item_index,detective->inventory.items[item_index]->name);
        removeFromInventory("",detective,detective->inventory.items[item_index]);
    return;
}

int found_location = 0;
for (int i = 0; i < detective->current_location->size; i++) {
    printf("%s",location);
    if (strcmp(location, detective->current_location->items[i]->name) == 0) {
        found_location = 1;
        if (detective->current_location->items[i]->is_container) {
            addItemToContainer(detective->current_location->items[i],detective->inventory.items[item_index]);
            removeFromInventoryInternal(detective,detective->inventory.items[item_index]);
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
        if(!detective->current_location->items[item_index]->is_collectable){
            printf("This item cannot be picked up\n");
            return;
        }
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
                if(!detective->current_location->items[container_index]->items[item_index]->is_collectable){
                    printf("This item cannot be picked up\n");
                    return;
                }
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