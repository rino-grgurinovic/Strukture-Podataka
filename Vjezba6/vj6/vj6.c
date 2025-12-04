/*
6. Napisati program koji čita datoteku racuni.txt u kojoj su zapisani nazivi svih datoteka koji
predstavljaju pojedini račun. Na početku svake datoteke je zapisan datum u kojem vremenu je
račun izdat u formatu YYYY-MM-DD. Svaki sljedeći red u datoteci predstavlja artikl u formatu
naziv, količina, cijena. Potrebno je formirati vezanu listu računa sortiranu po datumu. Svaki čvor
vezane liste sadržava vezanu listu artikala sortiranu po nazivu artikla. Nakon toga potrebno je
omogućiti upit kojim će korisnik saznati koliko je novaca sveukupno potrošeno na specifični
artikl u određenom vremenskom razdoblju i u kojoj je količini isti kupljen.
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Structura artikla
typedef struct Item* ItemPosition;
typedef struct Item {
	char name[50];
	int amount;
	float price;
	ItemPosition next;
} Item;

//Struktura racuna
typedef struct Receipt* ReceiptPosition;
typedef struct Receipt {
	char date[11];
	ReceiptPosition next;
	ItemPosition items;
} Receipt;

//Funkcije
ReceiptPosition ReadReceiptFromFile(ReceiptPosition, const char*);
ReceiptPosition CreateReceipt(const char*);
ItemPosition CreateItem(const char*, int, float);
int CalculateTotals(ReceiptPosition, const char*, const char*, const char*, int*, float*);
int FreeReceipts(ReceiptPosition);

int main() {
	ReceiptPosition headMain = NULL;
	FILE* racuni = fopen("racuni.txt", "r");
	if (!racuni) {
		printf("Greska pri otvaranju datoteke.\n");
		return 1;
	}
	char fileName[100];
	while (fscanf(racuni, "%s", fileName) != EOF) {
		headMain = ReadReceiptFromFile(headMain, fileName);
		if (!headMain) {
			fclose(racuni);
			return 1;
		}
	}
	//Ispis racuna i artikala za provjeru
	ReceiptPosition tempReceipt = headMain;
	while (tempReceipt) {
		printf("Racun datum: %s\n", tempReceipt->date);
		ItemPosition tempItem = tempReceipt->items;
		while (tempItem) {
			printf("\tArtikl: %s, Kolicina: %d, Cijena: %.2f\n", tempItem->name, tempItem->amount, tempItem->price);
			tempItem = tempItem->next;
		}
		tempReceipt = tempReceipt->next;
	}
	//Unos upita
	char item[50], dateStart[11], dateEnd[11];
	printf("Unesite ime artikla: ");
	scanf("%s", item);
	printf("Unesite pocetni datum (YYYY-MM-DD): ");
	scanf("%s", dateStart);
	printf("Unesite krajnji datum (YYYY-MM-DD): ");
	scanf("%s", dateEnd);
	int totalAmount = 0;
	float totalPrice = 0.0f;
	if (CalculateTotals(headMain, item, dateStart, dateEnd, &totalAmount, &totalPrice) == EXIT_SUCCESS) {
		printf("Ukupna kolicina artikla %s u periodu od %s do %s je: %d\n", item, dateStart, dateEnd, totalAmount);
		printf("Ukupna cijena artikla %s u periodu od %s do %s je: %.2f\n", item, dateStart, dateEnd, totalPrice);
	}
	FreeReceipts(headMain);
	fclose(racuni);
	return 0;
}

//Funkcija za citanje racuna iz datoteke
ReceiptPosition ReadReceiptFromFile(ReceiptPosition head, const char* fileName) {
	FILE* file = fopen(fileName, "r");
	if (!file) {
		printf("Greska pri otvaranju datoteke.\n");
		return NULL;
	}
	char date[11];
	fscanf(file, "%s", date);
	ReceiptPosition newRecipt = CreateReceipt(date);
	if (newRecipt == NULL) {
		fclose(file);
		return NULL;
	}
	if (head == NULL || strcmp(head->date, date) > 0) { //Sortiranje po datumu
		newRecipt->next = head;
		head = newRecipt;
	}
	else {
		ReceiptPosition current = head;
		while (current->next && strcmp(current->next->date, date) < 0) {
			current = current->next;
		}
		newRecipt->next = current->next;
		current->next = newRecipt;
	}
	char name[50];
	int amount;
	float price;
	while (fscanf(file, "%s %d %f", name, &amount, &price) != EOF) {
		ItemPosition newItem = CreateItem(name, amount, price);
		if (newItem == NULL) {
			fclose(file);
			return NULL;
		}
		ItemPosition itemCurrent = newRecipt->items;
		if (itemCurrent == NULL || strcmp(itemCurrent->name, name) > 0) { //Sortiranje artikala po nazivu
			newItem->next = itemCurrent;
			newRecipt->items = newItem;
		}
		else {
			while (itemCurrent->next && strcmp(itemCurrent->next->name, name) < 0) {
				itemCurrent = itemCurrent->next;
			}
			newItem->next = itemCurrent->next;
			itemCurrent->next = newItem;
		}
	}
	fclose(file);
	return head;
}

//Funkcija za kreiranje novog racuna
ReceiptPosition CreateReceipt(const char* date) {
	ReceiptPosition newReceipt = (ReceiptPosition)malloc(sizeof(Receipt));
	if (newReceipt == NULL) {
		printf("Greska pri alokaciji memorije.\n");
		return NULL;
	}
	strcpy(newReceipt->date, date);
	newReceipt->items = NULL;
	newReceipt->next = NULL;
	return newReceipt;
}

//Funkcija za kreiranje novog artikla
ItemPosition CreateItem(const char* name, int amount, float price) {
	ItemPosition newItem = (ItemPosition)malloc(sizeof(Item));
	if (newItem == NULL) {
		printf("Greska pri alokaciji memorije.\n");
		return NULL;
	}
	strcpy(newItem->name, name);
	newItem->amount = amount;
	newItem->price = price;
	newItem->next = NULL;
	return newItem;
}

//Funkcija za izracun ukupne kolicine i cijene artikla u zadanom periodu
int CalculateTotals(ReceiptPosition head, const char* itemName, const char* dateStart, const char* dateEnd, int* totalAmount, float* totalPrice) {
	if (head == NULL || totalAmount == NULL || totalPrice == NULL) {
		return EXIT_FAILURE;
	}
	int flag = 0;
	ReceiptPosition tempReceipt = head;
	while (tempReceipt) {
		if (strcmp(tempReceipt->date, dateStart) >= 0 && strcmp(tempReceipt->date, dateEnd) <= 0) { //Provjera datuma
			ItemPosition tempItem = tempReceipt->items;
			while (tempItem) {
				if (strcmp(tempItem->name, itemName) == 0) { //Provjera imena artikla
					*totalAmount += tempItem->amount;
					*totalPrice += tempItem->amount * tempItem->price;
					flag = 1;
				}
				tempItem = tempItem->next;
			}
		}
		tempReceipt = tempReceipt->next;
	}
	if (!flag) {
		printf("Artikl nije pronaden u zadanom periodu.\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

//Funkcija za oslobadjanje memorije
int FreeReceipts(ReceiptPosition head) {
	ReceiptPosition tempReceipt = head;
	while (tempReceipt) {
		ItemPosition tempItem = tempReceipt->items;
		while (tempItem) {
			ItemPosition currentItem = tempItem;
			tempItem = tempItem->next;
			free(currentItem);
		}
		ReceiptPosition currentReceipt = tempReceipt;
		tempReceipt = tempReceipt->next;
		free(currentReceipt);
	}
	return EXIT_SUCCESS;
}