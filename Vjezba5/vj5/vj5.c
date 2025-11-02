/*
5. Napisati program koji iz datoteke čita postfiks izraz i zatim korištenjem stoga računa
rezultat. Stog je potrebno realizirati preko vezane liste.
*/

#include <stdio.h>
#include <stdlib.h>

//Struktura liste
typedef struct List* Stack;
typedef struct List {
	int number;
	Stack next;
} List;

//Funkcije
int push(Stack, int);
int pop(Stack);
int evaluatePostfix(const char*);

int main() {
	const char* filename = "postfix.txt"; //Ime datoteke s postfiks izrazom
	int result = evaluatePostfix(filename);
	printf("Rjesenje zadanog postfix izraza je: %d\n", result);
	return 0;
}

//Funkcija za dodavanje elementa na stog
int push(Stack stack, int number) {
	Stack newNode = (Stack)malloc(sizeof(List));
	if (newNode == NULL) {
		printf("Greska pri alokaciji memorije!\n");
		return EXIT_FAILURE;
	}
	newNode->number = number;
	newNode->next = stack->next;
	stack->next = newNode;
	return EXIT_SUCCESS;
}

//Funkcija za uklanjanje elementa sa stoga
int pop(Stack stack) {
	if (stack->next == NULL) {
		printf("Stog je prazan!\n");
		exit(EXIT_FAILURE);
	}
	Stack temp = stack->next;
	int number = temp->number;
	stack->next = temp->next;
	free(temp);
	return number;
}

//Funkcija za evaluaciju postfiks izraza iz datoteke
int evaluatePostfix(const char* filename) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("Greska pri otvaranju datoteke!\n");
		return EXIT_FAILURE;
	}
	Stack stack = (Stack)malloc(sizeof(List));
	if (stack == NULL) {
		printf("Greska pri alokaciji memorije!\n");
		return EXIT_FAILURE;
	}
	stack->next = NULL;
	char ch;
	while ((ch = fgetc(file)) != EOF) { //Citanje karaktera iz datoteke
		if (ch >= '0' && ch <= '9') { //Ako je element broj
			push(stack, ch - '0');
		} else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') { //Ako je element operator
			int b = pop(stack);
			int a = pop(stack);
			int result;
			switch (ch) {
			case '+':
				result = a + b;
				break;
			case '-':
				result = a - b;
				break;
			case '*':
				result = a * b;
				break;
			case '/':
				if (b == 0) {
					printf("Dijeljenje s nulom!\n");
					return EXIT_FAILURE;
				}
				result = a / b; 
				break;
			default:
				printf("Nepoznat operator: %c\n", ch);
				return EXIT_FAILURE;
			}
			push(stack, result);
		}
	}
	int finalResult = pop(stack);
	if (stack->next != NULL) {
		printf("Greska: Stog nije prazan nakon evaluacije!\n");
		return EXIT_FAILURE;
	}
	free(stack);
	fclose(file);
	return finalResult;
}