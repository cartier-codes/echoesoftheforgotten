#include "headers/dialogue.h"

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