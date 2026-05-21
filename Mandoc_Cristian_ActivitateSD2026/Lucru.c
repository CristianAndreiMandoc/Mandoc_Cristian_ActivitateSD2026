//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//// ==========================================
//// 0. DATELE (Entitatea Avion)
//// ==========================================
//typedef struct {
//    int id;
//    char model[50];
//    int capacitate; // Numar de locuri (va fi folosit pentru salarii/incasari)
//} Avion;
//
//// ==========================================
//// 1. HASH TABLE (Cautare rapida dupa ID)
//// ==========================================
//#define H_SIZE 10
//typedef struct NodeH {
//    Avion info;
//    struct NodeH* next;
//} NodeH;
//
//void insertHash(NodeH** ht, Avion a) {
//    int p = a.id % H_SIZE;
//    NodeH* nou = (NodeH*)malloc(sizeof(NodeH));
//    nou->info = a;
//    nou->next = ht[p];
//    ht[p] = nou;
//}
//
//void searchHash(NodeH** ht, int id, FILE* fout) {
//    NodeH* temp = ht[id % H_SIZE];
//    while (temp) {
//        if (temp->info.id == id) {
//            fprintf(fout, "Gasit Avion in Hash: %s (Capacitate: %d locuri)\n", temp->info.model, temp->info.capacitate);
//            return;
//        }
//        temp = temp->next;
//    }
//    fprintf(fout, "Avionul cu ID %d nu a fost gasit.\n", id);
//}
//
//// ==========================================
//// 2. MAX-HEAP (Cele mai incapatatoare avioane)
//// ==========================================
//typedef struct {
//    Avion* arr;
//    int size;
//} Heap;
//
//void insertHeap(Heap* h, Avion a) {
//    int i = h->size++;
//    int p = (i - 1) / 2;
//    Avion aux;
//    h->arr[i] = a;
//
//    // Comparam capacitatile (ca la salariu)
//    while (i > 0 && h->arr[i].capacitate > h->arr[p].capacitate) {
//        aux = h->arr[i]; h->arr[i] = h->arr[p]; h->arr[p] = aux;
//        i = p; p = (i - 1) / 2;
//    }
//}
//
//Avion extractHeap(Heap* h) {
//    Avion max = h->arr[0];
//    Avion aux;
//    int i = 0, st, dr, m_idx;
//    h->arr[0] = h->arr[--h->size];
//
//    while (1) {
//        st = 2 * i + 1; dr = 2 * i + 2; m_idx = i;
//        if (st < h->size && h->arr[st].capacitate > h->arr[m_idx].capacitate) m_idx = st;
//        if (dr < h->size && h->arr[dr].capacitate > h->arr[m_idx].capacitate) m_idx = dr;
//        if (m_idx == i) break;
//
//        aux = h->arr[i]; h->arr[i] = h->arr[m_idx]; h->arr[m_idx] = aux;
//        i = m_idx;
//    }
//    return max;
//}
//
//// ==========================================
//// 3. BST (Ordonat alfabetic dupa Model)
//// ==========================================
//typedef struct NodeB {
//    Avion info;
//    struct NodeB* left, * right;
//} NodeB;
//
//NodeB* insertBST(NodeB* root, Avion a) {
//    if (!root) {
//        NodeB* nou = (NodeB*)malloc(sizeof(NodeB));
//        nou->info = a; nou->left = nou->right = NULL;
//        return nou;
//    }
//    // Ordonam dupa nume (model) folosind strcmp
//    if (strcmp(a.model, root->info.model) < 0) root->left = insertBST(root->left, a);
//    else if (strcmp(a.model, root->info.model) > 0) root->right = insertBST(root->right, a);
//    return root;
//}
//
//void printBSTConditionat(NodeB* root, int prag_capacitate, FILE* fout) {
//    if (root) {
//        printBSTConditionat(root->left, prag_capacitate, fout);
//        if (root->info.capacitate > prag_capacitate) {
//            fprintf(fout, "Capacitate mare (>%d): %s\n", prag_capacitate, root->info.model);
//        }
//        printBSTConditionat(root->right, prag_capacitate, fout);
//    }
//}
//
//// ==========================================
//// 4. AVL (Ordonat dupa Capacitate)
//// ==========================================
//typedef struct NodeAVL {
//    Avion info;
//    struct NodeAVL* left, * right;
//    int ht;
//} NodeAVL;
//
//int maxV(int a, int b) { return a > b ? a : b; }
//int getH(NodeAVL* n) { return n ? n->ht : 0; }
//int getBal(NodeAVL* n) { return n ? getH(n->left) - getH(n->right) : 0; }
//
//NodeAVL* rotD(NodeAVL* y) {
//    NodeAVL* x = y->left; y->left = x->right; x->right = y;
//    y->ht = maxV(getH(y->left), getH(y->right)) + 1;
//    x->ht = maxV(getH(x->left), getH(x->right)) + 1;
//    return x;
//}
//
//NodeAVL* rotS(NodeAVL* x) {
//    NodeAVL* y = x->right; x->right = y->left; y->left = x;
//    x->ht = maxV(getH(x->left), getH(x->right)) + 1;
//    y->ht = maxV(getH(y->left), getH(y->right)) + 1;
//    return y;
//}
//
//NodeAVL* insertAVL(NodeAVL* root, Avion a) {
//    int bal;
//    if (!root) {
//        NodeAVL* nou = (NodeAVL*)malloc(sizeof(NodeAVL));
//        nou->info = a; nou->left = nou->right = NULL; nou->ht = 1;
//        return nou;
//    }
//
//    // Inseram dupa capacitate
//    if (a.capacitate < root->info.capacitate) root->left = insertAVL(root->left, a);
//    else if (a.capacitate > root->info.capacitate) root->right = insertAVL(root->right, a);
//    else return root;
//
//    root->ht = 1 + maxV(getH(root->left), getH(root->right));
//    bal = getBal(root);
//
//    if (bal > 1 && a.capacitate < root->left->info.capacitate) return rotD(root);
//    if (bal < -1 && a.capacitate > root->right->info.capacitate) return rotS(root);
//    if (bal > 1 && a.capacitate > root->left->info.capacitate) { root->left = rotS(root->left); return rotD(root); }
//    if (bal < -1 && a.capacitate < root->right->info.capacitate) { root->right = rotD(root->right); return rotS(root); }
//
//    return root;
//}
//
//void printAVL(NodeAVL* root, FILE* fout) {
//    if (root) {
//        printAVL(root->left, fout);
//        fprintf(fout, "AVL Capacitate: %d locuri -> %s\n", root->info.capacitate, root->info.model);
//        printAVL(root->right, fout);
//    }
//}
//
//// ==========================================
//// 5. MAIN
//// ==========================================
//int main() {
//    FILE* fin = fopen("avioane.txt", "r");
//    FILE* fout = fopen("raport.out.txt", "w");
//    Avion a;
//
//    NodeH* ht[H_SIZE] = { NULL };
//    Heap hp; hp.size = 0; hp.arr = (Avion*)malloc(100 * sizeof(Avion));
//    NodeB* bst = NULL;
//    NodeAVL* avl = NULL;
//
//    if (!fin || !fout) return 1;
//
//    // Citim avioanele
//    while (fscanf(fin, "%d %49s %d", &a.id, a.model, &a.capacitate) == 3) {
//        insertHash(ht, a);
//        insertHeap(&hp, a);
//        bst = insertBST(bst, a);
//        avl = insertAVL(avl, a);
//    }
//
//    fprintf(fout, "=== RAPORT AVIOANE ===\n\n");
//
//    // Test HashTable
//    searchHash(ht, 102, fout); // Inlocuieste 102 cu un ID pe care il pui tu in fisier
//
//    // Test BST conditionat
//    fprintf(fout, "\n-- Avioane cu capacitate > 200 (din BST) --\n");
//    printBSTConditionat(bst, 200, fout);
//
//    // Test AVL
//    fprintf(fout, "\n-- Toate Avioanele ordonate dupa capacitate (din AVL) --\n");
//    printAVL(avl, fout);
//
//    // Test Extragere Heap
//    fprintf(fout, "\n-- Top 3 Cele mai mari avioane (Extrase din Heap) --\n");
//    for (int i = 0; i < 3 && hp.size > 0; i++) {
//        a = extractHeap(&hp);
//        fprintf(fout, "Locul %d: %s (%d locuri)\n", i + 1, a.model, a.capacitate);
//    }
//
//    fclose(fin);
//    fclose(fout);
//    free(hp.arr);
//
//    printf("Toate structurile au rulat perfect! Verifica raport.out.txt\n");
//    return 0;
//}