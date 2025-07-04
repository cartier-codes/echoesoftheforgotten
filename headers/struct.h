#ifndef STRUCT_H
#define STRUCT_H

#include <stdbool.h>
typedef struct Inventory Inventory; typedef struct Journal Journal; typedef struct Location Location; typedef struct Item Item; typedef struct Event Event; typedef struct Detective Detective; typedef struct ItemCollection ItemCollection; typedef struct CaseFile CaseFile; typedef struct DialogueTree DialogueTree;typedef struct EMSEvent EMSEvent;typedef struct Character Character;typedef struct EMS EMS; typedef struct CFCollection CFCollection;
typedef struct SNTRPH SNTRPH;
typedef struct Address Address;
typedef struct Person Person;
typedef struct ModificationLog ModificationLog;
typedef struct AdoptionReport AdoptionReport;
typedef struct Evidence Evidence;
typedef struct AuditLog AuditLog;
typedef struct DisciplinaryAction DisciplinaryAction;
typedef struct Officer Officer;
typedef struct EvidenceArchive EvidenceArchive;
typedef struct AddressList AddressList;
typedef struct PersonList PersonList;
typedef struct ReportList ReportList;
typedef struct LogList LogList;
typedef struct CaseList CaseList;
typedef struct OfficerList OfficerList;
typedef struct GBOX GBOX;
typedef struct GMessage GMessage;
typedef struct Incident Incident;
typedef struct IncidentList IncidentList;

#define SLEEP(ms) Sleep(ms);
#define MAX_NAME_LENGTH 50
#define MAX_ADDRESS_LENGTH 100
#define MAX_ARRESTS 10
#define MAX_PERSONS 100 // Adjust as needed
#define MAX_ADDRESSES 200
#define MAX_CF 50
#define MAX_AR 50
#define MAX_EV 50
#define MAX_PASSWORD_LENGTH 64
#define PAGE_SIZE 10

struct Address
{
    char address_id[10];
    char street[MAX_ADDRESS_LENGTH];
    char city[MAX_ADDRESS_LENGTH];
    char zip_code[MAX_ADDRESS_LENGTH];
    char case_no[20][20];
    Person *person_list[10];
    int person_count;
    int case_count;
    bool flagged;
};
struct GBOX
{
    GMessage *messages[100];
    int message_count;
};
struct AddressList
{
    Address *addresses[MAX_ADDRESSES];
    int address_count;
};
struct Person
{
    char person_id[10];
    char first_name[MAX_NAME_LENGTH];
    char last_name[MAX_NAME_LENGTH];
    Address *address;
    char criminal_record[500];
    char relationship_status[50];
    char DOB[30];
    char status[50];
};
struct PersonList
{
    Person *person_list[MAX_ADDRESSES];
    int person_count;
};
struct AdoptionReport
{
    char report_id[20];
    Person *child;
    Person *adoptive_parents[2];
    Person *original_parents[2];
    char adoption_notes[300];
};
struct ReportList
{
    AdoptionReport *reports[MAX_AR];
    int report_count;
};
struct Evidence
{
    char evidence_id[10];
    char name[50];
    char status[20];
    char type[50];
    char case_id[10];
    char description[500];
    char file_path[50];
    char date[20];
    char custody_chain[5][50];
    bool is_restricted;
    char notes[100];
};
struct EvidenceArchive
{
    Evidence *evidence_items[MAX_ADDRESSES];
    int total_items;
};
struct AuditLog
{
    char audit_id[10];
    char actionType[50];
    char record_id[10];
    char timestamp[20];
    Officer *officer;
    char action_description[200];
};
struct LogList
{
    AuditLog *logs[MAX_ADDRESSES];
    int log_count;
};
struct DisciplinaryAction
{
    char d_id[10];
    Officer *officer;
    char date[20];
    char summary[200];
    int severity;
};
struct Officer
{
    char officer_id[20];
    char title[20];
    char username[50];
    char password[50];
    Person *base;
    char b_no[20];
    int clearance;
    DisciplinaryAction *disciplinary_actions[3];
    GBOX *mailbox;
};
struct OfficerList
{
    Officer *officers[MAX_ADDRESSES];
    int officer_count;
};

struct CaseList{
    CaseFile *cases[MAX_ADDRESSES];
    CaseFile *archive[MAX_ADDRESSES];
    CaseFile *deleted[MAX_ADDRESSES];
    int case_count;
    int archive_count;
    int deleted_count;

};
 struct Incident {
    char incident_id[10];
    char date[11];
    char type[50];              // YYYY-MM-DD
    char time[6];               // HH:MM
    char location[100];
    char description[256];
    char lead[10];
    char status[20];            // e.g. "Open", "Closed"
};

struct IncidentList{
    Incident *incidents[MAX_ADDRESSES];
    int i_count;
} ;


struct SNTRPH
{
    PersonList personlist;
    AddressList addressList;
    EvidenceArchive evidenceArchive;
    ReportList reportList;
    LogList logList;
    CaseList caseList;
    OfficerList officerList;
    IncidentList incidentList;

    Officer *current_user;
    
};
struct GMessage
{
    Officer *sender;
    Officer *recipient;
    char subject[100];
    char timestamp[30];
    int clearance;
    char body[1000];
    bool read;
};
struct Item { char name[100]; int size; int capacity; bool is_collectable; char description[5000]; bool is_container; Item **items; int container_index;};
struct CaseFile {
    char case_no[20];
    char name[100];                // Optional title or case name
    char date[20];
    char lead[50];                 // Officer ID or name
    Address *location;             // Pointer to Address
    char summary[1000];
    char type[100];
    Person *victims[5];
    int victim_count;

    Person *suspects[5];
    int suspect_count;

    Evidence *evidence[20];
    int evidence_count;

    char evidence_notes[600];      // Summary or key points

    bool is_locked;
    char status[20];               // e.g. "OPEN", "CLOSED", "ARCHIVED"
};

struct Weapon { char name[100]; char description[1000]; int damage; };
struct Event { char name[100]; char content[1000]; char timestamp[80]; };
struct Journal { int size; int capacity; Event *events; };
struct Inventory { int size; int cf_size; int cf_cap;int capacity; Item **items; CaseFile *case_files;};
struct Location { char name[100]; char description[1000]; char locked_item[100]; int size; int capacity; int case_file_num; int case_file_cap; int loc_size; int loc_cap; CaseFile *case_files; Item **items; Location *north; Location *west; Location *east; Location *south; Location **inner_locations; Location *outer_location;};
struct ItemCollection{ int size; int capacity; Item **items; };
struct CFCollection{ int size; int capacity; CaseFile *case_files; };
struct Detective { char officer_id[20]; Inventory inventory; Journal journal; Location *current_location; };

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
//27/06/2025

#endif
