#include <iostream>
#include "raylib.h"

const int SIZE = 3;
const int CELL = 150;
const int WIDTH = CELL * SIZE;
const int HEIGHT = CELL * SIZE;

int board[3][3] = {0};
int player = 1;
bool gameOver = false;
int winner = 0;

int CheckWinner()
{
    for(int i=0;i<3;i++)
    {
        if(board[i][0] && board[i][0]==board[i][1] && board[i][1]==board[i][2])
            return board[i][0];

        if(board[0][i] && board[0][i]==board[1][i] && board[1][i]==board[2][i])
            return board[0][i];
    }

    if(board[0][0] && board[0][0]==board[1][1] && board[1][1]==board[2][2])
        return board[0][0];

    if(board[0][2] && board[0][2]==board[1][1] && board[1][1]==board[2][0])
        return board[0][2];

    return 0;
}

bool IsFull()
{
    for(int r=0;r<3;r++)
        for(int c=0;c<3;c++)
            if(board[r][c]==0)
                return false;

    return true;
}

int main()
{
    InitWindow(WIDTH, HEIGHT, "Tic Tac Toe - Raylib");
    SetTargetFPS(60);

    while(!WindowShouldClose())
    {
        // Mouse input
        if(!gameOver && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            int col = GetMouseX() / CELL;
            int row = GetMouseY() / CELL;

            if(col<3 && row<3 && board[row][col] == 0)
            {
                board[row][col] = player;

                winner = CheckWinner();

                if(winner || IsFull()) gameOver = true;
                else player = (player == 1 ? 2 : 1);
            }
        }

        // Reset
        if(IsKeyPressed(KEY_R))
        {
            for(int r=0;r<3;r++)
                for(int c=0;c<3;c++)
                    board[r][c]=0;
            gameOver=false;
            player=1;
            winner=0;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Grid
        for(int i=1;i<3;i++)
        {
            DrawLine(i*CELL,0,i*CELL,HEIGHT,BLACK);
            DrawLine(0,i*CELL,WIDTH,i*CELL,BLACK);
        }

        // Symbols
        for(int r=0;r<3;r++)
            for(int c=0;c<3;c++)
            {
                if(board[r][c]==1)
                    DrawText("X", c*CELL+50, r*CELL+30, 80, DARKBLUE);
                else if(board[r][c]==2)
                    DrawText("O", c*CELL+50, r*CELL+30, 80, MAROON);
            }

        // Game over
        if(gameOver)
        {
            const char* msg =
                winner == 1 ? "PLAYER X WINS" :
                winner == 2 ? "PLAYER O WINS" :
                "DRAW";

            DrawText(msg, 40, HEIGHT/2 - 20, 30, RED);
            DrawText("Press R to Restart", 40, HEIGHT/2 + 20, 20, GRAY);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}