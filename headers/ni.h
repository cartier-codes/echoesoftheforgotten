//TODO: IMPLEMENT ALL CRUD FUNCTIONS FOR THESE STRUCTS
typedef struct {
    char incident_id[10];
    char date[11];              // YYYY-MM-DD
    char time[6];               // HH:MM
    char location[100];
    char description[256];
    char lead[10];
    char status[20];            // e.g. "Open", "Closed"
} Incident;

typedef struct {
    char arrest_id[10];
    char person_id[10];
    char case_id[10];           // Optional
    char date[11];
    char location[100];
    char charges[256];
    char lead[10];
    char outcome[50];           // e.g. "Held", "Released"
} Arrest;

typedef struct {
    char interrogation_id[10];
    char person_id[10];
    char case_id[10];
    char officer_id[10];
    char date[11];
    char transcript_path[100];
    char summary[256];
} Interrogation;

typedef struct {
    char log_id[10];
    char officer_id[10];
    char date[11];
    char shift_start[6];
    char shift_end[6];
    char route[100];
    char notes[256];
} PatrolLog;

typedef struct {
    char warrant_id[10];
    char person_id[10];
    char type[50];              // "Arrest", "Search"
    char issue_date[11];
    char expiry_date[11];
    char issuing_officer[10];
    char status[20];            // "Active", "Served", "Expired"
} Warrant;

typedef struct {
    char task_id[10];
    char officer_id[10];
    char assigned_by[10];
    char description[256];
    char due_date[11];
    char status[20];            // "Pending", "Complete"
} Task;

typedef struct {
    char report_id[10];
    char evidence_id[10];
    char technician[50];
    char date[11];
    char result_summary[256];
    char file_path[100];
} Forensics;

typedef struct {
    char vehicle_id[10];
    char owner_id[10];
    char license_plate[15];
    char make[50];
    char model[50];
    char color[20];
    char status[30];            // "Registered", "Impounded"
} Vehicle;

typedef struct {
    char contact_id[10];
    char person_id[10];
    char associate_id[10];
    char relation[50];          // "Sibling", "Suspect"
    char notes[256];
} Contact;

typedef struct {
    char media_id[10];
    char date[11];
    char outlet[50];
    char headline[100];
    char summary[256];
    char case_id[10];
} MediaLog;
