#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 10

typedef struct {
    int id;
    char nume[50];
    int punctaj;
} Student;

typedef struct HashNode {
    Student info;
    struct HashNode* next;
} HashNode;

typedef struct {
    HashNode* buckets[HASH_SIZE];
} HashTable;

typedef struct {
    Student* array;
    int size;
    int capacity;
} MaxHeap;

typedef struct BSTNode {
    Student info;
    struct BSTNode* left;
    struct BSTNode* right;
} BSTNode;

typedef struct AVLNode {
    Student info;
    struct AVLNode* left;
    struct AVLNode* right;
    int height;
} AVLNode;

unsigned int hashFunction(int id) {
    return id % HASH_SIZE;
}

HashTable* createHashTable() {
    int i;
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    for (i = 0; i < HASH_SIZE; i++) {
        ht->buckets[i] = NULL;
    }
    return ht;
}

void insertHash(HashTable* ht, Student s) {
    unsigned int index;
    HashNode* newNode;

    index = hashFunction(s.id);
    newNode = (HashNode*)malloc(sizeof(HashNode));
    newNode->info = s;
    newNode->next = ht->buckets[index];
    ht->buckets[index] = newNode;
}

void searchHash(HashTable* ht, int id, FILE* fout) {
    unsigned int index;
    HashNode* temp;

    index = hashFunction(id);
    temp = ht->buckets[index];
    while (temp != NULL) {
        if (temp->info.id == id) {
            fprintf(fout, "Gasit: %s (%d puncte)\n", temp->info.nume, temp->info.punctaj);
            return;
        }
        temp = temp->next;
    }
    fprintf(fout, "Studentul cu ID %d nu a fost gasit.\n", id);
}

MaxHeap* createMaxHeap(int capacity) {
    MaxHeap* heap = (MaxHeap*)malloc(sizeof(MaxHeap));
    heap->capacity = capacity;
    heap->size = 0;
    heap->array = (Student*)malloc(capacity * sizeof(Student));
    return heap;
}

void swapStudent(Student* a, Student* b) {
    Student temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(MaxHeap* heap, int index) {
    int parent;
    if (index == 0) return;

    parent = (index - 1) / 2;
    if (heap->array[parent].punctaj < heap->array[index].punctaj) {
        swapStudent(&heap->array[parent], &heap->array[index]);
        heapifyUp(heap, parent);
    }
}

void insertHeap(MaxHeap* heap, Student s) {
    if (heap->size == heap->capacity) return;
    heap->array[heap->size] = s;
    heapifyUp(heap, heap->size);
    heap->size++;
}

void heapifyDown(MaxHeap* heap, int index) {
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int largest = index;

    if (left < heap->size && heap->array[left].punctaj > heap->array[largest].punctaj)
        largest = left;
    if (right < heap->size && heap->array[right].punctaj > heap->array[largest].punctaj)
        largest = right;

    if (largest != index) {
        swapStudent(&heap->array[index], &heap->array[largest]);
        heapifyDown(heap, largest);
    }
}

Student extractMax(MaxHeap* heap) {
    Student empty;
    Student root;

    empty.id = -1;
    strcpy(empty.nume, "");
    empty.punctaj = -1;

    if (heap->size == 0) return empty;

    root = heap->array[0];
    heap->array[0] = heap->array[heap->size - 1];
    heap->size--;
    heapifyDown(heap, 0);
    return root;
}

BSTNode* createBSTNode(Student s) {
    BSTNode* node = (BSTNode*)malloc(sizeof(BSTNode));
    node->info = s;
    node->left = node->right = NULL;
    return node;
}

BSTNode* insertBST(BSTNode* root, Student s) {
    if (root == NULL) return createBSTNode(s);
    if (strcmp(s.nume, root->info.nume) < 0)
        root->left = insertBST(root->left, s);
    else if (strcmp(s.nume, root->info.nume) > 0)
        root->right = insertBST(root->right, s);
    return root;
}

void inorderBST(BSTNode* root, FILE* fout) {
    if (root != NULL) {
        inorderBST(root->left, fout);
        fprintf(fout, "Nume: %s | Punctaj: %d\n", root->info.nume, root->info.punctaj);
        inorderBST(root->right, fout);
    }
}

int getHeight(AVLNode* n) {
    if (n == NULL) return 0;
    return n->height;
}

int getMaxVal(int a, int b) {
    return (a > b) ? a : b;
}

int getBalance(AVLNode* n) {
    if (n == NULL) return 0;
    return getHeight(n->left) - getHeight(n->right);
}

AVLNode* createAVLNode(Student s) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    node->info = s;
    node->left = node->right = NULL;
    node->height = 1;
    return node;
}

AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = getMaxVal(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = getMaxVal(getHeight(x->left), getHeight(x->right)) + 1;
    return x;
}

AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = getMaxVal(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = getMaxVal(getHeight(y->left), getHeight(y->right)) + 1;
    return y;
}

AVLNode* insertAVL(AVLNode* node, Student s) {
    int balance;

    if (node == NULL) return createAVLNode(s);

    if (s.punctaj < node->info.punctaj)
        node->left = insertAVL(node->left, s);
    else if (s.punctaj > node->info.punctaj)
        node->right = insertAVL(node->right, s);
    else
        return node;

    node->height = 1 + getMaxVal(getHeight(node->left), getHeight(node->right));
    balance = getBalance(node);

    if (balance > 1 && s.punctaj < node->left->info.punctaj)
        return rightRotate(node);

    if (balance < -1 && s.punctaj > node->right->info.punctaj)
        return leftRotate(node);

    if (balance > 1 && s.punctaj > node->left->info.punctaj) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && s.punctaj < node->right->info.punctaj) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

void inorderAVL(AVLNode* root, FILE* fout) {
    if (root != NULL) {
        inorderAVL(root->left, fout);
        fprintf(fout, "Punctaj: %d -> %s\n", root->info.punctaj, root->info.nume);
        inorderAVL(root->right, fout);
    }
}

int main() {
    FILE* fin;
    FILE* fout;
    HashTable* ht;
    MaxHeap* heap;
    BSTNode* bstRoot = NULL;
    AVLNode* avlRoot = NULL;
    Student temp;
    Student top;
    int loc = 1;

    fin = fopen("date.in.txt", "r");
    if (fin == NULL) {
        printf("Eroare la deschiderea fisierului date.in!\n");
        return 1;
    }

    fout = fopen("raport.out.txt", "w");
    if (fout == NULL) {
        printf("Eroare la crearea fisierului raport.out!\n");
        fclose(fin);
        return 1;
    }

    ht = createHashTable();
    heap = createMaxHeap(100);

    while (fscanf(fin, "%d %49s %d", &temp.id, temp.nume, &temp.punctaj) == 3) {
        insertHash(ht, temp);
        insertHeap(heap, temp);
        bstRoot = insertBST(bstRoot, temp);
        avlRoot = insertAVL(avlRoot, temp);
    }

    fprintf(fout, "=== REZULTATE SISTEM (Limbaj C) ===\n\n");

    fprintf(fout, "1. HASH TABLE (Cautare ID 108):\n");
    searchHash(ht, 108, fout);
    fprintf(fout, "-----------------------------------\n\n");

    fprintf(fout, "2. ARBORE BST (Alfabetic dupa nume):\n");
    inorderBST(bstRoot, fout);
    fprintf(fout, "-----------------------------------\n\n");

    fprintf(fout, "3. ARBORE AVL (Crescator dupa punctaj):\n");
    inorderAVL(avlRoot, fout);
    fprintf(fout, "-----------------------------------\n\n");

    fprintf(fout, "4. MAX-HEAP (Top studenti):\n");
    while (heap->size > 0) {
        top = extractMax(heap);
        fprintf(fout, "Locul %d: %s (%d pct)\n", loc++, top.nume, top.punctaj);
    }

    fclose(fin);
    fclose(fout);

    printf("Toate structurile au fost construite cu succes! Verifica fisierul 'raport.out'.\n");

    return 0;
}