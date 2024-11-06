#include "headers/remove.h"
#include "headers/add.h"
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