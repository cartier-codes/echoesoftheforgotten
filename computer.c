#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <windows.h>
#include "headers/struct.h"
#include <time.h>
#include <conio.h>

#define SLEEP(ms) Sleep(ms);
#define MAX_NAME_LENGTH 50
#define MAX_ADDRESS_LENGTH 100
#define MAX_ARRESTS 10
#define MAX_PERSONS 100 // Adjust as needed
#define MAX_ADDRESSES 100
#define MAX_CF 50
#define MAX_AR 50
#define MAX_EV 50
#define MAX_PASSWORD_LENGTH 64

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
    char DOB[20];
    char status[50];
};
struct PersonList
{
    Person *person_list[MAX_PERSONS];
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
    Evidence *evidence_items[MAX_EV];
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
    char severity[100];
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
struct SNTRPH
{
    PersonList personlist;
    AddressList addressList;
    EvidenceArchive evidenceArchive;
    ReportList reportList;
    LogList logList;
    OfficerList officerList;
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

void increment_id(char *id_string)
{
    int length = strlen(id_string);
    int i;

    // Find the point where the numeric part begins
    int number_start = 0;
    while (number_start < length && isalpha(id_string[number_start]))
    {
        number_start++;
    }

    if (number_start == length || number_start == 0)
    {
        printf("Invalid ID format. ID should start with 1 or 2 letters followed by digits.\n");
        return;
    }

    // Extract numeric part and increment it
    int number = atoi(id_string + number_start);
    number++;

    // Rebuild the ID: Prefix + Incremented number
    snprintf(id_string + number_start, length - number_start + 1, "%0*ld", length - number_start, number);
}
void get_current_date(char *date_buffer)
{
    // Get current time
    time_t t = time(NULL);

    // Check if time is valid
    if (t == (time_t)(-1))
    {
        printf("Failed to get the current time\n");
        return;
    }

    // Convert time_t to struct tm (local time)
    struct tm *tm_info = localtime(&t);

    // Check if localtime failed
    if (tm_info == NULL)
    {
        printf("Failed to convert time\n");
        return;
    }

    // Format the date into the provided buffer (YYYY-MM-DD)
    strftime(date_buffer, 11, "%Y-%m-%d", tm_info); // Buffer should be at least 11 characters
}

void addLog(SNTRPH *sntrph, char *type, char *r_id, Officer *officer, char *desc)
{
    AuditLog *log = malloc(sizeof(AuditLog));
    char audit_id[10];
    if (sntrph->logList.log_count == 0)
    {
        strcpy(audit_id, "A000");
        strcpy(log->audit_id, audit_id);
    }
    else
    {
        strcpy(audit_id, sntrph->logList.logs[sntrph->logList.log_count - 1]->audit_id);
        increment_id(audit_id);
        strcpy(log->audit_id, audit_id);
    }
    char date[15];
    get_current_date(date);
    strcpy(log->timestamp, date);
    strcpy(log->actionType, type);
    strcpy(log->record_id, r_id);
    log->officer = officer;
    strcpy(log->action_description, desc);

    sntrph->logList.logs[sntrph->logList.log_count++] = log;
}
void PrintLine(char *text)
{
    printf("%s\n", text);
    SLEEP(800);
}
void startMenu(SNTRPH *sntrph);

void addEvidence(SNTRPH *sntrph)
{
    char name[50];
    char status[20];
    char type[50];
    char case_id[10];
    char desc[500];
    char file_path[50];
    char date[20];
    char custody_chain[5][50];
    bool is_restricted;
    char notes[100];
    Evidence *ev = malloc(sizeof(Evidence));

    get_current_date(date);
    char id[10];
    if (sntrph->evidenceArchive.total_items == 0)
    {
        strcpy(id, "E000");
        strcpy(ev->evidence_id, id);
    }
    else
    {
        strcpy(id, sntrph->evidenceArchive.evidence_items[sntrph->evidenceArchive.total_items - 1]->evidence_id);
        increment_id(id);
        strcpy(ev->evidence_id, id);
    }
    printf("> Enter the name of your evidence: ");
    fgets(name, sizeof(name), stdin);
    printf("> Enter the status of your evidence: ");
    fgets(status, sizeof(status), stdin);
    printf("> Enter the type of your evidence: ");
    fgets(type, sizeof(type), stdin);
    printf("> Enter the case ID of your evidence: ");
    fgets(case_id, sizeof(case_id), stdin);
    printf("> Enter the description of your evidence: ");
    fgets(desc, sizeof(desc), stdin);
    printf("> Enter the file path of your evidence: ");
    fgets(file_path, sizeof(file_path), stdin);
    printf("> Enter any notes on your evidence: ");
    fgets(notes, sizeof(notes), stdin);
    for (int i = 0; i < 5; i++)
    {
        char buffer[50];
        printf("> Enter an officer ID that handled this evidence: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        if (strcmp(buffer, "") == 0)
        {
            PrintLine("Exiting...");
            break;
        }
        strcpy(custody_chain[i], buffer);
    }
    ev->is_restricted = false;
    strcpy(ev->name, name);
    strcpy(ev->status, status);
    strcpy(ev->type, type);
    strcpy(ev->case_id, case_id);
    strcpy(ev->description, desc);
    strcpy(ev->file_path, file_path);
    strcpy(ev->notes, notes);
    strcpy(ev->date, date);
    sntrph->evidenceArchive.evidence_items[sntrph->evidenceArchive.total_items++] = ev;

    printf("[\u2713] EVIDENCE ADDED: %s", ev->evidence_id);
    char officer_name[120];
    snprintf(officer_name, sizeof(officer_name), "%s %s %s", sntrph->current_user->title, sntrph->current_user->base->first_name, sntrph->current_user->base->last_name);
    char description[200];
    snprintf(description, sizeof(description), "%s ADDED EVIDENCE: %s ON %s", officer_name, ev->evidence_id, ev->date);

    addLog(sntrph, "EVIDENCE ADDITION", ev->case_id, sntrph->current_user, description);
}
void deleteEvidence(SNTRPH *sntrph, const char *evidence_id_to_delete)
{
    bool found = false;
    char date[15];
    get_current_date(date);
    // Loop through the evidence archive to find the matching evidence ID
    for (int i = 0; i < sntrph->evidenceArchive.total_items; i++)
    {
        // If we find a match
        char officer_name[120];
        snprintf(officer_name, sizeof(officer_name), "%s %s %s", sntrph->current_user->title, sntrph->current_user->base->first_name, sntrph->current_user->base->last_name);
        if (strcmp(sntrph->evidenceArchive.evidence_items[i]->evidence_id, evidence_id_to_delete) == 0)
        {
            found = true;
            if (sntrph->current_user->clearance < 3)
            {
                PrintLine("ACCESS DENIED: INSUFFICIENT CLEARANCE\n");
                PrintLine("!ATTEMPT FOLLOWED IN AUDIT TRAIL!!");

                char description[200];
                snprintf(description, sizeof(description), "%s ATTEMPTED TO DELETE EVIDENCE: %s ON %s", officer_name, sntrph->evidenceArchive.evidence_items[i]->evidence_id, date);
                addLog(sntrph, "EVIDENCE DELETION ATTEMPT", sntrph->evidenceArchive.evidence_items[i]->evidence_id, sntrph->current_user, description);
            }
            // Print confirmation message
            printf("> Evidence with ID %s found. Deleting it...\n", evidence_id_to_delete);

            // Shift all evidence items after the deleted item to the left
            for (int j = i; j < sntrph->evidenceArchive.total_items - 1; j++)
            {
                sntrph->evidenceArchive.evidence_items[j] = sntrph->evidenceArchive.evidence_items[j + 1];
            }

            char description[200];
            snprintf(description, sizeof(description), "%s DELETED EVIDENCE: %s ON %s", officer_name, sntrph->evidenceArchive.evidence_items[i]->evidence_id, date);
            // Decrease the total item count
            sntrph->evidenceArchive.total_items--;
            addLog(sntrph, "EVIDENCE DELETION", sntrph->evidenceArchive.evidence_items[i]->evidence_id, sntrph->current_user, description);
            // Confirm the deletion
            break;
        }
    }

    // If no matching evidence was found
    if (!found)
    {
        PrintLine("> No evidence found with the given ID.");
    }
}

void printEvidence(Evidence *ev)
{
    printf("--------------------------------------------------\n");
    printf("             EVIDENCE RECORD DISPLAY\n");
    printf("--------------------------------------------------\n\n");

    printf("EVIDENCE ID:        %s\n", ev->evidence_id);
    printf("NAME:               %s\n", ev->name);
    printf("TYPE:               %s\n", ev->type);
    printf("STATUS:             %s\n", ev->status);
    printf("CASE ID:            %s\n", ev->case_id);
    printf("DATE LOGGED:        %s\n", ev->date);
    printf("FILE PATH:          %s\n", ev->file_path);
    printf("RESTRICTED ACCESS:  %s\n", ev->is_restricted ? "Yes" : "No");

    printf("\n--------------------------------------------------\n");
    printf("DESCRIPTION:\n");
    printf("%s\n", ev->description);

    printf("\nCUSTODY CHAIN:\n");
    for (int i = 0; i < 5; i++)
    {
        if (ev->custody_chain[i][0] != '\0')
        {
            printf("  - %s\n", ev->custody_chain[i]);
        }
    }

    if (ev->notes[0] != '\0')
    {
        printf("\nNOTES:\n");
        printf("%s\n", ev->notes);
    }

    printf("--------------------------------------------------\n");
    printf("[Press any key to return]\n");

    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;      // clear stdin
    getchar(); // wait for actual key press
}

void viewOfficer(Officer *off)
{
    printf("--------------------------------------------------\n");
    printf("           SAINT-RAPHAEL OFFICER TRAIL VIEW\n");
    printf("--------------------------------------------------\n\n");

    printf("TITLE:        %s\n", off->title);
    printf("FIRST NAME:        %s\n", off->base->first_name);
    printf("LAST NAME:        %s\n", off->base->last_name);
    printf("BADGE NO:        %s\n", off->b_no);
    printf("--------------------------------------------------\n\n");
    printf("CRIMINAL REC:        %s\n", off->base->criminal_record);
    printf("ADDRESS:        %s,%s,%s\n", off->base->address->street, off->base->address->city, off->base->address->zip_code);
    printf("--------------------------------------------------\n\n");
    if(off->disciplinary_actions[0] != NULL){
        for(int i = 0; i < 3; i++){
            if(off->disciplinary_actions[i] != NULL){
                printf("DISCIPLINARY <%d> SUMMARY:        %s\n", i+1, off->disciplinary_actions[i]->summary);
            }
        }
        printf("--------------------------------------------------\n\n");
    }
    printf("[Press Enter to EXIT]\n");
    getchar();
}

void viewOfficers(SNTRPH *sntrph)
{
    printf("--------------------------------------------------\n");
    printf("           SAINT-RAPHAEL OFFICERS TRAIL VIEW\n");
    printf("--------------------------------------------------\n\n");
    for (int i = 0; i < sntrph->officerList.officer_count; i++)
    {
        char officer_name[120];
        snprintf(officer_name, sizeof(officer_name), "%s %s %s",
                 sntrph->officerList.officers[i]->title,
                 sntrph->officerList.officers[i]->base->first_name,
                 sntrph->officerList.officers[i]->base->last_name);
        printf("Officer %d, ID: <%s>, %s\n", i + 1, sntrph->officerList.officers[i]->b_no, officer_name);
    }
    printf("--------------------------------------------------\n\n");
    printf("> Enter the number of an officer you'd like to have a more detailed view on: ");

    char choice[10];
    fgets(choice, sizeof(choice), stdin);
    int choice_int = atoi(choice) - 1;
    viewOfficer(sntrph->officerList.officers[choice_int]);
}
void auditTrail(LogList *logList)
{
    for (int i = 0; i < logList->log_count; i++)
    {
        AuditLog *log = logList->logs[i];

        // Create officer name line
        char officer_name[120];
        snprintf(officer_name, sizeof(officer_name), "%s %s %s",
                 log->officer->title,
                 log->officer->base->first_name,
                 log->officer->base->last_name);

        printf("--------------------------------------------------\n");
        printf("           SAINT-RAPHAEL AUDIT TRAIL IMPRINT\n");
        printf("--------------------------------------------------\n\n");

        printf("FROM:        %s\n", officer_name);
        printf("ACTION TYPE: %s\n", log->actionType);
        printf("RECORD ID:   %s\n", log->record_id);
        printf("DATE:        %s\n", log->timestamp);

        printf("\n--------------------------------------------------\n");
        printf("%s\n\n", log->action_description);
        printf("--------------------------------------------------\n");
        printf("[Press Enter to view next log]\n");

        getchar(); // Wait for user input to proceed to the next log
    }
}
void MailboxUpdate();

void startMenu(SNTRPH *sntrph)
{

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
    while (!unlocked)
    {
        char username[64];
        printf("Username: ");
        fgets(username, sizeof(username), stdin);
        username[strcspn(username, "\n")] = 0;

        char password[64];
        printf("Password: ");
        fgets(password, sizeof(password), stdin);
        password[strcspn(password, "\n")] = 0;
        PrintLine("[✓] Credentials received.");
        bool found_user = false;
        PrintLine("[✓] Verifying officer record...");
        for (int i = 0; i < sntrph->officerList.officer_count; i++)
        {
            Officer *officer = sntrph->officerList.officers[i];
            if (strcmp(username, officer->username) == 0)
            {
                found_user = true;
                PrintLine("[✓] Processing credentials.");
                if (strcmp(password, officer->password) == 0)
                {
                    PrintLine("--------------------------------------------------");
                    SLEEP(500);
                    PrintLine("[✓] Password authenticated.");

                    char welcome_message[100];
                    snprintf(welcome_message, sizeof(welcome_message), "Welcome, %s %s %s.", officer->title, officer->base->first_name, officer->base->last_name);
                    PrintLine(welcome_message);

                    char clearance[60];
                    snprintf(clearance, sizeof(clearance), "Clearance Level: %d", officer->clearance);
                    PrintLine(clearance);

                    PrintLine("--------------------------------------------------");
                    PrintLine("Accessing system...");
                    SLEEP(1000);
                    unlocked = true;
                    sntrph->current_user = officer;

                    char date[15];
                    get_current_date(date);
                    char officer_name[120];
                    snprintf(officer_name, sizeof(officer_name), "%s %s %s", sntrph->current_user->title, sntrph->current_user->base->first_name, sntrph->current_user->base->last_name);
                    char description[200];
                    snprintf(description, sizeof(description), "%s ACCESSED SYSTEM ON %s", officer_name, date);

                    addLog(sntrph, "SYSTEM ACCESSED", "", sntrph->current_user, description);

                    MailboxUpdate(sntrph);
                    break;
                }
                else
                {
                    char date[15];
                    get_current_date(date);
                    char officer_name[120];
                    snprintf(officer_name, sizeof(officer_name), "%s %s %s", sntrph->current_user->title, sntrph->current_user->base->first_name, sntrph->current_user->base->last_name);
                    char description[200];
                    snprintf(description, sizeof(description), "PERSON ATTEMPTED TO ACCESS SYSTEM THROUGH %s's ACCOUNT ON %s", officer_name, date);

                    PrintLine("Invalid password. Please try again.");
                    addLog(sntrph, "SYSTEM ACCESS ATTEMPT", "", sntrph->current_user, description);
                }
            }
        }

        if (!found_user)
        {
            PrintLine("This user does not exist. Please try again.");
        }
    }
}
void printMessage(GMessage *msg)
{
    char sender_name[120];
    snprintf(sender_name, sizeof(sender_name), "%s %s %s", msg->sender->title, msg->sender->base->first_name, msg->sender->base->last_name);
    char recipient_name[120];
    snprintf(recipient_name, sizeof(recipient_name), "%s %s %s", msg->recipient->title, msg->recipient->base->first_name, msg->recipient->base->last_name);
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
    while ((c = getchar()) != '\n' && c != EOF)
        ;      // clear stdin
    getchar(); // wait for actual key press
}

void MailboxUpdate(SNTRPH *sntrph)
{
    for (int i = 0; i < sntrph->current_user->mailbox->message_count; i++)
    {
        if (!sntrph->current_user->mailbox->messages[i]->read)
        {
            printf("--------------------------------------------------\n");
            PrintLine("You have unread messages.");
            break;
        }
    }
}
// SEND MESSAGE DET88411;

void ViewMailbox(SNTRPH *sntrph)
{
    GBOX *mailbox = sntrph->current_user->mailbox;
    if (mailbox->message_count == 0)
    {
        PrintLine("You have no messages.");
        return;
    }

    for (int i = 0; i < mailbox->message_count; i++)
    {
        GMessage *msg = mailbox->messages[i];
        printf("\n[%d] %s | From: %s %s | %s\n", i + 1,
               msg->read ? "READ" : "UNREAD",
               msg->sender->base->first_name, msg->sender->base->last_name,
               msg->subject);
    }

    printf("\nEnter message number to view (or 0 to return): ");
    int choice;
    scanf("%d", &choice);
    getchar(); // Consume newline left by scanf

    if (choice > 0 && choice <= mailbox->message_count)
    {
        GMessage *selected = mailbox->messages[choice - 1];
        selected->read = true;
        printMessage(selected);
    }
}
void sendMessage(Officer *sender, char *token, SNTRPH *sntrph)
{
    char w_token[50];
    strcpy(w_token, token);
    for (int i = 0; i < sntrph->officerList.officer_count; i++)
    {
        if (strcmp(w_token, sntrph->officerList.officers[i]->b_no) == 0)
        {
            PrintLine("[✓] User Found.");
            if (sender->clearance + 2 <= sntrph->officerList.officers[i]->clearance)
            {
                PrintLine("[✗] You are not authorized to contact this officer.");
            }
            else
            {
                GMessage *msg = malloc(sizeof(GMessage)); // ✅ allocate on heap
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

                sntrph->officerList.officers[i]->mailbox->messages[sntrph->officerList.officers[i]->mailbox->message_count++] = msg;

                printMessage(sntrph->officerList.officers[i]->mailbox->messages[sntrph->officerList.officers[i]->mailbox->message_count - 1]);
                printf("--------------------------------------------------\n");
            }
        }
    }
}

Evidence *checkEvidence(SNTRPH *sntrph, char *token)
{
    for (int i = 0; i < sntrph->evidenceArchive.total_items; i++)
    {
        if (strcmp(token, sntrph->evidenceArchive.evidence_items[sntrph->evidenceArchive.total_items - 1]->evidence_id) == 0)
        {
            return sntrph->evidenceArchive.evidence_items[sntrph->evidenceArchive.total_items - 1];
        }
    }
    return false;
}

// Helper to trim newline from fgets input
void trim_newline(char *str)
{
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
}

// Find a person by ID
Person *findPersonById(SNTRPH *sntrph, const char *id)
{
    for (int i = 0; i < sntrph->personlist.person_count; i++)
    {
        if (strcmp(sntrph->personlist.person_list[i]->person_id, id) == 0)
        {
            return sntrph->personlist.person_list[i];
        }
    }
    return NULL;
}

// Main function to add a report
void addReport(SNTRPH *sntrph)
{
    // Allocate memory for new report
    AdoptionReport *rep = malloc(sizeof(AdoptionReport));
    if (!rep)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    // Generate new report ID
    if (sntrph->reportList.report_count == 0)
    {
        strcpy(rep->report_id, "AR000");
    }
    else
    {
        strcpy(rep->report_id, sntrph->reportList.reports[sntrph->reportList.report_count - 1]->report_id);
        increment_id(rep->report_id);
    }

    char input[15];

    // Get child
    printf("> Enter the child's ID: ");
    fgets(input, sizeof(input), stdin);
    trim_newline(input);
    rep->child = findPersonById(sntrph, input);

    // Adoptive Parent 1
    printf("> Enter the first adoptive parent's ID (or leave blank): ");
    fgets(input, sizeof(input), stdin);
    trim_newline(input);
    rep->adoptive_parents[0] = (strlen(input) > 0) ? findPersonById(sntrph, input) : NULL;

    // Adoptive Parent 2
    printf("> Enter the second adoptive parent's ID (or leave blank): ");
    fgets(input, sizeof(input), stdin);
    trim_newline(input);
    rep->adoptive_parents[1] = (strlen(input) > 0) ? findPersonById(sntrph, input) : NULL;

    // Original Parent 1
    printf("> Enter the first original parent's ID (or leave blank): ");
    fgets(input, sizeof(input), stdin);
    trim_newline(input);
    rep->original_parents[0] = (strlen(input) > 0) ? findPersonById(sntrph, input) : NULL;

    // Original Parent 2
    printf("> Enter the second original parent's ID (or leave blank): ");
    fgets(input, sizeof(input), stdin);
    trim_newline(input);
    rep->original_parents[1] = (strlen(input) > 0) ? findPersonById(sntrph, input) : NULL;

    // Add to report list
    sntrph->reportList.reports[sntrph->reportList.report_count++] = rep;

    printf("✅ Adoption report %s added successfully.\n", rep->report_id);
}

Officer *check_officer(SNTRPH *sntrph, char *token){
    Officer *off = malloc(sizeof(Officer));
    if(sntrph->officerList.officer_count < 1){
        return NULL;
    }
    for(int i = 0; i < sntrph->officerList.officer_count; i++){
        if(strcmp(token, sntrph->officerList.officers[i]->b_no) == 0){
            return sntrph->officerList.officers[i];
        }
    }

}

Officer *create_officer(const char *officer_id, const char *username, const char *password, const char *title, const char *first_name, const char *last_name, const char *badge_no, int clearance, SNTRPH *sntrph)
{
    Officer *officer = malloc(sizeof(Officer));
    GBOX *mailbox = malloc(sizeof(GBOX));
    memset(officer, 0, sizeof(Officer));
    memset(mailbox, 0, sizeof(GBOX));

    officer->base = malloc(sizeof(Person));
    memset(officer->base, 0, sizeof(Person));

    printf("Checkpoint: Memory zeroed for %s\n", officer_id);

    strcpy(officer->officer_id, officer_id);
    strcpy(officer->username, username);
    strcpy(officer->password, password);
    strcpy(officer->title, title);
    strcpy(officer->base->first_name, first_name);
    strcpy(officer->base->last_name, last_name);

    printf("Checkpoint: Basic info set for %s\n", officer_id);

    if (sntrph->personlist.person_count == 0 ||
        sntrph->personlist.person_list[sntrph->personlist.person_count - 1] == NULL)
    {
        strcpy(officer->base->person_id, "P000");
    }
    else
    {
        strcpy(officer->base->person_id, sntrph->personlist.person_list[sntrph->personlist.person_count - 1]->person_id);
        increment_id(officer->base->person_id);
    }

    printf("Checkpoint: Person ID assigned to %s\n", officer_id);

    strcpy(officer->b_no, badge_no);
    officer->clearance = clearance;
    officer->mailbox = mailbox;
    officer->base->address = NULL;
    printf("Checkpoint: Final fields set for %s\n", officer_id);

    // Add to SNTRPH lists
    sntrph->personlist.person_list[sntrph->personlist.person_count++] = officer->base;
    sntrph->officerList.officers[sntrph->officerList.officer_count++] = officer;

    return officer;
}

Address *create_address(const char *street, const char *city, const char *zip_code, Person *person, SNTRPH *sntrph)
{
    Address *address = malloc(sizeof(Address));
    memset(address, 0, sizeof(Address));

    printf("Checkpoint: Memory allocated for Address\n");

    if (sntrph->addressList.address_count == 0)
    {
        strcpy(address->address_id, "AD000");
    }
    else
    {
        char id[15];
        strcpy(id, sntrph->addressList.addresses[sntrph->addressList.address_count - 1]->address_id);
        increment_id(id);
        strcpy(address->address_id, id);
    }

    strcpy(address->street, street);
    strcpy(address->city, city);
    strcpy(address->zip_code, zip_code);

    printf("Checkpoint: Address details set for %s\n", address->address_id);

    address->person_list[0] = person;
    for (int i = 1; i < 10; i++)
    {
        address->person_list[i] = NULL;
    }
    address->person_count = 1;
    person->address = address;

    sntrph->addressList.addresses[sntrph->addressList.address_count++] = address;

    printf("Checkpoint: Address linked to %s\n", person->first_name);

    return address;
}

// Helper function to seed multiple officers automatically
int random_int(int min, int max)
{
    return min + rand() % (max - min + 1);
}

// Function to generate a random string from an array of names
const char *random_name(const char *names[], int name_count)
{
    return names[random_int(0, name_count - 1)];
}
void random_address(char *street, char *city, char *zip_code)
{
    // Example street names
    const char *street_names[] = {
        "1st Ave", "5th Ave", "Broadway", "Madison Ave", "Park Ave",
        "Wall St", "Lexington Ave", "W 42nd St", "E 34th St", "Canal St",
        "Delancey St", "Houston St", "Bleeker St", "Fulton St", "Columbus Ave",
        "Astor Pl", "Riverside Dr", "Amsterdam Ave", "Bowery", "Mulberry St",
        "8th Ave", "2nd Ave", "West End Ave", "Church St", "Grand St"};

    const char *boroughs[] = {
        "Manhattan", "Brooklyn", "Queens", "The Bronx", "Staten Island"};

    // 4 ZIP codes per borough, in order
    const char *zip_codes[] = {
        // Manhattan
        "10001", "10002", "10003", "10009",
        // Brooklyn
        "11201", "11211", "11215", "11206",
        // Queens
        "11351", "11354", "11368", "11377",
        // The Bronx
        "10451", "10452", "10462", "10467",
        // Staten Island
        "10301", "10304", "10312", "10314"};

    // Randomly select parts for the address
    const char *street_name = street_names[random_int(0, sizeof(street_names) / sizeof(street_names[0]) - 1)];
    const char *borough = boroughs[random_int(0, sizeof(boroughs) / sizeof(boroughs[0]) - 1)];
    const char *zip = zip_codes[random_int(0, sizeof(zip_codes) / sizeof(zip_codes[0]) - 1)];

    // Format the address components
    snprintf(street, 128, "%d %s", random_int(1, 9999), street_name); // Randomize street number
    snprintf(city, 128, "%s", borough);                               // Borough as city
    snprintf(zip_code, 128, "%s", zip);                               // Random ZIP code
}

// Helper function to seed multiple officers automatically
void seed_officers(int count, SNTRPH *sntrph)
{
    // Example random first and last names
    const char *first_names[] = {
        "Jessica", "Michael", "Ashley", "Joshua", "Amanda", "James", "Emily", "David",
        "Sarah", "Daniel", "Megan", "Matthew", "Laura", "Ryan", "Brittany", "Tyler",
        "Hannah", "Zachary", "Nicole", "Ethan"};

    const char *last_names[] = {
        "Smith", "Johnson", "Williams", "Brown", "Jones", "Miller", "Davis", "García",
        "Rodriguez", "Martinez", "Hernandez", "Lopez", "Wilson", "Taylor", "Moore",
        "Anderson", "Thomas", "Jackson", "White", "Martin"};

    srand(time(NULL)); // Seed the random number generator for randomness

    for (int i = 0; i < count; i++)
    {
        // Generate random details for each officer
        const char *first_name = random_name(first_names, sizeof(first_names) / sizeof(first_names[0]));
        const char *last_name = random_name(last_names, sizeof(last_names) / sizeof(last_names[0]));

        char officerid[15];
        char b_no[15];
        const char *clearance_prefixes[] = {"JDET", "DET", "SRDET", "LT"};
        int weighted_clearance[] = {1, 1, 2, 2, 3, 4}; // 1 and 2 are twice as likely

        int index = random_int(0, sizeof(weighted_clearance) / sizeof(weighted_clearance[0]) - 1);
        int clearance = weighted_clearance[index];
        const char *title;
        switch (clearance)
        {
        case 1:
            title = "Junior Detective";
            break;
        case 2:
            title = "Detective";
            break;
        case 3:
            title = "Senior Detective";
            break;
        case 4:
            if (random_int(0, 1) == 0)
                title = "Lieteunant";
            else
                title = "Inspector";
            break;
        default:
            title = "Unknown";
        }
        // Construct b_no based on clearance level
        sprintf(b_no, "%s%03d", clearance_prefixes[clearance - 1], random_int(1, 999));
        if (sntrph->officerList.officer_count == 0 || sntrph->officerList.officers[sntrph->officerList.officer_count - 1] == NULL)
        {
            strcpy(officerid, "OF000");
        }
        else
        {
            strcpy(officerid, sntrph->officerList.officers[sntrph->officerList.officer_count - 1]->officer_id);
            increment_id(officerid);
        }

        char username[32];
        sprintf(username, "%s%s", first_name, last_name); // Use first and last names for username
        char password[32];
        sprintf(password, "%s1234", first_name); // Simple password based on first name

        char street[128], city[128], zip_code[128];
        random_address(street, city, zip_code); // Generate address components

        // Create the officer first
        Officer *officer = create_officer(officerid, username, password, title, first_name, last_name, b_no, clearance, sntrph);

        // Now that we have the officer created, we can assign an address
        Address *address = create_address(street, city, zip_code, officer->base, sntrph);

        // Output the officer and address details
        printf("Created officer: %s %s, Title: %s, Clearance: %d, b_no: %s, Address: %s\n",
               first_name, last_name, title, clearance, b_no, street);
    }
}

void processCommand(SNTRPH *sntrph, char *input)
{
    char w_input[50];
    strcpy(w_input, input);
    char delimiters[10] = " \n";
    char *token = strtok(w_input, delimiters);
    if (strcmp(token, "SEND") == 0)
    {
        PrintLine("[✓] SEND Found.");
        char *token_2 = strtok(NULL, delimiters);
        if (strcmp(token_2, "MESSAGE") == 0)
        {
            PrintLine("[✓] MESSAGE Found.");
            char *bdg_no = strtok(NULL, delimiters);
            for (int i = 0; i < sntrph->officerList.officer_count; i++)
            {
                char name[120];
                snprintf(name, sizeof(name), "%s %s %s", sntrph->officerList.officers[i]->title, sntrph->officerList.officers[i]->base->first_name, sntrph->officerList.officers[i]->base->last_name);
                if (strcmp(bdg_no, sntrph->officerList.officers[i]->b_no) == 0)
                {
                    printf("[✓] Credentials match officer: %s\n", name);
                    sendMessage(sntrph->current_user, bdg_no, sntrph);
                }
            }
        }
    }
    else if (strcmp(token, "LOG") == 0)
    {
        char *token_2 = strtok(NULL, delimiters);
        if (strcmp(token_2, "OUT") == 0)
        {
            char officer_name[120];
            snprintf(officer_name, sizeof(officer_name), "%s %s %s", sntrph->current_user->title, sntrph->current_user->base->first_name, sntrph->current_user->base->last_name);
            char date[15];
            get_current_date(date);
            char description[200];
            snprintf(description, sizeof(description), "%s TERMINATED SAINT RAPHAEL ON %s", officer_name, date);
            addLog(sntrph, "SYSTEM TERMINATION", "", sntrph->current_user, description);
            sntrph->current_user = NULL;
            startMenu(sntrph);
        }
    }
    else if (strcmp(token, "VIEW") == 0)
    {
        char *token_2 = strtok(NULL, delimiters);
        if (strcmp(token_2, "MAILBOX") == 0)
        {
            ViewMailbox(sntrph);
        }
        else if (strcmp(token_2, "EVIDENCE") == 0)
        {
            char *token_3 = strtok(NULL, delimiters);
            Evidence *ev = checkEvidence(sntrph, token_3);
            printEvidence(ev);
        }
        else if (strcmp(token_2, "OFFICERS") == 0)
        {
            if (sntrph->current_user->clearance < 3)
            {
                PrintLine("ACCESS DENIED: INSUFFICIENT CLEARANCE\n");
                PrintLine("!ATTEMPT FOLLOWED IN AUDIT TRAIL!!");

                char description[200];
                char date[15];
                get_current_date(date);
                snprintf(description, sizeof(description), "%s ATTEMPTED TO VIEW INFORMATION OUT OF CLEARANCE BOUNDS ON %s", date);
                addLog(sntrph, "FORBIDDEN VIEWING ATTEMPT", "", sntrph->current_user, description);
            }
            viewOfficers(sntrph);
        }
        else if(strcmp(token_2, "OFFICER") == 0){
            char *token_3 = strtok(NULL, delimiters);
            Officer *off = check_officer(sntrph, token_3);
            viewOfficer(off);
        }
    }
    // sendMessage(sntrph->current_user, , &sntrph);
    else if (strcmp(token, "ADD") == 0)
    {
        char *token_2 = strtok(NULL, delimiters);
        if (strcmp(token_2, "EVIDENCE") == 0)
        {
            addEvidence(sntrph);
        }
        if (strcmp(token_2, "ADOPTION_REPORT") == 0)
        {
        }
    }
    else if (strcmp(token, "DELETE") == 0)
    {
        char *token_2 = strtok(NULL, delimiters);
        if (strcmp(token_2, "EVIDENCE") == 0)
        {
            char *token_3 = strtok(NULL, delimiters);
            deleteEvidence(sntrph, token_3);
        }
    }
    else if (strcmp(token, "AUDIT") == 0)
    {
        char *token_2 = strtok(NULL, delimiters);
        if (strcmp(token_2, "TRAIL") == 0)
        {
            auditTrail(&sntrph->logList);
            char officer_name[120];
            snprintf(officer_name, sizeof(officer_name), "%s %s %s", sntrph->current_user->title, sntrph->current_user->base->first_name, sntrph->current_user->base->last_name);
            char description[200];
            char date[15];
            get_current_date(date);
            snprintf(description, sizeof(description), "%s FOLLOWED THE AUDIT TRAIL ON %s", officer_name, date);
            addLog(sntrph, "AUDIT TRAIL FOLLOWED", "", sntrph->current_user, description);
        }
    }
}
void populateDB(SNTRPH *sntrph)
{
    printf("Checkpoint 1: Starting populateDB\n");

    // ---------- OFFICER 1 ----------
    Officer *officer = create_officer(
        "OF001", "MAXKRUGER", "eightballboy", "Sr Detective.",
        "Maximus", "Kruger", "SRDET88411", 3, sntrph);

    Address *player_address = create_address(
        "56 Leonard Street", "New York", "10013", officer->base, sntrph);

    printf("Checkpoint 10: Officer 1, Address 1 added to SNTRPH\n");

    // ---------- OFFICER 2 ----------
    Officer *officer1 = create_officer(
        "OF002", "EMORYKRUGER", "SNTRPH_1978", "Commissioner.",
        "Emory", "Kruger", "COMM117", 4, sntrph);

    Address *dad_address = create_address(
        "165 Duane Street, Apt 5", "New York", "10013", officer1->base, sntrph);

    printf("Checkpoint 19: Officer 2, Address 2 added to SNTRPH\n");

    // ---------- Debug Output ----------
    printf("Officer name 1: %s\n", sntrph->officerList.officers[0]->base->first_name);
    printf("Officer name 2: %s\n", sntrph->officerList.officers[1]->base->first_name);

    printf("Checkpoint 20: populateDB completed successfully\n");

    seed_officers(30, sntrph);
}

void initSNTRPH(SNTRPH *sntrph)
{
    if (!sntrph)
        return;

    // Initialize person list
    sntrph->personlist.person_count = 0;
    for (int i = 0; i < MAX_PERSONS; i++)
    {
        sntrph->personlist.person_list[i] = NULL;
    }

    // Initialize address list
    sntrph->addressList.address_count = 0;
    for (int i = 0; i < MAX_ADDRESSES; i++)
    {
        sntrph->addressList.addresses[i] = NULL;
    }

    // Initialize evidence archive
    sntrph->evidenceArchive.total_items = 0;
    for (int i = 0; i < MAX_EV; i++)
    {
        sntrph->evidenceArchive.evidence_items[i] = NULL;
    }

    // Initialize report list
    sntrph->reportList.report_count = 0;
    for (int i = 0; i < MAX_AR; i++)
    {
        sntrph->reportList.reports[i] = NULL;
    }

    // Initialize log list
    sntrph->logList.log_count = 0;
    for (int i = 0; i < MAX_ADDRESSES; i++)
    {
        sntrph->logList.logs[i] = NULL;
    }

    // Initialize officer list
    sntrph->officerList.officer_count = 0;
    for (int i = 0; i < MAX_ADDRESSES; i++)
    {
        sntrph->officerList.officers[i] = NULL;
    }

    // No current user logged in yet
    sntrph->current_user = NULL;
}
int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SNTRPH sntrph;
    initSNTRPH(&sntrph);
    populateDB(&sntrph);
    startMenu(&sntrph);

    char input[50];
    while (true)
    {
        printf("\n> Enter command:");
        fgets(input, sizeof(input), stdin);
        size_t length = strlen(input);
        if (length < 4)
        {
            printf("Invalid command.\n");
            continue;
        }
        input[strcspn(input, "\n")] = 0; // Remove newline
        if (strcmp(input, "quit") == 0)
        {
            break;
        }
        processCommand(&sntrph, input);
    }

    return 0;
}
