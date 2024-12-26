#ifndef STRUCT_H
#define STRUCT_H

#include <stdbool.h>
typedef struct Inventory Inventory; typedef struct Journal Journal; typedef struct Location Location; typedef struct Item Item; typedef struct Event Event; typedef struct Detective Detective; typedef struct ItemCollection ItemCollection; typedef struct CaseFile CaseFile; typedef struct DialogueTree DialogueTree;typedef struct EMSEvent EMSEvent;typedef struct Character Character;typedef struct EMS EMS; typedef struct CFCollection CFCollection;

struct Item { char name[100]; int size; int capacity; bool is_collectable; char description[5000]; bool is_container; Item **items; int container_index;};
struct CaseFile{char name[100]; char case_no [20]; char date[20]; char lead[50]; char location[200]; char summary[1000]; char victims[500];  char evidence[601]; char suspects[500];};
struct Weapon { char name[100]; char description[1000]; int damage; }; 
struct Event { char name[100]; char content[1000]; char timestamp[80]; };
struct Journal { int size; int capacity; Event *events; }; 
struct Inventory { int size; int cf_size; int cf_cap;int capacity; Item **items; CaseFile *case_files;};
struct Location { char name[100]; char description[1000]; char locked_item[100]; int size; int capacity; int case_file_num; int case_file_cap; int loc_size; int loc_cap; CaseFile *case_files; Item **items; Location *north; Location *west; Location *east; Location *south; Location **inner_locations; Location *outer_location;};
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
    int event_index;
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
#endif
