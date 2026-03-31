//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//typedef struct {
//    int id;
//    char marca[50];
//    char model[50];
//    int an_fabricatie;
//    float pret;
//} Masina;
//
//typedef struct Nod {
//    Masina date;
//    struct Nod* prev;
//    struct Nod* next;
//} Nod;
//
//Nod* creareNod(Masina m) {
//    Nod* nou = (Nod*)malloc(sizeof(Nod));
//    if (nou == NULL) {
//        exit(1);
//    }
//    nou->date = m;
//    nou->prev = NULL;
//    nou->next = NULL;
//    return nou;
//}
//
//void inserareFinal(Nod** cap, Nod** coada, Masina m) {
//    Nod* nou = creareNod(m);
//    if (*cap == NULL) {
//        *cap = nou;
//        *coada = nou;
//    }
//    else {
//        (*coada)->next = nou;
//        nou->prev = *coada;
//        *coada = nou;
//    }
//}
//
//void afisareInversa(Nod* coada) {
//    Nod* temp = coada;
//    printf("--- Afisare masini (de la coada la cap) ---\n");
//    while (temp != NULL) {
//        printf("ID: %d | %s %s | An: %d | Pret: %.2f\n",
//            temp->date.id, temp->date.marca, temp->date.model,
//            temp->date.an_fabricatie, temp->date.pret);
//        temp = temp->prev;
//    }
//    printf("-------------------------------------------\n");
//}
//
//int citireDinFisier(const char* numeFisier, Masina vector[], int maxElemente) {
//    FILE* fisier = fopen(numeFisier, "r");
//    if (fisier == NULL) {
//        return -1;
//    }
//
//    int n = 0;
//    while (n < maxElemente && fscanf(fisier, "%d %49s %49s %d %f",
//        &vector[n].id, vector[n].marca, vector[n].model,
//        &vector[n].an_fabricatie, &vector[n].pret) == 5) {
//        n++;
//    }
//
//    fclose(fisier);
//    return n;
//}
//
//void filtrareSiScriereInFisier(const char* numeFisier, Nod* cap, int anMinim) {
//    FILE* fisier = fopen(numeFisier, "w");
//    if (fisier == NULL) {
//        return;
//    }
//
//    Nod* temp = cap;
//    fprintf(fisier, "Masini fabricate dupa anul %d:\n\n", anMinim);
//
//    while (temp != NULL) {
//        if (temp->date.an_fabricatie > anMinim) {
//            fprintf(fisier, "%d %s %s %d %.2f\n",
//                temp->date.id, temp->date.marca, temp->date.model,
//                temp->date.an_fabricatie, temp->date.pret);
//        }
//        temp = temp->next;
//    }
//
//    fclose(fisier);
//}
//
//void distrugereLista(Nod** cap, Nod** coada) {
//    Nod* temp = *cap;
//    Nod* urmator;
//    while (temp != NULL) {
//        urmator = temp->next;
//        free(temp);
//        temp = urmator;
//    }
//    *cap = NULL;
//    *coada = NULL;
//}
//
//int main() {
//    Masina parcAuto[100];
//    Nod* capLista = NULL;
//    Nod* coadaLista = NULL;
//    int numarMasini = 0;
//
//    numarMasini = citireDinFisier("masini_in.txt", parcAuto, 100);
//
//    if (numarMasini == -1) {
//        printf("Nu s-a putut deschide fisierul masini_in.txt\n");
//        return 1;
//    }
//
//    for (int i = 0; i < numarMasini; i++) {
//        inserareFinal(&capLista, &coadaLista, parcAuto[i]);
//    }
//
//    afisareInversa(coadaLista);
//
//    filtrareSiScriereInFisier("masini_noi_out.txt", capLista, 2015);
//
//    distrugereLista(&capLista, &coadaLista);
//
//    return 0;
//}