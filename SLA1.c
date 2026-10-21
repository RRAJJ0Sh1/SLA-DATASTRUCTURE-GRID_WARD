#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==========================================
// DATA STRUCTURES
// ==========================================

// 1. SENSOR NETWORK (Doubly Linked List)
typedef struct SensorNode {
    int sensor_id;
    char location[50];
    float aqi;
    float temperature;
    struct SensorNode* prev;
    struct SensorNode* next;
} SensorNode;

// 2. EMERGENCY RESPONSE (Priority Queue)
typedef struct TaskNode {
    int task_id;
    char description[100];
    int priority; 
    struct TaskNode* next;
} TaskNode;

// 3. CLIMATE ALERTS (Stack)
typedef struct AlertNode {
    int alert_id;
    char warning_message[100];
    struct AlertNode* next;
} AlertNode;

SensorNode* sensor_head = NULL;
SensorNode* sensor_tail = NULL;
TaskNode* task_queue_head = NULL;
AlertNode* alert_stack_top = NULL;

int global_sensor_id = 101;
int global_task_id = 1001;
int global_alert_id = 5001;

// ==========================================
// MINDFUL HELPER FUNCTIONS
// ==========================================

void safe_input_string(char* buffer, int max_size) {
    if (fgets(buffer, max_size, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        } else {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
    }
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ==========================================
// CORE DSA FUNCTIONS
// ==========================================

void add_sensor(char* loc, float aqi, float temp) {
    SensorNode* newNode = (SensorNode*)malloc(sizeof(SensorNode));
    if (newNode == NULL) {
        printf("Error: Memory allocation failed!\n");
        return;
    }
    newNode->sensor_id = global_sensor_id++;
    strncpy(newNode->location, loc, 49);
    newNode->location[49] = '\0'; 
    newNode->aqi = (aqi < 0) ? 0 : aqi; 
    newNode->temperature = temp;
    newNode->prev = NULL;
    newNode->next = NULL;

    if (sensor_head == NULL) {
        sensor_head = sensor_tail = newNode;
    } else {
        sensor_tail->next = newNode;
        newNode->prev = sensor_tail;
        sensor_tail = newNode;
    }
}

void add_task(char* desc, int priority) {
    if (priority < 1) priority = 1;
    if (priority > 5) priority = 5;

    TaskNode* newNode = (TaskNode*)malloc(sizeof(TaskNode));
    if (newNode == NULL) return; 
    
    newNode->task_id = global_task_id++;
    strncpy(newNode->description, desc, 99);
    newNode->description[99] = '\0';
    newNode->priority = priority;
    newNode->next = NULL;

    if (task_queue_head == NULL || priority < task_queue_head->priority) {
        newNode->next = task_queue_head;
        task_queue_head = newNode;
    } else {
        TaskNode* temp = task_queue_head;
        while (temp->next != NULL && temp->next->priority <= priority) {
            temp = temp->next;
        }
        newNode->next = temp->next;
        temp->next = newNode;
    }
}

void push_alert(char* msg) {
    AlertNode* newNode = (AlertNode*)malloc(sizeof(AlertNode));
    if (newNode == NULL) return;
    newNode->alert_id = global_alert_id++;
    strncpy(newNode->warning_message, msg, 99);
    newNode->warning_message[99] = '\0';
    
    newNode->next = alert_stack_top;
    alert_stack_top = newNode;
}

void dismiss_top_alert() {
    if (alert_stack_top == NULL) {
        printf("\n\033[1;33m[!] No active alerts to dismiss.\033[0m\n");
        return;
    }
    AlertNode* temp = alert_stack_top;
    alert_stack_top = alert_stack_top->next;
    printf("\n\033[1;32m[+] Alert #%d Dismissed.\033[0m\n", temp->alert_id);
    free(temp); 
}

void resolve_top_task() {
    if (task_queue_head == NULL) { 
        printf("\n\033[1;33m[!] No tasks in queue to resolve.\033[0m\n");
        return;
    }
    TaskNode* temp = task_queue_head;
    task_queue_head = task_queue_head->next;
    printf("\n\033[1;32m[+] Resolved Task #%d: %s\033[0m\n", temp->task_id, temp->description);
    free(temp); 
}

// ==========================================
// MEMORY CLEANUP ON EXIT
// ==========================================
void free_all_memory() {
    SensorNode* s_temp;
    while (sensor_head != NULL) {
        s_temp = sensor_head;
        sensor_head = sensor_head->next;
        free(s_temp);
    }
    TaskNode* t_temp;
    while (task_queue_head != NULL) {
        t_temp = task_queue_head;
        task_queue_head = task_queue_head->next;
        free(t_temp);
    }
    AlertNode* a_temp;
    while (alert_stack_top != NULL) {
        a_temp = alert_stack_top;
        alert_stack_top = alert_stack_top->next;
        free(a_temp);
    }
    printf("\n[ System ] All memory successfully freed. No leaks.\n");
}

// ==========================================
// DISPLAY FUNCTIONS
// ==========================================

void display_sensors() {
    printf("\n\033[1;36m--- WARD SENSOR NETWORK (Air & Climate) ---\033[0m\n");
    printf("%-5s | %-15s | %-6s | %-6s\n", "ID", "Location", "AQI", "Temp(C)");
    printf("----------------------------------------------\n");
    
    SensorNode* temp = sensor_head;
    if (temp == NULL) {
        printf("No sensors active.\n");
        return;
    }
    while (temp != NULL) {
        if (temp->aqi > 150) printf("\033[1;31m"); 
        else if (temp->aqi > 100) printf("\033[1;33m"); 
        else printf("\033[1;32m"); 
        
        printf("%-5d | %-15s | %-6.1f | %-6.1f\033[0m\n", 
               temp->sensor_id, temp->location, temp->aqi, temp->temperature);
        temp = temp->next;
    }
    printf("----------------------------------------------\n");
}

void display_tasks() {
    printf("\n\033[1;35m--- EMERGENCY RESPONSE QUEUE ---\033[0m\n");
    printf("%-5s | %-10s | %-30s\n", "ID", "Priority", "Description");
    printf("---------------------------------------------------\n");
    
    TaskNode* temp = task_queue_head;
    if (temp == NULL) {
        printf("No pending tasks. City is safe!\n");
        return;
    }
    while (temp != NULL) {
        if (temp->priority == 1) printf("\033[1;31m"); 
        printf("%-5d | %-10d | %-30s\033[0m\n", temp->task_id, temp->priority, temp->description);
        temp = temp->next;
    }
    printf("---------------------------------------------------\n");
}

void display_alerts() {
    printf("\n\033[1;31m--- RECENT CLIMATE ALERTS (LIFO) ---\033[0m\n");
    AlertNode* temp = alert_stack_top;
    if (temp == NULL) {
        printf("No recent alerts.\n");
        return;
    }
    while (temp != NULL) {
        printf("[!] ALERT #%d: %s\n", temp->alert_id, temp->warning_message);
        temp = temp->next;
    }
    printf("------------------------------------\n");
}

void print_header() {
    system("clear"); 
    printf("\033[1;36m"); 
    printf("=========================================================\n");
    printf("                   G R I D   W A R D                     \n");
    printf("         Smart City & Climate Guardian System            \n");
    printf("                     [ STAGE 1 ]                         \n");
    printf("=========================================================\n");
    printf("\033[0m"); 
    printf(" Developers: Krishna Bhanushali (2) & Raj Joshi (18)\n");
    printf(" Class/Batch: SYCM2 / Batch-A\n");
    printf(" Inst: Shah & Anchor Kutchhi Engineering College\n");
    printf("=========================================================\n");
}

void initialize_dummy_data() {
    add_sensor("Sector 4 Park", 85.0, 31.2);
    add_sensor("Industrial Hub", 180.5, 36.5);
    add_task("Clear fallen tree on Main St.", 2);
    add_task("Drain blocked in Sector 4", 1);
    push_alert("Heatwave warning: Temperatures exceeding 36C.");
}

int main() {
    initialize_dummy_data();
    int choice;
    char buffer[100];
    float val1, val2;
    int priority;
    
    while (1) {
        print_header();
        display_alerts();
        display_sensors();
        display_tasks();
        
        printf("\n\033[1;32m[ Control Panel ]\033[0m\n");
        printf("1. Add New Sensor Data\n");
        printf("2. Report City Issue (Priority Queue)\n");
        printf("3. Broadcast Climate Alert (Push)\n");
        printf("4. Resolve Top Priority Task (Dequeue)\n");
        printf("5. Dismiss Latest Alert (Pop)\n");
        printf("6. Exit System (Free Memory)\n");
        printf("\nEnter command (1-6): ");
        
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer(); 
            continue;
        }
        clear_input_buffer(); 

        switch (choice) {
            case 1:
                printf("Enter Location Name: ");
                safe_input_string(buffer, sizeof(buffer));
                printf("Enter AQI Level: ");
                if(scanf("%f", &val1) != 1) { clear_input_buffer(); continue; }
                printf("Enter Temperature (C): ");
                if(scanf("%f", &val2) != 1) { clear_input_buffer(); continue; }
                add_sensor(buffer, val1, val2);
                break;
            case 2:
                printf("Enter Issue Description: ");
                safe_input_string(buffer, sizeof(buffer));
                printf("Enter Priority (1=Critical, 5=Low): ");
                if(scanf("%d", &priority) != 1) { clear_input_buffer(); continue; }
                add_task(buffer, priority);
                break;
            case 3:
                printf("Enter Alert Message: ");
                safe_input_string(buffer, sizeof(buffer));
                push_alert(buffer);
                break;
            case 4:
                resolve_top_task();
                printf("Press Enter to continue...");
                getchar();
                break;
            case 5:
                dismiss_top_alert();
                printf("Press Enter to continue...");
                getchar();
                break;
            case 6:
                printf("\nShutting down Grid Ward system...\n");
                free_all_memory(); 
                return 0;
            default:
                break;
        }
    }
    return 0;
}