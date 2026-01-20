#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define ROOM_NUM 10

//Strukture

typedef struct Treasure* TreasurePtr;
typedef struct Treasure {
	TreasurePtr next;
} Treasure;

typedef struct Player* PlayerPtr;
typedef struct Player {
	char class[20];
	int maxHealth;
	int health;
	int attack;
	int critChance;
	int experience;
	int level;
	int score;
	TreasurePtr items;
} Player;

typedef struct Monster* MonsterPtr;
typedef struct Monster {
	char name[20];
	int maxHealth;
	int health;
	int attack;
	int dodgeChance;
} Monster;

typedef struct Room* RoomPtr;
typedef struct Room {
	int objective; // 0 - monster/boss, 1 - treasure
	RoomPtr next;
} Room;

typedef struct Dungeon* DungeonPtr;
typedef struct Dungeon {
	RoomPtr firstRoom;
	DungeonPtr next;
} Dungeon;

//Funkcije

PlayerPtr CreatePlayer();
MonsterPtr CreateMonster(int, int);
TreasurePtr CreateTreasure();
RoomPtr CreateRoom();
DungeonPtr CreateDungeon();
int ChooseClass(PlayerPtr);
int EncounterRoom(PlayerPtr, MonsterPtr);
int TreasureRoom(PlayerPtr);
int CleanupTreasures(TreasurePtr);
int CleanupDungeon(DungeonPtr);

//Main

int main() {
	srand((unsigned int)time(NULL));
	PlayerPtr player = CreatePlayer();
	if (player == NULL) {
		return 1;
	}
	DungeonPtr dungeon = CreateDungeon();
	if (dungeon == NULL) {
		free(player);
		return 1;
	}
	int StageNum = 1;
	while (1)
	{
		RoomPtr currentRoom = dungeon->firstRoom;
		int roomNum = 0;
		while (currentRoom != NULL) {
			system("cls");
			printf("Stage %d\nSoba %d/%d:\n", StageNum, ++roomNum, ROOM_NUM);
			if (roomNum == ROOM_NUM) {
				if (EncounterRoom(player, CreateMonster(1, StageNum)) == 1) {
					CleanupTreasures(player->items);
					free(player);
					free(dungeon);
					return 1;
				}
				if (player->health <= 0) {
					CleanupTreasures(player->items);
					free(player);
					free(dungeon);
					return 0;
				}
			}
			else if (currentRoom->objective == 0) {
				if (EncounterRoom(player, CreateMonster(0, StageNum)) == 1) {
					CleanupTreasures(player->items);
					free(player);
					free(dungeon);
					return 1;
				}
				if (player->health <= 0) {
					CleanupTreasures(player->items);
					free(player);
					free(dungeon);
					return 0;
				}
			}
			else {
				if (TreasureRoom(player) == 1) {
					CleanupTreasures(player->items);
					free(player);
					free(dungeon);
					return 1;
				}
			}
			currentRoom = currentRoom->next;
		}
		dungeon->next = CreateDungeon();
		if (dungeon->next == NULL) {
			CleanupTreasures(player->items);
			free(player);
			CleanupDungeon(dungeon);
			return 1;
		}
		dungeon = dungeon->next;
		StageNum++;
	}
	CleanupTreasures(player->items);
	free(player);
	CleanupDungeon(dungeon);
	return 0;
}

//Funkcija za kreiranje igraca

PlayerPtr CreatePlayer() {
	PlayerPtr player = (PlayerPtr)malloc(sizeof(Player));
	if (player == NULL) {
		printf("Greska pri alokaciji memorije za igraca!\n");
		return NULL;
	}
	ChooseClass(player);
	if (!strcmp(player->class, "Warrior")) {
		player->maxHealth = 150;
		player->attack = 15;
		player->critChance = 10;
	}
	else if (!strcmp(player->class, "Rogue")) {
		player->maxHealth = 100;
		player->attack = 10;
		player->critChance = 25;
	}
	else if (!strcmp(player->class, "Mage")) {
		player->maxHealth = 80;
		player->attack = 20;
		player->critChance = 15;
	}
	player->health = player->maxHealth;
	player->experience = 0;
	player->level = 1;
	player->score = 0;
	player->items = NULL;
	return player;
}

//Funkcija za kreiranje cudovista

MonsterPtr CreateMonster(int isBoss, int difficulty) {
	MonsterPtr monster = (MonsterPtr)malloc(sizeof(Monster));
	if (monster == NULL) {
		printf("Greska pri alokaciji memorije za cudoviste!\n");
		return NULL;
	}
	if (isBoss) {
		strcpy(monster->name, "Boss");
		monster->maxHealth = 100 * difficulty;
		monster->health = monster->maxHealth;
		monster->attack = 20 * difficulty;
		monster->dodgeChance = 20 * difficulty;
		return monster;
	}
	int temp = (rand() % 100);
	if (temp < (85 - 10 * difficulty)) {
		strcpy(monster->name, "Slime");
		monster->maxHealth = 30 * difficulty;
		monster->attack = 5 * difficulty;
		monster->dodgeChance = 0;
	}
	else if (temp < (100 - 5 * difficulty)) {
		strcpy(monster->name, "Goblin");
		monster->maxHealth = 50 * difficulty;
		monster->attack = 10 * difficulty;
		monster->dodgeChance = 0;
	}
	else {
		strcpy(monster->name, "Orc");
		monster->maxHealth = 80 * difficulty;
		monster->attack = 15 * difficulty;
		monster->dodgeChance = 0;
	}
	monster->health = monster->maxHealth;
	return monster;
}

//Funkcija za kreiranje treasurea

TreasurePtr CreateTreasure() {
	TreasurePtr treasure = (TreasurePtr)malloc(sizeof(Treasure));
	if (treasure == NULL) {
		printf("Greska pri alokaciji memorije za treasure!\n");
		return NULL;
	}
	treasure->next = NULL;
	return treasure;
}

//Funkcija za kreiranje sobe

RoomPtr CreateRoom() {
	RoomPtr newRoom = (RoomPtr)malloc(sizeof(Room));
	if (newRoom == NULL) {
		printf("Greska pri alokaciji memorije za sobu!\n");
		return NULL;
	}
	newRoom->objective = rand() % 2;
	newRoom->next = NULL;
	return newRoom;
}

//Funkcija za kreiranje dungeona

DungeonPtr CreateDungeon() {
	DungeonPtr dungeon = (DungeonPtr)malloc(sizeof(Dungeon));
	if (dungeon == NULL) {
		printf("Greska pri alokaciji memorije za dungeon!\n");
		return NULL;
	}
	dungeon->firstRoom = NULL;
	dungeon->next = NULL;
	RoomPtr currentRoom = NULL;
	for (int i = 0; i < ROOM_NUM; i++) {
		RoomPtr newRoom = CreateRoom();
		if (newRoom == NULL) {
			return NULL;
		}
		if (dungeon->firstRoom == NULL) {
			dungeon->firstRoom = newRoom;
			currentRoom = newRoom;
		}
		else {
			currentRoom->next = newRoom;
			currentRoom = newRoom;
		}
	}
	return dungeon;
}

//Funkcija za sobu u kojoj se boris

int EncounterRoom(PlayerPtr player, MonsterPtr monster) {
	if (monster == NULL) {
		return EXIT_FAILURE;
	}
	if (!strcmp(monster->name, "Boss")) {
		printf("Uspjesno si dosao do Bossa! Pripremi se za borbu!\n");
	}
	else {
		printf("Cudoviste %s je ispred tebe! Pripremi se za borbu!\n", monster->name);
	}
	while (monster->health > 0 && player->health > 0) {
		printf("--------------------------------------------------\n");
		printf("Player:\t\t\t%s\n", monster->name);
		printf("Health: %d/%d\t\tHealth: %d/%d\n", player->health, player->maxHealth, monster->health, monster->maxHealth);
		printf("Attack: %d\t\tAttack: %d\n", player->attack, monster->attack);
		printf("Crit Chance: %d\n", player->critChance);
		printf("Level: %d\n", player->level);
		printf("Experience: %d/100\n", player->experience);
		printf("Score: %d\n\n", player->score);
		TreasurePtr currentItem = player->items;
		if (currentItem == NULL) {
			printf("Nemas health potiona.\n");
		}
		else {
			printf("Tvoji health potioni:\n");
			while (currentItem != NULL) {
				printf("- Health potion (+%.0f health)\n", 0.2 * player->maxHealth);
				currentItem = currentItem->next;
			}
		}
		printf("\nOdaberi opciju:\n");
		printf("1 - Napadni\n");
		printf("2 - Koristi health potion\n");
		printf("Unesi izbor: ");
		int choice;
		scanf("%d", &choice);
		getchar();
		if (choice == 1) {
			if (!strcmp(monster->name, "Boss")) {
				if (rand() % 100 < monster->dodgeChance) {
					printf("Boss je izbjegao tvoj napad!\n");
					player->health -= monster->attack;
					continue;
				}
			}
			int damage = player->attack;
			if (rand() % 100 < player->critChance) {
				damage *= 2;
				printf("\nCritical hit!\n");
			}
			monster->health -= damage;
			if (monster->health > 0) {
				player->health -= monster->attack;
			}
			else {
				printf("\nPobijedio si %s!\n", monster->name);
				if (!strcmp(monster->name, "Boss")) {
					player->experience += 50;
					if (player->experience >= 100) {
						LevelUpPlayer(player);
					}
					player->score += 100;
				}
				else {
					player->experience += 20;
					if (player->experience >= 100) {
						LevelUpPlayer(player);
					}
					player->score += 20;
				}
			}
		}
		else if (choice == 2) {
			if (player->items != NULL) {
				player->health += 0.2 * player->maxHealth;
				if (player->health > player->maxHealth) {
					player->health = player->maxHealth;
				}
				TreasurePtr temp = player->items;
				player->items = player->items->next;
				free(temp);
				printf("Koristio si health potion! Tvoj health je sada %d\n", player->health);
				player->health -= monster->attack;
			}
			else {
				printf("Nemas health potiona!\n");
			}
		}
		else {
			printf("Nepostojeca opcija!\n");
		}
		if (player->health <= 0) {
			printf("\nIzgubio si protiv %s!\n", monster->name);
			printf("Tvoj score: %d\n", player->score);
			free(monster);
			return EXIT_SUCCESS;
		}
	}
	free(monster);
	printf("Pritisni Enter za nastavak...\n");
	getchar();
	return EXIT_SUCCESS;
}

//Funkcija za sobu sa treasureom

int TreasureRoom(PlayerPtr player) {
	printf("Nasao si treasure chest!\n");
	if (rand() % 3) {
		printf("Nasao si HEALTH POTION koji te heala za 20% tvog max healtha\n");
		TreasurePtr potion = CreateTreasure();
		if (potion == NULL) {
			return EXIT_FAILURE;
		}
		potion->next = player->items;
		player->items = potion;
	}
	else if (rand() % 2) {
		printf("Nasao si EXPERIENCE ORB koji ti daje 30 expiriencea\n");
		player->experience += 30;
		if (player->experience >= 100) {
			LevelUpPlayer(player);
		}
	}
	else {
		if (rand() % 4) {
			printf("Nasao si CRIT CHANCE BOOST koji ti povecava crit chance za 5\n");
			player->critChance += 5;
		}
		else if (rand() % 2) {
			printf("Nasao si ATTACK BOOST koji ti povecava attack za 5\n");
			player->attack += 5;
		}
		else {
			printf("Nasao si HEALTH BOOST koji ti povecava health za 20\n");
			player->health += 10;
		}
	}
	printf("Pritisni Enter za nastavak...\n");
	getchar();
	return EXIT_SUCCESS;
}

//Funkcija za level up igraca

int LevelUpPlayer(PlayerPtr player) {
	if (player->class == "Warrior") {
		player->maxHealth += 30;
		player->attack += 5;
		player->critChance += 2;
	}
	else if (player->class == "Rogue") {
		player->maxHealth += 20;
		player->attack += 3;
		player->critChance += 5;
	}
	else if (player->class == "Mage") {
		player->maxHealth += 15;
		player->attack += 7;
		player->critChance += 3;
	}
	player->health = player->maxHealth;
	player->experience = 0;
	player->score += 50;
	player->level += 1;
	printf("\nCestitamo! Dosegao si level %d!\n", player->level);
	printf("Vas health je sada %d\n", player->health);
	printf("Vas attack je sada %d\n", player->attack);
	printf("Vas crit chance je sada %d\n", player->critChance);
	return EXIT_SUCCESS;
}

//Funkcija za odabir klase igraca

int ChooseClass(PlayerPtr player) {
	while (1)
	{
		int choice;
		printf("Odaberite klasu:\n");
		printf("1 - Warrior (Visok health, srednji attack, nizak crit chance)\n");
		printf("2 - Rogue (Srednji health, nizak attack, visok crit chance)\n");
		printf("3 - Mage (Nizak health, visok attack, srednji crit chance)\n");
		printf("Unesi izbor: ");
		scanf("%d", &choice);
		getchar();
		if (choice == 1) {
			strcpy(player->class, "Warrior");
			return EXIT_SUCCESS;
		}
		else if (choice == 2) {
			strcpy(player->class, "Rogue");
			return EXIT_SUCCESS;
		}
		else if (choice == 3) {
			strcpy(player->class, "Mage");
			return EXIT_SUCCESS;
		}
		else {
			printf("Nepostojeca opcija! Pokusaj ponovo.\n");
		}
	}
}

//Funkcija za ociscavanje memorije od treasurea

int CleanupTreasures(TreasurePtr head) {
	TreasurePtr current = head;
	while (current != NULL) {
		TreasurePtr temp = current;
		current = current->next;
		free(temp);
	}
	return EXIT_SUCCESS;
}

//Funkcija za ociscavanje memorije od dungeona

int CleanupDungeon(DungeonPtr dungeon) {
	RoomPtr currentRoom = dungeon->firstRoom;
	while (currentRoom != NULL) {
		RoomPtr tempRoom = currentRoom;
		currentRoom = currentRoom->next;
		free(tempRoom);
	}
	free(dungeon);
	return EXIT_SUCCESS;
}