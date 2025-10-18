/*
Napisati program koji prvo pročita koliko redaka ima datoteka, tj. koliko ima studenata
zapisanih u datoteci. Nakon toga potrebno je dinamički alocirati prostor za niz struktura
studenata (ime, prezime, bodovi) i učitati iz datoteke sve zapise. Na ekran ispisati ime,
prezime, apsolutni i relativni broj bodova.
Napomena: Svaki redak datoteke sadrži ime i prezime studenta, te broj bodova na kolokviju.
relatvan_br_bodova = br_bodova/max_br_bodova*100
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define MAX_BODOVA 50.0

typedef struct {
	char ime[20];
	char prezime[20];
	int bodovi;
	float relativni;
} Student;

int readFile(char* fileName);
Student* readStudents(char* fileName, int brojStudenata);

int main() {
	int brojStudenata = readFile("studenti.txt");
	printf("Broj studenata: %d\n", brojStudenata);

	Student* studenti = readStudents("studenti.txt", brojStudenata);
	printf("Ime:\t\tPrezime:\tBroj bodova:\tRelativan broj bodova:\n");
	for (int i = 0; i < brojStudenata; i++) {
		printf("%s\t\t%s\t\t%d\t\t%.2f\n", studenti[i].ime, studenti[i].prezime, studenti[i].bodovi, studenti[i].relativni);
	}
	free(studenti);

	return 0;
}

//Otvara file i broji retke/studente
int readFile(char* fileName) {
	FILE* f = fopen(fileName, "r");
	if (f == NULL) {
		printf("Greška pri otvaranju!\n");
		return -1;
	}
	int count = 0;
	char c;
	for (c = getc(f); c != EOF; c = getc(f)) {
		if (c == '\n') {
			count++;
		}
	}
	fclose(f);
	return count;
}

//Otvara file i sprema podatke
Student* readStudents(char* fileName, int brojStudenata) {
	FILE* f = fopen(fileName, "r");
	if (f == NULL) {
		printf("Greška pri otvaranju!\n");
		return NULL;
	}
	Student* studenti = (Student*)malloc(brojStudenata * sizeof(Student));
	if (studenti == NULL) {
		printf("Greška pri alokaciji!\n");
		fclose(f);
		return NULL;
	}
	for (int i = 0; i < brojStudenata; i++) {
		fscanf(f, "%s %s %d", studenti[i].ime, studenti[i].prezime, &studenti[i].bodovi);
		studenti[i].relativni = (float)studenti[i].bodovi / MAX_BODOVA * 100;
	}
	fclose(f);
	return studenti;
}