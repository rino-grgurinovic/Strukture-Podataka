/*
2. Definirati strukturu osoba (ime, prezime, godina rođenja) i napisati program koji:
A. dinamički dodaje novi element na početak liste,
B. ispisuje listu,
C. dinamički dodaje novi element na kraj liste,
D. pronalazi element u listi (po prezimenu),
E. briše određeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.

3. Prethodnom zadatku dodati funkcije:
A. dinamički dodaje novi element iza određenog elementa,
B. dinamički dodaje novi element ispred određenog elementa,
C. sortira listu po prezimenima osoba koristeci bubble sort,
D. upisuje listu u datoteku,
E. čita listu iz datoteke.
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
Position PrintList(Position);
Position AddToEnd(Position);
Position FindByLastName(Position);
Position DeleteFromList(Position);
Position AddAfterElement(Position);
Position AddBeforeElement(Position);
Position SortByLastName(Position);
Position AddToFile(Position);
Position ReadFromFile(Position);

int main() {
	Position temp, head = NULL; //Pokazivac na pocetak liste
	do {
		int choice = 0;
		printf("\nIzbornik:\n");
		printf("1 - Dodavanje na pocetak\n");
		printf("2 - Ispis\n");
		printf("3 - Dodavanje na kraj\n");
		printf("4 - Trazenje po prezimenu\n");
		printf("5 - Brisanje\n");
		printf("6 - Dodavanje iza odredenog elementa\n");
		printf("7 - Dodavanje ispred odredenog elementa\n");
		printf("8 - Sortiranje po prezimenima\n");
		printf("9 - Upis u datoteku\n");
		printf("10 - Citanje iz datoteke\n");
		printf("Pritisnite bilo koju drugu tipku za izlaz\n");
		scanf("%d", &choice);
		switch (choice)
		{
		case 1:
			head = AddToBeginning(head);
			break;
		case 2:
			head = PrintList(head);
			break;
		case 3:
			head = AddToEnd(head);
			break;
		case 4:
			temp = FindByLastName(head);
			if (temp != NULL) {
				printf("Osoba pronadena: %s %s, godina rodenja: %d\n", temp->firstName, temp->lastName, temp->birthYear);
			}
			break;
		case 5:
			head = DeleteFromList(head);
			break;
		case 6:
			head = AddAfterElement(head);
			break;
		case 7:
			head = AddBeforeElement(head);
			break;
		case 8:
			head = SortByLastName(head);
			break;
		case 9:
			head = AddToFile(head);
			break;
		case 10:
			head = ReadFromFile(head);
			break;
		default:
			return 0;
		}
	} while (1);
	return 0;
}

//Funkcija za stvaranje nove osobe
Position CreatePerson() {
	Position newPerson = (Position)malloc(sizeof(Person)); //Alokacija memorije za novu osobu
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
	newPerson->next = NULL; //Inicijaliziranje pokazivaca na sljedeci element
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
	head = newPerson; //Pocetak liste sada je novi element
	printf("Osoba uspjesno dodana u listu.\n");
	return head;
}

//Funkcija za ispis liste
Position PrintList(Position head) {
	if (head == NULL) {
		printf("Greska! Nema ljudi za ispisati!\n");
		return NULL;
	}
	Position temp = head;
	printf("%-20s %-20s %-20s\n", "IME", "PREZIME", "GODINA RODENJA");
	printf("-----------------------------------------------------------------------\n");
	do {
		printf("%-20s %-20s %04d\n", temp->firstName, temp->lastName, temp->birthYear);
		temp = temp->next; //Prelazenje na sljedeci element
	} while (temp != NULL);
	return head;
}

//Funkcija za dodavanje osobe na kraj liste
Position AddToEnd(Position head) {
	Position newPerson = CreatePerson();
	if (newPerson == NULL) {
		printf("Greska pri kreiranju nove osobe!\n");
		return head;
	}
	if (head == NULL) { //Ako je lista prazna, novi element postaje pocetak liste
		head = newPerson;
		head->next = NULL;
		printf("Osoba uspjesno dodana u listu.\n");
		return head;
	}
	Position temp = head;
	while (temp->next != NULL) { //Pronalazenje kraja liste
		temp = temp->next;
	}
	temp->next = newPerson; //Dodavanje novog elementa na kraj liste
	printf("Osoba uspjesno dodana u listu.\n");
	return head;
}

//Funkcija za pronalazenje osobe po prezimenu
Position FindByLastName(Position head) {
	if (head == NULL) {
		printf("Greska! Nema upisanih ljudi!\n");
		return NULL;
	}
	char lastName[30];
	printf("Unesite prezime osobe koju trazite: ");
	scanf("%s", lastName);
	Position current = head;
	while (current != NULL) {
		if (strcmp(current->lastName, lastName) == 0) { //Usporedivanje prezimena
			return current;
		}
		current = current->next; //Prelazenje na sljedeci element
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
	Position current = FindByLastName(head);
	if (current == NULL) {
		return head; //Osoba nije pronadena
	}
	Position previous = head;
	while (previous->next != current && previous->next != NULL) { //Pronalazenje prethodnog elementa
		previous = previous->next;
	}
	previous->next = current->next; //Preskakanje trenutnog elementa
	current->next = NULL; //Postavljanje pokazivaca na sljedeci element na NULL
	free(current); //Oslobadanje memorije
	printf("Osoba uspjesno izbrisana iz liste.\n");
	return head;
}

//Funkcija za dodavanje osobe iza odredenog elementa
Position AddAfterElement(Position head) {
	if (head == NULL) {
		printf("Greska! Nema upisanih ljudi!\n");
		return NULL;
	}
	Position target = FindByLastName(head); //Trazenje elementa nakon kojeg se dodaje novi
	if (target == NULL) {
		return head; //Osoba nije pronadena
	}
	Position newPerson = CreatePerson();
	if (newPerson == NULL) {
		printf("Greska pri kreiranju nove osobe!\n");
		return head;
	}
	newPerson->next = target->next; //Novi element pokazuje na sljedeci element ciljanog
	target->next = newPerson; //Ciljani element pokazuje na novi element
	printf("Osoba uspjesno dodana u listu.\n");
	return head;
}

//Funkcija za dodavanje osobe ispred odredenog elementa
Position AddBeforeElement(Position head) {
	if (head == NULL) {
		printf("Greska! Nema upisanih ljudi!\n");
		return NULL;
	}
	Position target = FindByLastName(head); //Trazenje elementa ispred kojeg se dodaje novi
	if (target == NULL) {
		return head; //Osoba nije pronadena
	}
	Position newPerson = CreatePerson();
	if (newPerson == NULL) {
		printf("Greska pri kreiranju nove osobe!\n");
		return head;
	}
	if (head == target) { //Ako je ciljano mjesto pocetak liste
		newPerson->next = head; //Novi element pokazuje na trenutni pocetak liste
		head = newPerson; //Pocetak liste sada je novi element
		printf("Osoba uspjesno dodana u listu.\n");
		return head;
	}
	Position previous = head;
	while (previous->next != target && previous->next != NULL) { //Pronalazenje prethodnog elementa
		previous = previous->next;
	}
	newPerson->next = target; //Novi element pokazuje na ciljani element
	previous->next = newPerson; //Prethodni element pokazuje na novi element
	printf("Osoba uspjesno dodana u listu.\n");
	return head;
}

//Funkcija za sortiranje liste po prezimenima
Position SortByLastName(Position head) {
	if (head == NULL) {
		printf("Greska! Nema upisanih ljudi!\n");
		return NULL;
	}
	if (head->next == NULL) {
		printf("Lista sadrzi samo jedan element.\n");
		return head;
	}
	int swapped;
	do {
		swapped = 0; //Varijabla za provjeru je li doslo do zamjene
		Position current = head;
		Position previous = NULL;
		while (current->next != NULL) {
			if (strcmp(current->lastName, current->next->lastName) > 0) { //Usporedivanje prezimena
				Position temp = current->next;
				current->next = temp->next;
				temp->next = current;
				if (previous == NULL) {
					head = temp; //Azuriranje pocetka liste ako je prvi element zamijenjen
				}
				else {
					previous->next = temp; //Povezivanje prethodnog elementa s novim redoslijedom
				}
				previous = temp; //Azuriranje prethodnog elementa
				swapped = 1; //Postavljanje varijable da je doslo do zamjene
			} else {
				previous = current; //Azuriranje prethodnog elementa
				current = current->next; //Prelazenje na sljedeci element
			}
		}
	} while (swapped); //Ponavljanje dok vise ne bude zamjena
	printf("Lista uspjesno sortirana po prezimenima.\n");
	return head;
}

//Funkcija za upis liste u datoteku
Position AddToFile(Position head) {
	if (head == NULL) {
		printf("Greska! Nema upisanih ljudi!\n");
		return NULL;
	}
	FILE* file = fopen("people.txt", "w"); //Otvaranje datoteke za pisanje
	if (file == NULL) {
		printf("Greska pri otvaranju datoteke!\n");
		return head;
	}
	Position temp = head;
	while (temp != NULL) { //Prolazenje kroz listu i upisivanje u datoteku
		fprintf(file, "%s %s %d\n", temp->firstName, temp->lastName, temp->birthYear);
		temp = temp->next;
	}
	fclose(file); //Zatvaranje datoteke
	printf("Lista uspjesno upisana u datoteku.\n");
	return head;
}

//Funkcija za citanje liste iz datoteke
Position ReadFromFile(Position head) {
	FILE* file = fopen("list.txt", "r"); //Otvaranje datoteke za citanje
	if (file == NULL) {
		printf("Greska pri otvaranju datoteke!\n");
		return head;
	}
	char firstName[20], lastName[30];
	int birthYear;
	while (fscanf(file, "%s %s %d", firstName, lastName, &birthYear) != EOF) { //Citanje iz datoteke
		Position newPerson = (Position)malloc(sizeof(Person)); //Alokacija memorije za novu osobu
		if (newPerson == NULL) {
			printf("Greska pri alokaciji memorije!\n");
			fclose(file); //Zatvaranje datoteke prije izlaza
			return head;
		}
		strcpy(newPerson->firstName, firstName);
		strcpy(newPerson->lastName, lastName);
		newPerson->birthYear = birthYear;
		if (head == NULL) { //Ako je lista prazna, novi element postaje pocetak liste
			head = newPerson;
			head->next = NULL;
		}
		else {
			Position temp = head;
			while (temp->next != NULL) { //Pronalazenje kraja liste
				temp = temp->next;
			}
			temp->next = newPerson; //Dodavanje novog elementa na kraj liste
			newPerson->next = NULL; //Postavljanje pokazivaca na sljedeci element na NULL
		}
	}
	fclose(file); //Zatvaranje datoteke
	printf("Lista uspjesno ucitana iz datoteke.\n");
	return head;
}