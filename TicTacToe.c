#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char board[3][3];
int player, computer, player1Score = 0, player2Score = 0;

void resetBoard()
{
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            board[i][j] = ' ';
        }
    }
}

void displayBoard()
{
    int i, j;
    printf("\n\n");
    for (i = 0; i < 3; i++)
    {
        printf(" | ");
        for (j = 0; j < 3; j++)
        {
            printf("%c | ", board[i][j]);
        }
        printf("\n");
    }
}

int checkWin(char symbol)
{
    int i;
    // Check rows
    for (i = 0; i < 3; i++)
    {
        if (board[i][0] == symbol && board[i][1] == symbol && board[i][2] == symbol)
            return 1;
    }

    // Check columns
    for (i = 0; i < 3; i++)
    {
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

int checkDraw()
{
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (board[i][j] == ' ')
                return 0;
        }
    }
    return 1;
}

void playerMove()
{
    int row, col;
    printf("Player %d's turn\n", player);
    printf("Enter row (0-2) and column (0-2): ");
    scanf("%d %d", &row, &col);

    if (row < 0 || row > 2 || col < 0 || col > 2 || board[row][col] != ' ')
    {
        printf("Invalid move! Try again.\n");
        playerMove();
        return;
    }

    board[row][col] = (player == 1) ? 'X' : 'O';
}

void computerMove()
{
    int i, j, moveFound = 0;

    // Try to win
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (board[i][j] == ' ')
            {
                board[i][j] = 'O';
                if (checkWin('O'))
                {
                    moveFound = 1;
                    break;
                }
                board[i][j] = ' ';
            }
        }
        if (moveFound)
            break;
    }

    if (!moveFound)
    {
        // Try to block the player
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 3; j++)
            {
                if (board[i][j] == ' ')
                {
                    board[i][j] = 'X';
                    if (checkWin('X'))
                    {
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

    if (!moveFound)
    {
        // Take the center if available
        if (board[1][1] == ' ')
            board[1][1] = 'O';
        else
        {
            // Make a random move
            do
            {
                i = rand() % 3;
                j = rand() % 3;
            } while (board[i][j] != ' ');
            board[i][j] = 'O';
        }
    }
}

int main()
{
    char again;
    srand(time(NULL));

    do
    {
        resetBoard();
        player1Score = 0;
        player2Score = 0;

        printf("\nSingle Player (1) or Multiplayer (2)? ");
        int mode;
        scanf("%d", &mode);

        if (mode == 1)
        {
            printf("\nSingle Player mode\n");
            player = 1;
            computer = 2;

            do
            {
                displayBoard();
                playerMove();
                if (checkWin('X'))
                {
                    displayBoard();
                    printf("You won!\n");
                    player1Score++;
                }
                else if (checkWin('O'))
                {
                    displayBoard();
                    printf("Computer won!\n");
                    computer++;
                }
                else if (checkDraw())
                {
                    displayBoard();
                    printf("It's a draw!\n");
                }
                else
                {
                    computerMove();
                }
            } while (!checkWin('X') && !checkWin('O') && !checkDraw());

            printf("Your Score: %d\n", player1Score);
            printf("Computer Score: %d\n", computer);

            if (player1Score == 3)
                printf("\nYou won the match!\n");
            else if (computer == 3)
                printf("\nComputer won the match!\n");
        }
        else if (mode == 2)
        {
            printf("\nMultiplayer mode\n");
            player = 1;

            do
            {
                displayBoard();
                playerMove();
                if (checkWin('X'))
                {
                    displayBoard();
                    printf("Player 1 won!\n");
                    player1Score++;
                }
                else if (checkWin('O'))
                {
                    displayBoard();
                    printf("Player 2 won!\n");
                    player2Score++;
                }
                else if (checkDraw())
                {
                    displayBoard();
                    printf("It's a draw!\n");
                }
                else
                {
                    player = (player == 1) ? 2 : 1;
                }
            } while (!checkWin('X') && !checkWin('O') && !checkDraw());

            printf("Player 1 Score: %d\n", player1Score);
            printf("Player 2 Score: %d\n", player2Score);

            if (player1Score == 3)
                printf("\nPlayer 1 won the match!\n");
            else if (player2Score == 3)
                printf("\nPlayer 2 won the match!\n");
        }
        else
        {
            printf("Invalid choice!\n");
        }

        printf("\nPlay again? (y/n): ");
        scanf(" %c", &again);
    } while (again == 'y' || again == 'Y');

    return 0;
}
