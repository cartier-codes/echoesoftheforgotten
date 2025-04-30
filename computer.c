#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include "headers/struct.h"

#define MAX_NAME_LENGTH 50
#define MAX_ADDRESS_LENGTH 100
#define MAX_ARRESTS 10
#define MAX_PERSONS 100 // Adjust as needed
#define MAX_ADDRESSES 50
#define MAX_CF 50
#define MAX_AR 50

typedef struct SNTRPH SNTRPH;
typedef struct Address Address;
typedef struct Person Person;
typedef struct ModificationLog ModificationLog;
typedef struct AdoptionReport AdoptionReport;

struct Address
{
    char address_id[10];
    char street[MAX_ADDRESS_LENGTH];
    char city[MAX_ADDRESS_LENGTH];
    char case_no[20][20];
    char person_list[20][20];
    int person_count;

};

struct Person
{
    char person_id[10];
    char first_name[MAX_NAME_LENGTH];
    char last_name[MAX_NAME_LENGTH];
    char address_id[40];
    char criminal_record[500];
};

struct ModificationLog{
    char timestamp[20];
    char detective_name[20];
    char case_no[20];
    char modification[300];
};

struct AdoptionReport{
    char report_id[20];
    char child_name[50];
    char adoptive_parents[2][50];
    char original_parents[2][50];
    char adoption_notes[300];
};

struct SNTRPH{
    Address address_lst[MAX_ADDRESS_LENGTH];
    Person person_lst[MAX_PERSONS];
    ModificationLog ml_lst[MAX_CF];
    AdoptionReport ar_lst[MAX_AR];
    CaseFile cf_lst[MAX_CF];
    int address_count;
    int person_count;
    int ml_count;
    int ar_count;
    int cf_count;
};
void printPerson(const struct Person *p) {
    printf("------------------------------------------------------------\n");
    printf("| %-15s: %-40s |\n", "Person ID", p->person_id);
    printf("| %-15s: %-40s |\n", "First Name", p->first_name);
    printf("| %-15s: %-40s |\n", "Last Name", p->last_name);
    printf("| %-15s: %-40s |\n", "Address ID", p->address_id);
    printf("------------------------------------------------------------\n");
    printf("| %-15s: %-40s |\n", "Criminal Record", "");

    // Pretty print the criminal record in wrapped lines (optional)
    int width = 56;
    const char *text = p->criminal_record;
    while (*text) {
        printf("| %-15s  ", "");  // Empty label space for wrapped lines
        for (int i = 0; i < width && *text; ++i) {
            putchar(*text++);
        }
        printf("%*s|\n", (int)(width - (text - (p->criminal_record))), ""); // padding
    }

    printf("------------------------------------------------------------\n");
}
void printAddress(const struct Address *a) {
    printf("------------------------------------------------------------\n");
    printf("| %-15s: %-40s |\n", "Address ID", a->address_id);
    printf("| %-15s: %-40s |\n", "Street", a->street);
    printf("| %-15s: %-40s |\n", "City", a->city);
    printf("------------------------------------------------------------\n");

    printf("| %-15s: %-40s |\n", "Case Numbers", "");
    for (int i = 0; i < 20 && a->case_no[i][0] != '\0'; ++i) {
        printf("| %-15s  %-40s |\n", "", a->case_no[i]);
    }

    printf("------------------------------------------------------------\n");

    printf("| %-15s: %-40s |\n", "Person List", "");
    for (int i = 0; i < 20 && a->person_list[i][0] != '\0'; ++i) {
        printf("| %-15s  %-40s |\n", "", a->person_list[i]);
    }

    printf("------------------------------------------------------------\n");
}
void printAdoptionReport(struct AdoptionReport *ar) {
    printf("\n==================================================================================================================================================================");
    printf("\n\nAdoption Report: %s", ar->report_id);  // Report ID
    printf("\nChild Name: %s\n\n", ar->child_name); // Child's name
    printf("Adoptive Parents: %s, %s\n", ar->adoptive_parents[0], ar->adoptive_parents[1]); // Adoptive parents' names
    printf("Original Parents: %s, %s\n", ar->original_parents[0], ar->original_parents[1]); // Original parents' names
    printf("Adoption Notes: %s\n\n", ar->adoption_notes); // Adoption notes
    printf("==================================================================================================================================================================\n\n");
}
void printModificationLog(struct ModificationLog *log) {
    printf("\n==================================================================================================================================================================");
    printf("\n\nModification Log - Case No: %s", log->case_no);  // Case number
    printf("\nTimestamp: %s\n", log->timestamp); // Timestamp of the modification
    printf("Detective: %s\n", log->detective_name); // Detective's name
    printf("Modification Details: %s\n\n", log->modification); // Details of the modification
    printf("==================================================================================================================================================================\n\n");
}

void printcf(struct CaseFile *cf){
            printf("\n==================================================================================================================================================================");
            printf("\n\nCase File: %s",cf->name);
            printf("\nCase ID: %s\n\n", cf->case_no);
            printf("Lead: %s\n", cf->lead);
            printf("%s\n", cf->location);
            printf("Summary: %s\n\n", cf->summary);
            printf("%s\n", cf->victims);
            printf("%s\n\n", cf->evidence);
            printf("%s\n\n", cf->suspects); // Corrected
            printf("==================================================================================================================================================================\n\n");
}
void populateDB(SNTRPH *sntrph) {
    // Initialize counts
    sntrph->address_count = 0;
    sntrph->person_count = 0;
    sntrph->ml_count = 0;
    sntrph->ar_count = 0;
    sntrph->cf_count = 0;

    // Populate Addresses
    strcpy(sntrph->address_lst[sntrph->address_count].address_id, "A001");
    strcpy(sntrph->address_lst[sntrph->address_count].street, "123 MAIN ST");
    strcpy(sntrph->address_lst[sntrph->address_count].city, "NEW YORK");
    strcpy(sntrph->address_lst[sntrph->address_count].case_no[0], "C001");
    strcpy(sntrph->address_lst[sntrph->address_count].person_list[0], "P001");
    sntrph->address_lst[sntrph->address_count].person_count = 1;
    sntrph->address_count++;

    strcpy(sntrph->address_lst[sntrph->address_count].address_id, "A002");
    strcpy(sntrph->address_lst[sntrph->address_count].street, "456 ELM AVE");
    strcpy(sntrph->address_lst[sntrph->address_count].city, "BROOKLYN");
    strcpy(sntrph->address_lst[sntrph->address_count].case_no[0], "C002");
    strcpy(sntrph->address_lst[sntrph->address_count].person_list[0], "P002");
    sntrph->address_lst[sntrph->address_count].person_count = 1;
    sntrph->address_count++;

    strcpy(sntrph->address_lst[sntrph->address_count].address_id, "A003");
    strcpy(sntrph->address_lst[sntrph->address_count].street, "789 OAK LN");
    strcpy(sntrph->address_lst[sntrph->address_count].city, "QUEENS");
    strcpy(sntrph->address_lst[sntrph->address_count].case_no[0], "C003");
    strcpy(sntrph->address_lst[sntrph->address_count].person_list[0], "P003");
    sntrph->address_lst[sntrph->address_count].person_count = 1;
    sntrph->address_count++;

    strcpy(sntrph->address_lst[sntrph->address_count].address_id, "A004");
    strcpy(sntrph->address_lst[sntrph->address_count].street, "101 PINE RD");
    strcpy(sntrph->address_lst[sntrph->address_count].city, "BRONX");
    strcpy(sntrph->address_lst[sntrph->address_count].case_no[0], "C004");
    strcpy(sntrph->address_lst[sntrph->address_count].person_list[0], "P004");
    sntrph->address_lst[sntrph->address_count].person_count = 1;
    sntrph->address_count++;

    strcpy(sntrph->address_lst[sntrph->address_count].address_id, "A005");
    strcpy(sntrph->address_lst[sntrph->address_count].street, "222 WILLOW DR");
    strcpy(sntrph->address_lst[sntrph->address_count].city, "STATEN ISLAND");
    strcpy(sntrph->address_lst[sntrph->address_count].case_no[0], "C005");
    strcpy(sntrph->address_lst[sntrph->address_count].person_list[0], "P005");
    sntrph->address_lst[sntrph->address_count].person_count = 1;
    sntrph->address_count++;

    // Populate Persons
    strcpy(sntrph->person_lst[sntrph->person_count].person_id, "P001");
    strcpy(sntrph->person_lst[sntrph->person_count].first_name, "ALFRED");
    strcpy(sntrph->person_lst[sntrph->person_count].last_name, "ZIMMERMAN");
    strcpy(sntrph->person_lst[sntrph->person_count].address_id, "NO KNOWN ADDRESS, SEEN TO RESIDE IN RNP");
    strcpy(sntrph->person_lst[sntrph->person_count].criminal_record, "(SUSPECT IN MISSING PERSON CASE CASE ID: C037992.)");
    sntrph->person_count++;

    strcpy(sntrph->person_lst[sntrph->person_count].person_id, "P002");
    strcpy(sntrph->person_lst[sntrph->person_count].first_name, "JANE");
    strcpy(sntrph->person_lst[sntrph->person_count].last_name, "SMITH");
    strcpy(sntrph->person_lst[sntrph->person_count].address_id, "A002");
    strcpy(sntrph->person_lst[sntrph->person_count].criminal_record, "NONE.");
    sntrph->person_count++;

    strcpy(sntrph->person_lst[sntrph->person_count].person_id, "P003");
    strcpy(sntrph->person_lst[sntrph->person_count].first_name, "ROBERT");
    strcpy(sntrph->person_lst[sntrph->person_count].last_name, "JONES");
    strcpy(sntrph->person_lst[sntrph->person_count].address_id, "A003");
    strcpy(sntrph->person_lst[sntrph->person_count].criminal_record, "ONE PRIOR MISDEMEANOR.");
    sntrph->person_count++;

    strcpy(sntrph->person_lst[sntrph->person_count].person_id, "P004");
    strcpy(sntrph->person_lst[sntrph->person_count].first_name, "ALICE");
    strcpy(sntrph->person_lst[sntrph->person_count].last_name, "BROWN");
    strcpy(sntrph->person_lst[sntrph->person_count].address_id, "A004");
    strcpy(sntrph->person_lst[sntrph->person_count].criminal_record, "CLEAN RECORD.");
    sntrph->person_count++;

    strcpy(sntrph->person_lst[sntrph->person_count].person_id, "P005");
    strcpy(sntrph->person_lst[sntrph->person_count].first_name, "MICHAEL");
    strcpy(sntrph->person_lst[sntrph->person_count].last_name, "WILSON");
    strcpy(sntrph->person_lst[sntrph->person_count].address_id, "A005");
    strcpy(sntrph->person_lst[sntrph->person_count].criminal_record, "SEVERAL PARKING TICKETS.");
    sntrph->person_count++;

    // Populate Modification Logs
    strcpy(sntrph->ml_lst[sntrph->ml_count].timestamp, "2025-04-28 10:00:00");
    strcpy(sntrph->ml_lst[sntrph->ml_count].detective_name, "DETECTIVE JOHNSON");
    strcpy(sntrph->ml_lst[sntrph->ml_count].case_no, "C001");
    strcpy(sntrph->ml_lst[sntrph->ml_count].modification, "ADDED SUSPECT JOHN DOE TO CASE C001.");
    sntrph->ml_count++;

    strcpy(sntrph->ml_lst[sntrph->ml_count].timestamp, "2025-04-29 12:00:00");
    strcpy(sntrph->ml_lst[sntrph->ml_count].detective_name, "DETECTIVE JOHNSON");
    strcpy(sntrph->ml_lst[sntrph->ml_count].case_no, "C001");
    strcpy(sntrph->ml_lst[sntrph->ml_count].modification, "ADDED SUSPECT JANE SMITH TO CASE C001.");
    sntrph->ml_count++;


    strcpy(sntrph->ml_lst[sntrph->ml_count].timestamp, "2025-04-28 10:30:00");
    strcpy(sntrph->ml_lst[sntrph->ml_count].detective_name, "DETECTIVE WILLIAMS");
    strcpy(sntrph->ml_lst[sntrph->ml_count].case_no, "C002");
    strcpy(sntrph->ml_lst[sntrph->ml_count].modification, "UPDATED ADDRESS FOR JANE SMITH.");
    sntrph->ml_count++;

    strcpy(sntrph->ml_lst[sntrph->ml_count].timestamp, "2025-04-28 11:00:00");
    strcpy(sntrph->ml_lst[sntrph->ml_count].detective_name, "DETECTIVE BROWN");
    strcpy(sntrph->ml_lst[sntrph->ml_count].case_no, "C003");
    strcpy(sntrph->ml_lst[sntrph->ml_count].modification, "REVIEWED WITNESS STATEMENTS FOR CASE C003.");
    sntrph->ml_count++;

    strcpy(sntrph->ml_lst[sntrph->ml_count].timestamp, "2025-04-28 11:30:00");
    strcpy(sntrph->ml_lst[sntrph->ml_count].detective_name, "DETECTIVE GREEN");
    strcpy(sntrph->ml_lst[sntrph->ml_count].case_no, "C004");
    strcpy(sntrph->ml_lst[sntrph->ml_count].modification, "COLLECTED FORENSIC EVIDENCE AT THE SCENE OF C004.");
    sntrph->ml_count++;

    strcpy(sntrph->ml_lst[sntrph->ml_count].timestamp, "2025-04-28 12:00:00");
    strcpy(sntrph->ml_lst[sntrph->ml_count].detective_name, "DETECTIVE BLACK");
    strcpy(sntrph->ml_lst[sntrph->ml_count].case_no, "C005");
    strcpy(sntrph->ml_lst[sntrph->ml_count].modification, "ISSUED AN ARREST WARRANT FOR A SUSPECT IN CASE C005.");
    sntrph->ml_count++;

    // Populate Adoption Reports
    strcpy(sntrph->ar_lst[sntrph->ar_count].report_id, "AR001");
    strcpy(sntrph->ar_lst[sntrph->ar_count].child_name, "ALICE BROWN");
    strcpy(sntrph->ar_lst[sntrph->ar_count].adoptive_parents[0], "ROBERT MILLER");
    strcpy(sntrph->ar_lst[sntrph->ar_count].adoptive_parents[1], "SUSAN MILLER");
    strcpy(sntrph->ar_lst[sntrph->ar_count].original_parents[0], "UNKNOWN");
    strcpy(sntrph->ar_lst[sntrph->ar_count].original_parents[1], "UNKNOWN");
    strcpy(sntrph->ar_lst[sntrph->ar_count].adoption_notes, "FINALIZED ADOPTION ON 2025-04-15.");
    sntrph->ar_count++;

    strcpy(sntrph->ar_lst[sntrph->ar_count].report_id, "AR002");
    strcpy(sntrph->ar_lst[sntrph->ar_count].child_name, "CHARLIE DAVIS");
    strcpy(sntrph->ar_lst[sntrph->ar_count].adoptive_parents[0], "DAVID WILSON");
    strcpy(sntrph->ar_lst[sntrph->ar_count].adoptive_parents[1], "LINDA WILSON");
    strcpy(sntrph->ar_lst[sntrph->ar_count].original_parents[0], "SARAH GREEN");
    strcpy(sntrph->ar_lst[sntrph->ar_count].original_parents[1], "MICHAEL GREEN");
    strcpy(sntrph->ar_lst[sntrph->ar_count].adoption_notes, "OPEN ADOPTION WITH REGULAR VISITS FROM ORIGINAL PARENTS.");
    sntrph->ar_count++;

    strcpy(sntrph->ar_lst[sntrph->ar_count].report_id, "AR003");
    strcpy(sntrph->ar_lst[sntrph->ar_count].child_name, "SOPHIA LEE");
    strcpy(sntrph->ar_lst[sntrph->ar_count].adoptive_parents[0], "JAMES BAKER");
    strcpy(sntrph->ar_lst[sntrph->ar_count].adoptive_parents[1], "EMILY BAKER");
    strcpy(sntrph->ar_lst[sntrph->ar_count].original_parents[0], "UNKNOWN");
    strcpy(sntrph->ar_lst[sntrph->ar_count].original_parents[1], "UNKNOWN");
    strcpy(sntrph->ar_lst[sntrph->ar_count].adoption_notes, "INTERNATIONAL ADOPTION, CHILD ARRIVED ON 2025-04-20.");
    sntrph->ar_count++;

    strcpy(sntrph->ar_lst[sntrph->ar_count].report_id, "AR004");
    strcpy(sntrph->ar_lst[sntrph->ar_count].child_name, "ETHAN MOORE");
    strcpy(sntrph->ar_lst[sntrph->ar_count].adoptive_parents[0], "KEVIN WHITE");
    strcpy(sntrph->ar_lst[sntrph->ar_count].adoptive_parents[1], "OLIVIA WHITE");
    strcpy(sntrph->ar_lst[sntrph->ar_count].original_parents[0], "DANIEL CLARK");
    strcpy(sntrph->ar_lst[sntrph->ar_count].original_parents[1], "JESSICA CLARK");
    strcpy(sntrph->ar_lst[sntrph->ar_count].adoption_notes, "POST-PLACEMENT SUPERVISION REQUIRED.");
    sntrph->ar_count++;

    strcpy(sntrph->ar_lst[sntrph->ar_count].report_id, "AR005");
    strcpy(sntrph->ar_lst[sntrph->ar_count].child_name, "OLIVIA HALL");
    strcpy(sntrph->ar_lst[sntrph->ar_count].adoptive_parents[0], "THOMAS LEWIS");
    strcpy(sntrph->ar_lst[sntrph->ar_count].adoptive_parents[1], "GRACE LEWIS");
    strcpy(sntrph->ar_lst[sntrph->ar_count].original_parents[0], "SAMUEL REED");
    strcpy(sntrph->ar_lst[sntrph->ar_count].original_parents[1], "VICTORIA REED");
    strcpy(sntrph->ar_lst[sntrph->ar_count].adoption_notes, "SIBLING GROUP ADOPTION.");
    sntrph->ar_count++;

    // Populate Case Files
    strcpy(sntrph->cf_lst[sntrph->cf_count].name, "THEFT AT FIRST NATIONAL BANK");
    strcpy(sntrph->cf_lst[sntrph->cf_count].case_no, "CF001");
    strcpy(sntrph->cf_lst[sntrph->cf_count].date, "2025-04-27");
    strcpy(sntrph->cf_lst[sntrph->cf_count].lead, "DETECTIVE DAVIES");
    strcpy(sntrph->cf_lst[sntrph->cf_count].location, "10 DOWNING STREET, NEW YORK");
    strcpy(sntrph->cf_lst[sntrph->cf_count].summary, "REPORT OF A DAYTIME ROBBERY INVOLVING A SIGNIFICANT AMOUNT OF CASH.");
    strcpy(sntrph->cf_lst[sntrph->cf_count].victims, "FIRST NATIONAL BANK EMPLOYEES AND CUSTOMERS PRESENT DURING THE INCIDENT.");
    strcpy(sntrph->cf_lst[sntrph->cf_count].evidence, "SECURITY CAMERA FOOTAGE, WITNESS TESTIMONIES, FINGERPRINTS FOUND AT THE SCENE.");
    strcpy(sntrph->cf_lst[sntrph->cf_count].suspects, "CURRENTLY UNKNOWN, INVESTIGATION ONGOING.");
    sntrph->cf_count++;

    strcpy(sntrph->cf_lst[sntrph->cf_count].name, "MISSING PERSON - EMILY CARTER");
    strcpy(sntrph->cf_lst[sntrph->cf_count].case_no, "CF002");
    strcpy(sntrph->cf_lst[sntrph->cf_count].date, "2025-04-26");
    strcpy(sntrph->cf_lst[sntrph->cf_count].lead, "DETECTIVE MILLER");
    strcpy(sntrph->cf_lst[sntrph->cf_count].location, "CENTRAL PARK AREA, NEW YORK");
    strcpy(sntrph->cf_lst[sntrph->cf_count].summary, "EMILY CARTER, A 15-YEAR-OLD FEMALE, WAS LAST SEEN WALKING HER DOG IN CENTRAL PARK.");
    strcpy(sntrph->cf_lst[sntrph->cf_count].victims, "EMILY CARTER.");
    strcpy(sntrph->cf_lst[sntrph->cf_count].evidence, "DESCRIPTION OF CLOTHING, LAST KNOWN LOCATION, WITNESS REPORTS OF A POSSIBLE INTERACTION.");
    strcpy(sntrph->cf_lst[sntrph->cf_count].suspects, "NONE IDENTIFIED YET.");
    sntrph->cf_count++;

    strcpy(sntrph->cf_lst[sntrph->cf_count].name, "THE CAMPSIDE KIDNAPPING");
    strcpy(sntrph->cf_lst[sntrph->cf_count].case_no, "C037992");
    strcpy(sntrph->cf_lst[sntrph->cf_count].date, "23/03/1978");
    strcpy(sntrph->cf_lst[sntrph->cf_count].lead, "DETECTIVE CHRIS LEVINS (CIRCA 1993: MAX KRUGER.)");
    strcpy(sntrph->cf_lst[sntrph->cf_count].location, "RIDGEWOOD");
    strcpy(sntrph->cf_lst[sntrph->cf_count].summary, "IN THE LATE NIGHT OF MARCH 23, 1968, MICHAEL O'CONNELL, A FOUR YEAR OLD TODDLER, WAS REPORTED MISSING BY HIS PARENTS. MICHAEL WAS LAST SEEN IN HIS PARENTS' TENT ASLEEP IN RIDGEWOOD NATIONAL PARK, WHICH IS KNOWN FOR ITS CLEAR TRAILS AND BEAUTIFUL SCENERY. ACCORDING TO HIS FAMILY, THEY HAVE NO RECOLLECTION OF ANY PEOPLE OF INTEREST. THEY DO NOT REMEMBER THE DAY PRIOR. THEY WOKE THE NEXT MORNING, AND MICHAEL WAS NOWHERE TO BE FOUND.");
    strcpy(sntrph->cf_lst[sntrph->cf_count].victims, "VICTIM INFORMATION:\nNAME: MICHAEL O'CONNELL\nAGE: 4\nOCCUPATION: N/A\nPHYSICAL DESCRIPTION:\n\tHEIGHT: 3\"4\n\tWEIGHT: 40LBS\n\tHAIR: BROWN, BUZZED\n\tEYES: GREEN\n\tDISTINGUISHING FEATURES:\n\t\tSCAR THROUGH HIS RIGHT EYEBROW\nLAST KNOWN WHEREABOUTS:\n\tLOCATION: 200M NORTH WEST OF RIDGEWOOD NATIONAL PARK'S MAIN ENTRANCE\n\tTIME: 2200, 23/03/1978");
    strcpy(sntrph->cf_lst[sntrph->cf_count].evidence, "EVIDENCE:\n\tLARGE ADULT SIZED FOOTPRINTS FOUND LEADING FROM THE TENT TOWARD THE REAR ENTRANCE.\n\tSUSPICIOUS ACTIVITY:\n\t\tA FISHERMAN, IDENTIFIED AS ALFRED ZIMMERMAN, WAS SEEN LINGERING AROUND THE CAMPSITE EARLIER THAT EVENING.\n\t\tZIMMERMAN WAS DETAINED ON CHILD ABDUCTION CHARGES, BUT RELEASED UNDER INSUFFICIENT EVIDENCE.\n\t\tWITNESSES REPORTED ZIMMERMAN STARING AT THE CHILDREN FROM ACROSS THE LAKE, ACTING \"FEVERISH\"\n\tSEARCH EFFORTS:\n\t\tSEARCH TEAMS COMBED THROUGH THE PARK OVER THE SPAN OF 3 DAYS.\n\t\tNO PHYSICAL EVIDENCE OF MICHAEL WAS FOUND, NO SIGNS OF STRUGGLE OR FOUL PLAY AT THE SITE.");
    strcpy(sntrph->cf_lst[sntrph->cf_count].suspects, "PRIMARY SUSPECT:\n\tFORMER FELON- ALFRED ZIMMERMAN");
    sntrph->cf_count++;
    strcpy(sntrph->cf_lst[sntrph->cf_count].name, "BURGLARY AT RESIDENTIAL BUILDING");
    strcpy(sntrph->cf_lst[sntrph->cf_count].case_no, "CF003");
    strcpy(sntrph->cf_lst[sntrph->cf_count].date, "2025-04-25");
    strcpy(sntrph->cf_lst[sntrph->cf_count].lead, "DETECTIVE TAYLOR");
    strcpy(sntrph->cf_lst[sntrph->cf_count].location, "25 PINE STREET, APARTMENT 3B, NEW YORK");
    strcpy(sntrph->cf_lst[sntrph->cf_count].summary, "REPORT OF FORCED ENTRY AND THEFT OF VALUABLES FROM A RESIDENTIAL APARTMENT.");
    strcpy(sntrph->cf_lst[sntrph->cf_count].victims, "RESIDENT OF APARTMENT 3B.");
    strcpy(sntrph->cf_lst[sntrph->cf_count].evidence, "PRY MARKS ON THE DOOR, FOOTPRINTS NEAR THE WINDOW, POSSIBLE GLOVE FRAGMENT.");
    strcpy(sntrph->cf_lst[sntrph->cf_count].suspects, "UNKNOWN INDIVIDUAL(S).");
    sntrph->cf_count++;

    strcpy(sntrph->cf_lst[sntrph->cf_count].name, "ASSAULT IN TIMES SQUARE");
    strcpy(sntrph->cf_lst[sntrph->cf_count].case_no, "CF004");
    strcpy(sntrph->cf_lst[sntrph->cf_count].date, "2025-04-24");
    strcpy(sntrph->cf_lst[sntrph->cf_count].lead, "DETECTIVE HARRIS");
    strcpy(sntrph->cf_lst[sntrph->cf_count].location, "TIMES SQUARE, NEW YORK");
    strcpy(sntrph->cf_lst[sntrph->cf_count].summary, "REPORT OF A PHYSICAL ALTERCATION RESULTING IN INJURIES TO THE VICTIM.");
    strcpy(sntrph->cf_lst[sntrph->cf_count].victims, "ADULT MALE, SUFFERED MINOR INJURIES.");
    strcpy(sntrph->cf_lst[sntrph->cf_count].evidence, "WITNESS STATEMENTS, SECURITY CAMERA FOOTAGE FROM NEARBY BUSINESSES.");
    strcpy(sntrph->cf_lst[sntrph->cf_count].suspects, "DESCRIPTION PROVIDED BY WITNESSES.");
    sntrph->cf_count++;

    strcpy(sntrph->cf_lst[sntrph->cf_count].name, "VANDALISM AT ART GALLERY");
    strcpy(sntrph->cf_lst[sntrph->cf_count].case_no, "CF005");
    strcpy(sntrph->cf_lst[sntrph->cf_count].date, "2025-04-23");
    strcpy(sntrph->cf_lst[sntrph->cf_count].lead, "DETECTIVE CLARK");
    strcpy(sntrph->cf_lst[sntrph->cf_count].location, "12 GALLERY LANE, NEW YORK");
    strcpy(sntrph->cf_lst[sntrph->cf_count].summary, "REPORT OF DAMAGE TO ART EXHIBITS WITHIN A GALLERY.");
    strcpy(sntrph->cf_lst[sntrph->cf_count].victims, "OWNER OF THE ART GALLERY.");
    strcpy(sntrph->cf_lst[sntrph->cf_count].evidence, "PAINT SPATTER EVIDENCE, SECURITY CAMERA FOOTAGE OF THE INCIDENT.");
    strcpy(sntrph->cf_lst[sntrph->cf_count].suspects, "POSSIBLE FORMER PATRON WITH A GRUDGE.");
    sntrph->cf_count++;
}

void startMenu(SNTRPH *sntrph);

void startMenu(SNTRPH *sntrph){
    int choice;
    printf("\n===========NYPD COMPUTER==========\n");
}

void chooseLogs(struct ModificationLog logs[], int count){
    for(int i = 0; i < count; i++){
        printf("%d.) CASE: %s, MODIFICATION: %s\n", i+1, logs[i].case_no, logs[i].modification);
    }
    int choice;
    printf("\n > Enter choice: ");
    scanf("%d", &choice);

    printModificationLog(&logs[choice-1]);
}

void searchCommand(SNTRPH *sntrph, char *input){
    printf("Debug: Search Input: %s\n", input);
    char writeableinput[50];
    strcpy(writeableinput, input);
    char *nexttoken = strtok(writeableinput, " ");
    if(strcmp(nexttoken, "PEOPLE") == 0){
        bool found = false;
        char *lasttoken = strtok(NULL, "");
        for(int i  = 0; i < sntrph->person_count; i++){
            char buffer[200];
            snprintf(buffer, sizeof(buffer), "%s %s", sntrph->person_lst[i].first_name, sntrph->person_lst[i].last_name);
            if(strcmp(lasttoken, buffer) == 0){
                printf("DEBUG: %s was found.\n", buffer);
                printPerson(&sntrph->person_lst[i]);
                 found = true;
                return;

            }else{
                if(strcmp(lasttoken, sntrph->person_lst[i].person_id) == 0){
                    printf("DEBUG: %s was found via ID:.\n", lasttoken);
                   printPerson(&sntrph->person_lst[i]);
                    found = true;
                   return;
                }
            }
        }
        if(!found){
        printf("This person (%s) is not in SAINT records. Please try again with a different field.\n", lasttoken);
        }
     }
    else if(strcmp(nexttoken, "CASE") == 0){
        bool found = false;
        char *lasttoken = strtok(NULL, "");
        for(int i  = 0; i < sntrph->cf_count; i++){
            if(strcmp(lasttoken, sntrph->cf_lst[i].case_no) == 0){
                printf("DEBUG: %s was found via ID:.\n", lasttoken);
                   printcf(&sntrph->cf_lst[i]);
                   found = true;
                   return;
                }
            else if(strcmp(lasttoken, sntrph->cf_lst[i].lead) == 0){
                    printf("DEBUG: %s was found via lead:.\n", lasttoken);
                   printcf(&sntrph->cf_lst[i]);
                   found = true;
                   return;
            }
            }
            if(!found){
            printf("This case (%s) is not in SAINT records. Please try again with a different field.\n", lasttoken);
            }
        }
    else if(strcmp(nexttoken, "ADDRESS") == 0){
        bool found = false;
        char *lasttoken = strtok(NULL, "");
        for(int i  = 0; i < sntrph->address_count; i++){
            if(strcmp(lasttoken, sntrph->address_lst[i].address_id) == 0){
                printf("DEBUG: %s was found via ID:.\n", lasttoken);
                   printAddress(&sntrph->address_lst[i]);
                   found = true;
                   return;
                }
            else if(lasttoken[0] == 'P'){
                for(int j = 0; j < sntrph->address_lst[i].person_count; j++){
                    if(strcmp(lasttoken, sntrph->address_lst[i].person_list[j]) == 0){
                        printf("DEBUG: Address was found via ID: %s.\n", lasttoken);
                        printAddress(&sntrph->address_lst[i]);
                        found = true;
                        return;
                    }
                }
            }
            }
            if(!found){
                printf("This address (%s) is not in SAINT records. Please try again with a different field.\n", lasttoken);
            }
        }
    else if(strcmp(nexttoken, "ADOPTION") == 0){
        bool found = false;
        char *lasttoken = strtok(NULL,"");
        for(int i = 0; i < sntrph->ar_count; i++){
            if(strcmp(lasttoken, sntrph->ar_lst[i].report_id) == 0){
                printf("DEBUG: Adoption was found via ID: %s.\n", lasttoken);
                printAdoptionReport(&sntrph->ar_lst[i]);
                found = true;
                return;
            }
            else if(strcmp(lasttoken, sntrph->ar_lst[i].child_name) == 0){
                printf("DEBUG: Adoption was found via child's name: %s.\n", lasttoken);
                printAdoptionReport(&sntrph->ar_lst[i]);
                found = true;
                return;
            }
            if (strcmp(lasttoken, sntrph->ar_lst[i].adoptive_parents[0]) == 0 ||
                strcmp(lasttoken, sntrph->ar_lst[i].adoptive_parents[1]) == 0) {
                printf("DEBUG: Adoption was found via adoptive parent: %s.\n", lasttoken);
                printAdoptionReport(&sntrph->ar_lst[i]);
                found = true;
                return;  // Exit after the first match
            }
                if (strcmp(lasttoken, sntrph->ar_lst[i].original_parents[0]) == 0 ||
                    strcmp(lasttoken, sntrph->ar_lst[i].original_parents[1]) == 0) {
                printf("DEBUG: Adoption was found via original parent: %s.\n", lasttoken);
                printAdoptionReport(&sntrph->ar_lst[i]);
                found = true;
                return;  // Exit after the first match
            }

        }
        if(!found){
            printf("This adoption (%s) is not in SAINT records. Please try again with a different field.\n", lasttoken);
        }
    }
    else if(strcmp(nexttoken, "LOG") == 0){
        bool found = false;
        char *lasttoken = strtok(NULL, "");
        printf("DEBUG: Last Token: %s.\n", lasttoken);
        ModificationLog logs[5];
        printf("DEBUG: Logs initialised.\n");
        int count = 0;
        printf("DEBUG: Count initialised.\n");
        for(int i = 0; i < sntrph->ml_count; i++){
        printf("DEBUG: Loop initialised.\n");
            if(strcmp(lasttoken, sntrph->ml_lst[i].case_no) == 0){
                printf("DEBUG: Comparison initialised.\n");
                if(count > 4){
                    chooseLogs(logs, count);
                    printf("There is a maximum of 5 modification logs that can be accessed by a case number at a time.\n");
                    return;
                }
                else{
                    logs[count] = sntrph->ml_lst[i];
                    printf("DEBUG: Successfully added.\n");
                    count++;
                    found = true;
                    continue;
                }
            }
        }
        if(!found){
            printf("This modification log (%s) is not in SAINT records. Please try again with a different field.\n", lasttoken);
        }
        else{
        chooseLogs(logs, count);
        }
    }
    }

void increment_id(const char* current_id, char* new_id) {
    // Assume the ID format is a letter prefix + a 3-digit number (e.g., "A001")
    char prefix[10];
    int number;

    // Split into prefix and number
    sscanf(current_id, "%[A-Z]%d", prefix, &number);

    // Increment the number
    number++;

    // Rebuild the new ID (keep the same number of digits with leading zeros)
    sprintf(new_id, "%s%03d", prefix, number);
}

void createCommand(SNTRPH *sntrph, char *input) {
    if (strcmp(input, "ADDRESS") == 0) {
        Address address;

        // Generate new address ID
        char current_id[10];
        if (sntrph->address_count > 0) {
            strcpy(current_id, sntrph->address_lst[sntrph->address_count - 1].address_id);
        } else {
            strcpy(current_id, "A000");  // Start from A000 if it's the first
        }

        char new_id[10];
        increment_id(current_id, new_id);  // Assumes this function works
        strcpy(address.address_id, new_id);

        // Get street input
        char street[MAX_ADDRESS_LENGTH];
        printf("\n>Enter the street of the address: ");
        fgets(street, sizeof(street), stdin);
        street[strcspn(street, "\n")] = 0;
        strcpy(address.street, street);

        // Get city input
        char city[MAX_ADDRESS_LENGTH];
        printf("\n>Enter the city of the address: ");
        fgets(city, sizeof(city), stdin);
        city[strcspn(city, "\n")] = 0;
        strcpy(address.city, city);

        // Case numbers
        int case_count;
        printf("\n>Enter how many cases you'd like to associate with this address: ");
        scanf("%d", &case_count);
        while (getchar() != '\n');  // clear buffer

        for (int i = 0; i < case_count; i++) {
            char buffer[20];
            printf(">Enter case number %d: ", i + 1);
            fgets(buffer, sizeof(buffer), stdin);
            while (getchar() != '\n');  // clear buffer
            buffer[strcspn(buffer, "\n")] = 0;
            strcpy(address.case_no[i], buffer);
        }

        // People
        int person_count;
        printf("\n>Enter how many people you'd like to associate with this address: ");
        scanf("%d", &person_count);
        while (getchar() != '\n');  // clear buffer

        for (int i = 0; i < person_count; i++) {
            char buffer[20];
            printf(">Enter person ID %d: ", i + 1);
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = 0;
            strcpy(address.person_list[i], buffer);
        }
        address.person_count = person_count;

        // Save the address
        sntrph->address_lst[sntrph->address_count] = address;
        sntrph->address_count++;

        printf("\nAddress created with ID: %s\n", address.address_id);
        printAddress(&sntrph->address_lst[sntrph->address_count-1]);
    }
    else if (strcmp(input, "PERSON")) {
        Person person;
        char current_id[10];
        if (sntrph->person_count > 0) {
            strcpy(current_id, sntrph->person_lst[sntrph->person_count - 1].person_id);
        } else {
            strcpy(current_id, "P000");  // Start from A000 if it's the first
        }

        char new_id[10];
        increment_id(current_id, new_id);  // Assumes this function works
        strcpy(person.person_id, new_id);

        char first_name[MAX_NAME_LENGTH];
        printf("\n>Enter the first name of this person: ");
        fgets(first_name, sizeof(first_name), stdin);
        first_name[strcspn(first_name, "\n")] = 0;
        strcpy(person.first_name, first_name);

        char last_name[MAX_NAME_LENGTH];
        printf("\n>Enter the last name of this person: ");
        fgets(last_name, sizeof(last_name), stdin);
        last_name[strcspn(last_name, "\n")] = 0;
        strcpy(person.last_name, last_name);

        char address_id[40];
        printf("\n>Enter the corresponding address_id of this person's residence: ");
        fgets(address_id, sizeof(address_id), stdin);
        address_id[strcspn(address_id, "\n")] = 0;
        strcpy(person.address_id, address_id);

        char criminal_record[500];
        printf("\n>Enter the corresponding criminal_record of this person's residence: ");
        fgets(criminal_record, sizeof(criminal_record), stdin);
        criminal_record[strcspn(criminal_record, "\n")] = 0;
        strcpy(person.criminal_record, criminal_record);

        sntrph->person_lst[sntrph->person_count] = person;
        sntrph->person_count++;

        printf("\nPerson created with ID: %s\n", person.person_id);
        printPerson(&sntrph->person_lst[sntrph->person_count-1]);
    }

    else if(strcmp(input, "CASE") == 0){
        CaseFile cf;
        char name[MAX_NAME_LENGTH];
        printf("\n>Enter the name of this person: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = 0;
        strcpy(cf.name, name);

        // Generate new address ID
        char current_id[10];
        if (sntrph->cf_count > 0) {
            strcpy(current_id, sntrph->cf_lst[sntrph->cf_count - 1].cf_id);
        } else {
            strcpy(current_id, "A000");  // Start from A000 if it's the first
        }

        char new_id[10];
        increment_id(current_id, new_id);  // Assumes this function works
        strcpy(cf.case_no, new_id);

    }
}




void processCommand(SNTRPH *sntrph, char *input){
    printf("Debug: Input: %s\n", input);
    char writeableinput[50];
    strcpy(writeableinput, input);
    char *delimiter = " ";
    char *token = strtok(writeableinput, delimiter);
    char *rest = strtok (NULL, "");
    token[strcspn(token, "\n")] = 0;
    if(strcmp(token, "SEARCH")== 0){
        searchCommand(sntrph, rest);
    }
    else if(strcmp(token, "CREATE") == 0){
        createCommand(sntrph, rest);
    }

}
//SEARCH, CREATE, DELETE, EXAMINE, FILTER


int main(){
    SNTRPH sntrph;
    populateDB(&sntrph);
    processCommand(&sntrph, "CREATE ADDRESS");

    return 0;
}
