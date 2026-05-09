//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <stdbool.h>
//
//#define ALPHABET_SIZE 26
//
//typedef struct TrieNode {
//    struct TrieNode* children[ALPHABET_SIZE];
//    bool isEndOfWord;
//} TrieNode;
//
//typedef struct DLLNode {
//    char query[100];
//    struct DLLNode* prev;
//    struct DLLNode* next;
//} DLLNode;
//
//typedef struct {
//    DLLNode* head;
//    DLLNode* tail;
//    int size;
//    int capacity;
//} HistoryQueue;
//
//TrieNode* createTrieNode() {
//    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
//    node->isEndOfWord = false;
//    for (int i = 0; i < ALPHABET_SIZE; i++) {
//        node->children[i] = NULL;
//    }
//    return node;
//}
//
//void insertWord(TrieNode* root, const char* word) {
//    TrieNode* current = root;
//    for (int i = 0; word[i] != '\0'; i++) {
//        int index = word[i] - 'a';
//        if (!current->children[index]) {
//            current->children[index] = createTrieNode();
//        }
//        current = current->children[index];
//    }
//    current->isEndOfWord = true;
//}
//
//bool searchWord(TrieNode* root, const char* word) {
//    TrieNode* current = root;
//    for (int i = 0; word[i] != '\0'; i++) {
//        int index = word[i] - 'a';
//        if (!current->children[index]) return false;
//        current = current->children[index];
//    }
//    return current != NULL && current->isEndOfWord;
//}
//
//void dfsTrie(TrieNode* node, char* buffer, int depth) {
//    if (node->isEndOfWord) {
//        buffer[depth] = '\0';
//        printf(" -> %s\n", buffer);
//    }
//    for (int i = 0; i < ALPHABET_SIZE; i++) {
//        if (node->children[i]) {
//            buffer[depth] = i + 'a';
//            dfsTrie(node->children[i], buffer, depth + 1);
//        }
//    }
//}
//
//void autocomplete(TrieNode* root, const char* prefix) {
//    TrieNode* current = root;
//    for (int i = 0; prefix[i] != '\0'; i++) {
//        int index = prefix[i] - 'a';
//        if (!current->children[index]) {
//            printf("Fara sugestii pentru: %s\n", prefix);
//            return;
//        }
//        current = current->children[index];
//    }
//
//    char buffer[100];
//    strcpy(buffer, prefix);
//    printf("Sugestii pentru '%s':\n", prefix);
//    dfsTrie(current, buffer, strlen(prefix));
//}
//
//HistoryQueue* createHistory(int capacity) {
//    HistoryQueue* hq = (HistoryQueue*)malloc(sizeof(HistoryQueue));
//    hq->head = hq->tail = NULL;
//    hq->size = 0;
//    hq->capacity = capacity;
//    return hq;
//}
//
//void addHistory(HistoryQueue* hq, const char* query) {
//    DLLNode* newNode = (DLLNode*)malloc(sizeof(DLLNode));
//    strcpy(newNode->query, query);
//    newNode->next = hq->head;
//    newNode->prev = NULL;
//
//    if (hq->head != NULL) hq->head->prev = newNode;
//    hq->head = newNode;
//
//    if (hq->tail == NULL) hq->tail = newNode;
//
//    hq->size++;
//
//    if (hq->size > hq->capacity) {
//        DLLNode* temp = hq->tail;
//        hq->tail = hq->tail->prev;
//        hq->tail->next = NULL;
//        free(temp);
//        hq->size--;
//    }
//}
//
//void printHistory(HistoryQueue* hq) {
//    DLLNode* current = hq->head;
//    printf("Istoric cautari recente:\n");
//    while (current != NULL) {
//        printf(" [ %s ] ", current->query);
//        current = current->next;
//    }
//    printf("\n");
//}
//
//int main() {
//    TrieNode* dictionary = createTrieNode();
//    insertWord(dictionary, "algoritm");
//    insertWord(dictionary, "algebra");
//    insertWord(dictionary, "alocare");
//    insertWord(dictionary, "arbore");
//    insertWord(dictionary, "arhitectura");
//    insertWord(dictionary, "baza");
//    insertWord(dictionary, "bifurcatie");
//
//    HistoryQueue* history = createHistory(3);
//
//    printf("--- SISTEM DE CAUTARE ---\n\n");
//
//    const char* q1 = "alg";
//    addHistory(history, q1);
//    autocomplete(dictionary, q1);
//    printf("\n");
//
//    const char* q2 = "ar";
//    addHistory(history, q2);
//    autocomplete(dictionary, q2);
//    printf("\n");
//
//    const char* q3 = "b";
//    addHistory(history, q3);
//    autocomplete(dictionary, q3);
//    printf("\n");
//
//    const char* q4 = "alocare";
//    addHistory(history, q4);
//
//    printHistory(history);
//
//    return 0;
//}