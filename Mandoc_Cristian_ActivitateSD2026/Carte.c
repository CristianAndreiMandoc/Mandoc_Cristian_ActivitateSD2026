#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==========================================
// 0. DATELE (Entitatea Carte)
// ==========================================
typedef struct {
    int id;
    char titlu[50];
    int nrPagini; // Vom folosi numarul de pagini pentru Heap si AVL
} Carte;

// ==========================================
// 1. HASH TABLE (Cautare rapida dupa ID)
// ==========================================
#define H_SIZE 10
typedef struct NodeH {
    Carte info;
    struct NodeH* next;
} NodeH;

void insertHash(NodeH** ht, Carte c) {
    int p = c.id % H_SIZE;
    NodeH* nou = (NodeH*)malloc(sizeof(NodeH));
    nou->info = c;
    nou->next = ht[p];
    ht[p] = nou;
}

void searchHash(NodeH** ht, int id, FILE* fout) {
    NodeH* temp = ht[id % H_SIZE];
    while (temp) {
        if (temp->info.id == id) {
            fprintf(fout, "Gasit Carte in Hash: %s (%d pagini)\n", temp->info.titlu, temp->info.nrPagini);
            return;
        }
        temp = temp->next;
    }
    fprintf(fout, "Cartea cu ID %d nu a fost gasita.\n", id);
}

// ==========================================
// 2. MAX-HEAP (Cele mai voluminoase carti)
// ==========================================
typedef struct {
    Carte* arr;
    int size;
} Heap;

void insertHeap(Heap* h, Carte c) {
    int i = h->size++;
    int p = (i - 1) / 2;
    Carte aux;
    h->arr[i] = c;

    // Comparam numarul de pagini
    while (i > 0 && h->arr[i].nrPagini > h->arr[p].nrPagini) {
        aux = h->arr[i]; h->arr[i] = h->arr[p]; h->arr[p] = aux;
        i = p; p = (i - 1) / 2;
    }
}

Carte extractHeap(Heap* h) {
    Carte max = h->arr[0];
    Carte aux;
    int i = 0, st, dr, m_idx;
    h->arr[0] = h->arr[--h->size];

    while (1) {
        st = 2 * i + 1; dr = 2 * i + 2; m_idx = i;
        if (st < h->size && h->arr[st].nrPagini > h->arr[m_idx].nrPagini) m_idx = st;
        if (dr < h->size && h->arr[dr].nrPagini > h->arr[m_idx].nrPagini) m_idx = dr;
        if (m_idx == i) break;

        aux = h->arr[i]; h->arr[i] = h->arr[m_idx]; h->arr[m_idx] = aux;
        i = m_idx;
    }
    return max;
}

// ==========================================
// 3. BST (Ordonat alfabetic dupa Titlu)
// ==========================================
typedef struct NodeB {
    Carte info;
    struct NodeB* left, * right;
} NodeB;

NodeB* insertBST(NodeB* root, Carte c) {
    if (!root) {
        NodeB* nou = (NodeB*)malloc(sizeof(NodeB));
        nou->info = c; nou->left = nou->right = NULL;
        return nou;
    }
    // Ordonam dupa titlu folosind strcmp
    if (strcmp(c.titlu, root->info.titlu) < 0) root->left = insertBST(root->left, c);
    else if (strcmp(c.titlu, root->info.titlu) > 0) root->right = insertBST(root->right, c);
    return root;
}

void printBSTConditionat(NodeB* root, int prag_pagini, FILE* fout) {
    if (root) {
        printBSTConditionat(root->left, prag_pagini, fout);
        if (root->info.nrPagini > prag_pagini) {
            fprintf(fout, "Carte mare (>%d pagini): %s (%d pagini)\n", prag_pagini, root->info.titlu, root->info.nrPagini);
        }
        printBSTConditionat(root->right, prag_pagini, fout);
    }
}

// ==========================================
// 4. AVL (Ordonat dupa Numar Pagini)
// ==========================================
typedef struct NodeAVL {
    Carte info;
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

NodeAVL* insertAVL(NodeAVL* root, Carte c) {
    int bal;
    if (!root) {
        NodeAVL* nou = (NodeAVL*)malloc(sizeof(NodeAVL));
        nou->info = c; nou->left = nou->right = NULL; nou->ht = 1;
        return nou;
    }

    // Inseram dupa numarul de pagini
    if (c.nrPagini < root->info.nrPagini) root->left = insertAVL(root->left, c);
    else if (c.nrPagini > root->info.nrPagini) root->right = insertAVL(root->right, c);
    else return root;

    root->ht = 1 + maxV(getH(root->left), getH(root->right));
    bal = getBal(root);

    if (bal > 1 && c.nrPagini < root->left->info.nrPagini) return rotD(root);
    if (bal < -1 && c.nrPagini > root->right->info.nrPagini) return rotS(root);
    if (bal > 1 && c.nrPagini > root->left->info.nrPagini) { root->left = rotS(root->left); return rotD(root); }
    if (bal < -1 && c.nrPagini < root->right->info.nrPagini) { root->right = rotD(root->right); return rotS(root); }

    return root;
}

void printAVL(NodeAVL* root, FILE* fout) {
    if (root) {
        printAVL(root->left, fout);
        fprintf(fout, "AVL Volum: %d pagini -> %s\n", root->info.nrPagini, root->info.titlu);
        printAVL(root->right, fout);
    }
}

// ==========================================
// 5. MAIN
// ==========================================
int main() {
    FILE* fin = fopen("carti.txt", "r");
    FILE* fout = fopen("raport_carti.txt", "w");
    Carte c;

    NodeH* ht[H_SIZE] = { NULL };
    Heap hp; hp.size = 0; hp.arr = (Carte*)malloc(100 * sizeof(Carte));
    NodeB* bst = NULL;
    NodeAVL* avl = NULL;

    if (!fin || !fout) return 1;

    // Citim cartile
    while (fscanf(fin, "%d %49s %d", &c.id, c.titlu, &c.nrPagini) == 3) {
        insertHash(ht, c);
        insertHeap(&hp, c);
        bst = insertBST(bst, c);
        avl = insertAVL(avl, c);
    }

    fprintf(fout, "=== RAPORT BIBLIOTECA ===\n\n");

    // Test HashTable
    searchHash(ht, 202, fout);

    // Test BST conditionat
    fprintf(fout, "\n-- Carti cu peste 300 de pagini (din BST) --\n");
    printBSTConditionat(bst, 300, fout);

    // Test AVL
    fprintf(fout, "\n-- Toate Cartile ordonate crescator dupa Nr. Pagini (din AVL) --\n");
    printAVL(avl, fout);

    // Test Extragere Heap
    fprintf(fout, "\n-- Top 3 Cele mai groase carti (Extrase din Heap) --\n");
    for (int i = 0; i < 3 && hp.size > 0; i++) {
        c = extractHeap(&hp);
        fprintf(fout, "Locul %d: %s (%d pagini)\n", i + 1, c.titlu, c.nrPagini);
    }

    fclose(fin);
    fclose(fout);
    free(hp.arr);

    return 0;
}