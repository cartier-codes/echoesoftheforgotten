#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <windows.h>
#include "headers/struct.h"
#include "headers/initialise.h"
#include "headers/print.h"
#include "headers/add.h"
#include "headers/remove.h"
#include "headers/dialogue.h"
#include "headers/ems.h"
#include "headers/commands.h"

int main(void)
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
    Location office_street;
    Location bodega;
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
    initialiseRoom(&bodega, "Bodega","Test", "");
    initialiseRoom(&office_street, "City","The street outside the police department is a patchwork of oil stains and faded asphalt, worn down by years of heavy footsteps and quiet patrol cars. Faded yellow lines mark the cracked road, and a flickering streetlamp throws uneven shadows across the chipped concrete steps leading up to the station. A patrol car idles by the curb, its engine a low grumble in the still night air. The faint scent of gasoline and damp pavement lingers, mingling with the distant murmur of city life. Above, the dull hum of a neon sign buzzes, casting a pale blue reflection in the rain-soaked street below.","");
    initialiseRoom(&office, "Office", "Your office, with its smoke-stained walls and yellowing case files scattered across a cluttered desk, hums with the persistent drone of an old fan and carries the faint scent of stale coffee and cigarettes.", "Office Key");

    addConnection(&lobby, &office, "east");
    addConnection(&lobby, &office_street,"south");
    addInnerToRoom(&office_street, &bodega);


    initialiseItemLookup(&ItemCO);
    initialiseCFLookup(&CFCO);
    initialiseCase(&casefileno1,"The Campside Kidnapping","037-992", "23/03/1978", "Detective Chris Levins (circa 1993: Max Kruger.)", "In the late night of March 23, 1978, Michael O\'Connell, a four year old toddler, was reported missing by his parents. Michael was last seen in his parents\' tent asleep in Ridgewood National Park, which is known for its clear trails and beautiful scenery. According to his family, they have no recollection of any people of interest. They do not remember the day prior. They woke the next morning, and Michael was nowhere to be found.", "Victim Information:\nName: Michael O\'Connell\nAge: 4\nOccupation: N/A\nPhysical Description:\n\tHeight: 3\"4\n\tWeight: 40lbs\n\tHair: Brown, buzzed\n\tEyes: Green\n\tDistinguishing Features:\n\t\tScar through his right eyebrow\nLast Known Whereabouts:\n\tLocation: 200m north west of Rigewood National Park\'s main entrance\n\tTime: 2200, 23/03/1978", "Location: Ridgewood", "Evidence:\n\tLarge adult sized footprints found leading from the tent toward the rear entrance.\n\tSuspicious Activity:\n\t\tA fisherman, identified as Alfred Zimmerman, was seen lingering around the campsite earlier that evening.\n\t\tZimmerman was detained on child abduction charges, but released under insufficient evidence.\n\t\tWitnesses reported Zimmerman staring at the children from across the lake, acting \"feverish\"\n\tSearch Efforts:\n\t\tSearh teams combed through the park over the span of 3 days.\n\t\tNo physical evidence of Michael was found, no signs of struggle or foul play at the site.", "Primary Suspect:\n\tFormer felon- Alfred Zimmerman");
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
    free(office_street.inner_locations);

    return 0;

}
