#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==========================================
// 0. DATELE (Entitatea Restaurant)
// ==========================================
typedef struct {
    int id;
    char nume[50];
    int incasari; // Vom folosi incasarile pentru ordonare in Heap si AVL
} Restaurant;

// ==========================================
// 1. HASH TABLE (Cautare rapida dupa ID)
// ==========================================
#define H_SIZE 10
typedef struct NodeH {
    Restaurant info;
    struct NodeH* next;
} NodeH;

void insertHash(NodeH** ht, Restaurant r) {
    int p = r.id % H_SIZE;
    NodeH* nou = (NodeH*)malloc(sizeof(NodeH));
    nou->info = r;
    nou->next = ht[p];
    ht[p] = nou;
}

void searchHash(NodeH** ht, int id, FILE* fout) {
    NodeH* temp = ht[id % H_SIZE];
    while (temp) {
        if (temp->info.id == id) {
            fprintf(fout, "Gasit Restaurant in Hash: %s (Incasari: %d RON)\n", temp->info.nume, temp->info.incasari);
            return;
        }
        temp = temp->next;
    }
    fprintf(fout, "Restaurantul cu ID %d nu a fost gasit.\n", id);
}

// ==========================================
// 2. MAX-HEAP (Cele mai profitabile restaurante)
// ==========================================
typedef struct {
    Restaurant* arr;
    int size;
} Heap;

void insertHeap(Heap* h, Restaurant r) {
    int i = h->size++;
    int p = (i - 1) / 2;
    Restaurant aux;
    h->arr[i] = r;

    // Comparam incasarile
    while (i > 0 && h->arr[i].incasari > h->arr[p].incasari) {
        aux = h->arr[i]; h->arr[i] = h->arr[p]; h->arr[p] = aux;
        i = p; p = (i - 1) / 2;
    }
}

Restaurant extractHeap(Heap* h) {
    Restaurant max = h->arr[0];
    Restaurant aux;
    int i = 0, st, dr, m_idx;
    h->arr[0] = h->arr[--h->size];

    while (1) {
        st = 2 * i + 1; dr = 2 * i + 2; m_idx = i;
        if (st < h->size && h->arr[st].incasari > h->arr[m_idx].incasari) m_idx = st;
        if (dr < h->size && h->arr[dr].incasari > h->arr[m_idx].incasari) m_idx = dr;
        if (m_idx == i) break;

        aux = h->arr[i]; h->arr[i] = h->arr[m_idx]; h->arr[m_idx] = aux;
        i = m_idx;
    }
    return max;
}

// ==========================================
// 3. BST (Ordonat alfabetic dupa Nume)
// ==========================================
typedef struct NodeB {
    Restaurant info;
    struct NodeB* left, * right;
} NodeB;

NodeB* insertBST(NodeB* root, Restaurant r) {
    if (!root) {
        NodeB* nou = (NodeB*)malloc(sizeof(NodeB));
        nou->info = r; nou->left = nou->right = NULL;
        return nou;
    }
    // Ordonam dupa nume folosind strcmp
    if (strcmp(r.nume, root->info.nume) < 0) root->left = insertBST(root->left, r);
    else if (strcmp(r.nume, root->info.nume) > 0) root->right = insertBST(root->right, r);
    return root;
}

void printBSTConditionat(NodeB* root, int prag_incasari, FILE* fout) {
    if (root) {
        printBSTConditionat(root->left, prag_incasari, fout);
        if (root->info.incasari > prag_incasari) {
            fprintf(fout, "Incasari mari (>%d): %s\n", prag_incasari, root->info.nume);
        }
        printBSTConditionat(root->right, prag_incasari, fout);
    }
}

// ==========================================
// 4. AVL (Ordonat dupa Incasari)
// ==========================================
typedef struct NodeAVL {
    Restaurant info;
    struct NodeAVL* left, * right;
    int ht;
} NodeAVL;

int maxV(int a, int b) { return a > b ? a : b; }
int getH(NodeAVL* n) { return n ? n->ht : 0; }
int getBal(NodeAVL* n) { return n ? getH(n->left) - getH(n->right) : 0; }

NodeAVL* rotD(NodeAVL* y) {
    NodeAVL* x = y->left; y->left = x->right; x->right = y;
    y->ht = maxV(getH(y->left), getH(y->right)) + 1;
    x->ht = maxV(getH(x->left), getH(x->right)) + 1;
    return x;
}

NodeAVL* rotS(NodeAVL* x) {
    NodeAVL* y = x->right; x->right = y->left; y->left = x;
    x->ht = maxV(getH(x->left), getH(x->right)) + 1;
    y->ht = maxV(getH(y->left), getH(y->right)) + 1;
    return y;
}

NodeAVL* insertAVL(NodeAVL* root, Restaurant r) {
    int bal;
    if (!root) {
        NodeAVL* nou = (NodeAVL*)malloc(sizeof(NodeAVL));
        nou->info = r; nou->left = nou->right = NULL; nou->ht = 1;
        return nou;
    }

    // Inseram dupa incasari
    if (r.incasari < root->info.incasari) root->left = insertAVL(root->left, r);
    else if (r.incasari > root->info.incasari) root->right = insertAVL(root->right, r);
    else return root;

    root->ht = 1 + maxV(getH(root->left), getH(root->right));
    bal = getBal(root);

    if (bal > 1 && r.incasari < root->left->info.incasari) return rotD(root);
    if (bal < -1 && r.incasari > root->right->info.incasari) return rotS(root);
    if (bal > 1 && r.incasari > root->left->info.incasari) { root->left = rotS(root->left); return rotD(root); }
    if (bal < -1 && r.incasari < root->right->info.incasari) { root->right = rotD(root->right); return rotS(root); }

    return root;
}

void printAVL(NodeAVL* root, FILE* fout) {
    if (root) {
        printAVL(root->left, fout);
        fprintf(fout, "AVL Incasari: %d RON -> %s\n", root->info.incasari, root->info.nume);
        printAVL(root->right, fout);
    }
}

// ==========================================
// 5. MAIN
// ==========================================
int main() {
    FILE* fin = fopen("restaurantet.txt", "r");
    FILE* fout = fopen("raport14.out.txt", "w");
    Restaurant r;

    NodeH* ht[H_SIZE] = { NULL };
    Heap hp; hp.size = 0; hp.arr = (Restaurant*)malloc(100 * sizeof(Restaurant));
    NodeB* bst = NULL;
    NodeAVL* avl = NULL;

    if (!fin || !fout) return 1;

    // Citim restaurantele
    while (fscanf(fin, "%d %49s %d", &r.id, r.nume, &r.incasari) == 3) {
        insertHash(ht, r);
        insertHeap(&hp, r);
        bst = insertBST(bst, r);
        avl = insertAVL(avl, r);
    }

    fprintf(fout, "=== RAPORT RESTAURANTE ===\n\n");

    // Test HashTable
    searchHash(ht, 503, fout); // Inlocuieste 503 cu un ID pe care il pui tu in fisier

    // Test BST conditionat
    fprintf(fout, "\n-- Restaurante cu incasari > 10000 (din BST) --\n");
    printBSTConditionat(bst, 10000, fout);

    // Test AVL
    fprintf(fout, "\n-- Toate Restaurantele ordonate dupa incasari (din AVL) --\n");
    printAVL(avl, fout);

    // Test Extragere Heap
    fprintf(fout, "\n-- Top 3 Cele mai profitabile restaurante (Extrase din Heap) --\n");
    for (int i = 0; i < 3 && hp.size > 0; i++) {
        r = extractHeap(&hp);
        fprintf(fout, "Locul %d: %s (%d RON)\n", i + 1, r.nume, r.incasari);
    }

    fclose(fin);
    fclose(fout);
    free(hp.arr);

    printf("Toate structurile au rulat perfect! Verifica raport.out.txt\n");
    return 0;
}