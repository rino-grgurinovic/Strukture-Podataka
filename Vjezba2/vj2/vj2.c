/*
2. Definirati strukturu osoba (ime, prezime, godina rođenja) i napisati program koji:
A. dinamički dodaje novi element na početak liste,
B. ispisuje listu,
C. dinamički dodaje novi element na kraj liste,
D. pronalazi element u listi (po prezimenu),
E. briše određeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Struktura osobe
typedef struct Person* Position;
typedef struct Person {
	char firstName[20];
	char lastName[30];
	int birthYear;
	Position next;
} Person;

//Funkcije
Position CreatePerson();
Position AddToBeginning(Position);
int PrintList(Position);
Position AddToEnd(Position);
Position FindByLastName(Position);
Position DeleteFromList(Position);

int main() {
	Position head = NULL; //Pokazivac na pocetak liste
	do {
		int choice = 0;
		printf("\nIzbornik:\n");
		printf("1 - Dodavanje na pocetak\n");
		printf("2 - Ispis\n");
		printf("3 - Dodavanje na kraj\n");
		printf("4 - Trazenje po prezimenu\n");
		printf("5 - Brisanje\n");
		printf("Pritisnite bilo koju drugu tipku za izlaz\n");
		scanf("%d", &choice);
		switch (choice)
		{
		case 1:
			head = AddToBeginning(head);
			break;
		case 2:
			PrintList(head);
			break;
		case 3:
			head = AddToEnd(head);
			break;
		case 4:
			FindByLastName(head);
			break;
		case 5:
			head = DeleteFromList(head);
			break;
		default:
			return 0;
		}
	} while (1);
	return 0;
}

//Funkcija za stvaranje nove osobe
Position CreatePerson() {
	Position newPerson = (Position)malloc(sizeof(Person));
	if (newPerson == NULL) {
		printf("Greska pri alokaciji memorije!\n");
		return NULL;
	}
	printf("Unesite ime: ");
	scanf("%s", newPerson->firstName);
	printf("Unesite prezime: ");
	scanf("%s", newPerson->lastName);
	printf("Unesite godinu rodenja: ");
	scanf(" %d", &newPerson->birthYear);
	newPerson->next = NULL; //Inicijalizacija pokazivaca na sljedeci element
	return newPerson;
}

//Funkcija za dodavanje osobe na pocetak liste
Position AddToBeginning(Position head) {
	Position newPerson = CreatePerson();
	if (newPerson == NULL) {
		printf("Greska pri kreiranju nove osobe!\n");
		return head;
	}
	newPerson->next = head; //Novi element pokazuje na trenutni pocetak liste
	return newPerson;
}

//Funkcija za ispis liste
int PrintList(Position head) {
	if (head == NULL) {
		printf("Greska! Nema ljudi za ispisati!\n");
		return 1;
	}
	Position temp = head;
	printf("%-20s %-20s %-20s\n", "IME", "PREZIME", "GODINA RODENJA");
	printf("-----------------------------------------------------------------------\n");
	do {
		printf("%-20s %-20s %04d\n", temp->firstName, temp->lastName, temp->birthYear);
		temp = temp->next;
	} while (temp != NULL);
	return 0;
}

//Funkcija za dodavanje osobe na kraj liste
Position AddToEnd(Position head) {
	Position newPerson = CreatePerson();
	if (newPerson == NULL) {
		printf("Greska pri kreiranju nove osobe!\n");
		return head;
	}
	if (head == NULL) {
		return newPerson;
	}
	Position temp = head;
	while (temp->next != NULL) { //Pronalazi kraj liste
		temp = temp->next;
	}
	temp->next = newPerson; //Novi element dodaje se na kraj liste
	return head;
}

//Funkcija za pronalazenje osobe po prezimenu
Position FindByLastName(Position head) {
	char lastName[30];
	printf("Unesite prezime osobe koju trazite: ");
	scanf("%s", lastName);
	Position current = head;
	while (current != NULL) {
		if (strcmp(current->lastName, lastName) == 0) { //Usporedba prezimena
			printf("Osoba pronadena: %s %s, godina rodenja: %d\n", current->firstName, current->lastName, current->birthYear);
			return current;
		}
		current = current->next;
	}
	printf("Osoba s prezimenom %s nije pronadena.\n", lastName);
	return NULL;
}

//Funkcija za brisanje osobe iz liste
Position DeleteFromList(Position head) {
	if (head == NULL) {
		printf("Greska! Nema upisanih ljudi!\n");
		return NULL;
	}
	char lastName[30];
	printf("Unesite prezime osobe koju zelite izbrisati: ");
	scanf("%s", lastName);
	Position current = head;
	Position previous = NULL;
	while (current != NULL && strcmp(current->lastName, lastName) != 0) { //Trazenje osobe
		previous = current;
		current = current->next;
	}
	if (current == NULL) {
		printf("Osoba s prezimenom %s nije pronadena.\n", lastName);
		return head;
	}
	if (previous == NULL) { //Brisanje prvog elementa
		head = current->next;
	} else {
		previous->next = current->next; //Preskakanje trenutnog elementa
	}
	free(current);
	printf("Osoba s prezimenom %s je izbrisana iz liste.\n", lastName);
	return head;
}