#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define TABLE_SIZE 100
#define MAX_CITIES 50
#define INF INT_MAX

typedef struct HashNode {
    char name[50];
    int id;
    struct HashNode* next;
} HashNode;

typedef struct {
    HashNode* buckets[TABLE_SIZE];
} HashTable;

typedef struct Edge {
    int dest;
    int weight;
    struct Edge* next;
} Edge;

typedef struct {
    Edge* head;
} AdjList;

typedef struct {
    int numVertices;
    AdjList* array;
} Graph;

typedef struct {
    int v;
    int dist;
} MinHeapNode;

typedef struct {
    int size;
    int capacity;
    int* pos;
    MinHeapNode** array;
} MinHeap;

typedef struct BSTNode {
    int awb;
    char destination[50];
    struct BSTNode* left;
    struct BSTNode* right;
} BSTNode;

unsigned int hash(const char* str) {
    unsigned int hashValue = 0;
    while (*str) {
        hashValue = (hashValue << 5) + *str++;
    }
    return hashValue % TABLE_SIZE;
}

HashTable* createHashTable() {
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    for (int i = 0; i < TABLE_SIZE; i++) ht->buckets[i] = NULL;
    return ht;
}

void insertHash(HashTable* ht, const char* name, int id) {
    unsigned int index = hash(name);
    HashNode* newNode = (HashNode*)malloc(sizeof(HashNode));
    strcpy(newNode->name, name);
    newNode->id = id;
    newNode->next = ht->buckets[index];
    ht->buckets[index] = newNode;
}

int getCityId(HashTable* ht, const char* name) {
    unsigned int index = hash(name);
    HashNode* temp = ht->buckets[index];
    while (temp) {
        if (strcmp(temp->name, name) == 0) return temp->id;
        temp = temp->next;
    }
    return -1;
}

Graph* createGraph(int V) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->numVertices = V;
    graph->array = (AdjList*)malloc(V * sizeof(AdjList));
    for (int i = 0; i < V; ++i) graph->array[i].head = NULL;
    return graph;
}

void addEdge(Graph* graph, int src, int dest, int weight) {
    Edge* newNode = (Edge*)malloc(sizeof(Edge));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;

    newNode = (Edge*)malloc(sizeof(Edge));
    newNode->dest = src;
    newNode->weight = weight;
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}

MinHeapNode* newMinHeapNode(int v, int dist) {
    MinHeapNode* minHeapNode = (MinHeapNode*)malloc(sizeof(MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}

MinHeap* createMinHeap(int capacity) {
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    minHeap->pos = (int*)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (MinHeapNode**)malloc(capacity * sizeof(MinHeapNode*));
    return minHeap;
}

void swapMinHeapNode(MinHeapNode** a, MinHeapNode** b) {
    MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(MinHeap* minHeap, int idx) {
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->dist < minHeap->array[smallest]->dist)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->dist < minHeap->array[smallest]->dist)
        smallest = right;

    if (smallest != idx) {
        MinHeapNode* smallestNode = minHeap->array[smallest];
        MinHeapNode* idxNode = minHeap->array[idx];
        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

int isEmpty(MinHeap* minHeap) {
    return minHeap->size == 0;
}

MinHeapNode* extractMin(MinHeap* minHeap) {
    if (isEmpty(minHeap)) return NULL;
    MinHeapNode* root = minHeap->array[0];
    MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;
    minHeap->pos[root->v] = minHeap->size - 1;
    minHeap->pos[lastNode->v] = 0;
    --minHeap->size;
    minHeapify(minHeap, 0);
    return root;
}

void decreaseKey(MinHeap* minHeap, int v, int dist) {
    int i = minHeap->pos[v];
    minHeap->array[i]->dist = dist;
    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist) {
        minHeap->pos[minHeap->array[i]->v] = (i - 1) / 2;
        minHeap->pos[minHeap->array[(i - 1) / 2]->v] = i;
        swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

void dijkstra(Graph* graph, int src, int dest) {
    int V = graph->numVertices;
    int dist[MAX_CITIES];
    int parent[MAX_CITIES];
    MinHeap* minHeap = createMinHeap(V);

    for (int v = 0; v < V; ++v) {
        dist[v] = INF;
        parent[v] = -1;
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->pos[v] = v;
    }

    minHeap->array[src] = newMinHeapNode(src, dist[src]);
    minHeap->pos[src] = src;
    dist[src] = 0;
    decreaseKey(minHeap, src, dist[src]);
    minHeap->size = V;

    while (!isEmpty(minHeap)) {
        MinHeapNode* minHeapNode = extractMin(minHeap);
        int u = minHeapNode->v;

        Edge* pCrawl = graph->array[u].head;
        while (pCrawl != NULL) {
            int v = pCrawl->dest;
            if (minHeap->pos[v] < minHeap->size && dist[u] != INF && pCrawl->weight + dist[u] < dist[v]) {
                dist[v] = dist[u] + pCrawl->weight;
                parent[v] = u;
                decreaseKey(minHeap, v, dist[v]);
            }
            pCrawl = pCrawl->next;
        }
    }

    if (dist[dest] == INF) {
        printf("Nu exista ruta disponibila.\n");
        return;
    }

    printf("Distanta minima: %d km\n", dist[dest]);
    printf("Traseu (ID orase, invers): ");
    int curr = dest;
    while (curr != -1) {
        printf("%d ", curr);
        curr = parent[curr];
    }
    printf("\n");
}

BSTNode* insertPackage(BSTNode* root, int awb, const char* dest) {
    if (root == NULL) {
        BSTNode* temp = (BSTNode*)malloc(sizeof(BSTNode));
        temp->awb = awb;
        strcpy(temp->destination, dest);
        temp->left = temp->right = NULL;
        return temp;
    }
    if (awb < root->awb) root->left = insertPackage(root->left, awb, dest);
    else if (awb > root->awb) root->right = insertPackage(root->right, awb, dest);
    return root;
}

void inorderPackages(BSTNode* root) {
    if (root != NULL) {
        inorderPackages(root->left);
        printf("AWB: %d -> Destinatie: %s\n", root->awb, root->destination);
        inorderPackages(root->right);
    }
}

int main() {
    HashTable* cityHash = createHashTable();
    insertHash(cityHash, "Bucuresti", 0);
    insertHash(cityHash, "Cluj", 1);
    insertHash(cityHash, "Timisoara", 2);
    insertHash(cityHash, "Iasi", 3);
    insertHash(cityHash, "Constanta", 4);

    Graph* map = createGraph(5);
    addEdge(map, 0, 1, 450);
    addEdge(map, 0, 4, 225);
    addEdge(map, 0, 3, 389);
    addEdge(map, 1, 2, 315);
    addEdge(map, 3, 1, 390);

    BSTNode* inventory = NULL;
    inventory = insertPackage(inventory, 90214, "Cluj");
    inventory = insertPackage(inventory, 11023, "Timisoara");
    inventory = insertPackage(inventory, 84321, "Constanta");
    inventory = insertPackage(inventory, 55210, "Iasi");

    printf("--- BAZA DE DATE LOGISTICA ---\n");
    inorderPackages(inventory);

    printf("\n--- RUTARE OPTIMA ---\n");
    int srcId = getCityId(cityHash, "Bucuresti");
    int destId = getCityId(cityHash, "Timisoara");

    if (srcId != -1 && destId != -1) {
        printf("Calculam ruta de la Bucuresti(%d) la Timisoara(%d)...\n", srcId, destId);
        dijkstra(map, srcId, destId);
    }

    return 0;
}