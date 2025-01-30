#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define GRID_SIZE 5
#define TREASURES 3
#define TRAPS 3
#define POWER_UPS 2
#define MAX_MOVES 20
#define INITIAL_HEALTH 3

// Function prototypes
void initializeGrid(char grid[GRID_SIZE][GRID_SIZE], bool revealed[GRID_SIZE][GRID_SIZE]);
void placeItems(char grid[GRID_SIZE][GRID_SIZE], char item, int count);
void displayGrid(char grid[GRID_SIZE][GRID_SIZE], bool revealed[GRID_SIZE][GRID_SIZE], int playerX, int playerY, int computerX, int computerY);
bool isValidMove(int x, int y);
int playGame(char grid[GRID_SIZE][GRID_SIZE], bool revealed[GRID_SIZE][GRID_SIZE]);
void computerMove(char grid[GRID_SIZE][GRID_SIZE], bool revealed[GRID_SIZE][GRID_SIZE], int *computerX, int *computerY);

int main() {
    char grid[GRID_SIZE][GRID_SIZE];
    bool revealed[GRID_SIZE][GRID_SIZE] = {false};
    srand(time(NULL));

    initializeGrid(grid, revealed);
    placeItems(grid, 'T', TREASURES); // Place treasures
    placeItems(grid, 'X', TRAPS);     // Place traps
    placeItems(grid, '+', POWER_UPS); // Place power-ups

    printf("Welcome to the Treasure Hunt Game!\n");
    printf("Navigate the grid to find treasures (T), avoid traps (X), and collect power-ups (+).\n");
    printf("You have %d moves to find treasures and avoid traps!\n\n", MAX_MOVES);

    int score = playGame(grid, revealed);

    printf("Game Over! Your final score is: %d\n", score);
    return 0;
}

void initializeGrid(char grid[GRID_SIZE][GRID_SIZE], bool revealed[GRID_SIZE][GRID_SIZE]) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j] = '.';
            revealed[i][j] = false;
        }
    }
}

void placeItems(char grid[GRID_SIZE][GRID_SIZE], char item, int count) {
    while (count > 0) {
        int x = rand() % GRID_SIZE;
        int y = rand() % GRID_SIZE;
        if (grid[x][y] == '.') {
            grid[x][y] = item;
            count--;
        }
    }
}

void displayGrid(char grid[GRID_SIZE][GRID_SIZE], bool revealed[GRID_SIZE][GRID_SIZE], int playerX, int playerY, int computerX, int computerY) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (i == playerX && j == playerY && i == computerX && j == computerY) {
                printf("PC "); // Both player and computer at the same position
            } else if (i == playerX && j == playerY) {
                printf("P "); // Player's position
            } else if (i == computerX && j == computerY) {
                printf("C "); // Computer's position
            } else if (revealed[i][j]) {
                printf("%c ", grid[i][j]);
            } else {
                printf("? ");
            }
        }
        printf("\n");
    }
}

bool isValidMove(int x, int y) {
    return x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE;
}

void computerMove(char grid[GRID_SIZE][GRID_SIZE], bool revealed[GRID_SIZE][GRID_SIZE], int *computerX, int *computerY) {
    int newX, newY;
    do {
        newX = *computerX + (rand() % 3 - 1); // Random move: -1, 0, or +1
        newY = *computerY + (rand() % 3 - 1);
    } while (!isValidMove(newX, newY));

    revealed[newX][newY] = true; // Mark the new position as revealed

    char cell = grid[newX][newY];
    printf("Computer moved to (%d, %d): ", newX, newY);
    if (cell == 'T') {
        printf("found a treasure!\n");
    } else if (cell == 'X') {
        printf("hit a trap!\n");
    } else if (cell == '+') {
        printf("found a power-up!\n");
    } else {
        printf("nothing here.\n");
    }

    *computerX = newX;
    *computerY = newY;
}

int playGame(char grid[GRID_SIZE][GRID_SIZE], bool revealed[GRID_SIZE][GRID_SIZE]) {
    int treasuresFound = 0;
    int powerUps = 0;
    int health = INITIAL_HEALTH;
    int moves = MAX_MOVES;
    int playerX = 0, playerY = 0;
    int computerX = GRID_SIZE - 1, computerY = GRID_SIZE - 1;
    char move;

    revealed[playerX][playerY] = true; // Player's starting position is revealed
    revealed[computerX][computerY] = true; // Computer's starting position is revealed

    while (moves > 0 && health > 0) {
        printf("\nCurrent Grid:\n");
        displayGrid(grid, revealed, playerX, playerY, computerX, computerY);
        printf("\nTreasures found: %d, Power-ups: %d, Health: %d, Remaining moves: %d\n", treasuresFound, powerUps, health, moves);
        printf("Enter move (W/A/S/D): ");
        scanf(" %c", &move);

        // Update player's position
        int newX = playerX;
        int newY = playerY;
        if (move == 'W' || move == 'w') newX--;
        else if (move == 'A' || move == 'a') newY--;
        else if (move == 'S' || move == 's') newX++;
        else if (move == 'D' || move == 'd') newY++;
        else {
            printf("Invalid move!\n");
            continue;
        }

        if (!isValidMove(newX, newY)) {
            printf("Out of bounds!\n");
            continue;
        }

        revealed[newX][newY] = true; // Mark the new position as revealed

        char cell = grid[newX][newY];
        if (cell == 'T') {
            printf("You found a treasure!\n");
            treasuresFound++;
        } else if (cell == 'X') {
            if (powerUps > 0) {
                printf("You hit a trap but used a power-up to survive!\n");
                powerUps--;
            } else {
                printf("You hit a trap! Health reduced by 1.\n");
                health--;
            }
        } else if (cell == '+') {
            printf("You found a power-up!\n");
            powerUps++;
        } else {
            printf("Nothing here.\n");
        }

        playerX = newX;
        playerY = newY;

        // Computer's turn
        computerMove(grid, revealed, &computerX, &computerY);

        moves--;

        if (treasuresFound == TREASURES) {
            printf("Congratulations! You found all the treasures!\n");
            return treasuresFound * 10 + powerUps * 5 + health * 10;
        }
    }

    if (health <= 0) {
        printf("You ran out of health!\n");
    } else {
        printf("You ran out of moves!\n");
    }

    return treasuresFound * 10 + powerUps * 5 + health * 10;
}
