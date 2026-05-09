#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct {
	int id;
	char nume[50];
	int salariu;
}Angajat;

#define H_SIZE 10
typedef struct NodeH {
	Angajat info;
	struct NodeH* next;
}NodeH;

typedef struct {
	Angajat* arr;
	int size;
}Heap;

void insertHash(NodeH** ht, Angajat a) {
	int p = a.id % H_SIZE;
	NodeH* nou = (NodeH*)malloc(sizeof(NodeH));
	nou->info = a;
	nou->next = ht[p];
	ht[p] = nou;
}

void insertHeap(Heap* h, Angajat a) {
	int i = h->size++;
	int p = (i - 1) / 2;
	h->arr[i] = a;
	while (i > 0 && h->arr[i].salariu > h->arr[p].salariu) {
		Angajat aux = h->arr[i]; h->arr[i] = h->arr[p]; h->arr[p] = aux;
		i = p; p = (i - 1) / 2;
	}
}

void searchHash(NodeH** ht, int id, FILE* fout) {
	NodeH* temp = ht[id % H_SIZE];
	while (temp) {
		if (temp->info.id == id) {
			fprintf(fout, " Gasit: %s (%d RON)\n", temp->info.nume, temp->info.salariu);
			return;
		}
		temp = temp->next;

	}
	fprintf(fout, " Nu s-a gasit ID-ul %d\n", id);
}

Angajat extraHeap(Heap* h) {
	Angajat max = h->arr[0];
	h->arr[0] = h->arr[--h->size];
	int i = 0, st, dr, m_idx;
	while (1) {
		st = 2 * i + 1; dr = 2 * i + 2; m_idx = i;
		if (st<h->size && h->arr[st].salariu > h->arr[m_idx].salariu) m_idx = st;
		if (dr<h->size && h->arr[dr].salariu > h->arr[m_idx].salariu) m_idx = dr;
		if (i == m_idx) break;

		Angajat aux = h->arr[i]; h->arr[i] = h->arr[m_idx]; h->arr[m_idx] = aux;
		i = m_idx;

	}
	return max;
}

int main() {
	FILE* fin = fopen("angajati.txt", "r");
	FILE* fout = fopen("raport.txt", "w");
	Angajat a;
	Heap hp;
	hp.size = 0;
	hp.arr = (Angajat*)malloc(100 * sizeof(Angajat));
	NodeH* ht[H_SIZE] = { NULL };
	if (!fin || !fout) return 1;
	while (fscanf(fin, "%d %s %d", &a.id, a.nume, &a.salariu)==3) {
		insertHash(ht, a);
		insertHeap(&hp, a);
	}

	fprintf(fout, "---HashTable---\n");
	searchHash(ht, 1002, fout);

	fprintf(fout, "---Heap---\n");
	while (hp.size > 0) {
		a = extraHeap(&hp);
		fprintf(fout, " %s -> %d RON\n", a.nume, a.salariu);
	}

	free(hp.arr);
	fclose(fin);
	fclose(fout);
	return 0;
}
