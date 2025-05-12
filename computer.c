#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include "headers/struct.h"

#define SLEEP(ms) usleep((ms) * 1000)
#define MAX_NAME_LENGTH 50
#define MAX_ADDRESS_LENGTH 100
#define MAX_ARRESTS 10
#define MAX_PERSONS 100 // Adjust as needed
#define MAX_ADDRESSES 50
#define MAX_CF 50
#define MAX_AR 50
#define MAX_EV 50

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
typedef struct OfficerList OfficerList;
typedef struct GBOX GBOX;
typedef struct GMessage GMessage;

struct Address
{
    char address_id[10];
    char street[MAX_ADDRESS_LENGTH];
    char city[MAX_ADDRESS_LENGTH];
    char zip_code[MAX_ADDRESS_LENGTH];
    char case_no[20][20];
    Person *person_list[10];
    int person_count;

};
struct GBOX{
    GMessage *messages[100];
    int message_count;
};
struct AddressList{
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
    char DOB[20];
    char status[50];
};

struct PersonList{
    Person *person_list[MAX_PERSONS];
    int person_count;
};


struct AdoptionReport{
    char report_id[20];
    Person *child;
    Person *adoptive_parents[2];
    Person *original_parents[2];
    char adoption_notes[300];
};

struct ReportList{
    AdoptionReport *reports[MAX_AR];
    int report_count;
};

struct Evidence{
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

struct EvidenceArchive{
    Evidence *evidence_items[MAX_EV];
    int total_items;
};

struct AuditLog{
    char audit_id[10];
    char actionType[50];
    char record_id[10];
    char timestamp[20];
    Officer *officer;
    char action_description[200];
};

struct LogList{
  AuditLog *logs[MAX_ADDRESSES];
  int log_count;
};
struct DisciplinaryAction{
    char d_id[10];
    Officer *officer;
    char date[20];
    char summary[200];
    char severity[100];
};
struct Officer{
    char officer_id[20];
    char title[20];
    char username[50];
    char password[50];
    char first_name[50];
    char last_name[50];
    char b_no[20];
    int clearance;
    DisciplinaryAction *disciplinary_actions[3];
    GBOX *mailbox;
};

struct OfficerList{
    Officer *officers[MAX_ADDRESSES];
    int officer_count;
};

struct SNTRPH{
    PersonList personlist;
    AddressList addressList;
    EvidenceArchive evidenceArchive;
    ReportList reportList;
    LogList logList;
    OfficerList officerList;
    Officer *current_user;
};

struct GMessage{
    Officer *sender;
    Officer *recipient;
    char subject[100];
    char timestamp[30];
    int clearance;
    char body[1000];
    bool read;
};


void PrintLine(char *text){
    printf("%s\n", text);
    SLEEP(800);
}
void startMenu(SNTRPH *sntrph);

void MailboxUpdate();

void startMenu(SNTRPH *sntrph) {
    system("clear"); // Clear screen on macOS

    PrintLine("--------------------------------------------------");
    PrintLine("  SAINT. RAPHAEL DATABASE SYSTEM v1.0\n");
    PrintLine("  New York City Police Department");
    PrintLine("  © 1994 NYPD Internal Systems Division");
    PrintLine("--------------------------------------------------");

    PrintLine("Initialising system modules...");
    PrintLine("[✓] Core Database");
    PrintLine("[✓] Evidence Archives");
    PrintLine("[✓] Officer Records");
    PrintLine("[✓] Case File Index");
    PrintLine("[✓] Secure Communications Link");
    PrintLine("[✓] Internal Audit Trail");

    PrintLine("\nSystem status: ONLINE");
    PrintLine("--------------------------------------------------");
    PrintLine("Authentication Required:");

    bool unlocked = false;
    while (!unlocked) {
        char username[64];
        printf("Username: ");
        fgets(username, sizeof(username), stdin);
        username[strcspn(username, "\n")] = 0;

        char* password = getpass("Password: ");
        password[strcspn(password, "\n")] = 0;
        PrintLine("[✓] Credentials received.");
        bool found_user = false;
        for (int i = 0; i < sntrph->officerList.officer_count; i++) {
            Officer *officer = sntrph->officerList.officers[i];
            PrintLine("[✓] Verifying officer record...");
            if (strcmp(username, officer->username) == 0) {
                found_user = true;
                PrintLine("[✓] Processing credentials.");
                if (strcmp(password, officer->password) == 0) {
                    PrintLine("--------------------------------------------------");
                    SLEEP(500);
                    PrintLine("[✓] Password authenticated.");

                    char welcome_message[100];
                    snprintf(welcome_message, sizeof(welcome_message), "Welcome, Officer %s %s.", officer->first_name, officer->last_name);
                    PrintLine(welcome_message);

                    char clearance[60];
                    snprintf(clearance, sizeof(clearance), "Clearance Level: %d", officer->clearance);
                    PrintLine(clearance);

                    PrintLine("--------------------------------------------------");
                    PrintLine("Accessing system...");
                    SLEEP(1000);
                    unlocked = true;
                    sntrph->current_user = officer;
                    MailboxUpdate(sntrph);
                    break;
                }
                else {
                    PrintLine("Invalid password. Please try again.");
                }
            }
        }

        if (!found_user) {
            PrintLine("This user does not exist. Please try again.");
        }
    }
}
void printMessage(GMessage *msg){
    char sender_name[120];
    snprintf(sender_name, sizeof(sender_name), "%s %s %s",msg->sender->title, msg->sender->first_name, msg->sender->last_name);
    char recipient_name[120];
    snprintf(recipient_name, sizeof(recipient_name), "%s %s %s",msg->recipient->title, msg->recipient->first_name, msg->recipient->last_name);
    printf("--------------------------------------------------\n");
        printf("  COMM-LINK SECURE MESSAGE TRANSMISSION\n");
        printf("--------------------------------------------------\n\n");
        printf("FROM:        %s\n", sender_name);
        printf("TO:          %s\n", recipient_name);
        printf("SUBJECT:     %s\n", msg->subject);
        printf("DATE:        %s\n", msg->timestamp);
        printf("CLEARANCE LEVEL:   %d\n\n", msg->clearance);

        printf("--------------------------------------------------\n");
        printf("%s\n\n", msg->body);
        printf("--------------------------------------------------\n");
        printf("[Press any key to return]\n");
        int c;
        while ((c = getchar()) != '\n' && c != EOF); // clear stdin
        getchar(); // wait for actual key press


}

void MailboxUpdate(SNTRPH *sntrph){
    for(int i = 0; i < sntrph->current_user->mailbox->message_count; i++){
        if(!sntrph->current_user->mailbox->messages[i]->read){
            printf("--------------------------------------------------\n");
            PrintLine("You have unread messages.");
            break;
        }
    }
}
// SEND MESSAGE DET88411;

void ViewMailbox(SNTRPH *sntrph) {
    GBOX *mailbox = sntrph->current_user->mailbox;
    if (mailbox->message_count == 0) {
        PrintLine("You have no messages.");
        return;
    }

    for (int i = 0; i < mailbox->message_count; i++) {
        GMessage *msg = mailbox->messages[i];
        printf("\n[%d] %s | From: %s %s | %s\n", i + 1,
               msg->read ? "READ" : "UNREAD",
               msg->sender->first_name, msg->sender->last_name,
               msg->subject);
    }

    printf("\nEnter message number to view (or 0 to return): ");
    int choice;
    scanf("%d", &choice);
    getchar(); // Consume newline left by scanf

    if (choice > 0 && choice <= mailbox->message_count) {
        GMessage *selected = mailbox->messages[choice - 1];
        selected->read = true;
        printMessage(selected);
    }
}
void sendMessage(Officer *sender, char *token, SNTRPH *sntrph){
    char w_token[50];
    strcpy(w_token, token);
    for(int i = 0; i < sntrph->officerList.officer_count; i++){
        if(strcmp(w_token, sntrph->officerList.officers[i]->b_no) == 0){
            PrintLine("[✓] User Found.");
            if(sender->clearance + 2 <= sntrph->officerList.officers[i]->clearance){
                PrintLine("[✗] You are not authorized to contact this officer.");
            }
            else{
                GMessage *msg = malloc(sizeof(GMessage));  // ✅ allocate on heap
                msg->sender = sender;
                msg->recipient = sntrph->officerList.officers[i];
                msg->clearance = msg->recipient->clearance;
                msg->read = false;

                PrintLine("[✓] Clearance verified.");

                char subject[100];
                printf("Subject:");
                fgets(subject, sizeof(subject), stdin);

                char timestamp[20];
                printf("Timestamp:");
                fgets(timestamp, sizeof(timestamp), stdin);

                char body[1000];
                printf("Message:");
                fgets(body, sizeof(body), stdin);

                subject[strcspn(subject, "\n")] = 0;
                timestamp[strcspn(timestamp, "\n")] = 0;
                body[strcspn(body, "\n")] = 0;

                strcpy(msg->subject, subject);
                strcpy(msg->timestamp, timestamp);
                strcpy(msg->body, body);

                sntrph->officerList.officers[i]->mailbox->messages[
                    sntrph->officerList.officers[i]->mailbox->message_count++
                ] = msg;

                printMessage(sntrph->officerList.officers[i]->mailbox->messages[sntrph->officerList.officers[i]->mailbox->message_count-1]);
                printf("--------------------------------------------------\n");

            }
        }
    }
}


void populateDB(SNTRPH *sntrph) {
    Officer *officer = malloc(sizeof(Officer));
    GBOX *mailbox = malloc(sizeof(GBOX));


    memset(officer, 0, sizeof(Officer));
    memset(mailbox, 0, sizeof(GBOX));

    strcpy(officer->officer_id, "OF001");
    strcpy(officer->username, "MAXKRUGER");
    strcpy(officer->password, "eightballboy");
    strcpy(officer->title, "Sr Detective.");
    strcpy(officer->first_name, "Maximus");
    strcpy(officer->last_name, "Kruger");
    strcpy(officer->b_no, "DET88411");
    officer->clearance = 3;
    officer->mailbox = mailbox;

    Officer *officer1 = malloc(sizeof(Officer));
    GBOX *mailbox2 = malloc(sizeof(GBOX));
    memset(officer1, 0, sizeof(Officer));
    memset(mailbox2, 0, sizeof(GBOX));

    strcpy(officer1->officer_id, "OF002");
    strcpy(officer1->username, "EMORYKRUGER");
    strcpy(officer1->password, "SNTRPH_1978");
    strcpy(officer1->title, "Commissioner.");
    strcpy(officer1->first_name, "Emory");
    strcpy(officer1->last_name, "Kruger");
    strcpy(officer1->b_no, "COMM117");
    officer1->clearance = 4;
    officer1->mailbox = mailbox2;

    sntrph->officerList.officers[sntrph->officerList.officer_count++] = officer;
    sntrph->officerList.officers[sntrph->officerList.officer_count++] = officer1;

    printf("Officer name: %s\n", sntrph->officerList.officers[0]->first_name);
    printf("Officer name: %s\n", sntrph->officerList.officers[1]->first_name);
}

void processCommand(SNTRPH *sntrph, char *input){
    char w_input[50];
    strcpy(w_input, input);
    char delimiters[10] = " \n";
    char *token = strtok(w_input, delimiters);
    if(strcmp(token, "SEND") == 0){
        PrintLine("[✓] SEND Found.");
       char *token_2 =strtok(NULL, delimiters);
       if(strcmp(token_2, "MESSAGE") == 0){
           PrintLine("[✓] MESSAGE Found.");
           char *bdg_no =strtok(NULL, delimiters);
           for(int i = 0; i < sntrph->officerList.officer_count; i++){
               char name[120];
               snprintf(name, sizeof(name), "%s %s %s", sntrph->officerList.officers[i]->title, sntrph->officerList.officers[i]->first_name, sntrph->officerList.officers[i]->last_name);
               if(strcmp(bdg_no, sntrph->officerList.officers[i]->b_no) == 0){
                   printf("[✓] Credentials match officer: %s\n", name);
                   sendMessage(sntrph->current_user, bdg_no , sntrph);
               }
           }

       }
    }
    else if(strcmp(token, "LOG") == 0){
              char *token_2 =strtok(NULL, delimiters);
              if(strcmp(token_2, "OUT") == 0){
                  sntrph->current_user = NULL;
                  startMenu(sntrph);
              }
          }
    else if(strcmp(token, "VIEW") == 0){
                    char *token_2 =strtok(NULL, delimiters);
                    if(strcmp(token_2, "MAILBOX") == 0){
                        ViewMailbox(sntrph);
                    }
                }
    // sendMessage(sntrph->current_user, , &sntrph);
}

void initSNTRPH(SNTRPH *sntrph) {
    if (!sntrph) return;

    // Initialize person list
    sntrph->personlist.person_count = 0;
    for (int i = 0; i < MAX_PERSONS; i++) {
        sntrph->personlist.person_list[i] = NULL;
    }

    // Initialize address list
    sntrph->addressList.address_count = 0;
    for (int i = 0; i < MAX_ADDRESSES; i++) {
        sntrph->addressList.addresses[i] = NULL;
    }

    // Initialize evidence archive
    sntrph->evidenceArchive.total_items = 0;
    for (int i = 0; i < MAX_EV; i++) {
        sntrph->evidenceArchive.evidence_items[i] = NULL;
    }

    // Initialize report list
    sntrph->reportList.report_count = 0;
    for (int i = 0; i < MAX_AR; i++) {
        sntrph->reportList.reports[i] = NULL;
    }

    // Initialize log list
    sntrph->logList.log_count = 0;
    for (int i = 0; i < MAX_ADDRESSES; i++) {
        sntrph->logList.logs[i] = NULL;
    }

    // Initialize officer list
    sntrph->officerList.officer_count = 0;
    for (int i = 0; i < MAX_ADDRESSES; i++) {
        sntrph->officerList.officers[i] = NULL;
    }

    // No current user logged in yet
    sntrph->current_user = NULL;
}
int main() {
    SNTRPH sntrph;
    initSNTRPH(&sntrph);
    populateDB(&sntrph);
    startMenu(&sntrph);

    char input[50];
    while (true) {
        printf("\n> Enter command:");
        fgets(input, sizeof(input), stdin);
        size_t length = strlen(input);
        if(length < 4){
            printf("Invalid command.\n");
            continue;
        }
        input[strcspn(input, "\n")] = 0; // Remove newline
        if (strcmp(input, "quit") == 0) {
            break;
        }
        processCommand(&sntrph, input);
    }

    return 0;
}
