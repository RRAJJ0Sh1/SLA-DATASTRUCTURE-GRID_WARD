#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==========================================
// 1. NON-LINEAR STRUCTURE: BINARY SEARCH TREE
// (Monitors SDG 13 - Sorted by AQI)
// ==========================================
typedef struct TreeNode {
    int sensor_id;
    char location[50];
    float aqi;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

TreeNode* bst_root = NULL;
int global_sensor_id = 201;

// ==========================================
// 2. NON-LINEAR STRUCTURE: GRAPH (Adjacency Matrix)
// (Monitors SDG 11 - City Routing)
// ==========================================
#define MAX_WARDS 10
int city_graph[MAX_WARDS][MAX_WARDS];
char ward_names[MAX_WARDS][50];
int total_wards = 0;

// ==========================================
// BST FUNCTIONS (Sensors)
// ==========================================

TreeNode* create_sensor_node(char* loc, float aqi) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    newNode->sensor_id = global_sensor_id++;
    strncpy(newNode->location, loc, 49);
    newNode->location[49] = '\0';
    newNode->aqi = (aqi < 0) ? 0 : aqi;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

TreeNode* insert_bst(TreeNode* root, char* loc, float aqi) {
    if (root == NULL) return create_sensor_node(loc, aqi);
    
    // Sort logic: Lower AQI goes left, Higher AQI goes right
    if (aqi < root->aqi) {
        root->left = insert_bst(root->left, loc, aqi);
    } else {
        root->right = insert_bst(root->right, loc, aqi);
    }
    return root;
}

// In-Order Traversal prints the data sorted from Cleanest to Most Polluted
void inorder_traversal(TreeNode* root) {
    if (root != NULL) {
        inorder_traversal(root->left);
        
        if (root->aqi > 150) printf("\033[1;31m");      // Red (Hazard)
        else if (root->aqi > 100) printf("\033[1;33m"); // Yellow (Warning)
        else printf("\033[1;32m");                      // Green (Safe)
        
        printf("%-5d | %-15s | %-6.1f\033[0m\n", root->sensor_id, root->location, root->aqi);
        
        inorder_traversal(root->right);
    }
}

// ==========================================
// GRAPH FUNCTIONS (City Map & Routing)
// ==========================================

void add_ward_to_graph(char* name) {
    if (total_wards < MAX_WARDS) {
        strncpy(ward_names[total_wards], name, 49);
        ward_names[total_wards][49] = '\0';
        total_wards++;
    }
}

void add_road(int source_idx, int dest_idx) {
    if (source_idx >= 0 && source_idx < total_wards && dest_idx >= 0 && dest_idx < total_wards) {
        city_graph[source_idx][dest_idx] = 1;
        city_graph[dest_idx][source_idx] = 1; // Undirected graph
    }
}

// DFS: Uses recursion (call stack) to map a continuous patrol route
void dfs_patrol(int current, int visited[]) {
    printf("%s -> ", ward_names[current]);
    visited[current] = 1;
    
    for (int i = 0; i < total_wards; i++) {
        if (city_graph[current][i] == 1 && !visited[i]) {
            dfs_patrol(i, visited);
        }
    }
}

// BFS: Finds the shortest path layer by layer for emergency dispatch
void bfs_dispatch(int start_idx) {
    int visited[MAX_WARDS] = {0};
    int queue[MAX_WARDS]; // Algorithmic requirement for BFS logic
    int front = 0, rear = 0;
    
    printf("\033[1;36m[ Dispatch Route ]:\033[0m ");
    queue[rear++] = start_idx;
    visited[start_idx] = 1;
    
    while (front < rear) {
        int current = queue[front++];
        printf("%s ", ward_names[current]);
        if (front < rear) printf(">> ");
        
        for (int i = 0; i < total_wards; i++) {
            if (city_graph[current][i] == 1 && !visited[i]) {
                queue[rear++] = i;
                visited[i] = 1;
            }
        }
    }
    printf("\n");
}

// ==========================================
// MINDFUL CLEANUP
// ==========================================

// Post-Order Traversal safely deletes children before the parent
void free_bst(TreeNode* root) {
    if (root != NULL) {
        free_bst(root->left);
        free_bst(root->right);
        free(root);
    }
}

// ==========================================
// DASHBOARD & MAIN
// ==========================================

void print_header() {
    system("clear"); 
    printf("\033[1;36m=========================================================\n");
    printf("                   G R I D   W A R D                     \n");
    printf("         Smart City & Climate Guardian System            \n");
    printf("                     [ STAGE 2 ]                         \n");
    printf("=========================================================\033[0m\n");
    printf(" Developers: Krishna Bhanushali (2) & Raj Joshi (18)\n");
    printf(" Class/Batch: SYCM2 / Batch-A\n");
    printf(" Inst: Shah & Anchor Kutchhi Engineering College\n");
    printf("=========================================================\n");
}

void initialize_phase2_data() {
    // 1. Initialize BST Data
    bst_root = insert_bst(bst_root, "Central Hub", 110.5);
    bst_root = insert_bst(bst_root, "Green Park", 45.0);
    bst_root = insert_bst(bst_root, "Industrial Zone", 185.2);
    bst_root = insert_bst(bst_root, "River Side", 85.0);

    // 2. Initialize Graph Data (City Map)
    for(int i=0; i<MAX_WARDS; i++) {
        for(int j=0; j<MAX_WARDS; j++) city_graph[i][j] = 0;
    }
    
    add_ward_to_graph("Central Hub");    // Index 0
    add_ward_to_graph("Green Park");     // Index 1
    add_ward_to_graph("Industrial Zone");// Index 2
    add_ward_to_graph("River Side");     // Index 3
    
    // Connect the roads
    add_road(0, 1); // Central to Green Park
    add_road(0, 3); // Central to River Side
    add_road(1, 2); // Green Park to Industrial
    add_road(3, 2); // River Side to Industrial
}

int main() {
    initialize_phase2_data();
    int choice;
    char buffer[50];
    float aqi;
    
    while (1) {
        print_header();
        
        printf("\n\033[1;36m--- CLIMATE SENSORS (Sorted by AQI via BST) ---\033[0m\n");
        printf("%-5s | %-15s | %-6s\n", "ID", "Location", "AQI");
        printf("---------------------------------------\n");
        if (bst_root != NULL) inorder_traversal(bst_root);
        else printf("No sensors active.\n");
        printf("---------------------------------------\n");
        
        printf("\n\033[1;32m[ Phase 2 Control Panel ]\033[0m\n");
        printf("1. Add New Sensor (BST Insert)\n");
        printf("2. Run Emergency Dispatch (Graph BFS)\n");
        printf("3. Map City Patrol Route (Graph DFS)\n");
        printf("4. Exit System (Post-Order Free)\n");
        printf("\nEnter command (1-4): ");
        
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n'); 
            continue;
        }

        switch (choice) {
            case 1:
                printf("Enter Location Name: ");
                scanf(" %[^\n]", buffer);
                printf("Enter AQI Level: ");
                scanf("%f", &aqi);
                bst_root = insert_bst(bst_root, buffer, aqi);
                add_ward_to_graph(buffer); // Add to graph map as well
                break;
            case 2:
                printf("\n\033[1;31m[!] Initiating Emergency Response (BFS)...\033[0m\n");
                bfs_dispatch(0); // Starts from Central Hub
                printf("\nPress Enter to continue...");
                getchar(); getchar();
                break;
            case 3:
                printf("\n\033[1;33m[!] Generating Full City Patrol Route (DFS)...\033[0m\n");
                int visited[MAX_WARDS] = {0};
                printf("\033[1;36m[ Patrol Route ]:\033[0m ");
                dfs_patrol(0, visited);
                printf("END\n\nPress Enter to continue...");
                getchar(); getchar();
                break;
            case 4:
                printf("\nFreeing Non-Linear Data Structures...\n");
                free_bst(bst_root);
                printf("[ System ] Memory successfully freed. Goodbye.\n");
                return 0;
        }
    }
    return 0;
}