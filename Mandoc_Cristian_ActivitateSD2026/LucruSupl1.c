//#include<stdio.h>
//#include<stdlib.h>
//#define _CRT_SECURE_NO_WARNINGS
//
//struct Student {
//	int id;
//	int grupa;
//	char* nume;
//	float medie;
//	char serie;
//};
//
//struct Student initializare(int id, int grupa, const char* nume, float medie, char serie) {
//
//	struct Student s;
//	s.id = id;
//	s.grupa = grupa;
//	s.nume = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
//	strcpy_s(s.nume, strlen(nume) + 1, nume);
//	s.medie = medie;
//	s.serie = serie;
//
//	return s;
//}
//
//void afisare(struct Student s) {
//	if (s.nume != NULL)
//		printf("%d Stdentul %s din grupa %d, seria %c, are media %2.5f.\n", s.id, s.nume, s.grupa, s.serie, s.medie);
//	else
//		printf("%d Stdentul din grupa %d, seria %c, are media %2.5f.\n", s.id, s.grupa, s.serie, s.medie);
//}
//
//void modificareMedie(struct Student* s, float medieNoua) {
//	if (medieNoua > 0)
//		s->medie = medieNoua;
//}
//
//void dezalocare(struct Student* s) {
//	if (s->nume != NULL) {
//		free(s->nume);
//		s->nume = NULL;
//	}
//}
//
//int main()
//{
//	struct Student s;
//	s = initializare(1, 8402, "Mandoc Andrei", 8.6, 'A');
//	afisare(s);
//	modificareMedie(&s, 9.6);
//	afisare(s);
//	dezalocare(&s);
//	afisare(s);
//
//	return 0;
//}