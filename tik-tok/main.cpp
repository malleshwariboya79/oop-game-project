#include <iostream>
#include <cstdlib>
#include <ctime>
#include "raylib.h"

//---------------- WINDOW & BOARD ----------------
const int CELL = 150;
const int GRID = 3;
const int BOARD_SIZE = CELL * GRID;

const int MENU_WIDTH = 350;
const int WIDTH = BOARD_SIZE + MENU_WIDTH;
const int HEIGHT = 580;

//---------------- GAME DATA ---------------------
int board[3][3] = {0};
int player = 1;
bool gameOver = false;
int winner = 0;
int gameMode = 0;        // 0 menu | 1 PVP | 2 AI

//------------------------------------------------

int CheckWinner()
{
    for(int i=0;i<3;i++)
    {
        if(board[i][0] && board[i][0]==board[i][1] && board[i][1]==board[i][2]) return board[i][0];
        if(board[0][i] && board[0][i]==board[1][i] && board[1][i]==board[2][i]) return board[0][i];
    }

    if(board[0][0] && board[0][0]==board[1][1] && board[1][1]==board[2][2]) return board[0][0];
    if(board[0][2] && board[0][2]==board[1][1] && board[1][1]==board[2][0]) return board[0][2];

    return 0;
}

bool IsFull()
{
    for(auto &row : board)
        for(int cell : row)
            if(cell==0) return false;
    return true;
}

void ResetGame()
{
    for(auto &row : board)
        for(int &cell : row)
            cell = 0;

    player=1;
    winner=0;
    gameOver=false;
}

void ComputerMove()
{
    struct Cell { int r,c; };
    Cell freeCells[9];
    int count=0;

    for(int r=0;r<3;r++)
        for(int c=0;c<3;c++)
            if(board[r][c]==0)
                freeCells[count++] = {r,c};

    int choice = rand() % count;
    board[ freeCells[choice].r ][ freeCells[choice].c ] = 2;
}

//------------------------------------------------

int main()
{
    srand(time(NULL));
    InitWindow(WIDTH, HEIGHT, "Tic Tac Toe Deluxe");
    SetTargetFPS(60);

    Rectangle btnPVP  = {BOARD_SIZE+80, 160, 200, 60};
    Rectangle btnAI   = {BOARD_SIZE+80, 240, 200, 60};
    Rectangle btnBack = {BOARD_SIZE+80, 340, 200, 55};

    while(!WindowShouldClose())
    {
        Vector2 mouse = GetMousePosition();

        //------------------------------------------------ MENU
        if(gameMode==0 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if(CheckCollisionPointRec(mouse, btnPVP))
            {
                gameMode=1;
                ResetGame();
            }
            else if(CheckCollisionPointRec(mouse, btnAI))
            {
                gameMode=2;
                ResetGame();
            }
        }

        //------------------------------------------------ PLAY
        if(gameMode!=0 && !gameOver && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            int mx = GetMouseX();
            int my = GetMouseY();

            if(mx < BOARD_SIZE && my < BOARD_SIZE)
            {
                int c = mx / CELL;
                int r = my / CELL;

                if(board[r][c]==0)
                {
                    board[r][c] = player;

                    winner = CheckWinner();

                    if(winner || IsFull())
                        gameOver = true;
                    else
                        player = (player==1 ? 2 : 1);

                    //---------------- AI
                    if(gameMode==2 && player==2 && !gameOver)
                    {
                        ComputerMove();

                        winner = CheckWinner();
                        if(winner || IsFull())
                            gameOver = true;
                        else
                            player = 1;
                    }
                }
            }
        }

        //------------------------------------------------ BACK BUTTON
        if(gameOver && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if(CheckCollisionPointRec(mouse, btnBack))
            {
                ResetGame();
                gameMode=0;
            }
        }

        //------------------------------------------------ DRAW
        BeginDrawing();
        ClearBackground(Color{24,30,45,255});

        //--------------------------------------- BOARD
        DrawRectangle(0,0,BOARD_SIZE,BOARD_SIZE, Color{20,26,44,255});

        // Grid
        for(int i=1;i<3;i++)
        {
            DrawLine(i*CELL,0,i*CELL,BOARD_SIZE, SKYBLUE);
            DrawLine(0,i*CELL,BOARD_SIZE,i*CELL, SKYBLUE);
        }

        // Symbols
        for(int r=0;r<3;r++)
            for(int c=0;c<3;c++)
            {
                if(board[r][c]==1)
                    DrawText("X", c*CELL+45, r*CELL+30, 90, ORANGE);

                else if(board[r][c]==2)
                    DrawText("O", c*CELL+45, r*CELL+30, 90, GREEN);
            }

        //--------------------------------------- SIDE PANEL
        DrawRectangle(BOARD_SIZE,0, MENU_WIDTH, HEIGHT, Color{14,18,30,255});

        DrawText("TIC TAC TOE", BOARD_SIZE+45, 50, 38, SKYBLUE);

        //--------------------------------------- BUTTON HOVER
        Color pvpCol  = CheckCollisionPointRec(mouse, btnPVP)  ? ORANGE : GOLD;
        Color aiCol   = CheckCollisionPointRec(mouse, btnAI)   ? GREEN  : LIME;
        Color backCol = CheckCollisionPointRec(mouse, btnBack) ? SKYBLUE : LIGHTGRAY;

        //--------------------------------------- BUTTONS

        if(gameMode==0)
        {
            DrawRectangleRounded(btnPVP, 0.3f, 8, pvpCol);
            DrawRectangleRounded(btnAI,  0.3f, 8, aiCol);

            DrawText("TWO PLAYER", btnPVP.x+25, btnPVP.y+20, 20, BLACK);
            DrawText("VS COMPUTER", btnAI.x+15, btnAI.y+20, 20, BLACK);
        }

        //--------------------------------------- GAME RESULT
        if(gameOver)
        {
            const char* msg =
                winner==1 ? "X WINS !" :
                winner==2 ? "O WINS !" :
                "DRAW !";

            DrawText(msg, BOARD_SIZE+90, 120, 30, YELLOW);

            DrawRectangleRounded(btnBack, 0.3f, 8, backCol);
            DrawText("BACK TO MENU", btnBack.x+20, btnBack.y+17, 20, BLACK);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
