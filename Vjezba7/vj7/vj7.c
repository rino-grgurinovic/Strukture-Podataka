/*
7. Napisati program koji pomoću vezanih listi (stabala) predstavlja strukturu direktorija.
Omogućiti unos novih direktorija i pod-direktorija, ispis sadržaja direktorija i
povratak u prethodni direktorij. Točnije program treba preko menija simulirati
korištenje DOS naredbi: 1- "md", 2 - "cd dir", 3 - "cd..", 4 - "dir" i 5 – izlaz.
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Struktura za direktorij
typedef struct Directory* Dir;
typedef struct Directory {
	char name[50];
	Dir parent;
	Dir subdirs;
	Dir next;
} Directory;

//Funkcije
Dir createDirectory(const char*, Dir);
int addSubdirectory(Dir, const char*);
int listDirectories(Dir);

int main() {
	Dir root = createDirectory("root", NULL);
	Dir current = root;
	int choice;
	char dirName[50];
	while (1) {
		printf("\nTrenutni direktorij: %s\n", current->name);
		printf("Meni:\n");
		printf("1 - md (stvori direktorij)\n");
		printf("2 - cd dir (promijeni direktorij)\n");
		printf("3 - cd.. (povratak u prethodni direktorij)\n");
		printf("4 - dir (ispis pod-direktorija)\n");
		printf("5 - izlaz\n");
		printf("Unesite izbor: ");
		scanf("%d", &choice);
		switch (choice) {
		case 1:
			printf("Unesite ime direktorija za stvaranje: ");
			scanf("%s", dirName);
			addSubdirectory(current, dirName);
			break;
		case 2:
			printf("Unesite ime direktorija za promjenu: ");
			scanf("%s", dirName);
			Dir temp = current->subdirs;
			while (temp) {
				if (strcmp(temp->name, dirName) == 0) { //Pronalaženje direktorija
					current = temp;
					break;
				}
				temp = temp->next;
			}
			if (!temp) {
				printf("Direktorij ne postoji.\n");
			}
			break;
		case 3:
			if (current->parent) {
				current = current->parent;
			}
			else {
				printf("Vec ste u korijenskom direktoriju.\n");
			}
			break;
		case 4:
			listDirectories(current);
			break;
		case 5:
			return 0;
		default:
			printf("Nepostojeca opcija. Pokusajte ponovo.\n");
			break;
		}
	}
	return 0;
}

//Funkcija za stvaranje direktorija
Dir createDirectory(const char* name, Dir parent) {
	Dir newDir = (Dir)malloc(sizeof(Directory));
	if (!newDir) {
		printf("Greska pri alokaciji memorije!\n");
		return NULL;
	}
	strcpy(newDir->name, name);
	newDir->parent = parent;
	newDir->subdirs = NULL;
	newDir->next = NULL;
	return newDir;
}

//Funkcija za dodavanje pod-direktorija
int addSubdirectory(Dir current, const char* name) {
	Dir newDir = createDirectory(name, current);
	if (!newDir) {
		return EXIT_FAILURE;
	}
	if (!current->subdirs) { //Ako nema pod-direktorija, dodavanje kao prvog
		current->subdirs = newDir;
	} else {
		Dir temp = current->subdirs; //Inače, dodavanje na kraj liste
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = newDir;
	}
	return EXIT_SUCCESS;
}

//Funkcija za ispis pod-direktorija
int listDirectories(Dir current) {
	Dir temp = current->subdirs;
	if (!temp) {
		printf("Nema pod-direktorija.\n");
		return EXIT_FAILURE;
	}
	while (temp) {
		printf("%s\n", temp->name);
		temp = temp->next;
	}
	return EXIT_SUCCESS;
}