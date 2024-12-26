#include "headers/ems.h"

void updateEMS(EMS *ems, Detective *detective, enum Command command, char *token, CFCollection *CFCO)
{
    switch (command)
    {
    case MOVE:
        for (int i = 0; i < ems->size; i++)
        {
            switch (ems->event_queue[i].type)
            {
            case DIALOGUE:
                if ((ems->event_queue[i].location == detective->current_location && ems->event_queue[i].event_state == false) && (ems->event_queue[i].dependency_index < 0 || ems->event_queue[ems->event_queue[i].dependency_index].event_state))
                {
                    traverseDialogue(ems, &ems->event_queue[i].Dialogue, detective);
                    if (ems->event_queue[i].cf_size > 0)
                    {
                        for (int j = 0; j < ems->event_queue[i].cf_size; j++)
                        {
                            addCaseToInventory(&ems->event_queue[i].cf_gained[j], &detective->inventory);
                        }
                    }
                    if (ems->event_queue[i].size > 0)
                    {
                        for (int j = 0; j < ems->event_queue[i].size; j++)
                        {
                            addToInventory(ems->event_queue[i].items_gained[j], &detective->inventory);
                        }
                    }
                    printf("\nYou have acquired items.\n");
                    printf("Before setting: event_state = %d\n", ems->event_queue[i].event_state); // Debugging
                    ems->event_queue[i].event_state = true;
                    printf("After setting: event_state = %d\n", ems->event_queue[i].event_state); // Debugging
                };
                break;
            case COMBAT:
                break;
            case RANDOM:
                break;
            }
        }
        break;
    case EXAMINE:
        for (int i = 0; i < ems->size; i++)
        {
            switch (ems->event_queue[i].type)
            {
            case DIALOGUE:
                if ((ems->event_queue[i].location == NULL ||ems->event_queue[i].location == detective->current_location) && (!ems->event_queue[i].event_state) && (ems->event_queue[i].dependency_index < 0 || ems->event_queue[ems->event_queue[i].dependency_index].event_state))
                {
                    traverseDialogue(ems,&ems->event_queue[i].Dialogue, detective);
                    ems->event_queue[i].event_state = true;
                }
                break;
            case COMBAT:
                break;
            case RANDOM:
                break;
            }
        }
        break;
    case COMBAT:
        break;
    case RANDOM:
        break;
    case OPEN:
        break;
    }
}

void triggerEMS(EMS *ems, int event_index, Detective *detective, enum EventType event){
    switch(event){
        case DIALOGUE:
            if ((ems->event_queue[event_index].location == NULL ||ems->event_queue[event_index].location == detective->current_location) && (!ems->event_queue[event_index].event_state) && (ems->event_queue[event_index].dependency_index < 0 || ems->event_queue[ems->event_queue[event_index].dependency_index].event_state)){
                traverseDialogue(ems, &ems->event_queue[event_index].Dialogue, detective);
                ems->event_queue[event_index].event_state = true;
            }
        case COMBAT:
            if ((ems->event_queue[event_index].location == NULL ||ems->event_queue[event_index].location == detective->current_location) && (!ems->event_queue[event_index].event_state) && (ems->event_queue[event_index].dependency_index < 0 || ems->event_queue[ems->event_queue[event_index].dependency_index].event_state)){
                printf("\n## COMBAT INITIATED ##\n");
                ems->event_queue[event_index].event_state = true;
            }
        case RANDOM:
            break;
    }
}