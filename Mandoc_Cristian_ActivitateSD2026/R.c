#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==========================================
// 0. DATELE (Am înlocuit Element cu Angajat)
// ==========================================
typedef struct {
    int id;
    char nume[50];
    int salariu;
} Angajat;

// ==========================================
// 1. HASH TABLE (Căutare după ID)
// ==========================================
#define H_SIZE 10
typedef struct NodeH {
    Angajat info;
    struct NodeH* next;
} NodeH;

void insertHash(NodeH** ht, Angajat a) {
    int p = a.id % H_SIZE;
    NodeH* nou = (NodeH*)malloc(sizeof(NodeH));
    nou->info = a;
    nou->next = ht[p];
    ht[p] = nou;
}

void searchHash(NodeH** ht, int id, FILE* fout) {
    NodeH* temp = ht[id % H_SIZE];
    while (temp) {
        if (temp->info.id == id) {
            fprintf(fout, "Gasit Angajat in Hash: %s (Salariu: %d)\n", temp->info.nume, temp->info.salariu);
            return;
        }
        temp = temp->next;
    }
    fprintf(fout, "Angajatul nu a fost gasit.\n");
}

// ==========================================
// 2. MAX-HEAP (Cei mai bine plătiți angajați)
// ==========================================
typedef struct {
    Angajat* arr;
    int size;
} Heap;

void insertHeap(Heap* h, Angajat a) {
    int i = h->size++;
    int p = (i - 1) / 2;
    Angajat aux;
    h->arr[i] = a;

    // Comparăm salariile
    while (i > 0 && h->arr[i].salariu > h->arr[p].salariu) {
        aux = h->arr[i]; h->arr[i] = h->arr[p]; h->arr[p] = aux;
        i = p; p = (i - 1) / 2;
    }
}

Angajat extractHeap(Heap* h) {
    Angajat max = h->arr[0];
    Angajat aux;
    int i = 0, st, dr, m_idx;
    h->arr[0] = h->arr[--h->size];

    while (1) {
        st = 2 * i + 1; dr = 2 * i + 2; m_idx = i;
        if (st < h->size && h->arr[st].salariu > h->arr[m_idx].salariu) m_idx = st;
        if (dr < h->size && h->arr[dr].salariu > h->arr[m_idx].salariu) m_idx = dr;
        if (m_idx == i) break;

        aux = h->arr[i]; h->arr[i] = h->arr[m_idx]; h->arr[m_idx] = aux;
        i = m_idx;
    }
    return max;
}

// ==========================================
// 3. BST (Ordonat alfabetic după nume)
// ==========================================
typedef struct NodeB {
    Angajat info;
    struct NodeB* left, * right;
} NodeB;

NodeB* insertBST(NodeB* root, Angajat a) {
    if (!root) {
        NodeB* nou = (NodeB*)malloc(sizeof(NodeB));
        nou->info = a; nou->left = nou->right = NULL;
        return nou;
    }
    // Ordonăm după nume folosind strcmp
    if (strcmp(a.nume, root->info.nume) < 0) root->left = insertBST(root->left, a);
    else if (strcmp(a.nume, root->info.nume) > 0) root->right = insertBST(root->right, a);
    return root;
}

void printBSTConditionat(NodeB* root, int prag_salariu, FILE* fout) {
    if (root) {
        printBSTConditionat(root->left, prag_salariu, fout);
        if (root->info.salariu > prag_salariu) {
            fprintf(fout, "Salariu mare (>%d): %s\n", prag_salariu, root->info.nume);
        }
        printBSTConditionat(root->right, prag_salariu, fout);
    }
}

// ==========================================
// 4. AVL (Ordonat după salariu)
// ==========================================
typedef struct NodeAVL {
    Angajat info;
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

NodeAVL* insertAVL(NodeAVL* root, Angajat a) {
    int bal;
    if (!root) {
        NodeAVL* nou = (NodeAVL*)malloc(sizeof(NodeAVL));
        nou->info = a; nou->left = nou->right = NULL; nou->ht = 1;
        return nou;
    }

    // Inserăm după salariu
    if (a.salariu < root->info.salariu) root->left = insertAVL(root->left, a);
    else if (a.salariu > root->info.salariu) root->right = insertAVL(root->right, a);
    else return root;

    root->ht = 1 + maxV(getH(root->left), getH(root->right));
    bal = getBal(root);

    if (bal > 1 && a.salariu < root->left->info.salariu) return rotD(root);
    if (bal < -1 && a.salariu > root->right->info.salariu) return rotS(root);
    if (bal > 1 && a.salariu > root->left->info.salariu) { root->left = rotS(root->left); return rotD(root); }
    if (bal < -1 && a.salariu < root->right->info.salariu) { root->right = rotD(root->right); return rotS(root); }

    return root;
}

void printAVL(NodeAVL* root, FILE* fout) {
    if (root) {
        printAVL(root->left, fout);
        fprintf(fout, "AVL Salariu: %d RON -> %s\n", root->info.salariu, root->info.nume);
        printAVL(root->right, fout);
    }
}

// ==========================================
// 5. MAIN
// ==========================================
int main() {
    FILE* fin = fopen("angajati.txt", "r");
    FILE* fout = fopen("raport.out.txt", "w");
    Angajat a;

    NodeH* ht[H_SIZE] = { NULL };
    Heap hp; hp.size = 0; hp.arr = (Angajat*)malloc(100 * sizeof(Angajat));
    NodeB* bst = NULL;
    NodeAVL* avl = NULL;

    if (!fin || !fout) return 1;

    // Citim angajații
    while (fscanf(fin, "%d %49s %d", &a.id, a.nume, &a.salariu) == 3) {
        insertHash(ht, a);
        insertHeap(&hp, a);
        bst = insertBST(bst, a);
        avl = insertAVL(avl, a);
    }

    fprintf(fout, "=== RAPORT ANGAJATI ===\n\n");

    // Test HashTable
    searchHash(ht, 1008, fout);

    // Test BST conditionat
    fprintf(fout, "\n-- Angajati cu salariu > 5000 (din BST) --\n");
    printBSTConditionat(bst, 5000, fout);

    // Test AVL
    fprintf(fout, "\n-- Toti Angajatii ordonati dupa salariu (din AVL) --\n");
    printAVL(avl, fout);

    // Test Extragere Heap
    fprintf(fout, "\n-- Top 3 Salarii (Extrase din Heap) --\n");
    for (int i = 0; i < 3 && hp.size > 0; i++) {
        a = extractHeap(&hp);
        fprintf(fout, "Locul %d: %s (%d RON)\n", i + 1, a.nume, a.salariu);
    }

    fclose(fin);
    fclose(fout);
    free(hp.arr);

    return 0;
}