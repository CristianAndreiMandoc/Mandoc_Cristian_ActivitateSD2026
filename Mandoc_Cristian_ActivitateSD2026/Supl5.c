#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Produs Produs;
typedef struct Nod Nod;
typedef struct HashTable HashTable;

struct Produs {
	char* nume;
	int cod;
	float pret;
};

struct Nod {
	Produs info;
	Nod* next;
};

struct HashTable {
	int dimensiune;
	Nod** vector;
};

Produs initProdus(const char* nume, int cod, float pret) {
	Produs p;
	p.cod = cod;
	p.pret = pret;

	if (nume != NULL) {
		p.nume = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
		strcpy(p.nume, nume);
	}
	else {
		p.nume = NULL;
	}

	return p;
}

void afisareProdus(Produs p) {
	printf("\n[Cod: %d] Produs: %s | Pret: %.2f lei", p.cod, p.nume, p.pret);
}

void afisareListaProduse(Nod* cap) {
	while (cap != NULL) {
		afisareProdus(cap->info);
		cap = cap->next;
	}
}

void inserareLaSfarsit(Nod** cap, Produs p) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->next = NULL;
	nou->info = p;

	if (*cap == NULL) {
		*cap = nou;
	}
	else {
		Nod* aux = *cap;
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = nou;
	}
}

HashTable initHashTable(int size) {
	HashTable tabela;
	tabela.dimensiune = size;
	tabela.vector = (Nod**)malloc(sizeof(Nod*) * size);
	for (int i = 0; i < size; i++) {
		tabela.vector[i] = NULL;
	}
	return tabela;
}

// Functia de Hash calculeaza pozitia pe baza codului produsului
int hash(int dim, int codProdus) {
	return codProdus % dim;
}

void inserareHashTable(HashTable tabela, Produs p) {
	if (tabela.dimensiune > 0) {
		int pozitie = hash(tabela.dimensiune, p.cod);
		if (pozitie >= 0 && pozitie < tabela.dimensiune) {
			inserareLaSfarsit(&(tabela.vector[pozitie]), p);
		}
	}
}

void afisareHashTable(HashTable tabela) {
	for (int i = 0; i < tabela.dimensiune; i++) {
		printf("\n--- Pozitie Hash: %d ---", i);
		if (tabela.vector[i] != NULL) {
			afisareListaProduse(tabela.vector[i]);
		}
		else {
			printf("\n(lista goala)");
		}
		printf("\n");
	}
}

// Calculeaza valoarea totala (pretul) produselor de pe o anumita pozitie
float calculValoareProduseLista(HashTable tabela, int pozitie) {
	float total = 0;
	if (pozitie >= 0 && pozitie < tabela.dimensiune) {
		Nod* cap = tabela.vector[pozitie];
		while (cap != NULL) {
			total += cap->info.pret;
			cap = cap->next;
		}
	}
	return total;
}

void dezalocareLista(Nod** cap) {
	while ((*cap) != NULL) {
		free((*cap)->info.nume); // eliberam string-ul
		Nod* copie = *cap;
		*cap = (*cap)->next;
		free(copie); // eliberam nodul
	}
}

void dezalocareHashTable(HashTable tabela) {
	for (int i = 0; i < tabela.dimensiune; i++) {
		dezalocareLista(&(tabela.vector[i]));
	}
	free(tabela.vector);
}

int main() {
	// Initializam tabela cu 5 pozitii (de la 0 la 4)
	HashTable tabela = initHashTable(5);

	// Inseram produse. Observa cum codurile (100, 101, 105) vor genera coliziuni sau se vor duce pe pozitii diferite.
	// Ex: 100 % 5 = 0; 105 % 5 = 0 -> Coliziune pe pozitia 0.
	inserareHashTable(tabela, initProdus("Telefon", 100, 2500.0));
	inserareHashTable(tabela, initProdus("Incarcator", 101, 100.0));
	inserareHashTable(tabela, initProdus("Husa", 102, 50.0));
	inserareHashTable(tabela, initProdus("Tableta", 105, 1800.0));

	printf("\n=== Tabela de Dispersie ===");
	afisareHashTable(tabela);

	printf("\nValoarea totala a produselor de pe pozitia 0 este: %.2f lei", calculValoareProduseLista(tabela, 0));

	dezalocareHashTable(tabela);

	printf("\n\n");
	return 0;
}