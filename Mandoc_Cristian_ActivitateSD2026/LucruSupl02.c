//#include<stdio.h>
//#include<stdlib.h>
//#define _CRT_SECURE_NO_WARNINGS
//
//struct Vehicul {
//	int id;
//	int puterecp;
//	char* marca;
//	float pret;
//	char segment;
//};
//
//struct Vehicul initializare(int id, int puterecp, const char* marca, float pret, char segment) {
//	struct Vehicul v;
//	v.id = id;
//	v.puterecp = puterecp;
//	v.marca = (char*)malloc(sizeof(char) * (strlen(marca) + 1));
//	strcpy_s(v.marca, strlen(marca) + 1, marca);
//	v.pret = pret;
//	v.segment = segment;
//
//	return v;
//}
//
//void afisare(struct Vehicul v) {
//	if (v.marca != NULL)
//		printf("Vehiculul cu ID %d, marca %s si segment de piata %c are o putere de %d cai putere si pret de %2.2f euro.\n", v.id, v.marca, v.segment, v.puterecp, v.pret);
//	else
//		printf("Vehiculul cu ID %d si segment de piata %c are o putere de %d cai putere si pret de %2.2f euro.\n", v.id, v.segment, v.puterecp, v.pret);
//}
//
//void afisareVector(struct Vehicul* vector, int nrElemente) {
//	for (int i=0; i < nrElemente; i++) {
//		afisare(vector[i]);
//	}
//}
//
// 
//
//
//int main()
//{
//	struct Vehicul* vehicule = NULL;
//	int nrVehicule = 3;
//	vehicule = (struct Vehicule*)malloc(sizeof(struct Vehicul) * nrVehicule);
//	vehicule[0] = initializare(1, 150, "BMW", 19000.00, 'E');
//	vehicule[1] = initializare(2, 120, "Volkswagen", 8000.00, 'D');
//	vehicule[2] = initializare(3, 200, "Mercedes", 40000.00, 'E');
//
//	afisareVector(vehicule, nrVehicule);
//	
//	return 0;
//}