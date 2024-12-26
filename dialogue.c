#include "headers/dialogue.h"
#include "headers/ems.h"
#include "headers/struct.h"

void traverseDialogue(EMS *ems, DialogueTree *root, Detective *detective){
    if(root == NULL){
        return;
    }
    if(strcmp(root->speaker->name,"Sr Detective. Kruger") == 0){
        printf("%s: %s\n", root->speaker->name, root->text);
        if(root->option_1 == NULL){
            return;
        }
        else{
            traverseDialogue(ems, root->option_1, detective);
        }
    }
    else{
        printf("%s: %s\n", root->speaker->name, root->text);
        if((root->option_1 == NULL && root->option_2 == NULL)){
            return;
        }
        else if(root->option_2 == NULL){
            traverseDialogue(ems, root->option_1, detective);
        }
        else{
        printf("Option 1: %s\n\n", root->option_1->text);
        printf("Option 2: %s\n\n", root->option_2->text);
        printf("\nEnter 1/2: ");
        char res[10];
        fgets(res, sizeof(res), stdin);
        res[strcspn(res,"\n")] = 0;
        if(strcmp(res, "1") == 0){
            if(root->option_1->event_index < 0){
                traverseDialogue(ems, root->option_1, detective);
            }
            else{
                triggerEMS(ems, root->option_1->event_index, detective, ems->event_queue[root->event_index].type);
            }
        }
        else if(strcmp(res, "2") == 0){
            if(root->option_2->event_index < 0){
                traverseDialogue(ems, root->option_2, detective);
            }
            else{
                triggerEMS(ems, root->option_2->event_index, detective, ems->event_queue[root->event_index].type);
            }
        }
        }
    }
}