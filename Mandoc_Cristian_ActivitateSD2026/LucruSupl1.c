#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define _CRT_SECURE_NO_WARNINGS

struct Vehicul {
	int id;
	int putereCp;
	char* marca;
	float pret;
	char seria;
};

struct Vehicul initializare(int id, int putereCp, const char* marca, float pret, char seria) {

	struct Vehicul v;
	v.id = id;
	v.putereCp = putereCp;
	v.marca = (char*)malloc(sizeof(char) * (strlen(marca) + 1)) ;
	strcpy_s(v.marca, strlen(marca) + 1, marca);
	v.pret = pret;
	v.seria = seria;
	return v;
}

void afisare(struct Vehicul v) {
	if (v.marca != NULL) 
		printf("%d Vehiculul %s cu seria %c are %d cai putere si pretul de %2.5f./n ", v.id, v.marca, v.seria, v.putereCp, v.pret);
	else
		printf("%d Vehiculul cu seria %c are %d cai putere si pretul de %2.5f./n ", v.id, v.seria, v.putereCp, v.pret);
	
}

void modificarePret(struct Vehicul* v, float pretNou) {
	if (pretNou > 0) {
		v->pret = pretNou;
	}
}

void dezalocare(struct Vehicul* v) {
	if (v->marca != NULL) {
		free(v->marca);
		v->marca = NULL;
	}
}


int main()
{
	struct Vehicul v;
	v = initializare(1, 150, "Audi", 20345.3, 'A');
	afisare(v);
	modificarePret(&v, 18997.5);
	afisare(v);
	dezalocare(&v);
	afisare(v);

	return 0;
}