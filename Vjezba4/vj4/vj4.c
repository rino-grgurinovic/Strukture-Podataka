/*
4. Napisati program za zbrajanje i množenje polinoma. Koeficijenti i eksponenti se
čitaju iz datoteke.
Napomena: Eksponenti u datoteci nisu nužno sortirani.
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Struktura polinoma
typedef struct Polynomial* Position;
typedef struct Polynomial {
	int coefficient;
	int exponent;
	Position next;
} Polynomial;

//Funkcije
Position createTerm(int, int);
int insertTerm(Position, int, int);
int readPolynomialFromFile(Position, const char*);
int printPolynomial(Position);
Position addPolynomials(Position, Position);
Position multiplyPolynomials(Position, Position);
int freePolynomial(Position);

int main() {
	Position firstPoly = createTerm(0, 0); //Stvaranje praznog polinoma 1
	Position secondPoly = createTerm(0, 0); //Stvaranje praznog polinoma 2
	readPolynomialFromFile(firstPoly, "poly1.txt");
	readPolynomialFromFile(secondPoly, "poly2.txt");
	printf("Polinom 1: ");
	printPolynomial(firstPoly);
	printf("Polinom 2: ");
	printPolynomial(secondPoly);
	Position sumOfPolys = addPolynomials(firstPoly, secondPoly); //Stvaranje polinoma za zbroj
	printf("Zbroj: ");
	printPolynomial(sumOfPolys);
	Position productOfPolys = multiplyPolynomials(firstPoly, secondPoly); //Stvaranje polinoma za umnozak
	printf("Umnozak: ");
	printPolynomial(productOfPolys);
	freePolynomial(firstPoly);
	freePolynomial(secondPoly);
	freePolynomial(sumOfPolys);
	freePolynomial(productOfPolys);
	return 0;
}

//Funkcija za stvaranje novog clana polinoma
Position createTerm(int coeff, int exp) {
	Position newTerm = (Position)malloc(sizeof(Polynomial));
	if (!newTerm) {
		printf("Greska pri alokaciji memorije!\n");
		return EXIT_FAILURE;
	}
	newTerm->coefficient = coeff;
	newTerm->exponent = exp;
	newTerm->next = NULL;
	return newTerm;
}

//Funkcija za umetanje clana u polinom
int insertTerm(Position head, int coeff, int exp) {
	Position newTerm = createTerm(coeff, exp);
	Position current = head;
	while (current->next && current->next->exponent > exp) { //Sortirano umetanje
		current = current->next;
	}
	if (current->next && current->next->exponent == exp) { //Ako vec postoji clan s istim eksponentom, zbrajanje koeficijenata
		current->next->coefficient += coeff;
		free(newTerm);
	}
	else { //Umetanje novog clana
		newTerm->next = current->next;
		current->next = newTerm;
	}
	return EXIT_SUCCESS;
}

//Funkcija za citanje polinoma iz datoteke
int readPolynomialFromFile(Position head, const char* filename) {
	FILE* file = fopen(filename, "r");
	if (!file) {
		printf("Greska pri otvaranju datoteke!\n");
		return EXIT_FAILURE;
	}
	int coeff, exp;
	while (fscanf(file, "%d %d", &coeff, &exp) == 2) { //Citanje koeficijenta i eksponenta
		insertTerm(head, coeff, exp);
	}
	fclose(file);
	return EXIT_SUCCESS;
}

//Funkcija za ispis polinoma
int printPolynomial(Position head) {
	Position current = head->next; //Preskakanje glavnog clana
	while (current) {
		printf("%dx^%d ", current->coefficient, current->exponent);
		if (current->next && current->next->coefficient >= 0) //Dodavanje znaka plus izmedu clanova
			printf("+ ");
		current = current->next;
	}
	printf("\n");
	return EXIT_SUCCESS;
}

//Funkcija za zbrajanje polinoma
Position addPolynomials(Position p1, Position p2) {
	Position result = createTerm(0, 0);
	Position t1 = p1->next;
	Position t2 = p2->next;
	while (t1 || t2) { //Dok ima clanova u barem jednom polinomu
		if (t1 && (!t2 || t1->exponent > t2->exponent)) { //Umetanje clana iz prvog polinoma
			insertTerm(result, t1->coefficient, t1->exponent);
			t1 = t1->next;
		}
		else if (t2 && (!t1 || t2->exponent > t1->exponent)) { //Umetanje clana iz drugog polinoma
			insertTerm(result, t2->coefficient, t2->exponent);
			t2 = t2->next;
		}
		else { //Isti eksponent, zbrajanje koeficijenata
			insertTerm(result, t1->coefficient + t2->coefficient, t1->exponent);
			t1 = t1->next;
			t2 = t2->next;
		}
	}
	return result;
}

//Funkcija za mnozenje polinoma
Position multiplyPolynomials(Position p1, Position p2) {
	Position result = createTerm(0, 0);
	for (Position t1 = p1->next; t1; t1 = t1->next) {
		for (Position t2 = p2->next; t2; t2 = t2->next) { //Mnozenje svakog clana iz prvog polinoma sa svakim clanom iz drugog polinoma
			insertTerm(result, t1->coefficient * t2->coefficient, t1->exponent + t2->exponent);
		}
	}
	return result;
}

//Funkcija za oslobadjanje memorije polinoma
int freePolynomial(Position head) {
	Position current = head;
	while (current) {
		Position temp = current; 
		current = current->next;
		free(temp);
	}
	return EXIT_SUCCESS;
}