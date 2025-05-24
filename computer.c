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
            char confirmation[50];
            snprintf(confirmation, sizeof(confirmation), "[✓] DELETED EVIDENCE <%s>", evidence_id_to_delete);
            PrintLine(confirmation);

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
int random_int(int min, int max)
{
    return min + rand() % (max - min + 1);
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
    printf("ADDRESS:        %s, %s, %s\n", off->base->address->street, off->base->address->city, off->base->address->zip_code);
    printf("STATUS:        %s\n", off->base->address);
    printf("--------------------------------------------------\n\n");
    if (off->disciplinary_actions[0] != NULL)
    {
        for (int i = 0; i < 3; i++)
        {
            if (off->disciplinary_actions[i] != NULL)
            {
                printf("DISCIPLINARY <%d> SUMMARY:        %s\n", i + 1, off->disciplinary_actions[i]->summary);
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
                get_current_date(timestamp);

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
        if (strcmp(token, sntrph->evidenceArchive.evidence_items[i]->evidence_id) == 0)
        {
            return sntrph->evidenceArchive.evidence_items[i];
        }
    }
    return false;
}

void trim_newline(char *str)
{
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
}

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

Officer *check_officer(SNTRPH *sntrph, char *token)
{
    printf("[CHECKPOINT C1] Entered check_officer()\n");

    if (sntrph == NULL)
    {
        printf("[ERROR] sntrph is NULL\n");
        return NULL;
    }

    if (sntrph->officerList.officer_count < 1)
    {
        printf("[CHECKPOINT C2] Officer count < 1\n");
        return NULL;
    }

    if (sntrph->officerList.officers == NULL)
    {
        printf("[ERROR] officerList.officers is NULL\n");
        return NULL;
    }

    for (int i = 0; i < sntrph->officerList.officer_count; i++)
    {
        if (sntrph->officerList.officers[i] == NULL)
        {
            printf("[WARNING] Officer at index %d is NULL\n", i);
            continue;
        }

        if (sntrph->officerList.officers[i]->b_no == NULL)
        {
            printf("[WARNING] Officer at index %d has NULL b_no\n", i);
            continue;
        }

        printf("[CHECKPOINT C3] Comparing token '%s' with b_no '%s'\n", token, sntrph->officerList.officers[i]->b_no);

        if (strcmp(token, sntrph->officerList.officers[i]->b_no) == 0)
        {
            printf("[CHECKPOINT C4] Match found at index %d\n", i);
            return sntrph->officerList.officers[i];
        }
    }

    printf("[CHECKPOINT C5] No matching officer found\n");
    return NULL;
}

AuditLog *check_log(SNTRPH *sntrph, char *token){
    if(sntrph->logList.log_count == 0){
        PrintLine("!! AUDIT TRAIL IS EMPTY !!");
    }
    if(strcmp(token, "") == 0){
        PrintLine("!! TOKEN IS EMPTY !!");
        return;
    }
    for(int i = 0; i < sntrph->logList.log_count; i++){
        if(strcmp(token, sntrph->logList.logs[i]) == 0){
            return sntrph->logList.logs[i];
        }
    }
}

Address *check_addressP(SNTRPH *sntrph, char *person){
    if(sntrph->addressList.address_count == 0){
        PrintLine("!! ADDRESSES ARE EMPTY !!");
        return NULL;
    }
    if(strcmp(person, "") == 0){
        PrintLine("!! TOKEN IS EMPTY !!");
        return NULL;
    }
    for(int i = 0; i < sntrph->addressList.address_count; i++){
        for(int j = 0; j < sntrph->addressList.addresses[i]->person_count; j++){
            if(strcmp(person, sntrph->addressList.addresses[i]->person_list[j]->person_id) == 0){
                return sntrph->addressList.addresses[i];
            }
        }
    }

}

Person *check_person(SNTRPH *sntrph, char *token) {
    if (sntrph->personlist.person_count == 0) {
        PrintLine("!! PERSON LIST IS EMPTY !!");
        return NULL;
    }

    if (token == NULL || strcmp(token, "") == 0) {
        PrintLine("!! TOKEN IS EMPTY !!");
        return NULL;
    }

    Person *matches[50];
    int match_count = 0;

    for (int i = 0; i < sntrph->personlist.person_count; i++) {
        Person *p = sntrph->personlist.person_list[i];

        if (strcmp(token, p->person_id) == 0 ||
            strcmp(token, p->first_name) == 0 ||
            strcmp(token, p->last_name) == 0 ||
            strcmp(token, p->DOB) == 0 ||
            strcmp(token, p->status) == 0 ||
            strcmp(token, p->relationship_status) == 0 ||
            strstr(p->criminal_record, token) != NULL)
        {
            if (match_count < 50) {
                matches[match_count++] = p;
            }
        }
    }

    if (match_count == 0) {
        PrintLine("!! NO MATCH FOUND !!");
        return NULL;
    }

    if (match_count == 1) {
        return matches[0];
    }

    // Multiple matches found - display options
    printf("Multiple matches found:\n");
    for (int i = 0; i < match_count; i++) {
        printf("%d) ID: %s, Name: %s %s, DOB: %s\n", i + 1,
               matches[i]->person_id,
               matches[i]->first_name,
               matches[i]->last_name,
               matches[i]->DOB);
    }

    // Prompt user to select
    int choice = 0;
    char input[10];
    while (1) {
        printf("Enter the number of the person you want to select (1-%d): ", match_count);
        fgets(input, sizeof(input), stdin);
        choice = atoi(input);

        if (choice >= 1 && choice <= match_count) {
            return matches[choice - 1];
        } else {
            printf("Invalid selection. Please try again.\n");
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

    for (int i = 0; i < 3; i++)
    {
        officer->disciplinary_actions[i] = NULL;
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
void addOfficer_Input(SNTRPH *sntrph)
{
    char first_name[50];
    printf("> Enter the first name of the new officer: ");
    fgets(first_name, sizeof(first_name), stdin);
    first_name[strcspn(first_name, "\n")] = 0;
    char last_name[50];
    printf("> Enter the last name of the new officer: ");
    fgets(last_name, sizeof(last_name), stdin);
    last_name[strcspn(last_name, "\n")] = 0;
    char criminal_record[500];
    printf("> Enter the new officer's criminal history(if applicable): ");
    fgets(criminal_record, sizeof(criminal_record), stdin);
    criminal_record[strcspn(criminal_record, "\n")] = 0;
    char DOB[20];
    printf("> Enter the new officer's date of birth: ");
    fgets(DOB, sizeof(DOB), stdin);
    DOB[strcspn(DOB, "\n")] = 0;
    char relationship_status[50];
    printf("> Enter the new officer's relationship status: ");
    fgets(relationship_status, sizeof(relationship_status), stdin);
    relationship_status[strcspn(relationship_status, "\n")] = 0;
    char status[50];
    strcpy(status, "ALIVE");

    char officerid[15];
    char personid[15];
    char b_no[15];
    const char *clearance_prefixes[] = {"JDET", "DET", "SRDET", "LT"};
    char clearance_input[5];
    printf("> Enter the clearance of the new officer: ");
    fgets(clearance_input, sizeof(clearance_input), stdin);
    int clearance = atoi(clearance_input);
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
        title = "Lieteunant";
        break;
    case 5:
        title = "Inspector";
        break;
    default:
        title = "Unknown";
    }
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
    if (sntrph->personlist.person_count == 0 || sntrph->personlist.person_list[sntrph->personlist.person_count - 1] == NULL)
    {
        strcpy(personid, "P000");
    }
    else
    {
        strcpy(personid, sntrph->personlist.person_list[sntrph->personlist.person_count - 1]->person_id);
        increment_id(personid);
    }

    char username[50];
    snprintf(username, sizeof(username), "%s%s", first_name, last_name);
    char password[50];
    snprintf(password, sizeof(password), "%s1234", first_name);

    char street[50];
    printf("> Enter the street name of the new officer's address: ");
    fgets(street, sizeof(street), stdin);
    street[strcspn(street, "\n")] = 0;
    char city[50];
    printf("> Enter the name of the city the new officer's address is in: ");
    fgets(city, sizeof(city), stdin);
    city[strcspn(city, "\n")] = 0;
    char zip[50];
    printf("> Enter the zip code the new officer's address is in: ");
    fgets(zip, sizeof(zip), stdin);
    zip[strcspn(zip, "\n")] = 0;

    Officer *officer = create_officer(officerid, username, password, title, first_name, last_name, b_no, clearance, sntrph);
    Address *address = create_address(street, city, zip, officer->base, sntrph);

    printf("[✓] OFFICER CREATED: %s %s, Title: %s, Clearance: %d, b_no: %s, Address: %s\n", first_name, last_name, title, clearance, b_no, street);
}

void editOfficer(SNTRPH *sntrph, char *token)
{
    Officer *officer = check_officer(sntrph, token);
    bool editing = true;
    while (editing)
    {
        printf("\n--------------------------------------------------\n");
        printf("            INFORMATION ALTERING SYSTEM - OFFICERS\n");
        printf("--------------------------------------------------\n");
        printf("1.) Clearance/Title\n");
        printf("2.) First Name\n");
        printf("3.) Last Name\n");
        printf("4.) Criminal Record\n");
        printf("5.) Relationship Status\n");
        printf("6.) Date of birth\n");
        printf("7.) Status\n");
        printf("8.) Username\n");
        printf("9.) Password\n");
        printf("10.) QUIT\n");
        char buffer[4];
        printf("> Enter your choice: ");
        fgets(buffer, sizeof(buffer), stdin);
        int buf = atoi(buffer);

        switch (buf)
        {
        case 1:
            printf("> Enter the clearance: ");
            char clearance_input[4];
            fgets(clearance_input, sizeof(clearance_input), stdin);
            int clearance = atoi(clearance_input);
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
                title = "Lieteunant";
                break;
            case 5:
                title = "Inspector";
                break;
            default:
                title = "Unknown";
            }
            strcpy(officer->title, title);
            officer->clearance = clearance;
            break;
        case 2:
            printf("> Enter the new first name: ");
            char first_name[50];
            fgets(first_name, sizeof(first_name), stdin);
            first_name[strcspn(first_name, "\n")] = '\0';
            strcpy(officer->base->first_name, first_name);
            break;
        case 3:
            printf("> Enter the new last name: ");
            char last_name[50];
            fgets(last_name, sizeof(last_name), stdin);
            last_name[strcspn(last_name, "\n")] = '\0';
            strcpy(officer->base->last_name, last_name);
            break;
        case 4:
            printf("> Enter the new criminal record: ");
            char criminal_record[500];
            fgets(criminal_record, sizeof(criminal_record), stdin);
            criminal_record[strcspn(criminal_record, "\n")] = '\0';
            strcpy(officer->base->criminal_record, criminal_record);
            break;
        case 5:
            printf("> Enter the new relationship status");
            char relationship_status[50];
            fgets(relationship_status, sizeof(relationship_status), stdin);
            relationship_status[strcspn(relationship_status, "\n")] = '\0';
            strcpy(officer->base->relationship_status, relationship_status);
            break;
        case 6:
            printf("> Enter the new DOB:");
            char DOB[20];
            fgets(DOB, sizeof(DOB), stdin);
            DOB[strcspn(DOB, "\n")] = '\0';
            strcpy(officer->base->DOB, DOB);
            break;
        case 7:
            printf("> Enter the new status; ");
            char status[50];
            fgets(status, sizeof(status), stdin);
            status[strcspn(status, "\n")] = '\0';
            strcpy(officer->base->status, status);
            break;
        case 8:
            printf("> Enter the new username: ");
            char username[50];
            fgets(username, sizeof(username), stdin);
            username[strcspn(username, "\n")] = '\0';
            strcpy(officer->username, username);
            break;
        case 9:
            printf("> Enter the new password: ");
            char password[50];
            fgets(password, sizeof(password), stdin);
            password[strcspn(password, "\n")] = '\0';
            strcpy(officer->password, password);
            break;
        case 10:
            char date[15];
            get_current_date(date);

            char officer_name[120];
            snprintf(officer_name, sizeof(officer_name), "%s %s %s", sntrph->current_user->title, sntrph->current_user->base->first_name, sntrph->current_user->base->last_name);

            char editofficer[120];
            snprintf(editofficer, sizeof(editofficer), "%s %s %s", officer->title, officer->base->first_name, officer->base->last_name);

            char description[200];
            snprintf(description, sizeof(description), "%s ALTERED %s's OFFICER PROFILE ON %s", officer_name, editofficer, date);
            addLog(sntrph, "OFFICER ALTERATION", officer->b_no, sntrph->current_user, description);
            editing = false;
            PrintLine("Quitting...");
            PrintLine("--------------------------------------------------");
            break;
        }
    }
}

void editDisciplinary(SNTRPH *sntrph, char *token){
    AuditLog *log = check_log(sntrph, token);
    bool editing = true;
    while (editing)
    {
        printf("\n--------------------------------------------------\n");
        printf("            INFORMATION ALTERING SYSTEM - AUDIT TRAIL\n");
        printf("--------------------------------------------------\n");
        printf("1.) Action Type\n");
        printf("2.) Record ID\n");
        printf("3.) Timestamp\n");
        printf("4.) Officer\n");
        printf("5.) Action Description\n");
        printf("6.) QUIT\n");
        char buffer[4];
        printf("> Enter your choice: ");
        fgets(buffer, sizeof(buffer), stdin);
        int buf = atoi(buffer);

        switch (buf)
        {
            case 1:
                char action_type[50];
                printf("> Enter your new action type: ");
                fgets(action_type, sizeof(action_type), stdin);
                action_type[strcspn(action_type, "\n")] = 0;
                strcpy(log->actionType, action_type);
                break;
            case 2:
                char record_id[10];
                printf("> Enter your new record ID: ");
                fgets(record_id, sizeof(record_id), stdin);
                record_id[strcspn(record_id, "\n")] = 0;
                strcpy(log->record_id, record_id);
                break;
            case 3:
                char timestamp[20];
                printf("> Enter your new timestamp: ");
                fgets(timestamp, sizeof(timestamp), stdin);
                timestamp[strcspn(timestamp, "\n")] = 0;
                strcpy(log->timestamp, timestamp);
                break;
            case 4:
                char officer_id[20];
                printf("> Enter the Officer ID: ");
                fgets(officer_id, sizeof(officer_id), stdin);
                officer_id[strcspn(officer_id, "\n")] = 0;
                Officer *officer = check_officer(sntrph, officer_id);
                log->officer = officer;
            case 5:
               char action_description[200];
                printf("> Enter your new action description: ");
                fgets(action_description, sizeof(action_description), stdin);
                action_description[strcspn(action_description, "\n")] = 0;
                strcpy(log->action_description, action_description);
                break;
            case 6:
                editing = false;
                break;
        }
    }
}

void editPerson(SNTRPH *sntrph, char *token){
    Person *person = check_person(sntrph, token);
    bool editing = true;
    while(editing){
        printf("\n--------------------------------------------------\n");
        printf("            INFORMATION ALTERING SYSTEM - PERSON\n");
        printf("--------------------------------------------------\n");
        printf("1.) First Name\n");
        printf("2.) Last Name\n");
        printf("3.) Criminal Record\n");
        printf("4.) Relationship Status\n");
        printf("5.) Date of birth\n");
        printf("6.) Status\n");
        printf("7.) QUIT\n");
        char buffer[4];
        printf("> Enter your choice: ");
        fgets(buffer, sizeof(buffer), stdin);
        int buf = atoi(buffer);

        switch (buf)
        {
        case 1:
            printf("> Enter the new first name: ");
            char first_name[50];
            fgets(first_name, sizeof(first_name), stdin);
            first_name[strcspn(first_name, "\n")] = '\0';
            strcpy(person->first_name, first_name);
            break;
        case 2:
            printf("> Enter the new last name: ");
            char last_name[50];
            fgets(last_name, sizeof(last_name), stdin);
            last_name[strcspn(last_name, "\n")] = '\0';
            strcpy(person->last_name, last_name);
            break;
        case 3:
            printf("> Enter the new criminal record: ");
            char criminal_record[500];
            fgets(criminal_record, sizeof(criminal_record), stdin);
            criminal_record[strcspn(criminal_record, "\n")] = '\0';
            strcpy(person->criminal_record, criminal_record);
            break;
        case 4:
            printf("> Enter the new relationship status");
            char relationship_status[50];
            fgets(relationship_status, sizeof(relationship_status), stdin);
            relationship_status[strcspn(relationship_status, "\n")] = '\0';
            strcpy(person->relationship_status, relationship_status);
            break;
        case 5:
            printf("> Enter the new DOB:");
            char DOB[20];
            fgets(DOB, sizeof(DOB), stdin);
            DOB[strcspn(DOB, "\n")] = '\0';
            strcpy(person->DOB, DOB);
            break;
        case 6:
            printf("> Enter the new status; ");
            char status[50];
            fgets(status, sizeof(status), stdin);
            status[strcspn(status, "\n")] = '\0';
            strcpy(person->status, status);
            break;
        case 7:
            char date[15];
            get_current_date(date);

            char officer_name[120];
            snprintf(officer_name, sizeof(officer_name), "%s %s %s", sntrph->current_user->title, sntrph->current_user->base->first_name, sntrph->current_user->base->last_name);

            char editperson[120];
            snprintf(editperson, sizeof(editperson), "%s %s",  person->first_name, person->last_name);

            char description[200];
            snprintf(description, sizeof(description), "%s ALTERED %s's PERSON PROFILE ON %s", officer_name, editperson, date);
            addLog(sntrph, "PERSON ALTERATION", person->person_id, sntrph->current_user, description);
            PrintLine("Quitting...");
            PrintLine("--------------------------------------------------");
            editing = false;
            break;
        }
    }
}

void editAddress(SNTRPH *sntrph, char *token){
    
}

void addDisciplinary(SNTRPH *sntrph, char *token)
{
    printf("[CHECKPOINT 1] Entered addDisciplinary\n");

    if (sntrph == NULL)
    {
        printf("[ERROR] sntrph is NULL\n");
        return;
    }
    if (sntrph->current_user == NULL)
    {
        printf("[ERROR] current_user is NULL\n");
        return;
    }

    if (sntrph->current_user->clearance < 3)
    {
        printf("[CHECKPOINT 2] Access denied due to insufficient clearance\n");
        PrintLine("ACCESS DENIED: INSUFFICIENT CLEARANCE\n");
        PrintLine("!ATTEMPT FOLLOWED IN AUDIT TRAIL!!");

        char date[15];
        get_current_date(date);

        if (sntrph->current_user->base == NULL)
        {
            printf("[ERROR] current_user->base is NULL\n");
            return;
        }

        char officer_name[120];
        snprintf(officer_name, sizeof(officer_name), "%s %s %s",
                 sntrph->current_user->title,
                 sntrph->current_user->base->first_name,
                 sntrph->current_user->base->last_name);

        char description[200];
        snprintf(description, sizeof(description), "%s ATTEMPTED TO ADD A DISCIPLINARY ON %s", officer_name, date);

        addLog(sntrph, "DISCIPLINARY ADDITION ATTEMPT", token, sntrph->current_user, description);
        return;
    }

    printf("[CHECKPOINT 3] Clearance sufficient, checking officer\n");

    Officer *officer = check_officer(sntrph, token);
    if (officer == NULL)
    {
        printf("[ERROR] Officer not found for token: %s\n", token);
        return;
    }

    printf("[CHECKPOINT 4] Officer found, allocating disciplinary action\n");

    DisciplinaryAction *da = malloc(sizeof(DisciplinaryAction));
    if (da == NULL)
    {
        printf("[ERROR] Memory allocation failed for DisciplinaryAction\n");
        return;
    }

    char d_id[10];
    char date[15];

    for (int i = 0; i < 3; i++)
    {
        if (officer->disciplinary_actions[i] == NULL)
        {
            printf("[CHECKPOINT 5] Found empty slot at index %d\n", i);

            snprintf(d_id, sizeof(d_id), "D00%d", i);
            strcpy(da->d_id, d_id);
            da->officer = officer;

            get_current_date(date);
            strcpy(da->date, date);

            char summary[200];
            printf("> Enter the summary of the disciplinary.\n");
            fgets(summary, sizeof(summary), stdin);

            char severity[5];
            printf("> Enter the severity of the disciplinary.\n");
            fgets(severity, sizeof(severity), stdin);
            int severity_int = atoi(severity);

            strcpy(da->summary, summary);
            da->severity = severity_int;

            officer->disciplinary_actions[i] = da;
            printf("[✓] DISCIPLINARY ADDED: %s \n", da->d_id);
            char date[15];
        get_current_date(date);

        char officer_name[120];
        snprintf(officer_name, sizeof(officer_name), "%s %s %s",
                 sntrph->current_user->title,
                 sntrph->current_user->base->first_name,
                 sntrph->current_user->base->last_name);
                         char badofficer[120];
        snprintf(badofficer, sizeof(badofficer), "%s %s %s",
                 officer->title,
                 officer->base->first_name,
                 officer->base->last_name);

        char description[200];
        snprintf(description, sizeof(description), "%s ADDED A DISCIPLINARY TO %s's OFFICER PROFILE ON %s", officer_name,badofficer, date);

        addLog(sntrph, "DISCIPLINARY ADDITION", token, sntrph->current_user, description);
            return;
        }
    }

    printf("[CHECKPOINT 6] No available slots for disciplinary actions\n");
}

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
        else if (strcmp(token_2, "OFFICER") == 0)
        {
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
        else if (strcmp(token_2, "ADOPTION_REPORT") == 0)
        {
        }
        else if (strcmp(token_2, "OFFICER") == 0)
        {
            addOfficer_Input(sntrph);
        }
        else if (strcmp(token_2, "DISCIPLINARY") == 0)
        {
            char *token_3 = strtok(NULL, delimiters);
            addDisciplinary(sntrph, token_3);
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
    else if (strcmp(token, "EDIT") == 0)
    {
        char *token_2 = strtok(NULL, delimiters);
        if (strcmp(token_2, "OFFICER") == 0)
        {
            char *token_3 = strtok(NULL, delimiters);
            editOfficer(sntrph, token_3);
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
        printf("\n> Enter command: ");
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
