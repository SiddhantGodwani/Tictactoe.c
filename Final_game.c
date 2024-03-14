#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PLAYERS 100
#define MAX_NAME_LENGTH 50

typedef struct {
    char name[MAX_NAME_LENGTH];
    int singleScore;
    int quickScore;
    int tournamentScore;
    int endlessScore;
} Player;

Player players[MAX_PLAYERS];
int numPlayers = 0;

void savePlayerData() {
    FILE* file = fopen("player_data.txt", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    fprintf(file, "%d\n", numPlayers);
    for (int i = 0; i < numPlayers; i++) {
        fprintf(file, "%s %d %d %d %d\n", players[i].name, players[i].singleScore, players[i].quickScore, players[i].tournamentScore, players[i].endlessScore);
    }

    fclose(file);
}

void loadPlayerData() {
    FILE* file = fopen("player_data.txt", "r");
    if (file == NULL) {
        printf("No player data found.\n");
        return;
    }

    fscanf(file, "%d", &numPlayers);
    for (int i = 0; i < numPlayers; i++) {
        fscanf(file, "%s %d %d %d %d", players[i].name, &players[i].singleScore, &players[i].quickScore, &players[i].tournamentScore, &players[i].endlessScore);
    }

    fclose(file);
}

int findPlayerIndex(const char* name) {
    for (int i = 0; i < numPlayers; i++) {
        if (strcmp(players[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

void addPlayer(const char* name) {
    if (numPlayers >= MAX_PLAYERS) {
        printf("Maximum number of players reached.\n");
        return;
    }

    int index = findPlayerIndex(name);
    if (index != -1) {
        printf("Player already exists.\n");
        return;
    }

    strcpy(players[numPlayers].name, name);
    players[numPlayers].singleScore = 0;
    players[numPlayers].quickScore = 0;
    players[numPlayers].tournamentScore = 0;
    players[numPlayers].endlessScore = 0;
    numPlayers++;
    savePlayerData();
}

int getScoreForMode(Player p, const char* mode) {
    if (strcmp(mode, "single") == 0) {
        return p.singleScore;
    } else if (strcmp(mode, "quick") == 0) {
        return p.quickScore;
    } else if (strcmp(mode, "tournament") == 0) {
        return p.tournamentScore;
    } else {
        return p.endlessScore;
    }
}

void showRankings(const char* mode) {
    Player sortedPlayers[MAX_PLAYERS];
    memcpy(sortedPlayers, players, sizeof(players));

    for (int i = 0; i < numPlayers - 1; i++) {
        for (int j = 0; j < numPlayers - i - 1; j++) {
            if (getScoreForMode(sortedPlayers[j], mode) < getScoreForMode(sortedPlayers[j + 1], mode)) {
                Player temp = sortedPlayers[j];
                sortedPlayers[j] = sortedPlayers[j + 1];
                sortedPlayers[j + 1] = temp;
            }
        }
    }

    printf("\nRankings for %s mode:\n", mode);
    int highScore = getScoreForMode(sortedPlayers[0], mode);
    for (int i = 0; i < numPlayers; i++) {
        int score = getScoreForMode(sortedPlayers[i], mode);
        printf("%d. %s - %d", i + 1, sortedPlayers[i].name, score);
        if (score == highScore) {
            printf(" (High Score!)");
        }
        printf("\n");
    }
}

char board[3][3];
int player, computer, currentPlayer;

void resetBoard() {
    int i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            board[i][j] = ' ';
        }
    }
}

void displayBoard() {
    int i, j;
    printf("\n\n");
    for (i = 0; i < 3; i++) {
        printf(" | ");
        for (j = 0; j < 3; j++) {
            printf("%c | ", board[i][j]);
        }
        printf("\n");
    }
}

int checkWin(char symbol) {
    int i;
    // Check rows
    for (i = 0; i < 3; i++) {
        if (board[i][0] == symbol && board[i][1] == symbol && board[i][2] == symbol)
            return 1;
    }

    // Check columns
    for (i = 0; i < 3; i++) {
        if (board[0][i] == symbol && board[1][i] == symbol && board[2][i] == symbol)
            return 1;
    }

    // Check diagonals
    if (board[0][0] == symbol && board[1][1] == symbol && board[2][2] == symbol)
        return 1;
    if (board[0][2] == symbol && board[1][1] == symbol && board[2][0] == symbol)
        return 1;

    return 0;
}

int checkDraw() {
    int i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (board[i][j] == ' ')
                return 0;
        }
    }
    return 1;
}

void playerMove() {
    int row, col;
    printf("Player %s's turn\n", players[currentPlayer].name);
    printf("Enter row (0-2) and column (0-2): ");
    scanf("%d %d", &row, &col);

    if (row < 0 || row > 2 || col < 0 || col > 2 || board[row][col] != ' ') {
        printf("Invalid move! Try again.\n");
        playerMove();
        return;
    }

    board[row][col] = 'X';
}

void computerMove() {
    int i, j, moveFound = 0;

    // Try to win
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = 'O';
                if (checkWin('O')) {
                    moveFound = 1;
                    break;
                }
                board[i][j] = ' ';
            }
        }
        if (moveFound)
            break;
    }

    if (!moveFound) {
        // Try to block the player
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = 'X';
                    if (checkWin('X')) {
                        board[i][j] = 'O';
                        moveFound = 1;
                        break;
                    }
                    board[i][j] = ' ';
                }
            }
            if (moveFound)
                break;
        }
    }

    if (!moveFound) {
        // Take the center if available
        if (board[1][1] == ' ')
            board[1][1] = 'O';
        else {
            // Make a random move
            do {
                i = rand() % 3;
                j = rand() % 3;
            } while (board[i][j] != ' ');
            board[i][j] = 'O';
        }
    }
}

int getPoints(int result) {
    if (result == 1) {
        return 10; // Win
    } else if (result == 0) {
        return 5; // Draw
    } else {
        return -5; // Loss
    }
}

void playSingleMatch() {
    resetBoard();
    player = 1;
    computer = 2;

    do {
        displayBoard();
        playerMove();
        int result = checkWin('X');

        if (!result) {
            computerMove();
            result = checkWin('O');
        }

        if (!result)
            result = checkDraw();

        if (result) {
            displayBoard();
            int points = getPoints(result);
            if (result == 1) {
                printf("You won!\n");
                players[currentPlayer].singleScore += points;
            } else if (result == 2) {
                printf("Computer won!\n");
                players[currentPlayer].singleScore -= points;
            } else {
                printf("It's a draw!\n");
                players[currentPlayer].singleScore += points / 2;
            }
            savePlayerData();
            break;
        }
    } while (!checkWin('X') && !checkWin('O') && !checkDraw());
}

void playQuickMatch() {
    int player1Score = 0, computerScore = 0;

    for (int i = 0; i < 3; i++) {
        playSingleMatch();
        printf("Player %s: %d, Computer: %d\n", players[currentPlayer].name, players[currentPlayer].singleScore, computerScore);
    }

    if (players[currentPlayer].singleScore > computerScore) {
        printf("You won the quick match!\n");
        players[currentPlayer].quickScore += 30;
    } else if (players[currentPlayer].singleScore < computerScore) {
        printf("Computer won the quick match!\n");
        players[currentPlayer].quickScore -= 30;
    } else {
        printf("The quick match was a draw!\n");
    }

    savePlayerData();
}

void playTournamentMatch() {
    int player1Score = 0, computerScore = 0;

    for (int i = 0; i < 10; i++) {
        playSingleMatch();
        printf("Player %s: %d, Computer: %d\n", players[currentPlayer].name, players[currentPlayer].singleScore, computerScore);
    }

    if (players[currentPlayer].singleScore > computerScore) {
        printf("You won the tournament match!\n");
        players[currentPlayer].tournamentScore += 100;
    } else if (players[currentPlayer].singleScore < computerScore) {
        printf("Computer won the tournament match!\n");
        players[currentPlayer].tournamentScore -= 100;
    } else {
        printf("The tournament match was a draw!\n");
    }

    savePlayerData();
}

void playEndlessMode() {
    int player1Score = 0, computerScore = 0;
    char choice;

    do {
        playSingleMatch();
        printf("Player %s: %d, Computer: %d\n", players[currentPlayer].name, players[currentPlayer].singleScore, computerScore);

        printf("Play again? (y/n): ");
        scanf(" %c", &choice);
    } while (choice == 'y' || choice == 'Y');

    if (players[currentPlayer].singleScore > computerScore) {
        printf("You won the endless mode!\n");
        players[currentPlayer].endlessScore += players[currentPlayer].singleScore;
    } else if (players[currentPlayer].singleScore < computerScore) {
        printf("Computer won the endless mode!\n");
        players[currentPlayer].endlessScore -= players[currentPlayer].singleScore;
    } else {
        printf("The endless mode was a draw!\n");
    }

    savePlayerData();
}

int main() {
    char again;
    srand(time(NULL));
    loadPlayerData();

    do {
        char name[MAX_NAME_LENGTH];
        printf("Enter your name: ");
        scanf("%s", name);

        int playerIndex = findPlayerIndex(name);
        if (playerIndex == -1) {
            addPlayer(name);
            playerIndex = numPlayers - 1;
        }

        currentPlayer = playerIndex;

        int mode;
        printf("\nSingle Player (1) or Multiplayer (2)? ");
        scanf("%d", &mode);

        if (mode == 1) {
            int gameType;
            printf("\nChoose game type:\n");
            printf("1. Single Match\n");
            printf("2. Quick Match (Best of 3)\n");
            printf("3. Tournament Match (10 rounds)\n");
            printf("4. Endless Mode\n");
            printf("Enter choice: ");
            scanf("%d", &gameType);

            switch (gameType) {
                case 1:
                    playSingleMatch();
                    break;
                case 2:
                    playQuickMatch();
                    break;
                case 3:
                    playTournamentMatch();
                    break;
                case 4:
                    playEndlessMode();
                    break;
                default:
                    printf("Invalid choice!\n");
                    break;
            }

            showRankings("single");
            showRankings("quick");
            showRankings("tournament");
            showRankings("endless");
        } else if (mode == 2) {
            // Multiplayer mode implementation
            // ...
        } else {
            printf("Invalid choice!\n");
        }

        printf("\nPlay again? (y/n): ");
        scanf(" %c", &again);
    } while (again == 'y' || again == 'Y');

    savePlayerData();
    return 0;
}
