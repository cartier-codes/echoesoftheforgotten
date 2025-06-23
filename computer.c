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
// Case-insensitive comparison
// HELPER FUNCTION
int ci_strncmp(const char *a, const char *b, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        char ca = tolower((unsigned char)a[i]);
        char cb = tolower((unsigned char)b[i]);
        if (ca != cb)
            return ca - cb;
        if (ca == '\0')
            break;
    }
    return 0;
}
// HELPER FUNCTION
int evalPersonCondition(Person *p, const char *field, const char *value)
{
    if (strcmp(field, "first_name") == 0)
        return strcmp(p->first_name, value) == 0;
    if (strcmp(field, "last_name") == 0)
        return strcmp(p->last_name, value) == 0;
    if (strcmp(field, "status") == 0)
        return strcmp(p->status, value) == 0;
    return 0;
}

int evalOfficerCondition(Officer *o, const char *field, const char *value)
{
    if (strcmp(field, "first_name") == 0)
        return strcmp(o->base->first_name, value) == 0;
    if (strcmp(field, "last_name") == 0)
        return strcmp(o->base->last_name, value) == 0;
    if (strcmp(field, "status") == 0)
        return strcmp(o->base->status, value) == 0;
    if (strcmp(field, "clearance") == 0)
    {
        char op = value[0];
        // =, >=, >, <, <=
        char v = value[1];
        if (isdigit(v))
        {
            int int_value = atoi(&value[1]);
            switch (op)
            {
            case '>':
                return o->clearance > int_value;
                break;
            case '<':
                return o->clearance < int_value;
                break;
            case '=':
                return o->clearance == int_value;
                break;
            }
        }
        else
        {
            char lop[3];
            strncpy(lop, value, 2);
            lop[2] = '\0';

            int int_value = atoi(&value[2]);
            if (strcmp(lop, ">=") == 0)
            {
                return o->clearance >= int_value;
            }
            else if (strcmp(lop, "<=") == 0)
            {
                return o->clearance <= int_value;
            }
        }
    }
    return 0;
}

int evalCaseCondition(CaseFile *c, const char *field, const char *value)
{
    if (strcmp(field, "lead") == 0)
        return strcmp(c->lead, value) == 0;
    if (strcmp(field, "type") == 0)
        return strcmp(c->type, value) == 0;
    if (strcmp(field, "status") == 0)
        return strcmp(c->status, value) == 0;
    if (strcmp(field, "locked") == 0)
        return c->is_locked;
    if (strcmp(field, "city") == 0)
        return strcmp(c->location->city, value) == 0;
}

int evalTrailCondition(AuditLog *a, const char *field, const char *value)
{
    if (strcmp(field, "officer") == 0)
        return strcmp(a->officer->b_no, value) == 0;
}
int evalAddCondition(Address *a, const char *field, const char *value)
{
    if (strcmp(field, "city") == 0)
        return strcmp(a->city, value) == 0;
    if (strcmp(field, "zip") == 0)
        return strcmp(a->zip_code, value) == 0;
    if (strcmp(field, "case_count") == 0)
    {
        char op = value[0];
        // =, >=, >, <, <=
        char v = value[1];
        if (isdigit(v))
        {
            int int_value = atoi(&value[1]);
            switch (op)
            {
            case '>':
                return a->case_count > int_value;
                break;
            case '<':
                return a->case_count < int_value;
                break;
            case '=':
                return a->case_count == int_value;
                break;
            }
        }
        else
        {
            char lop[3];
            strncpy(lop, value, 2);
            lop[2] = '\0';

            int int_value = atoi(&value[2]);
            if (strcmp(lop, ">=") == 0)
            {
                return a->case_count >= int_value;
            }
            else if (strcmp(lop, "<=") == 0)
            {
                return a->case_count <= int_value;
            }
        }
    }
}
// Replacement strstr_w
char *strstr_w(const char *haystack, const char *needle)
{
    if (!haystack || !needle)
        return NULL;

    size_t nlen = strlen(needle);
    if (nlen == 0)
        return NULL;

    // For very short tokens, allow substring match
    if (nlen <= 3)
    {
        for (; *haystack; haystack++)
        {
            if (ci_strncmp(haystack, needle, nlen) == 0)
                return (char *)haystack;
        }
        return NULL;
    }

    // For longer tokens, match full words only
    const char *p = haystack;
    while (*p)
    {
        // Skip non-alphanumeric
        while (*p && !isalnum(*p))
            p++;

        const char *word_start = p;

        // Move to end of word
        while (*p && isalnum(*p))
            p++;
        size_t word_len = p - word_start;

        if (word_len == nlen && ci_strncmp(word_start, needle, nlen) == 0)
            return (char *)word_start;
    }

    return NULL;
}
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
    name[strcspn(name, "\n")] = 0;
    printf("> Enter the status of your evidence: ");
    fgets(status, sizeof(status), stdin);
    status[strcspn(status, "\n")] = 0;
    printf("> Enter the type of your evidence: ");
    fgets(type, sizeof(type), stdin);
    type[strcspn(type, "\n")] = 0;
    printf("> Enter the case ID of your evidence: ");
    fgets(case_id, sizeof(case_id), stdin);
    case_id[strcspn(case_id, "\n")] = 0;
    printf("> Enter the description of your evidence: ");
    fgets(desc, sizeof(desc), stdin);
    desc[strcspn(desc, "\n")] = 0;
    printf("> Enter the file path of your evidence: ");
    fgets(file_path, sizeof(file_path), stdin);
    file_path[strcspn(file_path, "\n")] = 0;
    printf("> Enter any notes on your evidence: ");
    fgets(notes, sizeof(notes), stdin);
    notes[strcspn(notes, "\n")] = 0;
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
        strcpy(ev->custody_chain[i], buffer);
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
                PrintLine("!! ATTEMPT FOLLOWED IN AUDIT TRAIL !!");

                char description[200];
                snprintf(description, sizeof(description), "%s ATTEMPTED TO DELETE EVIDENCE: %s ON %s", officer_name, sntrph->evidenceArchive.evidence_items[i]->evidence_id, date);
                addLog(sntrph, "EVIDENCE DELETION ATTEMPT", sntrph->evidenceArchive.evidence_items[i]->evidence_id, sntrph->current_user, description);
                return;
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
            snprintf(description, sizeof(description), "%s DELETED EVIDENCE: %s ON %s", officer_name, evidence_id_to_delete, date);
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
        PrintLine("!! NO RECORD FOUND WITH THE GIVEN <ID> !!");
    }
}

void printEvidence(Evidence *ev)
{
    if (strcmp(ev->file_path, "") == 0)
    {
        strcpy(ev->file_path, "No file path assigned.");
    }
    else
    {
        // Remove newline character from file_path if it exists
        size_t len = strlen(ev->file_path);
        if (len > 0 && ev->file_path[len - 1] == '\n')
        {
            ev->file_path[len - 1] = '\0';
        }
        char file_path[100];
        snprintf(file_path, sizeof(file_path), "C:/Users/hannan.baane/echoesoftheforgotten/images/%s", ev->file_path);
        char command[150];
        snprintf(command, sizeof(command), "start %s", file_path);
        system(command); // Open the file path in the default application
    }
    printf("--------------------------------------------------\n");
    printf("             EVIDENCE RECORD DISPLAY\n");
    printf("--------------------------------------------------\n\n");

    printf("EVIDENCE ID:        %s\n", ev->evidence_id);
    printf("CASE ID:            %s\n", ev->case_id);
    printf("\n--------------------------------------------------\n");
    printf("NAME:               %s\n", ev->name);
    printf("TYPE:               %s\n", ev->type);
    printf("STATUS:             %s\n", ev->status);
    printf("DATE LOGGED:        %s\n", ev->date);
    printf("FILE PATH:          %s\n", ev->file_path);
    printf("RESTRICTED ACCESS:  %s\n", ev->is_restricted ? "Yes" : "No");

    printf("\n--------------------------------------------------\n");
    printf("DESCRIPTION:\n");
    printf("%s\n", ev->description);

    printf("\nCUSTODY CHAIN:\n");
    for (int i = 0; i < 5; i++)
    {
        if (strcmp(ev->custody_chain[i], "") == 0)
        {
            break;
        }
        printf("  - %s\n", ev->custody_chain[i]);
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

void deletePerson(SNTRPH *sntrph, char *person_id)
{
    bool found = false;
    char date[15];
    get_current_date(date);

    for (int i = 0; i < sntrph->personlist.person_count; i++)
    {
        char officer_name[120];
        snprintf(officer_name, sizeof(officer_name), "%s %s %s", sntrph->current_user->title, sntrph->current_user->base->first_name, sntrph->current_user->base->last_name);
        if (strcmp(person_id, sntrph->personlist.person_list[i]->person_id) == 0)
        {
            found = true;
            if (sntrph->current_user->clearance < 3)
            {
                PrintLine("ACCESS DENIED: INSUFFICIENT CLEARANCE\n");
                PrintLine("!! ATTEMPT FOLLOWED IN AUDIT TRAIL !!");

                char description[200];
                snprintf(description, sizeof(description), "%s ATTEMPTED TO DELETE PERSON ENTRY: %s ON %s", person_id, date);
                addLog(sntrph, "PERSON ENTRY DELETION ATTEMPT", person_id, sntrph->current_user, description);
                return;
            }
            for (int j = i; j < sntrph->personlist.person_count - 1; j++)
            {
                sntrph->personlist.person_list[j] = sntrph->personlist.person_list[j + 1];
            }
            // Decrease the total item count
            sntrph->personlist.person_count--;
            char confirmation[50];
            snprintf(confirmation, sizeof(confirmation), "[✓] DELETED A PERSON RECORD: <%s>", person_id);
            PrintLine(confirmation);

            char description[200];
            snprintf(description, sizeof(description), "%s DELETED A PERSON RECORD: %s ON %s", officer_name, person_id, date);
            addLog(sntrph, "RECORD DELETION", person_id, sntrph->current_user, description);
            break;
        }
        // Person 59, ID: <P061>, Danielle Kelly | ALIVE
    }
    if (!found)
    {
        PrintLine("!! NO RECORD FOUND WITH THE GIVEN <ID> !!");
    }
}

void deleteAddress(SNTRPH *sntrph, char *add_id)
{
    bool found = false;
    char date[15];
    get_current_date(date);

    char officer_name[120];
    snprintf(officer_name, sizeof(officer_name), "%s %s %s",
             sntrph->current_user->title,
             sntrph->current_user->base->first_name,
             sntrph->current_user->base->last_name);

    // Check person list for matching address and remove link
    for (int i = 0; i < sntrph->personlist.person_count; i++)
    {
        if (sntrph->personlist.person_list[i]->address &&
            strcmp(add_id, sntrph->personlist.person_list[i]->address->address_id) == 0)
        {

            found = true;

            if (sntrph->current_user->clearance < 3)
            {
                PrintLine("ACCESS DENIED: INSUFFICIENT CLEARANCE\n");
                PrintLine("!! ATTEMPT FOLLOWED IN AUDIT TRAIL !!");

                char description[200];
                snprintf(description, sizeof(description),
                         "%s ATTEMPTED TO DELETE ADDRESS ENTRY ON %s",
                         officer_name, date);
                addLog(sntrph, "ADDRESS ENTRY DELETION ATTEMPT", add_id,
                       sntrph->current_user, description);
                return;
            }

            sntrph->personlist.person_list[i]->address = NULL;
        }
    }

    // Remove from address list
    for (int i = 0; i < sntrph->addressList.address_count; i++)
    {
        if (strcmp(add_id, sntrph->addressList.addresses[i]->address_id) == 0)
        {
            found = true;

            for (int j = i; j < sntrph->addressList.address_count - 1; j++)
            {
                sntrph->addressList.addresses[j] = sntrph->addressList.addresses[j + 1];
            }

            sntrph->addressList.address_count--;

            char confirmation[50];
            snprintf(confirmation, sizeof(confirmation),
                     "[✓] DELETED AN ADDRESS RECORD: <%s>", add_id);
            PrintLine(confirmation);

            char description[200];
            snprintf(description, sizeof(description),
                     "%s DELETED AN ADDRESS RECORD: %s ON %s",
                     officer_name, add_id, date);
            addLog(sntrph, "RECORD DELETION", add_id,
                   sntrph->current_user, description);
            break;
        }
    }

    if (!found)
    {
        PrintLine("!! NO RECORD FOUND WITH THE GIVEN <ID> !!");
    }
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
    printf("--------------------------------------------------\n\n");
    printf("RELATIONSHIP STATUS:        %s\n", off->base->relationship_status);
    printf("STATUS:        %s\n", off->base->status);
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

void viewPerson(Person *person)
{

    printf("--------------------------------------------------\n");
    printf("           SAINT-RAPHAEL PERSON TRAIL VIEW\n");
    printf("--------------------------------------------------\n\n");

    printf("ID:        %s\n", person->person_id);
    printf("FIRST NAME:        %s\n", person->first_name);
    printf("LAST NAME:        %s\n", person->last_name);
    printf("--------------------------------------------------\n\n");
    printf("DOB:        %s\n", person->DOB);
    printf("RELATIONSHIP STATUS:        %s\n", person->relationship_status);
    printf("CRIMINAL RECORD:        %s\n", person->criminal_record);
    printf("--------------------------------------------------\n\n");
    printf("STATUS:        %s\n", person->status);
    if (person->address != NULL)
    {
        printf("ADDRESS:        %s, %s, %s\n", person->address->street, person->address->city, person->address->zip_code);
    }
    else
    {
        printf("ADDRESS:        No address assigned.\n");
    }
    printf("--------------------------------------------------\n");
    printf("[Press Enter to EXIT]\n");
    getchar();
}
void viewAddress(Address *address)
{
    printf("--------------------------------------------------\n");
    printf("           SAINT-RAPHAEL ADDRESS TRAIL VIEW\n");
    printf("--------------------------------------------------\n\n");

    printf("ID:          %s\n", address->address_id);
    printf("STREET:      %s\n", address->street);
    printf("CITY:        %s\n", address->city);
    printf("ZIP CODE:    %s\n", address->zip_code);

    printf("--------------------------------------------------\n\n");

    // Print linked residents
    printf("RESIDENTS (%d):\n", address->person_count);
    if (address->person_count == 0)
    {
        printf("  None assigned.\n");
    }
    else
    {
        for (int i = 0; i < address->person_count; i++)
        {
            Person *resident = address->person_list[i];
            printf("  - %s %s (ID: %s)\n", resident->first_name, resident->last_name, resident->person_id);
        }
    }

    printf("--------------------------------------------------\n\n");

    // Print linked cases (assuming address->case_no is an array of strings)
    printf("LINKED CASES:\n");
    int cases_found = 0;
    for (int i = 0; i < 20; i++)
    {
        if (address->case_no[i][0] != '\0')
        {
            printf("  - %s\n", address->case_no[i]);
            cases_found++;
        }
    }
    if (cases_found == 0)
    {
        printf("  No linked cases.\n");
    }

    printf("--------------------------------------------------\n");
    printf("[Press Enter to EXIT]\n");
    getchar();
}

void viewPeople(SNTRPH *sntrph)
{
    int person_count = sntrph->personlist.person_count;
    int page = 0;
    int total_pages = (person_count + PAGE_SIZE - 1) / PAGE_SIZE;
    char input[10];

    while (1)
    {
        system("cls"); // Use "clear" if you're on Unix

        printf("--------------------------------------------------\n");
        printf("           SAINT-RAPHAEL PERSON TRAIL VIEW\n");
        printf("--------------------------------------------------\n\n");

        int start = page * PAGE_SIZE;
        int end = start + PAGE_SIZE;
        if (end > person_count)
            end = person_count;

        for (int i = start; i < end; i++)
        {
            Person *person = sntrph->personlist.person_list[i];
            printf("Person %d, ID: <%s>, %s %s | %s\n",
                   i + 1,
                   person->person_id,
                   person->first_name,
                   person->last_name, person->status);
        }

        printf("--------------------------------------------------\n\n");
        printf("[n] Next | [p] Prev | [q] Quit | [1-%d] View person\n> ", person_count);

        fgets(input, sizeof(input), stdin);

        if (tolower(input[0]) == 'q')
        {
            break;
        }
        else if (tolower(input[0]) == 'n' && page < total_pages - 1)
        {
            page++;
        }
        else if (tolower(input[0]) == 'p' && page > 0)
        {
            page--;
        }
        else
        {
            int choice_int = atoi(input);
            if (choice_int > 0 && choice_int <= person_count)
            {
                viewPerson(sntrph->personlist.person_list[choice_int - 1]);
            }
        }
    }
}

void viewEvidence(SNTRPH *sntrph)
{
    int ev_count = sntrph->evidenceArchive.total_items;
    int page = 0;
    int total_pages = (ev_count + PAGE_SIZE - 1) / PAGE_SIZE;
    char input[10];

    while (1)
    {
        system("cls");
        printf("--------------------------------------------------\n");
        printf("           SAINT-RAPHAEL EVIDENCE ARCHIVE VIEW\n");
        printf("--------------------------------------------------\n\n");

        int start = page * PAGE_SIZE;
        int end = start + PAGE_SIZE;
        if (end > ev_count)
        {
            end = ev_count;
        }
        for (int i = start; i < end; i++)
        {
            Evidence *ev = sntrph->evidenceArchive.evidence_items[i];
            printf("Evidence %d, ID: <%s>, %s, %s | %s\n", i + 1, ev->evidence_id, ev->name, ev->type, ev->status);
        }
        printf("--------------------------------------------------\n\n");
        printf("[n] Next | [p] Prev | [q] Quit | [1-%d] View evidence\n> ", ev_count);

        fgets(input, sizeof(input), stdin);
        if (tolower(input[0]) == 'q')
        {
            break;
        }
        else if (tolower(input[0]) == 'n' && page < total_pages - 1)
        {
            page++;
        }
        else if (tolower(input[0]) == 'p' && page > 0)
        {
            page--;
        }
        else
        {
            int choice_int = atoi(input);
            if (choice_int > 0 && choice_int <= ev_count)
            {
                printEvidence(sntrph->evidenceArchive.evidence_items[choice_int - 1]);
            }
        }
    }
}

void viewAddresses(SNTRPH *sntrph)
{
    int address_count = sntrph->addressList.address_count;
    int page = 0;
    int total_pages = (address_count + PAGE_SIZE - 1) / PAGE_SIZE;
    char input[10];

    while (1)
    {
        system("cls");
        printf("--------------------------------------------------\n");
        printf("           SAINT-RAPHAEL ADDRESS TRAIL VIEW\n");
        printf("--------------------------------------------------\n\n");

        int start = page * PAGE_SIZE;
        int end = start + PAGE_SIZE;
        if (end > address_count)
        {
            end = address_count;
        }
        for (int i = start; i < end; i++)
        {
            Address *address = sntrph->addressList.addresses[i];
            printf("Address %d, ID: <%s>, %s, %s, %s | %d\n", i + 1, address->address_id, address->street, address->city, address->zip_code, address->person_count);
        }
        printf("--------------------------------------------------\n\n");
        printf("[n] Next | [p] Prev | [q] Quit | [1-%d] View address\n> ", address_count);
        fgets(input, sizeof(input), stdin);

        if (tolower(input[0]) == 'q')
        {
            break;
        }
        else if (tolower(input[0]) == 'n' && page < total_pages - 1)
        {
            page++;
        }
        else if (tolower(input[0]) == 'p' && page > 0)
        {
            page--;
        }
        else
        {
            int choice_int = atoi(input);
            if (choice_int > 0 && choice_int <= address_count)
            {
                viewAddress(sntrph->addressList.addresses[choice_int - 1]);
            }
        }
    }
}
void viewOfficers(SNTRPH *sntrph)
{
    int officer_count = sntrph->officerList.officer_count;
    int page = 0;
    int total_pages = (officer_count + PAGE_SIZE - 1) / PAGE_SIZE;
    char input[10];

    while (1)
    {
        system("cls"); // or use "clear" on Unix-like systems

        printf("--------------------------------------------------\n");
        printf("           SAINT-RAPHAEL OFFICERS TRAIL VIEW\n");
        printf("--------------------------------------------------\n\n");

        int start = page * PAGE_SIZE;
        int end = start + PAGE_SIZE;
        if (end > officer_count)
            end = officer_count;

        for (int i = start; i < end; i++)
        {
            char officer_name[120];
            snprintf(officer_name, sizeof(officer_name), "%s %s %s",
                     sntrph->officerList.officers[i]->title,
                     sntrph->officerList.officers[i]->base->first_name,
                     sntrph->officerList.officers[i]->base->last_name);

            printf("Officer %d, ID: <%s>, %s\n",
                   i + 1,
                   sntrph->officerList.officers[i]->b_no,
                   officer_name);
        }

        printf("--------------------------------------------------\n\n");
        printf("[n] Next | [p] Prev | [q] Quit | [1-%d] View officer\n> ", officer_count);

        fgets(input, sizeof(input), stdin);

        if (tolower(input[0]) == 'q')
        {
            break;
        }
        else if (tolower(input[0]) == 'n' && page < total_pages - 1)
        {
            page++;
        }
        else if (tolower(input[0]) == 'p' && page > 0)
        {
            page--;
        }
        else
        {
            int choice_int = atoi(input);
            if (choice_int > 0 && choice_int <= officer_count)
            {
                viewOfficer(sntrph->officerList.officers[choice_int - 1]);
            }
        }
    }
}

void viewLog( AuditLog *log)
{
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
        PrintLine("--------------------------------------------------");
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
                    SLEEP(500);
                    PrintLine("[✓] Password authenticated.");
                    PrintLine("--------------------------------------------------");

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
                    snprintf(officer_name, sizeof(officer_name), "%s %s %s", officer->title, officer->base->first_name, officer->base->last_name);
                    char description[200];
                    snprintf(description, sizeof(description), "PERSON ATTEMPTED TO ACCESS SYSTEM THROUGH %s's ACCOUNT ON %s", officer_name, date);

                    PrintLine("Invalid password. Please try again.");
                    addLog(sntrph, "SYSTEM ACCESS ATTEMPT", "", officer, description);
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
void bulkMessage(Officer *sender, SNTRPH *sntrph, char *subject, char *body)
{
    for (int i = 0; i < sntrph->officerList.officer_count; i++)
    {
        if (sender->clearance + 2 <= sntrph->officerList.officers[i]->clearance)
        {
            PrintLine("[✗] You are not authorized to contact this officer.");
        }
        else if (strcmp(sender->b_no, sntrph->officerList.officers[i]->b_no) == 0)
        {
            continue;
        }
        else
        {
            GMessage *msg = malloc(sizeof(GMessage)); // ✅ allocate on heap
            msg->sender = sender;
            msg->recipient = sntrph->officerList.officers[i];
            msg->clearance = msg->recipient->clearance;
            msg->read = false;

            char timestamp[20];
            get_current_date(timestamp);
            strcpy(msg->subject, subject);
            strcpy(msg->timestamp, timestamp);
            strcpy(msg->body, body);

            sntrph->officerList.officers[i]->mailbox->messages[sntrph->officerList.officers[i]->mailbox->message_count++] = msg;

            printf("MESSAGE SENT: %d, SUBJECT: %s, OFFICER: %s,| %s\n", i + 1, msg->subject, sntrph->officerList.officers[i]->b_no, msg->timestamp);
        }
    }

    printf("--------------------------------------------------\n");
}

void hailGabriel(SNTRPH *sntrph)
{

    char subject[100];
    printf("\nSubject:");
    fgets(subject, sizeof(subject), stdin);

    char body[1000];
    printf("\nMessage:");
    fgets(body, sizeof(body), stdin);

    subject[strcspn(subject, "\n")] = 0;
    body[strcspn(body, "\n")] = 0;

    bulkMessage(sntrph->current_user, sntrph, subject, body);
}
// HELPER FUNCTION
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

// HELPER FUNCTION
void trim_newline(char *str)
{
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
}

// HELPER FUNCTION
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

// HELPER FUNCTION
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

        if (strcmp(token, sntrph->officerList.officers[i]->b_no) == 0 || strcmp(token, sntrph->officerList.officers[i]->officer_id) == 0)
        {
            printf("[CHECKPOINT C4] Match found at index %d\n", i);
            return sntrph->officerList.officers[i];
        }
    }

    printf("[CHECKPOINT C5] No matching officer found\n");
    return NULL;
}

void deleteOfficer(SNTRPH *sntrph, char *off_id)
{
    bool found = false;
    char date[15];
    get_current_date(date);

    char officer_name[120];
    snprintf(officer_name, sizeof(officer_name), "%s %s %s",
             sntrph->current_user->title,
             sntrph->current_user->base->first_name,
             sntrph->current_user->base->last_name);

    for (int i = 0; i < sntrph->officerList.officer_count; i++)
    {
        Officer *off = sntrph->officerList.officers[i];

        // Correct condition: check both ID and badge number
        if (strcmp(off_id, off->officer_id) == 0 || strcmp(off_id, off->b_no) == 0)
        {
            found = true;

            if (sntrph->current_user->clearance < 3)
            {
                PrintLine("ACCESS DENIED: INSUFFICIENT CLEARANCE\n");
                PrintLine("!! ATTEMPT FOLLOWED IN AUDIT TRAIL !!");

                char description[200];
                snprintf(description, sizeof(description),
                         "%s ATTEMPTED TO DELETE OFFICER ENTRY: %s ON %s",
                         officer_name, off_id, date);
                addLog(sntrph, "OFFICER ENTRY DELETION ATTEMPT", off_id,
                       sntrph->current_user, description);
                return;
            }

            // Remove officer from list
            for (int j = i; j < sntrph->officerList.officer_count - 1; j++)
            {
                sntrph->officerList.officers[j] = sntrph->officerList.officers[j + 1];
            }
            sntrph->officerList.officer_count--;

            char confirmation[80];
            snprintf(confirmation, sizeof(confirmation),
                     "[✓] DELETED AN OFFICER RECORD: <%s>", off_id);
            PrintLine(confirmation);

            char description[200];
            snprintf(description, sizeof(description),
                     "%s DELETED AN OFFICER RECORD: %s ON %s",
                     officer_name, off_id, date);
            addLog(sntrph, "RECORD DELETION", off_id, sntrph->current_user, description);

            // Ask if the user wants to delete the corresponding person
            char *person_id = off->base->person_id;
            PrintLine("> Would you also like to delete the linked PERSON record? [y/n]: ");
            char input[10];
            fgets(input, sizeof(input), stdin);

            if (input[0] == 'y' || input[0] == 'Y')
            {
                for (int k = 0; k < sntrph->personlist.person_count; k++)
                {
                    if (strcmp(person_id, sntrph->personlist.person_list[k]->person_id) == 0)
                    {
                        for (int m = k; m < sntrph->personlist.person_count - 1; m++)
                        {
                            sntrph->personlist.person_list[m] = sntrph->personlist.person_list[m + 1];
                        }
                        sntrph->personlist.person_count--;

                        char msg[80];
                        snprintf(msg, sizeof(msg),
                                 "[✓] DELETED LINKED PERSON RECORD: <%s>", person_id);
                        PrintLine(msg);

                        snprintf(description, sizeof(description),
                                 "%s ALSO DELETED LINKED PERSON RECORD: %s ON %s",
                                 officer_name, person_id, date);
                        addLog(sntrph, "RECORD DELETION", person_id,
                               sntrph->current_user, description);
                        break;
                    }
                }
            }

            break; // Exit loop after processing
        }
    }

    if (!found)
    {
        PrintLine("!! NO OFFICER RECORD FOUND WITH THE GIVEN <ID> !!");
    }
}

// HELPER FUNCTION
AuditLog *check_log(SNTRPH *sntrph, char *token)
{
    if (sntrph->logList.log_count == 0)
    {
        PrintLine("!! AUDIT TRAIL IS EMPTY !!");
    }
    if (strcmp(token, "") == 0)
    {
        PrintLine("!! TOKEN IS EMPTY !!");
        return NULL;
    }
    for (int i = 0; i < sntrph->logList.log_count; i++)
    {
        if (strcmp(token, sntrph->logList.logs[i]->audit_id) == 0)
        {
            return sntrph->logList.logs[i];
        }
    }
}

// HELPER FUNCTION
Address *check_addressP(SNTRPH *sntrph, char *person)
{
    if (sntrph->addressList.address_count == 0)
    {
        PrintLine("!! ADDRESSES ARE EMPTY !!");
        return NULL;
    }
    if (strcmp(person, "") == 0)
    {
        PrintLine("!! TOKEN IS EMPTY !!");
        return NULL;
    }
    for (int i = 0; i < sntrph->addressList.address_count; i++)
    {
        for (int j = 0; j < sntrph->addressList.addresses[i]->person_count; j++)
        {
            if (strcmp(person, sntrph->addressList.addresses[i]->person_list[j]->person_id) == 0)
            {
                return sntrph->addressList.addresses[i];
            }
        }
    }
}

// HELPER FUNCTION
Address *check_address(SNTRPH *sntrph, char *token)
{

    if (sntrph->addressList.address_count == 0)
    {
        PrintLine("!! ADDRESSES ARE EMPTY !!");
        return NULL;
    }
    if (strcmp(token, "") == 0)
    {
        PrintLine("!! TOKEN IS EMPTY !!");
        return NULL;
    }
    for (int i = 0; i < sntrph->addressList.address_count; i++)
    {
        if (strcmp(token, sntrph->addressList.addresses[i]->address_id) == 0)
        {
            return sntrph->addressList.addresses[i];
        }
    }
}

// HELPER FUNCTION
Person *check_person(SNTRPH *sntrph, char *token)
{
    if (sntrph->personlist.person_count == 0)
    {
        PrintLine("!! PERSON LIST IS EMPTY !!");
        return NULL;
    }

    if (token == NULL || strcmp(token, "") == 0)
    {
        PrintLine("!! TOKEN IS EMPTY !!");
        return NULL;
    }

    Person *matches[50];
    int match_count = 0;

    for (int i = 0; i < sntrph->personlist.person_count; i++)
    {
        Person *p = sntrph->personlist.person_list[i];

        if (strcmp(token, p->person_id) == 0 ||
            strcmp(token, p->first_name) == 0 ||
            strcmp(token, p->last_name) == 0 ||
            strcmp(token, p->DOB) == 0 ||
            strcmp(token, p->status) == 0 ||
            strcmp(token, p->relationship_status) == 0 ||
            strstr(p->criminal_record, token) != NULL)
        {
            if (match_count < 50)
            {
                matches[match_count++] = p;
            }
        }
    }

    if (match_count == 0)
    {
        PrintLine("!! NO MATCH FOUND !!");
        return NULL;
    }

    if (match_count == 1)
    {
        return matches[0];
    }

    // Multiple matches found - display options
    printf("Multiple matches found:\n");
    for (int i = 0; i < match_count; i++)
    {
        printf("%d) ID: %s, Name: %s %s, DOB: %s\n", i + 1,
               matches[i]->person_id,
               matches[i]->first_name,
               matches[i]->last_name,
               matches[i]->DOB);
    }

    // Prompt user to select
    int choice = 0;
    char input[10];
    while (1)
    {
        printf("Enter the number of the person you want to select (1-%d): ", match_count);
        fgets(input, sizeof(input), stdin);
        choice = atoi(input);

        if (choice >= 1 && choice <= match_count)
        {
            return matches[choice - 1];
        }
        else
        {
            printf("Invalid selection. Please try again.\n");
        }
    }
}
// HELPER FUNCTION
void random_date(char *date, size_t size)
{
    // Generate a random date in the format YYYY-MM-DD
    int year = random_int(1940, 1994);
    int month = random_int(1, 12);
    int day = random_int(1, 28); // To avoid issues with February

    snprintf(date, size, "%04d-%02d-%02d", year, month, day);
}

// HELPER FUNCTION
Officer *create_officer(const char *officer_id, const char *username, const char *password, const char *title, const char *first_name, const char *last_name, const char *badge_no, int clearance, SNTRPH *sntrph)
{
    Officer *officer = malloc(sizeof(Officer));
    GBOX *mailbox = malloc(sizeof(GBOX));
    memset(officer, 0, sizeof(Officer));
    memset(mailbox, 0, sizeof(GBOX));

    officer->base = malloc(sizeof(Person));
    memset(officer->base, 0, sizeof(Person));

    printf("Checkpoint: Memory zeroed for %s\n", officer_id);

    char DOB[20];
    random_date(DOB, sizeof(DOB));
    char criminal_record[500] = "No criminal record.";
    char relationship_status[50] = "Single";

    strcpy(officer->officer_id, officer_id);
    strcpy(officer->username, username);
    strcpy(officer->password, password);
    strcpy(officer->title, title);
    strcpy(officer->base->first_name, first_name);
    strcpy(officer->base->last_name, last_name);
    strcpy(officer->base->DOB, DOB);
    strcpy(officer->base->criminal_record, criminal_record);
    strcpy(officer->base->relationship_status, relationship_status);
    strcpy(officer->base->status, "ALIVE");

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

// HELPER FUNCTION
Person *create_person(const char *first_name, const char *last_name, const char *DOB, const char *criminal_record, const char *relationship_status, SNTRPH *sntrph)
{
    Person *person = malloc(sizeof(Person));
    memset(person, 0, sizeof(Person));

    printf("Checkpoint: Memory allocated for Person\n");

    if (sntrph->personlist.person_count == 0 || sntrph->personlist.person_list[sntrph->personlist.person_count - 1] == NULL)
    {
        strcpy(person->person_id, "P000");
    }
    else
    {
        strcpy(person->person_id, sntrph->personlist.person_list[sntrph->personlist.person_count - 1]->person_id);
        increment_id(person->person_id);
    }

    strcpy(person->first_name, first_name);
    strcpy(person->last_name, last_name);
    strcpy(person->DOB, DOB);
    strcpy(person->criminal_record, criminal_record);
    strcpy(person->relationship_status, relationship_status);
    strcpy(person->status, "ALIVE");

    printf("Checkpoint: Person details set for %s\n", person->DOB);

    person->address = NULL;

    sntrph->personlist.person_list[sntrph->personlist.person_count++] = person;

    printf("Checkpoint: Person added to SNTRPH list\n");

    return person;
}
// HELPER FUNCTION
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
    address->flagged = false;

    printf("Checkpoint: Address details set for %s\n", address->address_id);

    address->person_list[0] = person;
    for (int i = 1; i < 10; i++)
    {
        address->person_list[i] = NULL;
    }
    address->person_count = 1;
    person->address = address;
    address->case_count = 0;

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

void addPerson(SNTRPH *sntrph)
{
    char first_name[50];
    printf("> Enter the person's first name: ");
    fgets(first_name, sizeof(first_name), stdin);
    first_name[strcspn(first_name, "\n")] = 0;

    char last_name[50];
    printf("> Enter the person's last name: ");
    fgets(last_name, sizeof(last_name), stdin);
    last_name[strcspn(last_name, "\n")] = 0;

    char criminal_record[500];
    printf("> Enter the person's known criminal history: ");
    fgets(criminal_record, sizeof(criminal_record), stdin);
    criminal_record[strcspn(criminal_record, "\n")] = 0;

    char relationship_status[50];
    printf("> Enter the person's relationship status: ");
    fgets(relationship_status, sizeof(relationship_status), stdin);
    relationship_status[strcspn(relationship_status, "\n")] = 0;

    char DOB[30];
    printf("> Enter the person's DOB: ");
    fgets(DOB, sizeof(DOB), stdin);
    DOB[strcspn(DOB, "\n")] = 0;

    Person *person = create_person(first_name, last_name, DOB, criminal_record, relationship_status, sntrph);
    PrintLine("[✓] PERSON ADDED \n");
}

void linkAddress(SNTRPH *sntrph, char *add, char *person)
{
    Address *address = check_address(sntrph, add);
    Person *p = check_person(sntrph, person);
    if (p == NULL || address == NULL)
    {
        printf("!! MATCH COULD NOT BE FOUND !!");
        return;
    }
    p->address = address;
    if (address->person_count == 10)
    {
        printf("!! MAXIMUM COUNT REACHED !!\n");
        return;
    }
    address->person_list[address->person_count++] = p;
    printf("[✓] ADDRESS LINKED\n");
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
        {
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
}

void viewCase(SNTRPH *sntrph, const char *case_no)
{
    bool found = false;

    for (int i = 0; i < sntrph->caseList.case_count; i++)
    {
        CaseFile *file = sntrph->caseList.cases[i];

        if (strcmp(file->case_no, case_no) == 0)
        {
            found = true;

            printf("--------------------------------------------------\n");
            printf("           SAINT-RAPHAEL CASE FILE VIEW\n");
            printf("--------------------------------------------------\n\n");

            printf("CASE NO:           %s\n", file->case_no);
            printf("NAME:              %s\n", file->name);
            printf("DATE:              %s\n", file->date);
            printf("LEAD OFFICER:      %s\n", file->lead);

            if (file->location != NULL)
            {
                printf("LOCATION:          %s, %s, %s\n",
                       file->location->street,
                       file->location->city,
                       file->location->zip_code);
            }
            else
            {
                printf("LOCATION:          No location assigned.\n");
            }

            printf("STATUS:            %s\n", file->status);
            printf("LOCKED:            %s\n", file->is_locked ? "Yes" : "No");

            printf("--------------------------------------------------\n\n");

            printf("SUMMARY:\n%s\n\n", file->summary);

            printf("TYPE:              %s\n", file->type);

            printf("--------------------------------------------------\n\n");

            printf("VICTIMS (%d):\n", file->victim_count);
            for (int v = 0; v < file->victim_count; v++)
            {
                Person *p = file->victims[v];
                printf("  - %s %s\n", p->first_name, p->last_name);
            }
            if (file->victim_count == 0)
            {
                printf("  None\n");
            }

            printf("\nSUSPECTS (%d):\n", file->suspect_count);
            for (int s = 0; s < file->suspect_count; s++)
            {
                Person *p = file->suspects[s];
                printf("  - %s %s\n", p->first_name, p->last_name);
            }
            if (file->suspect_count == 0)
            {
                printf("  None\n");
            }

            printf("\nEVIDENCE (%d):\n", file->evidence_count);
            for (int e = 0; e < file->evidence_count; e++)
            {
                Evidence *ev = file->evidence[e];
                printf("  - [%s] %s (Status: %s)\n", ev->evidence_id, ev->name, ev->status);
            }
            if (file->evidence_count == 0)
            {
                printf("  None\n");
            }

            printf("\nEVIDENCE NOTES:\n%s\n", file->evidence_notes);

            printf("--------------------------------------------------\n");
            printf("[Press Enter to EXIT]\n");
            getchar();

            break;
        }
    }

    if (!found)
    {
        printf("!! NO CASE FOUND WITH CASE NO: %s !!\n", case_no);
        printf("[Press Enter to EXIT]\n");
        getchar();
    }
}

void editLog(SNTRPH *sntrph, char *token)
{
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
        {
            char action_type[50];
            printf("> Enter your new action type: ");
            fgets(action_type, sizeof(action_type), stdin);
            action_type[strcspn(action_type, "\n")] = 0;
            strcpy(log->actionType, action_type);
            break;
        }
        case 2:
        {
            char record_id[10];
            printf("> Enter your new record ID: ");
            fgets(record_id, sizeof(record_id), stdin);
            record_id[strcspn(record_id, "\n")] = 0;
            strcpy(log->record_id, record_id);
            break;
        }
        case 3:
        {
            char timestamp[20];
            printf("> Enter your new timestamp: ");
            fgets(timestamp, sizeof(timestamp), stdin);
            timestamp[strcspn(timestamp, "\n")] = 0;
            strcpy(log->timestamp, timestamp);
            break;
        }
        case 4:
        {
            char officer_id[20];
            printf("> Enter the Officer ID: ");
            fgets(officer_id, sizeof(officer_id), stdin);
            officer_id[strcspn(officer_id, "\n")] = 0;
            Officer *officer = check_officer(sntrph, officer_id);
            log->officer = officer;
            break;
        }
        case 5:
        {
            char action_description[200];
            printf("> Enter your new action description: ");
            fgets(action_description, sizeof(action_description), stdin);
            action_description[strcspn(action_description, "\n")] = 0;
            strcpy(log->action_description, action_description);
            break;
        }
        case 6:
            editing = false;
            break;
        }
    }
}

void editPerson(SNTRPH *sntrph, char *token)
{
    Person *person = check_person(sntrph, token);
    bool editing = true;
    while (editing)
    {
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
        {
            char date[15];
            get_current_date(date);

            char officer_name[120];
            snprintf(officer_name, sizeof(officer_name), "%s %s %s", sntrph->current_user->title, sntrph->current_user->base->first_name, sntrph->current_user->base->last_name);

            char editperson[120];
            snprintf(editperson, sizeof(editperson), "%s %s", person->first_name, person->last_name);

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
}

void searchResultsP(SNTRPH *sntrph, Person *matches[], int count)
{

    int page = 0;
    int total_pages = (count + PAGE_SIZE - 1) / PAGE_SIZE;
    char input[10];

    while (1)
    {
        system("cls");
        printf("\n--------------------------------------------------\n");
        printf("            SAINT RAPHAEL EXPLORER SYSTEM - PEOPLE\n");
        printf("--------------------------------------------------\n");

        int start = page * PAGE_SIZE;
        int end = start + PAGE_SIZE;
        if (end > count)
        {
            end = count;
        }

        for (int i = start; i < end; i++)
        {
            Person *person = matches[i];
            printf("Person %d, ID: <%s>, %s %s | %s\n", i + 1, person->person_id, person->first_name, person->last_name, person->status);
        }
        printf("--------------------------------------------------\n\n");
        printf("[n] Next | [p] Prev | [q] Quit | [1-%d] View person\n> ", count);

        fgets(input, sizeof(input), stdin);

        if (tolower(input[0]) == 'q')
        {
            break;
        }
        else if (tolower(input[0]) == 'n' && page < total_pages - 1)
        {
            page++;
        }
        else if (tolower(input[0]) == 'p' && page > 0)
        {
            page--;
        }
        else
        {
            int choice_int = atoi(input);
            if (choice_int > 0 && choice_int <= count)
            {
                viewPerson(matches[choice_int - 1]);
            }
        }
    }
}
void searchResultsA(SNTRPH *sntrph, Address *matches[], int count)
{

    int page = 0;
    int total_pages = (count + PAGE_SIZE - 1) / PAGE_SIZE;
    char input[10];

    while (1)
    {
        system("cls");
        printf("\n--------------------------------------------------\n");
        printf("            SAINT RAPHAEL EXPLORER SYSTEM - PEOPLE\n");
        printf("--------------------------------------------------\n");

        int start = page * PAGE_SIZE;
        int end = start + PAGE_SIZE;
        if (end > count)
        {
            end = count;
        }

        for (int i = start; i < end; i++)
        {
            Address *address = matches[i];
            printf("Address %d, ID: <%s>, %s %s | C: %s\n", i + 1, address->address_id, address->city, address->zip_code, address->case_count);
        }
        printf("--------------------------------------------------\n\n");
        printf("[n] Next | [p] Prev | [q] Quit | [1-%d] View person\n> ", count);

        fgets(input, sizeof(input), stdin);

        if (tolower(input[0]) == 'q')
        {
            break;
        }
        else if (tolower(input[0]) == 'n' && page < total_pages - 1)
        {
            page++;
        }
        else if (tolower(input[0]) == 'p' && page > 0)
        {
            page--;
        }
        else
        {
            int choice_int = atoi(input);
            if (choice_int > 0 && choice_int <= count)
            {
                viewAddress(matches[choice_int - 1]);
            }
        }
    }
}
void searchResultsL(SNTRPH *sntrph, AuditLog *matches[], int count)
{

    int page = 0;
    int total_pages = (count + PAGE_SIZE - 1) / PAGE_SIZE;
    char input[10];

    while (1)
    {
        system("cls");
        printf("\n--------------------------------------------------\n");
        printf("            SAINT RAPHAEL EXPLORER SYSTEM - LOG\n");
        printf("--------------------------------------------------\n");

        int start = page * PAGE_SIZE;
        int end = start + PAGE_SIZE;
        if (end > count)
        {
            end = count;
        }

        for (int i = start; i < end; i++)
        {
            AuditLog *log = matches[i];
            printf("Log %d, ID: <%s>, %s| %s\n", i + 1, log->audit_id, log->actionType, log->timestamp);
        }
        printf("--------------------------------------------------\n\n");
        printf("[n] Next | [p] Prev | [q] Quit | [1-%d] View person\n> ", count);

        fgets(input, sizeof(input), stdin);

        if (tolower(input[0]) == 'q')
        {
            break;
        }
        else if (tolower(input[0]) == 'n' && page < total_pages - 1)
        {
            page++;
        }
        else if (tolower(input[0]) == 'p' && page > 0)
        {
            page--;
        }
        else
        {
            int choice_int = atoi(input);
            if (choice_int > 0 && choice_int <= count)
            {
                viewLog(matches[choice_int - 1]);
            }
        }
    }
}
void searchPerson(SNTRPH *sntrph, char *token)
{
    // Temporary array to store matched Person pointers
    Person *matches[100]; // Adjust size as needed
    int match_count = 0;

    for (int i = 0; i < sntrph->personlist.person_count; i++)
    {
        Person *current = sntrph->personlist.person_list[i];
        if (strstr_w(current->DOB, token) != NULL ||
            strstr_w(current->first_name, token) != NULL ||
            strstr_w(current->last_name, token) != NULL ||
            strstr_w(current->person_id, token) != NULL ||
            strstr_w(current->address->city, token) != NULL ||
            strstr_w(current->address->street, token) != NULL)

        {
            matches[match_count++] = current;
        }
    }

    if (match_count == 0)
    {
        printf("No matches found.\n");
        return;
    }

    searchResultsP(sntrph, matches, match_count);

    // Print all results at the end
}

void searchResultsO(SNTRPH *sntrph, Officer *matches[], int count)
{
    int page = 0;
    int total = (count + PAGE_SIZE - 1) / PAGE_SIZE;
    char input[10];

    while (1)
    {
        printf("\n--------------------------------------------------\n");
        printf("            SAINT RAPHAEL EXPLORER SYSTEM - OFFICERS\n");
        printf("--------------------------------------------------\n");

        int start = page * PAGE_SIZE;
        int end = start + PAGE_SIZE;
        if (end > count)
        {
            end = count;
        }

        for (int i = start; i < end; i++)
        {
            Officer *off = matches[i];
            printf("Officer %d, ID: <%s>, %s %s, Clearance: <%d> | %s\n", i + 1, off->b_no, off->base->first_name, off->base->last_name, off->clearance, off->base->status);
        }
        printf("--------------------------------------------------\n\n");
        printf("[n] Next | [p] Prev | [q] Quit | [1-%d] View officer\n> ", count);

        fgets(input, sizeof(input), stdin);
        if (tolower(input[0]) == 'q')
        {
            break;
        }
        else if (tolower(input[0]) == 'n' && page < total - 1)
        {
            page++;
        }
        else if (tolower(input[0]) == 'p' && page > 0)
        {
            page--;
        }
        else
        {
            int choice_int = atoi(input);
            if (choice_int > 0 && choice_int <= count)
            {
                viewOfficer(matches[choice_int - 1]);
            }
        }
    }
}

void searchOfficer(SNTRPH *sntrph, char *token)
{
    Officer *matches[100];
    int m_count = 0;

    for (int i = 0; i < sntrph->officerList.officer_count; i++)
    {
        Officer *current = sntrph->officerList.officers[i];
        if (strstr_w(current->base->DOB, token) != NULL ||
            strstr_w(current->base->first_name, token) != NULL ||
            strstr_w(current->base->last_name, token) != NULL ||
            strstr_w(current->base->person_id, token) != NULL ||
            strstr_w(current->title, token) != NULL ||
            strstr_w(current->officer_id, token) != NULL ||
            strstr_w(current->username, token) != NULL ||
            strstr_w(current->b_no, token) != NULL ||
            current->clearance == atoi(token))
        {
            matches[m_count++] = current;
        }
    }

    if (m_count == 0)
    {
        printf("No matches found.\n");
        return;
    }
    searchResultsO(sntrph, matches, m_count);
}

void searchResultsE(SNTRPH *sntrph, Evidence *matches[], int count)
{
    int page = 0;
    int total = (count + PAGE_SIZE - 1) / PAGE_SIZE;
    char input[10];

    while (1)
    {
        system("cls");
        printf("\n--------------------------------------------------\n");
        printf("            SAINT RAPHAEL EXPLORER SYSTEM - EVIDENCE\n");
        printf("--------------------------------------------------\n");

        int start = page * PAGE_SIZE;
        int end = start + PAGE_SIZE;

        if (end > count)
        {
            end = count;
        }

        for (int i = start; i < end; i++)
        {
            Evidence *ev = matches[i];
            printf("Evidence %d, ID: <%s>, %s, %s, %s | %s\n", i + 1, ev->evidence_id, ev->name, ev->type, ev->date, ev->status);
        }
        printf("--------------------------------------------------\n\n");
        printf("[n] Next | [p] Prev | [q] Quit | [1-%d] View evidence\n> ", count);
        fgets(input, sizeof(input), stdin);
        if (tolower(input[0]) == 'q')
        {
            break;
        }
        else if (tolower(input[0]) == 'n' && page < total - 1)
        {
            page++;
        }
        else if (tolower(input[0]) == 'p' && page > 0)
        {
            page--;
        }
        else
        {
            int choice_int = atoi(input);
            if (choice_int > 0 && choice_int <= count)
            {
                printEvidence(matches[choice_int - 1]);
            }
        }
    }
}

void searchEvidence(SNTRPH *sntrph, char *token)
{
    Evidence *matches[100];
    int m_count = 0;

    for (int i = 0; i < sntrph->evidenceArchive.total_items; i++)
    {
        Evidence *ev = sntrph->evidenceArchive.evidence_items[i];
        if (strstr_w(ev->evidence_id, token) != NULL || strstr_w(ev->name, token) != NULL || strstr_w(ev->status, token) != NULL || strstr_w(ev->type, token) != NULL || strstr_w(ev->case_id, token) != NULL || strstr_w(ev->description, token) != NULL || strstr_w(ev->date, token) != NULL || strstr_w(ev->notes, token) != NULL)
        {
            matches[m_count++] = ev;
        }
    }
    if (m_count == 0)
    {
        PrintLine("!! NO EVIDENCE RECORD FOUND WITH THE GIVEN <ID> !!");
        return;
    }

    searchResultsE(sntrph, matches, m_count);
}

void searchResultsC(SNTRPH *sntrph, CaseFile *matches[], int count)
{
    int page = 0;
    int total = (count + PAGE_SIZE - 1) / PAGE_SIZE;
    char input[10];

    while (1)
    {
        system("cls");
        printf("\n--------------------------------------------------\n");
        printf("            SAINT RAPHAEL EXPLORER SYSTEM - OFFICERS\n");
        printf("--------------------------------------------------\n");

        int start = page * PAGE_SIZE;
        int end = start + PAGE_SIZE;

        if (end > count)
        {
            end = count;
        }

        for (int i = start; i < end; i++)
        {
            CaseFile *file = matches[i];
            printf("Case %d, ID:<%s>, %s, %s, %s, %s | %s\n", i + 1, file->case_no, file->name, file->type, file->lead, file->date, file->status);
        }
        printf("--------------------------------------------------\n\n");
        printf("[n] Next | [p] Prev | [q] Quit | [1-%d] View case\n> ", count);
        fgets(input, sizeof(input), stdin);
        if (tolower(input[0]) == 'q')
        {
            break;
        }
        else if (tolower(input[0]) == 'n' && page < total - 1)
        {
            page++;
        }
        else if (tolower(input[0]) == 'p' && page > 0)
        {
            page--;
        }
        else
        {
            int choice_int = atoi(input);
            if (choice_int > 0 && choice_int <= count)
            {
                viewCase(sntrph, matches[choice_int - 1]->case_no);
            }
        }
    }
}

void searchCase(SNTRPH *sntrph, char *token)
{
    CaseFile *files[100];
    int m_count = 0;

    for (int i = 0; i < sntrph->caseList.case_count; i++)
    {
        CaseFile *file = sntrph->caseList.cases[i];
        if (file == NULL)
        {
            fprintf(stderr, "Debug: file is NULL at index %d\n", i);
            continue;
        }

        // Check each field before calling strstr_w
        if (file->case_no == NULL)
            fprintf(stderr, "Debug: file->case_no is NULL (case %d)\n", i);
        if (file->name == NULL)
            fprintf(stderr, "Debug: file->name is NULL (case %d)\n", i);
        if (file->date == NULL)
            fprintf(stderr, "Debug: file->date is NULL (case %d)\n", i);
        if (file->lead == NULL)
            fprintf(stderr, "Debug: file->lead is NULL (case %d)\n", i);
        if (file->location == NULL)
            fprintf(stderr, "Debug: file->location is NULL (case %d)\n", i);
        else
        {
            if (file->location->city == NULL)
                fprintf(stderr, "Debug: file->location->city is NULL (case %d)\n", i);
            if (file->location->street == NULL)
                fprintf(stderr, "Debug: file->location->street is NULL (case %d)\n", i);
        }
        if (file->summary == NULL)
            fprintf(stderr, "Debug: file->summary is NULL (case %d)\n", i);
        if (file->type == NULL)
            fprintf(stderr, "Debug: file->type is NULL (case %d)\n", i);
        if (file->status == NULL)
            fprintf(stderr, "Debug: file->status is NULL (case %d)\n", i);

        bool matched = false;
        if ((file->case_no && strstr_w(file->case_no, token) != NULL) ||
            (file->name && strstr_w(file->name, token) != NULL) ||
            (file->date && strstr_w(file->date, token) != NULL) ||
            (file->lead && strstr_w(file->lead, token) != NULL) ||
            (file->location && file->location->city && strstr_w(file->location->city, token) != NULL) ||
            (file->location && file->location->street && strstr_w(file->location->street, token) != NULL) ||
            (file->summary && strstr_w(file->summary, token) != NULL) ||
            (file->type && strstr_w(file->type, token) != NULL) ||
            (file->status && strstr_w(file->status, token) != NULL))
        {
            matched = true;
        }
        else
        {
            int max = (file->victim_count > file->suspect_count) ? file->victim_count : file->suspect_count;

            for (int j = 0; j < max; j++)
            {
                Person *victim = (j < file->victim_count) ? file->victims[j] : NULL;
                Person *suspect = (j < file->suspect_count) ? file->suspects[j] : NULL;

                Person *persons[2] = {victim, suspect};

                for (int k = 0; k < 2; k++)
                {
                    Person *p = persons[k];
                    if (p != NULL)
                    {
                        if ((p->DOB && strstr_w(p->DOB, token) != NULL) ||
                            (p->first_name && strstr_w(p->first_name, token) != NULL) ||
                            (p->last_name && strstr_w(p->last_name, token) != NULL) ||
                            (p->person_id && strstr_w(p->person_id, token) != NULL))
                        {
                            files[m_count++] = file;
                            // Once matched, no need to check other persons for this file
                            j = max; // break outer loop early
                            break;   // break inner loop
                        }
                    }
                }
            }
        }

    found_match:
        if (matched)
        {
            if (m_count < 100)
                files[m_count++] = file;
        }
    }

    if (m_count == 0)
    {
        PrintLine("!! NO EVIDENCE RECORD FOUND WITH THE GIVEN <TOKEN> !!");
        return;
    }

    searchResultsC(sntrph, files, m_count);
    // Process files here if needed
}

void editAddress(SNTRPH *sntrph, char *token)
{
    Address *address = check_addressP(sntrph, token);
    if (address == NULL)
    {
        PrintLine("!! ADDRESS NOT FOUND !!");
        return;
    }
    bool editing = true;
    while (editing)
    {
        printf("\n--------------------------------------------------\n");
        printf("            INFORMATION ALTERING SYSTEM - ADDRESS\n");
        printf("--------------------------------------------------\n");
        printf("1.) Street\n");
        printf("2.) City\n");
        printf("3.) Zip Code\n");
        printf("4.) QUIT\n");
        char buffer[4];
        printf("> Enter your choice: ");
        fgets(buffer, sizeof(buffer), stdin);
        int buf = atoi(buffer);

        switch (buf)
        {
        case 1:
        {
            printf("> Enter the new street: ");
            char street[50];
            fgets(street, sizeof(street), stdin);
            street[strcspn(street, "\n")] = '\0';
            strcpy(address->street, street);
            break;
        }
        case 2:
        {
            printf("> Enter the new city: ");
            char city[50];
            fgets(city, sizeof(city), stdin);
            city[strcspn(city, "\n")] = '\0';
            strcpy(address->city, city);
            break;
        }
        case 3:
        {
            printf("> Enter the new zip code: ");
            char zip_code[20];
            fgets(zip_code, sizeof(zip_code), stdin);
            zip_code[strcspn(zip_code, "\n")] = '\0';
            strcpy(address->zip_code, zip_code);
            break;
        }
        case 4:
        {
            char date[15];
            get_current_date(date);

            char officer_name[120];
            snprintf(officer_name, sizeof(officer_name), "%s %s %s", sntrph->current_user->title, sntrph->current_user->base->first_name, sntrph->current_user->base->last_name);

            char editaddress[120];
            snprintf(editaddress, sizeof(editaddress), "%s", address->street);

            char description[200];
            snprintf(description, sizeof(description), "%s ALTERED %s's ADDRESS ON %s", officer_name, editaddress, date);
            addLog(sntrph, "ADDRESS ALTERATION", address->address_id, sntrph->current_user, description);
            PrintLine("Quitting...");
            PrintLine("--------------------------------------------------");
            editing = false;
            break;
        }
        }
    }
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
            snprintf(description, sizeof(description), "%s ADDED A DISCIPLINARY TO %s's OFFICER PROFILE ON %s", officer_name, badofficer, date);

            addLog(sntrph, "DISCIPLINARY ADDITION", token, sntrph->current_user, description);
            return;
        }
    }

    printf("[CHECKPOINT 6] No available slots for disciplinary actions\n");
}

// HELPER FUNCTION
const char *random_name(const char *names[], int name_count)
{
    return names[random_int(0, name_count - 1)];
}
// HELPER FUNCTION
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
// HELPER FUNCTION
CaseFile *createCase(SNTRPH *sntrph, char *name, char *date, char *lead, char *summary, char *type, char *evidence_notes, bool locked, char *status)
{
    CaseFile *file = malloc(sizeof(CaseFile));
    memset(file, 0, sizeof(file));

    if (sntrph->caseList.case_count == 0 || sntrph->caseList.cases[sntrph->caseList.case_count - 1] != NULL)
    {
        strcpy(file->case_no, "C000");
    }
    else
    {
        strcpy(file->case_no, sntrph->caseList.cases[sntrph->caseList.case_count - 1]->case_no);
        increment_id(file->case_no);
    }

    strcpy(file->name, name);
    strcpy(file->date, date);
    strcpy(file->lead, lead);
    strcpy(file->summary, summary);
    strcpy(file->type, type);
    strcpy(file->evidence_notes, evidence_notes);
    strcpy(file->status, status);

    file->location = NULL;
    for (int i = 0; i < 5; i++)
    {
        file->victims[i] = NULL;
        file->suspects[i] = NULL;
    }

    for (int i = 0; i < 20; i++)
    {
        file->evidence[i] = NULL;
    }

    file->victim_count = 0;
    file->suspect_count = 0;
    file->evidence_count = 0;

    file->is_locked = locked;

    sntrph->caseList.cases[sntrph->caseList.case_count++] = file;
    return file;
}
void addCase(SNTRPH *sntrph)
{
    Address *location = malloc(sizeof(Address));
    Person *victims[5];
    Person *suspects[5];
    Evidence *evidence[20];

    char name[100];
    printf("> Enter the case name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;

    char date[20];
    get_current_date(date);

    char lead[50];
    printf("> Enter the case lead: ");
    fgets(lead, sizeof(lead), stdin);
    lead[strcspn(lead, "\n")] = 0;

    char summary[1000];
    printf("> Enter the case summary: ");
    fgets(summary, sizeof(summary), stdin);
    summary[strcspn(summary, "\n")] = 0;

    char type[1000];
    printf("> Enter the case type: ");
    fgets(type, sizeof(type), stdin);
    type[strcspn(type, "\n")] = 0;

    char notes[600];
    printf("> Enter any notes on the case/evidence: ");
    fgets(notes, sizeof(notes), stdin);
    notes[strcspn(notes, "\n")] = 0;

    char res[5];
    printf("> Should this case be restricted for clearances below 3? (Y/N): ");
    fgets(res, sizeof(res), stdin);
    res[strcspn(res, "\n")] = 0;

    bool restricted = (tolower(res[0]) == 'Y') ? true : false;

    char status[20];
    printf("> Enter the status of this case: ");
    fgets(status, sizeof(status), stdin);
    status[strcspn(status, "\n")] = 0;

    CaseFile *file = createCase(sntrph, name, date, lead, summary, type, notes, restricted, status);
    printf("Case File %s was added.\n", sntrph->caseList.cases[sntrph->caseList.case_count - 1]->name);
    return;
}

void deleteCase(SNTRPH *sntrph, char *token)
{
    bool found = false;
    char date[15];
    get_current_date(date);

    char officer_name[120];
    snprintf(officer_name, sizeof(officer_name), "%s %s %s", sntrph->current_user->title, sntrph->current_user->base->first_name, sntrph->current_user->base->last_name);

    for (int i = 0; i < sntrph->caseList.case_count; i++)
    {
        CaseFile *file = sntrph->caseList.cases[i];

        if (strcmp(file->case_no, token) == 0 || strcmp(file->name, token) == 0)
        {
            found = true;

            if (sntrph->current_user->clearance < 3)
            {
                PrintLine("ACCESS DENIED: INSUFFICIENT CLEARANCE\n");
                PrintLine("!! ATTEMPT FOLLOWED IN AUDIT TRAIL !!");

                char description[200];
                snprintf(description, sizeof(description),
                         "%s ATTEMPTED TO DELETE CASE ENTRY: %s ON %s",
                         officer_name, token, date);
                addLog(sntrph, "CASE ENTRY DELETION ATTEMPT", token,
                       sntrph->current_user, description);
                return;
            }

            // Prompt to delete linked evidence BEFORE deleting case
            char *id = file->case_no;
            printf("\n> Would you also like to delete all linked EVIDENCE records? [y/n]: ");
            char input[10];
            fgets(input, sizeof(input), stdin);

            if (input[0] == 'y' || input[0] == 'Y')
            {
                for (int k = 0; k < sntrph->evidenceArchive.total_items;)
                {
                    if (strcmp(id, sntrph->evidenceArchive.evidence_items[k]->case_id) == 0)
                    {
                        if (sntrph->caseList.deleted_count < MAX_ADDRESSES)
                        {
                            sntrph->caseList.deleted[sntrph->caseList.deleted_count++] = file;
                        }
                        else
                        {
                            PrintLine("Deleted case list full, cannot archive deleted case.\n");
                            // Optionally handle overflow here
                        }

                        for (int m = k; m < sntrph->evidenceArchive.total_items - 1; m++)
                        {
                            sntrph->evidenceArchive.evidence_items[m] = sntrph->evidenceArchive.evidence_items[m + 1];
                        }
                        sntrph->evidenceArchive.total_items--;

                        char msg[80];
                        snprintf(msg, sizeof(msg),
                                 "[✓] DELETED LINKED EVIDENCE RECORD: <%s>", id);
                        PrintLine(msg);

                        char description[200];
                        snprintf(description, sizeof(description),
                                 "%s ALSO DELETED LINKED EVIDENCE RECORD: %s ON %s",
                                 officer_name, id, date);
                        addLog(sntrph, "RECORD DELETION", id,
                               sntrph->current_user, description);

                        // Don't increment k here because items shifted left
                    }
                    else
                    {
                        k++;
                    }
                }
            }

            // Now delete the case itself
            for (int j = i; j < sntrph->caseList.case_count - 1; j++)
            {
                sntrph->caseList.cases[j] = sntrph->caseList.cases[j + 1];
            }
            sntrph->caseList.case_count--;

            char confirmation[80];
            snprintf(confirmation, sizeof(confirmation),
                     "[✓] DELETED A CASE RECORD: <%s>", token);
            PrintLine(confirmation);

            char description[200];
            snprintf(description, sizeof(description),
                     "%s DELETED A CASE RECORD: %s ON %s",
                     officer_name, token, date);
            addLog(sntrph, "RECORD DELETION", token, sntrph->current_user, description);

            break;
        }
    }

    if (!found)
    {
        PrintLine("!! NO CASE RECORD FOUND WITH THE GIVEN <ID> !!");
    }
}

void viewCases(SNTRPH *sntrph)

{
    int count = sntrph->caseList.case_count;
    int page = 0;
    int total = (count + PAGE_SIZE - 1) / PAGE_SIZE;
    char input[10];

    while (1)
    {
        system("cls");
        printf("--------------------------------------------------\n");
        printf("           SAINT-RAPHAEL CASE TRAIL VIEW\n");
        printf("--------------------------------------------------\n\n");

        int start = page * PAGE_SIZE;
        int end = start + PAGE_SIZE;
        if (end > count)
        {
            end = count;
        }

        for (int i = start; i < end; i++)
        {
            CaseFile *file = sntrph->caseList.cases[i];
            printf("Case: %d, Case No: <%s>, %s, %s | %s\n", i + 1, file->case_no, file->name, file->type, file->lead);
        }
        printf("--------------------------------------------------\n\n");
        printf("[n] Next | [p] Prev | [q] Quit | [1-%d] View person\n> ", count);

        fgets(input, sizeof(input), stdin);
        if (tolower(input[0]) == 'q')
        {
            break;
        }
        else if (tolower(input[0]) == 'n' && page < total - 1)
        {
            page++;
        }
        else if (tolower(input[0]) == 'p' && page > 0)
        {
            page--;
        }
        else
        {
            int choice_int = atoi(input);
            if (choice_int > 0 && choice_int <= count)
            {
                viewCase(sntrph, sntrph->caseList.cases[choice_int - 1]->case_no);
            }
        }
    }
}

void moveToArchive(SNTRPH *sntrph, char *token)
{
    bool found = false;
    char officer_name[120];
    snprintf(officer_name, sizeof(officer_name), "%s %s %s", sntrph->current_user->title, sntrph->current_user->base->first_name, sntrph->current_user->base->last_name);
    char description[250];
    char date[15];
    get_current_date(date);

    for (int i = 0; i < sntrph->caseList.case_count; i++)
    {
        if (strcmp(token, sntrph->caseList.cases[i]->case_no) == 0)
        {
            if (sntrph->caseList.archive_count < MAX_ADDRESSES)
            {
                sntrph->caseList.archive[sntrph->caseList.archive_count++] = sntrph->caseList.cases[i];
                for (int j = i; i < sntrph->caseList.case_count; j++)
                {
                    sntrph->caseList.cases[j] = sntrph->caseList.cases[j + 1];
                    printf("[✓] CASE ARCHIVED\n");
                    snprintf(description, sizeof(description), "%s ARCHIVED CASE: %s ON %s", officer_name, token, date);
                    return;
                }
            }
        }
    }
    if (!found)
    {
        PrintLine("!! NO CASE RECORD FOUND WITH THE GIVEN <ID> !!");
        return;
    }
}

void viewArchive(SNTRPH *sntrph, char *case_no)
{
    bool found = false;

    for (int i = 0; i < sntrph->caseList.archive_count; i++)
    {
        CaseFile *file = sntrph->caseList.archive[i];

        if (strcmp(file->case_no, case_no) == 0)
        {
            found = true;

            printf("--------------------------------------------------\n");
            printf("           SAINT-RAPHAEL CASE FILE VIEW\n");
            printf("--------------------------------------------------\n\n");

            printf("CASE NO:           %s\n", file->case_no);
            printf("NAME:              %s\n", file->name);
            printf("DATE:              %s\n", file->date);
            printf("LEAD OFFICER:      %s\n", file->lead);

            if (file->location != NULL)
            {
                printf("LOCATION:          %s, %s, %s\n",
                       file->location->street,
                       file->location->city,
                       file->location->zip_code);
            }
            else
            {
                printf("LOCATION:          No location assigned.\n");
            }

            printf("STATUS:            %s\n", file->status);
            printf("LOCKED:            %s\n", file->is_locked ? "Yes" : "No");

            printf("--------------------------------------------------\n\n");

            printf("SUMMARY:\n%s\n\n", file->summary);

            printf("TYPE:              %s\n", file->type);

            printf("--------------------------------------------------\n\n");

            printf("VICTIMS (%d):\n", file->victim_count);
            for (int v = 0; v < file->victim_count; v++)
            {
                Person *p = file->victims[v];
                printf("  - %s %s\n", p->first_name, p->last_name);
            }
            if (file->victim_count == 0)
            {
                printf("  None\n");
            }

            printf("\nSUSPECTS (%d):\n", file->suspect_count);
            for (int s = 0; s < file->suspect_count; s++)
            {
                Person *p = file->suspects[s];
                printf("  - %s %s\n", p->first_name, p->last_name);
            }
            if (file->suspect_count == 0)
            {
                printf("  None\n");
            }

            printf("\nEVIDENCE (%d):\n", file->evidence_count);
            for (int e = 0; e < file->evidence_count; e++)
            {
                Evidence *ev = file->evidence[e];
                printf("  - [%s] %s (Status: %s)\n", ev->evidence_id, ev->name, ev->status);
            }
            if (file->evidence_count == 0)
            {
                printf("  None\n");
            }

            printf("\nEVIDENCE NOTES:\n%s\n", file->evidence_notes);

            printf("--------------------------------------------------\n");
            printf("[Press Enter to EXIT]\n");
            getchar();

            break;
        }
    }

    if (!found)
    {
        printf("!! NO CASE FOUND WITH CASE NO: %s !!\n", case_no);
        printf("[Press Enter to EXIT]\n");
        getchar();
    }
}

void viewArchived(SNTRPH *sntrph, char *token)
{
    int count = sntrph->caseList.archive_count;
    int page = 0;
    int total = (count + PAGE_SIZE - 1) / PAGE_SIZE;
    char input[10];

    while (1)
    {
        system("cls");
        printf("--------------------------------------------------\n");
        printf("           SAINT-RAPHAEL CASE TRAIL VIEW\n");
        printf("--------------------------------------------------\n\n");

        int start = page * PAGE_SIZE;
        int end = start + PAGE_SIZE;
        if (end > count)
        {
            end = count;
        }

        for (int i = start; i < end; i++)
        {
            CaseFile *file = sntrph->caseList.archive[i];
            printf("Case: %d, Case No: <%s>, %s, %s | %s", i + 1, file->case_no, file->name, file->type, file->lead);
        }
        printf("--------------------------------------------------\n\n");
        printf("[n] Next | [p] Prev | [q] Quit | [1-%d] View person\n> ", count);

        fgets(input, sizeof(input), stdin);
        if (tolower(input[0]) == 'q')
        {
            break;
        }
        else if (tolower(input[0]) == 'n' && page < total - 1)
        {
            page++;
        }
        else if (tolower(input[0]) == 'p' && page > 0)
        {
            page--;
        }
        else
        {
            int choice_int = atoi(input);
            if (choice_int > 0 && choice_int <= count)
            {
                viewCase(sntrph, sntrph->caseList.archive[choice_int - 1]->case_no);
            }
        }
    }
}

void recoverArchived(SNTRPH *sntrph, char *token)
{
    bool found = false;
    char officer_name[120];
    snprintf(officer_name, sizeof(officer_name), "%s %s %s", sntrph->current_user->title, sntrph->current_user->base->first_name, sntrph->current_user->base->last_name);
    char description[250];
    char date[15];
    get_current_date(date);

    for (int i = 0; i < sntrph->caseList.archive_count; i++)
    {
        if (strcmp(token, sntrph->caseList.archive[i]->case_no) == 0)
        {
            if (sntrph->caseList.case_count < MAX_ADDRESSES)
            {
                sntrph->caseList.cases[sntrph->caseList.case_count++] = sntrph->caseList.archive[i];
                for (int j = i; j < sntrph->caseList.archive_count; j++)
                {
                    sntrph->caseList.archive[j] = sntrph->caseList.archive[j + 1];
                }
                printf("[✓] ARCHIVE MOVED\n");
                snprintf(description, sizeof(description), "%s MOVED ARCHIVED CASE: %s INTO CIRCULATION ON %s", officer_name, token, date);
                return;
            }
        }
    }

    if (!found)
    {
        PrintLine("!! NO CASE RECORD FOUND WITH THE GIVEN <ID> !!");
        return;
    }
}

// Returns dynamically allocated array of Person* that match filters, and sets filteredCount
Person **filterPeople(SNTRPH *sntrph, const char **fields, const char **values, const char **operators, int conditionsCount, int *filteredCount)
{
    PersonList *personlist = &sntrph->personlist;
    Person **filtered = malloc(personlist->person_count * sizeof(Person *));
    *filteredCount = 0;

    for (int i = 0; i < personlist->person_count; i++)
    {
        Person *p = personlist->person_list[i];
        int match = evalPersonCondition(p, fields[0], values[0]);

        for (int c = 1; c < conditionsCount; c++)
        {
            if (strcmp(operators[c - 1], "AND") == 0)
                match = match && evalPersonCondition(p, fields[c], values[c]);
            else if (strcmp(operators[c - 1], "OR") == 0)
                match = match || evalPersonCondition(p, fields[c], values[c]);
        }

        if (match)
            filtered[(*filteredCount)++] = p;
    }

    filtered = realloc(filtered, (*filteredCount) * sizeof(Person *));
    return filtered;
}

Officer **filterOfficers(SNTRPH *sntrph, const char **fields, char **values, const char **ops, int conditionsCount, int *filteredCount)
{
    OfficerList *offList = &sntrph->officerList;
    Officer **filtered = malloc(offList->officer_count * sizeof(Person *));
    *filteredCount = 0;

    for (int i = 0; i < offList->officer_count; i++)
    {
        Officer *off = offList->officers[i];
        int match = evalOfficerCondition(off, fields[0], values[0]);

        for (int c = 1; c < conditionsCount; c++)
        {
            if (strcmp(ops[c - 1], "AND") == 0)
                match = match && evalOfficerCondition(off, fields[c], values[c]);
            else if (strcmp(ops[c - 1], "OR") == 0)
                match = match || evalOfficerCondition(off, fields[c], values[c]);
        }
        if (match)
        {
            filtered[(*filteredCount)++] = off;
        }
    }
    filtered = realloc(filtered, (*filteredCount) * sizeof(Person *));
    return filtered;
}
CaseFile **filterCases(SNTRPH *sntrph, const char **fields, char **values, const char **ops, int conditionsCount, int *filteredCount)
{
    CaseList *caseList = &sntrph->caseList;
    CaseFile **filtered = malloc(caseList->case_count * sizeof(CaseFile *));
    *filteredCount = 0;

    for (int i = 0; i < caseList->case_count; i++)
    {
        CaseFile *cf = caseList->cases[i];
        int match = evalCaseCondition(cf, fields[0], values[0]);

        for (int c = 1; c < conditionsCount; c++)
        {
            if (strcmp(ops[c - 1], "AND") == 0)
                match = match && evalCaseCondition(cf, fields[c], values[c]);
            else if (strcmp(ops[c - 1], "OR") == 0)
                match = match || evalCaseCondition(cf, fields[c], values[c]);
        }
        if (match)
        {
            filtered[(*filteredCount)++] = cf;
        }
    }
    filtered = realloc(filtered, (*filteredCount) * sizeof(CaseFile *));
    return filtered;
}

AuditLog **filterLogs(SNTRPH *sntrph, const char **fields,  char **values, const char **ops, int conditionsCount, int *filteredCount)
{
    LogList *logList = &sntrph->logList;
    AuditLog **filtered = malloc(logList->log_count * sizeof(AuditLog *));
    *filteredCount = 0;
    for (int i = 0; i < logList->log_count; i++)
    {
        AuditLog *log = logList->logs[i];
        int match = evalTrailCondition(log, fields[0], values[0]);
        for (int c = 1; c < conditionsCount; c++)
        {

            if (strcmp(ops[c - 1], "AND") == 0)
                match = match && evalTrailCondition(log, fields[c], values[c]);
            else if (strcmp(ops[c - 1], "OR") == 0)
                match = match || evalTrailCondition(log, fields[c], values[c]);
        }
        if (match)
        {
            filtered[(*filteredCount)++] = log;
        }
    }

    filtered = realloc(filtered, (*filteredCount) * sizeof(AuditLog *));
    return filtered;
}
Address **filterAdds(SNTRPH *sntrph, const char **fields, char **values, const char **ops, int conditionsCount, int *filteredCount)
{
    AddressList *addList = &sntrph->addressList;
    Address **filtered = malloc(addList->address_count * sizeof(Address *));
    *filteredCount = 0;

    for (int i = 0; i < addList->address_count; i++)
    {
        Address *add = addList->addresses[i];
        int match = evalAddCondition(add, fields[0], values[0]);
        for (int c = 1; c < conditionsCount; c++)
        {
            if (strcmp(ops[c - 1], "AND") == 0)
                match = match && evalAddCondition(add, fields[c], values[c]);
            else if (strcmp(ops[c - 1], "OR") == 0)
                match = match || evalAddCondition(add, fields[c], values[c]);
        }
        if (match)
        {
            filtered[(*filteredCount)++] = add;
        }
    }
    filtered = realloc(filtered, (*filteredCount) * sizeof(Person *));
    return filtered;
}
void processFilterPeople(SNTRPH *sntrph, char *input)
{
    // Parses input starting from after "FILTER PEOPLE"
    // Example input: "--field first_name --value John AND --field status --value ALIVE"

    const char *fields[10];
    const char *values[10];
    const char *operators[9]; // One less than conditions
    int condCount = 0;
    int opCount = 0;

    char *token = strtok(input, " ");
    while (token != NULL)
    {
        if (strcmp(token, "--field") == 0)
        {
            token = strtok(NULL, " ");
            fields[condCount] = token;
        }
        else if (strcmp(token, "--value") == 0)
        {
            token = strtok(NULL, " ");
            values[condCount] = token;
            condCount++;
        }
        else if (strcmp(token, "AND") == 0 || strcmp(token, "OR") == 0)
        {
            operators[opCount++] = token;
        }
        token = strtok(NULL, " ");
    }

    if (condCount == 0)
    {
        printf("No filter conditions provided.\n");
        return;
    }

    int filteredCount = 0;
    Person **filteredPeople = filterPeople(sntrph, fields, values, operators, condCount, &filteredCount);

    searchResultsP(sntrph, filteredPeople, filteredCount);

    free(filteredPeople);
}
void processFilterOfficer(SNTRPH *sntrph, char *input)
{
    // Parses input starting from after "FILTER PEOPLE"
    // Example input: "--field first_name --value John AND --field status --value ALIVE"

    const char *fields[10];
    char *values[10];
    const char *operators[9]; // One less than conditions
    int condCount = 0;
    int opCount = 0;

    char *token = strtok(input, " ");
    while (token != NULL)
    {
        if (strcmp(token, "--field") == 0)
        {
            token = strtok(NULL, " ");
            fields[condCount] = token;
        }
        else if (strcmp(token, "--value") == 0)
        {
            token = strtok(NULL, " ");
            values[condCount] = malloc(strlen(token) + 1);
            strcpy(values[condCount], token);

            printf("Debug: Value: %s", token);
            condCount++;
        }
        else if (strcmp(token, "AND") == 0 || strcmp(token, "OR") == 0)
        {
            operators[opCount++] = token;
        }
        token = strtok(NULL, " ");
    }

    if (condCount == 0)
    {
        printf("No filter conditions provided.\n");
        return;
    }

    int filteredCount = 0;
    Officer **filteredOfficers = filterOfficers(sntrph, fields, values, operators, condCount, &filteredCount);
    searchResultsO(sntrph, filteredOfficers, filteredCount);
    for (int i = 0; i < condCount; i++)
    {
        free(values[i]);
    }

    free(filteredOfficers);
}
// HELPER FUNCTION
void processFilterCases(SNTRPH *sntrph, char *input)
{

    const char *fields[10];
    char *values[10];
    const char *operators[9]; // One less than conditions
    int condCount = 0;
    int opCount = 0;

    char *token = strtok(input, " ");
    while (token != NULL)
    {
        if (strcmp(token, "--field") == 0)
        {
            token = strtok(NULL, " ");
            fields[condCount] = token;
        }
        else if (strcmp(token, "--value") == 0)
        {
            token = strtok(NULL, " ");
            values[condCount] = token;
            condCount++;
        }
        else if (strcmp(token, "AND") == 0 || strcmp(token, "OR") == 0)
        {
            operators[opCount++] = token;
        }
        token = strtok(NULL, " ");
    }

    if (condCount == 0)
    {
        printf("No filter conditions provided.\n");
        return;
    }

    int filteredCount = 0;
    CaseFile **filteredCases = filterCases(sntrph, fields, values, operators, condCount, &filteredCount);
    searchResultsC(sntrph, filteredCases, filteredCount);
    free(filteredCases);
}

void processFilterAdd(SNTRPH *sntrph, char *input)
{
    // Parses input starting from after "FILTER PEOPLE"
    // Example input: "--field first_name --value John AND --field status --value ALIVE"

    const char *fields[10];
    char *values[10];
    const char *operators[9]; // One less than conditions
    int condCount = 0;
    int opCount = 0;

    char *token = strtok(input, " ");
    while (token != NULL)
    {
        if (strcmp(token, "--field") == 0)
        {
            token = strtok(NULL, " ");
            fields[condCount] = token;
        }
        else if (strcmp(token, "--value") == 0)
        {
            token = strtok(NULL, " ");
            values[condCount] = malloc(strlen(token) + 1);
            strcpy(values[condCount], token);

            printf("Debug: Value: %s", token);
            condCount++;
        }
        else if (strcmp(token, "AND") == 0 || strcmp(token, "OR") == 0)
        {
            operators[opCount++] = token;
        }
        token = strtok(NULL, " ");
    }

    if (condCount == 0)
    {
        printf("No filter conditions provided.\n");
        return;
    }

    int filteredCount = 0;
    Address **filteredAddresses = filterAdds(sntrph, fields, values, operators, condCount, &filteredCount);
    searchResultsA(sntrph, filteredAddresses, filteredCount);
    for (int i = 0; i < condCount; i++)
    {
        free(values[i]);
    }

    free(filteredAddresses);
}
void processFilterLog(SNTRPH *sntrph, char *input)
{
    // Parses input starting from after "FILTER PEOPLE"
    // Example input: "--field first_name --value John AND --field status --value ALIVE"

    const char *fields[10];
    char *values[10];
    const char *operators[9]; // One less than conditions
    int condCount = 0;
    int opCount = 0;

    char *token = strtok(input, " ");
    while (token != NULL)
    {
        if (strcmp(token, "--field") == 0)
        {
            token = strtok(NULL, " ");
            fields[condCount] = token;
        }
        else if (strcmp(token, "--value") == 0)
        {
            token = strtok(NULL, " ");
            values[condCount] = malloc(strlen(token) + 1);
            strcpy(values[condCount], token);

            printf("Debug: Value: %s", token);
            condCount++;
        }
        else if (strcmp(token, "AND") == 0 || strcmp(token, "OR") == 0)
        {
            operators[opCount++] = token;
        }
        token = strtok(NULL, " ");
    }

    if (condCount == 0)
    {
        printf("No filter conditions provided.\n");
        return;
    }

    int filteredCount = 0;
    AuditLog **filteredLogs = filterLogs(sntrph, fields, values, operators, condCount, &filteredCount);
    searchResultsL(sntrph, filteredLogs, filteredCount);
    for (int i = 0; i < condCount; i++)
    {
        free(values[i]);
    }

    free(filteredLogs);
}
// TODO: FILTER EVIDENCE, ADOPTION REPORTS, ADDRESSES
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
// HELPER FUNCTION
void seed_people(int count, SNTRPH *sntrph)
{
    // Example random first and last names
    const char *first_names[100] = {
        "Michael", "Jessica", "Christopher", "Ashley", "Matthew", "Emily", "Joshua", "Samantha",
        "Daniel", "Amanda", "David", "Sarah", "Andrew", "Stephanie", "Joseph", "Jennifer",
        "John", "Elizabeth", "Justin", "Nicole", "Anthony", "Brittany", "James", "Lauren",
        "Robert", "Rachel", "Nicholas", "Danielle", "Jonathan", "Kayla", "Kevin", "Tiffany",
        "William", "Rebecca", "Brian", "Michelle", "Thomas", "Christina", "Steven", "Victoria",
        "Eric", "Alexandra", "Alexander", "Megan", "Tyler", "Melissa", "Ryan", "Erica",
        "Brandon", "Courtney", "Zachary", "Angela", "Benjamin", "Kimberly", "Sean", "Jacqueline",
        "Timothy", "Alyssa", "Gregory", "Crystal", "Adam", "Vanessa", "Luis", "Maria",
        "Angel", "Jasmine", "Jason", "Brianna", "Carlos", "Gabrielle", "Peter", "Natalie",
        "Frank", "Haley", "Jorge", "Cassandra", "Victor", "Monica", "Manuel", "Paige",
        "Jesus", "Amber", "Edward", "Marissa", "Samuel", "Catherine", "Mark", "Sabrina",
        "Aaron", "Veronica", "Jeremy", "Leslie", "Kenneth", "Alexis", "Noah", "Heather",
        "Nathan", "Ariana", "Richard", "Katherine"};

    const char *last_names[50] = {
        "Smith", "Johnson", "Williams", "Brown", "Jones",
        "Garcia", "Rodriguez", "Martinez", "Hernandez", "Lopez",
        "Gonzalez", "Perez", "Sanchez", "Ramirez", "Torres",
        "Nguyen", "Lee", "Kim", "Chen", "Wong",
        "Cohen", "Levine", "Goldberg", "Rosenberg", "Katz",
        "Murphy", "Kelly", "Sullivan", "O'Brien", "Kennedy",
        "DiAngelo", "Esposito", "Russo", "Romano", "Bianchi",
        "Rivera", "Castro", "Ortiz", "Delgado", "Morales",
        "Thomas", "Jackson", "Lewis", "Walker", "Robinson",
        "Diaz", "Reyes", "Cruz", "Flores", "Washington"};

    char street[128], city[128], zip_code[128];

    srand(time(NULL)); // Seed the random number generator for randomness

    for (int i = 0; i < count; i++)
    {
        // Generate random details for each person
        const char *first_name = random_name(first_names, sizeof(first_names) / sizeof(first_names[0]));
        const char *last_name = random_name(last_names, sizeof(last_names) / sizeof(last_names[0]));

        random_address(street, city, zip_code); // Generate address components
        char personid[15];
        printf("Person count = %d\n", sntrph->personlist.person_count);
        printf("Person list pointer: %p\n", (void *)sntrph->personlist.person_list);
        if (sntrph->personlist.person_count == 0 || sntrph->personlist.person_list[sntrph->personlist.person_count - 1] == NULL)
        {
            strcpy(personid, "P000");
        }
        else
        {
            strcpy(personid, sntrph->personlist.person_list[sntrph->personlist.person_count - 1]->person_id);
            increment_id(personid);
        }

        char criminal_record[500] = ""; // Empty criminal record
        char DOB[20];
        random_date(DOB, sizeof(DOB));     // Empty date of birth
        char relationship_status[50] = ""; // Empty relationship status
        char status[50] = "ALIVE";         // Default status

        Person *person = create_person(first_name, last_name, DOB, criminal_record, relationship_status, sntrph);
        Address *address = create_address(street, city, zip_code, person, sntrph);
        printf("Created person: %s %s with DOB: %s\n", first_name, last_name, DOB);
    }
}

// HELPER FUNCTION
void seed_evidence(int count, SNTRPH *sntrph)
{
    printf("[DEBUG] Seeding %d evidence items...\n", count);

    const char *evidence_names[] = {"Blood Sample", "Fingerprint", "Weapon", "Note", "Clothing", "Phone", "Photograph", "Hair Strand"};
    const char *types[] = {"Biological", "Physical", "Digital", "Document"};
    const char *statuses[] = {"COLLECTED", "ANALYZED", "IN STORAGE", "RELEASED"};

    srand(time(NULL));

    for (int i = 0; i < count && sntrph->evidenceArchive.total_items < MAX_ADDRESSES; i++)
    {
        printf("[DEBUG] Creating evidence item %d...\n", i + 1);

        Evidence *ev = malloc(sizeof(Evidence));
        if (!ev)
        {
            printf("[ERROR] Memory allocation failed for Evidence %d\n", i + 1);
            continue;
        }

        snprintf(ev->evidence_id, sizeof(ev->evidence_id), "EV%03d", sntrph->evidenceArchive.total_items);
        strcpy(ev->name, evidence_names[rand() % (sizeof(evidence_names) / sizeof(char *))]);
        strcpy(ev->type, types[rand() % (sizeof(types) / sizeof(char *))]);
        strcpy(ev->status, statuses[rand() % (sizeof(statuses) / sizeof(char *))]);
        snprintf(ev->description, sizeof(ev->description), "Description for evidence %s", ev->name);
        snprintf(ev->file_path, sizeof(ev->file_path), "/evidence/%s.txt", ev->evidence_id);
        random_date(ev->date, sizeof(ev->date));
        ev->is_restricted = rand() % 2;
        strcpy(ev->notes, "Initial entry.");

        printf("[DEBUG] Evidence ID: %s\n", ev->evidence_id);
        printf("[DEBUG] Evidence name: %s | Type: %s | Status: %s\n", ev->name, ev->type, ev->status);
        printf("[DEBUG] Assigning custody chain...\n");

        int random = random_int(1, 5);
        for (int j = 0; j < random; j++)
        {
            if (sntrph->officerList.officer_count > 0)
            {
                Officer *officer = sntrph->officerList.officers[rand() % sntrph->officerList.officer_count];
                strncpy(ev->custody_chain[j], officer->officer_id, sizeof(ev->custody_chain[j]) - 1);
                ev->custody_chain[j][sizeof(ev->custody_chain[j]) - 1] = '\0';
                printf("[DEBUG] Custody %d: %s\n", j, ev->custody_chain[j]);
            }
            else
            {
                strcpy(ev->custody_chain[j], "N/A");
                printf("[WARN] No officers available for custody chain. Entry set to 'N/A'\n");
            }
        }

        sntrph->evidenceArchive.evidence_items[sntrph->evidenceArchive.total_items] = ev;
        sntrph->evidenceArchive.total_items++;

        printf("[DEBUG] Evidence item %s added to archive. Total: %d\n", ev->evidence_id, sntrph->evidenceArchive.total_items);
    }

    printf("[DEBUG] Evidence seeding completed. Total items: %d\n", sntrph->evidenceArchive.total_items);
}

// HELPER FUNCTION
void seed_cases(int count, SNTRPH *sntrph)
{
    const char *types[] = {
        "Homicide",
        "Burglary",
        "Missing Person",
        "Assault",
        "Fraud",
        "Kidnapping",
        "Arson",
        "Drug Offense",
        "Domestic Violence",
        "Robbery",
        "Weapon Possession",
        "Organized Crime",
        "Police Misconduct"};

    char case_no[20];
    srand(time(NULL));

    for (int i = 0; i < count && sntrph->caseList.case_count < MAX_ADDRESSES; i++)
    {
        snprintf(case_no, sizeof(case_no), "C%03d", sntrph->caseList.case_count + 1);

        CaseFile *casefile = malloc(sizeof(CaseFile));
        strcpy(casefile->case_no, case_no);
        snprintf(casefile->name, sizeof(casefile->name), "Case %d", i + 1);
        random_date(casefile->date, sizeof(casefile->date));
        Officer *officer = sntrph->officerList.officers[rand() % sntrph->officerList.officer_count];
        strcpy(casefile->lead, officer->b_no);

        // Random address
        casefile->location = sntrph->addressList.addresses[rand() % sntrph->addressList.address_count];

        strcpy(casefile->summary, "Generated case file for testing.");
        strcpy(casefile->type, types[rand() % 5]);

        // Add victims
        casefile->victim_count = rand() % 3 + 1;
        for (int j = 0; j < casefile->victim_count; j++)
        {
            casefile->victims[j] = sntrph->personlist.person_list[rand() % sntrph->personlist.person_count];
        }

        // Add suspects
        casefile->suspect_count = rand() % 3;
        for (int j = 0; j < casefile->suspect_count; j++)
        {
            casefile->suspects[j] = sntrph->personlist.person_list[rand() % sntrph->personlist.person_count];
        }

        // Add evidence
        casefile->evidence_count = rand() % 5 + 1;
        for (int j = 0; j < casefile->evidence_count; j++)
        {
            if (sntrph->evidenceArchive.total_items == 0)
                break;
            casefile->evidence[j] = sntrph->evidenceArchive.evidence_items[rand() % sntrph->evidenceArchive.total_items];
            strcpy(casefile->evidence[j]->case_id, case_no);
        }

        strcpy(casefile->evidence_notes, "Evidence reviewed.");
        casefile->is_locked = rand() % 2;
        strcpy(casefile->status, "OPEN");

        sntrph->caseList.cases[sntrph->caseList.case_count++] = casefile;
        printf("Seeded case: %s (%s)\n", casefile->case_no, casefile->type);
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
    else if (strcmp(token, "HAIL") == 0)
    {
        char *token_2 = strtok(NULL, delimiters);
        if (strcmp(token_2, "MESSAGE") == 0)
        {
            if (sntrph->current_user->clearance < 3)
            {
                printf("!! ACCESS DENEID: INSUFFICIENT CLEARANCE !!");
                printf("!! ATTEMPT LOGGED !!");
                return;
            }

            hailGabriel(sntrph);
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
            size_t len = strlen(token_3);
            if (len == 5)
            {
                Evidence *ev = checkEvidence(sntrph, token_3);
                printEvidence(ev);
                return;
            }
            viewEvidence(sntrph);
        }
        else if (strcmp(token_2, "ADDRESSES") == 0)
        {
            viewAddresses(sntrph);
        }
        else if (strcmp(token_2, "OFFICERS") == 0)
        {
            if (sntrph->current_user->clearance < 3)
            {
                PrintLine("ACCESS DENIED: INSUFFICIENT CLEARANCE\n");
                PrintLine("!! ATTEMPT FOLLOWED IN AUDIT TRAIL !!");

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
        else if (strcmp(token_2, "PERSON") == 0)
        {
            char *token_3 = strtok(NULL, delimiters);
            Person *person = check_person(sntrph, token_3);
            if (person != NULL)
            {
                viewPerson(person);
            }
            else
            {
                PrintLine("Person not found.");
            }
        }
        else if (strcmp(token_2, "PEOPLE") == 0)
        {
            viewPeople(sntrph);
        }
        else if (strcmp(token_2, "CASE") == 0)
        {
            char *token_3 = strtok(NULL, delimiters);
            viewCase(sntrph, token_3);
        }
        else if (strcmp(token_2, "CASES") == 0)
        {
            viewCases(sntrph);
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
        else if (strcmp(token_2, "PERSON") == 0)
        {
            addPerson(sntrph);
        }
        else if (strcmp(token_2, "CASE") == 0)
        {
            addCase(sntrph);
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
        else if (strcmp(token_2, "PERSON") == 0)
        {
            char *token_3 = strtok(NULL, delimiters);
            deletePerson(sntrph, token_3);
        }
        else if (strcmp(token_2, "ADDRESS") == 0)
        {
            char *token_3 = strtok(NULL, delimiters);
            deleteAddress(sntrph, token_3);
        }
        else if (strcmp(token_2, "OFFICER") == 0)
        {
            char *token_3 = strtok(NULL, delimiters);
            deleteOfficer(sntrph, token_3);
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
    else if (strcmp(token, "ALTER") == 0)
    {
        char *token_2 = strtok(NULL, delimiters);
        if (strcmp(token_2, "OFFICER") == 0)
        {
            char *token_3 = strtok(NULL, delimiters);
            editOfficer(sntrph, token_3);
        }
        else if (strcmp(token_2, "AUDIT") == 0)
        {
            char *token_3 = strtok(NULL, delimiters);
            editLog(sntrph, token_3);
        }
        else if (strcmp(token_2, "PERSON") == 0)
        {
            char *token_3 = strtok(NULL, delimiters);
            editPerson(sntrph, token_3);
        }
        else if (strcmp(token_2, "ADDRESS") == 0)
        {
            char *token_3 = strtok(NULL, delimiters);
            editAddress(sntrph, token_3);
        }
    }
    else if (strcmp(token, "LINK") == 0)
    {
        char *token_2 = strtok(NULL, delimiters);
        if (strcmp(token_2, "ADDRESS") == 0)
        {
            char *token_3 = strtok(NULL, delimiters);
            char *token_4 = strtok(NULL, delimiters);

            linkAddress(sntrph, token_3, token_4);
        }
    }
    else if (strcmp(token, "SEARCH") == 0)
    {
        char *token_2 = strtok(NULL, delimiters);
        if (strcmp(token_2, "PEOPLE") == 0)
        {
            char *token_3 = strtok(NULL, delimiters);
            searchPerson(sntrph, token_3);
        }
        if (strcmp(token_2, "OFFICERS") == 0)
        {
            char *token_3 = strtok(NULL, delimiters);
            searchOfficer(sntrph, token_3);
        }
        if (strcmp(token_2, "EVIDENCE") == 0)
        {
            char *token_3 = strtok(NULL, delimiters);
            searchEvidence(sntrph, token_3);
        }
        if (strcmp(token_2, "CASES") == 0)
        {
            char *token_3 = strtok(NULL, delimiters);
            searchCase(sntrph, token_3);
        }
    }
    else if (strcmp(token, "FILTER") == 0)
    {
        char *token_2 = strtok(NULL, delimiters);
        if (token_2 != NULL && strcmp(token_2, "PEOPLE") == 0)
        {
            // The rest of input after "FILTER PEOPLE"
            char *rest = strtok(NULL, "\n"); // strtok continues from last position
            if (rest != NULL)
            {
                while (*rest == ' ')
                    rest++; // skip leading spaces
                processFilterPeople(sntrph, rest);
                return;
            }
        }
        else if (token_2 != NULL && strcmp(token_2, "OFFICERS") == 0)
        {
            char *rest = strchr(input, ' '); // find first space after "FILTER OFFICERS"
            if (rest != NULL)
            {
                rest = strchr(rest + 1, ' '); // skip to after "OFFICERS"
            }

            if (rest != NULL)
            {
                while (*rest == ' ')
                    rest++;
                processFilterOfficer(sntrph, rest);
                return;
            }
        }
        else if (token_2 != NULL && strcmp(token_2, "CASES") == 0)
        {
            char *rest = strchr(input, ' '); // find first space after "FILTER OFFICERS"
            if (rest != NULL)
            {
                rest = strchr(rest + 1, ' '); // skip to after "OFFICERS"
            }

            if (rest != NULL)
            {
                while (*rest == ' ')
                    rest++;
                processFilterCases(sntrph, rest);
                return;
            }
        }
        else if (token_2 != NULL && strcmp(token_2, "ADDRESSES") == 0)
        {
            char *rest = strchr(input, ' '); // find first space after "FILTER OFFICERS"
            if (rest != NULL)
            {
                rest = strchr(rest + 1, ' '); // skip to after "OFFICERS"
            }

            if (rest != NULL)
            {
                while (*rest == ' ')
                    rest++;
                processFilterAdd(sntrph, rest);
                return;
            }
        }
          else if (token_2 != NULL && strcmp(token_2, "TRAIL") == 0)
        {
            char *rest = strchr(input, ' '); // find first space after "FILTER OFFICERS"
            if (rest != NULL)
            {
                rest = strchr(rest + 1, ' '); // skip to after "OFFICERS"
            }

            if (rest != NULL)
            {
                while (*rest == ' ')
                    rest++;
                processFilterLog(sntrph, rest);
                return;
            }
        }
        else
        {
            printf("No filter parameters provided.\n");
            return;
        }
    }
    else
    {
        printf("!! INVALID COMMAND!! : %s", input);
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

    seed_officers(60, sntrph);
    seed_people(60, sntrph);
    seed_evidence(80, sntrph);
    seed_cases(60, sntrph);
}
void free_SNTRPH(SNTRPH *system)
{
    // Free PersonList
    for (int i = 0; i < system->personlist.person_count; i++)
    {
        if (system->personlist.person_list[i])
        {
            free(system->personlist.person_list[i]);
        }
    }

    // Free AddressList
    for (int i = 0; i < system->addressList.address_count; i++)
    {
        if (system->addressList.addresses[i])
        {
            free(system->addressList.addresses[i]);
        }
    }

    // Free EvidenceArchive
    for (int i = 0; i < system->evidenceArchive.total_items; i++)
    {
        if (system->evidenceArchive.evidence_items[i])
        {
            free(system->evidenceArchive.evidence_items[i]);
        }
    }

    // Free ReportList
    for (int i = 0; i < system->reportList.report_count; i++)
    {
        if (system->reportList.reports[i])
        {
            free(system->reportList.reports[i]);
        }
    }

    // Free LogList
    for (int i = 0; i < system->logList.log_count; i++)
    {
        if (system->logList.logs[i])
        {
            free(system->logList.logs[i]);
        }
    }

    // Free OfficerList
    for (int i = 0; i < system->officerList.officer_count; i++)
    {
        Officer *officer = system->officerList.officers[i];
        if (officer)
        {
            // Free mailbox and its messages
            if (officer->mailbox)
            {
                for (int j = 0; j < officer->mailbox->message_count; j++)
                {
                    if (officer->mailbox->messages[j])
                    {
                        free(officer->mailbox->messages[j]);
                    }
                }
                free(officer->mailbox);
            }

            // Free disciplinary actions
            for (int j = 0; j < 3; j++)
            {
                if (officer->disciplinary_actions[j])
                {
                    free(officer->disciplinary_actions[j]);
                }
            }

            free(officer);
        }
    }

    for (int i = 0; i < system->caseList.case_count; i++)
    {
        CaseFile *c = system->caseList.cases[i];
        if (c == NULL)
            continue;

        // Free location
        if (c->location != NULL)
        {
            free(c->location);
            c->location = NULL;
        }

        // Free victims
        for (int v = 0; v < c->victim_count; v++)
        {
            if (c->victims[v] != NULL)
            {
                free(c->victims[v]);
                c->victims[v] = NULL;
            }
        }

        // Free suspects
        for (int s = 0; s < c->suspect_count; s++)
        {
            if (c->suspects[s] != NULL)
            {
                free(c->suspects[s]);
                c->suspects[s] = NULL;
            }
        }

        // Free evidence
        for (int e = 0; e < c->evidence_count; e++)
        {
            if (c->evidence[e] != NULL)
            {
                free(c->evidence[e]);
                c->evidence[e] = NULL;
            }
        }

        // Finally, free the CaseFile struct itself
        free(c);
        system->caseList.cases[i] = NULL;
    }

    system->caseList.case_count = 0;

    // If SNTRPH *system itself was malloc’d and you want to free it too:
    // free(system);
}
void initSNTRPH(SNTRPH *sntrph)
{
    if (!sntrph)
        return;

    // Initialize person list
    sntrph->personlist.person_count = 0;
    for (int i = 0; i < MAX_ADDRESSES; i++)
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
    for (int i = 0; i < MAX_ADDRESSES; i++)
    {
        sntrph->evidenceArchive.evidence_items[i] = NULL;
    }

    // Initialize report list
    sntrph->reportList.report_count = 0;
    for (int i = 0; i < MAX_ADDRESSES; i++)
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

    sntrph->caseList.case_count = 0;
    sntrph->caseList.archive_count = 0;
    sntrph->caseList.deleted_count = 0;
    for (int i = 0; i < MAX_ADDRESSES; i++)
    {
        sntrph->caseList.cases[i] = NULL;
        sntrph->caseList.archive[i] = NULL;
        sntrph->caseList.deleted[i] = NULL;
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

    char input[80] = "";
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
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n')
            input[len - 1] = '\0';

        // If input is too long, clear stdin buffer
        if (len == sizeof(input) - 1 && input[len - 1] != '\n')
        {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF)
                ;
        }
        if (strcmp(input, "quit") == 0)
        {
            free_SNTRPH(&sntrph);
            break;
        }
        printf("Debug: Value: %s", input);
        processCommand(&sntrph, input);
    }

    return 0;
}
