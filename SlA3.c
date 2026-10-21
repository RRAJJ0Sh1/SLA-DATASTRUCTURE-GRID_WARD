#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==========================================
// 1. LINEAR DATA STRUCTURES
// ==========================================

// Doubly Linked List (Chronological Sensor Registry)
typedef struct DLLNode {
    int id;
    char location[50];
    float aqi;
    struct DLLNode *prev, *next;
} DLLNode;

// Priority Queue (Emergency Tasks)
typedef struct TaskNode {
    int task_id, priority;
    char description[100];
    struct TaskNode* next;
} TaskNode;

// Stack (Climate Alerts)
typedef struct AlertNode {
    int alert_id;
    char warning[100];
    struct AlertNode* next;
} AlertNode;

DLLNode *dll_head = NULL, *dll_tail = NULL;
TaskNode *pq_head = NULL;
AlertNode *stack_top = NULL;

// ==========================================
// 2. NON-LINEAR DATA STRUCTURES
// ==========================================

// Binary Search Tree (Sensors Indexed by AQI)
typedef struct BSTNode {
    int id;
    char location[50];
    float aqi;
    struct BSTNode *left, *right;
} BSTNode;

BSTNode *bst_root = NULL;

// Graph / Adjacency Matrix (City Map & Routing)
#define MAX_WARDS 10
int city_graph[MAX_WARDS][MAX_WARDS] = {0};
char ward_names[MAX_WARDS][50];
int total_wards = 0;

// Global IDs
int g_sensor_id = 301, g_task_id = 1001, g_alert_id = 5001;

// ==========================================
// CORE INTEGRATED FUNCTIONS
// ==========================================

// GRAPH: Add Ward and Road
void add_ward(char* name) {
    if (total_wards < MAX_WARDS) {
        strncpy(ward_names[total_wards], name, 49);
        total_wards++;
    }
}
void add_road(int src, int dest) {
    city_graph[src][dest] = 1;
    city_graph[dest][src] = 1;
}

// UNIFIED SENSOR INSERTION: Adds to DLL (Linear) AND BST (Non-Linear)
BSTNode* insert_bst(BSTNode* root, int id, char* loc, float aqi) {
    if (!root) {
        BSTNode* node = (BSTNode*)malloc(sizeof(BSTNode));
        node->id = id; strncpy(node->location, loc, 49); node->aqi = aqi;
        node->left = node->right = NULL;
        return node;
    }
    if (aqi < root->aqi) root->left = insert_bst(root->left, id, loc, aqi);
    else root->right = insert_bst(root->right, id, loc, aqi);
    return root;
}

void deploy_sensor(char* loc, float aqi) {
    // 1. Add to Doubly Linked List (Registry)
    DLLNode* node = (DLLNode*)malloc(sizeof(DLLNode));
    node->id = g_sensor_id++;
    strncpy(node->location, loc, 49);
    node->aqi = aqi;
    node->prev = node->next = NULL;
    
    if (!dll_head) dll_head = dll_tail = node;
    else { dll_tail->next = node; node->prev = dll_tail; dll_tail = node; }
    
    // 2. Add to Binary Search Tree (Sorted Index)
    bst_root = insert_bst(bst_root, node->id, loc, aqi);
    
    // 3. Add to Graph (City Map)
    add_ward(loc);
}

// QUEUE & STACK FUNCTIONS
void add_task(char* desc, int priority) {
    TaskNode* node = (TaskNode*)malloc(sizeof(TaskNode));
    node->task_id = g_task_id++; strncpy(node->description, desc, 99); node->priority = priority;
    node->next = NULL;
    if (!pq_head || priority < pq_head->priority) { node->next = pq_head; pq_head = node; }
    else {
        TaskNode* temp = pq_head;
        while (temp->next && temp->next->priority <= priority) temp = temp->next;
        node->next = temp->next; temp->next = node;
    }
}

void push_alert(char* msg) {
    AlertNode* node = (AlertNode*)malloc(sizeof(AlertNode));
    node->alert_id = g_alert_id++; strncpy(node->warning, msg, 99);
    node->next = stack_top; stack_top = node;
}

// GRAPH TRAVERSALS (BFS / DFS)
void bfs_dispatch(int start) {
    int visited[MAX_WARDS] = {0}, queue[MAX_WARDS], front = 0, rear = 0;
    printf("\033[1;36m[ BFS Dispatch Route ]:\033[0m ");
    queue[rear++] = start; visited[start] = 1;
    while (front < rear) {
        int curr = queue[front++];
        printf("%s >> ", ward_names[curr]);
        for (int i = 0; i < total_wards; i++) {
            if (city_graph[curr][i] && !visited[i]) { queue[rear++] = i; visited[i] = 1; }
        }
    }
    printf("ARRIVED\n");
}

void dfs_patrol(int curr, int visited[]) {
    printf("%s -> ", ward_names[curr]);
    visited[curr] = 1;
    for (int i = 0; i < total_wards; i++) {
        if (city_graph[curr][i] && !visited[i]) dfs_patrol(i, visited);
    }
}

// ==========================================
// UNIFIED DISPLAY & CLEANUP
// ==========================================

void display_bst_inorder(BSTNode* root) {
    if (root) {
        display_bst_inorder(root->left);
        printf("ID: %-4d | %-15s | AQI: %.1f\n", root->id, root->location, root->aqi);
        display_bst_inorder(root->right);
    }
}

void free_everything(BSTNode* root) {
    if (root) { free_everything(root->left); free_everything(root->right); free(root); }
    while (dll_head) { DLLNode* t = dll_head; dll_head = dll_head->next; free(t); }
    while (pq_head) { TaskNode* t = pq_head; pq_head = pq_head->next; free(t); }
    while (stack_top) { AlertNode* t = stack_top; stack_top = stack_top->next; free(t); }
}

void initialize_unified_data() {
    deploy_sensor("Central Hub", 110.5);
    deploy_sensor("Green Park", 45.0);
    deploy_sensor("Industrial Zone", 185.2);
    add_road(0, 1); add_road(0, 2); // Connect Central to Green Park and Industrial
    
    add_task("Clear fallen tree on Main St.", 2);
    add_task("Chemical Spill in Industrial", 1);
    push_alert("Toxic emissions detected!");
}

int main() {
    initialize_unified_data();
    int choice; char buffer[100]; float aqi; int prio;
    
    while (1) {
        system("clear");
        printf("\033[1;36m=========================================================\n");
        printf("         G R I D   W A R D   [ STAGE 3 : UNIFIED ]       \n");
        printf("=========================================================\033[0m\n");
        
        printf("\n\033[1;31m[!] TOP ALERT (Stack):\033[0m %s\n", stack_top ? stack_top->warning : "None");
        printf("\033[1;35m[!] TOP TASK (PQ):\033[0m %s\n", pq_head ? pq_head->description : "None");
        
        printf("\n\033[1;32m[ Phase 3 Unified Control ]\033[0m\n");
        printf("1. Deploy Sensor (Adds to DLL, BST, & Graph)\n");
        printf("2. Report Emergency Task (Priority Queue)\n");
        printf("3. Resolve Top Task & Dispatch Team (BFS)\n");
        printf("4. Initiate City Patrol (DFS)\n");
        printf("5. View AQI Threat Index (BST In-Order)\n");
        printf("6. Shut Down (Free All Memory)\n");
        printf("Enter command: ");
        
        if (scanf("%d", &choice) != 1) { while(getchar() != '\n'); continue; }
        
        switch (choice) {
            case 1:
                printf("Location: "); scanf(" %[^\n]", buffer);
                printf("AQI: "); scanf("%f", &aqi);
                deploy_sensor(buffer, aqi);
                break;
            case 2:
                printf("Emergency: "); scanf(" %[^\n]", buffer);
                printf("Priority (1-5): "); scanf("%d", &prio);
                add_task(buffer, prio);
                break;
            case 3:
                if (pq_head) {
                    TaskNode* t = pq_head; pq_head = pq_head->next;
                    printf("\n\033[1;32m[+] Resolved: %s\033[0m\n", t->description); free(t);
                    bfs_dispatch(0); // Route team from Central Hub
                } else printf("\nNo tasks to resolve.\n");
                printf("\nPress Enter..."); getchar(); getchar();
                break;
            case 4:
                printf("\n\033[1;33m[ DFS Patrol Route ]:\033[0m ");
                int visited[MAX_WARDS] = {0}; dfs_patrol(0, visited);
                printf("END\n\nPress Enter..."); getchar(); getchar();
                break;
            case 5:
                printf("\n\033[1;36m--- THREAT INDEX (BST In-Order) ---\033[0m\n");
                display_bst_inorder(bst_root);
                printf("\nPress Enter..."); getchar(); getchar();
                break;
            case 6:
                free_everything(bst_root);
                printf("\n[ System ] All Linear & Non-Linear memory freed. Goodbye.\n");
                return 0;
        }
    }
    return 0;
}