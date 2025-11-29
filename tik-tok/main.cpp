// Console Tic-Tac-Toe (pure C++): supports Player-vs-Player and Player-vs-Computer (random AI)
#include <iostream>
#include <array>
#include <random>
#include <string>
#include <ctime>
#include <vector>

enum Cell
{
    EMPTY = 0,
    X = 1,
    O = 2
};

using Board = std::array<std::array<Cell, 3>, 3>;

static void ClearScreen()
{
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

void PrintBoard(const Board &b)
{
    ClearScreen();
    std::cout << "\n   1   2   3\n";
    for (int r = 0; r < 3; ++r)
    {
        std::cout << " " << char('A' + r) << " ";
        for (int c = 0; c < 3; ++c)
        {
            char ch = ' ';
            if (b[r][c] == X)
                ch = 'X';
            else if (b[r][c] == O)
                ch = 'O';
            std::cout << " " << ch << " ";
            if (c < 2)
                std::cout << "|";
        }
        std::cout << "\n";
        if (r < 2)
            std::cout << "  ---+---+---\n";
    }
}

void ResetBoard(Board &b)
{
    for (auto &row : b)
        for (auto &c : row)
            c = EMPTY;
}

int CheckWinner(const Board &b)
{
    // Rows & columns
    for (int i = 0; i < 3; ++i)
    {
        if (b[i][0] != EMPTY && b[i][0] == b[i][1] && b[i][1] == b[i][2])
            return b[i][0];
        if (b[0][i] != EMPTY && b[0][i] == b[1][i] && b[1][i] == b[2][i])
            return b[0][i];
    }
    // Diagonals
    if (b[0][0] != EMPTY && b[0][0] == b[1][1] && b[1][1] == b[2][2])
        return b[0][0];
    if (b[0][2] != EMPTY && b[0][2] == b[1][1] && b[1][1] == b[2][0])
        return b[0][2];
    // No winner
    return EMPTY;
}

bool IsBoardFull(const Board &b)
{
    for (auto &row : b)
        for (auto &c : row)
            if (c == EMPTY)
                return false;
    return true;
}

// Get a human move in format A1, B3, 1A, etc.
bool GetHumanMove(Board &b, Cell player)
{
    std::string s;
    std::cout << "Enter move for " << (player == X ? "X" : "O") << " (e.g. A1, b3): ";
    if (!(std::cin >> s))
        return false;
    if (s.size() < 2)
        return false;
    char a = s[0], bch = s[1];
    // normalize
    if (!isalpha(a))
        std::swap(a, bch);
    a = toupper(a);
    if (!isalpha(a) || !isdigit(bch))
        return false;
    int row = a - 'A';
    int col = (bch - '1');
    if (row < 0 || row > 2 || col < 0 || col > 2)
        return false;
    if (b[row][col] != EMPTY)
        return false;
    b[row][col] = player;
    return true;
}

// Random AI move
void RandomAIMove(Board &b, Cell player)
{
    std::vector<std::pair<int, int>> avail;
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            if (b[r][c] == EMPTY)
                avail.emplace_back(r, c);
    if (avail.empty())
        return;
    static std::mt19937_64 rng((unsigned)std::time(nullptr));
    std::uniform_int_distribution<size_t> dist(0, avail.size() - 1);
    auto p = avail[dist(rng)];
    b[p.first][p.second] = player;
}

int main()
{
    Board board{};
    ResetBoard(board);

    std::cout << "Simple Tic-Tac-Toe (console)\n";
    std::cout << "1) Player vs Player\n2) Player vs Computer (random)\nChoose mode (1 or 2): ";
    int mode = 1;
    if (!(std::cin >> mode))
        return 0;
    bool vsComputer = (mode == 2);

    Cell current = X;
    bool gameOver = false;
    int winner = EMPTY;

    while (true)
    {
        PrintBoard(board);
        if (gameOver)
        {
            if (winner == X)
                std::cout << "\nPlayer X wins!\n";
            else if (winner == O)
                std::cout << "\nPlayer O wins!\n";
            else
                std::cout << "\nIt's a draw!\n";
            std::cout << "Press R to restart, Q to quit: ";
            char ch;
            std::cin >> ch;
            if (ch == 'R' || ch == 'r')
            {
                ResetBoard(board);
                gameOver = false;
                winner = EMPTY;
                current = X;
                continue;
            }
            break;
        }

        if (current == X || !vsComputer)
        {
            // human move
            bool ok = GetHumanMove(board, current);
            if (!ok)
            {
                std::cout << "Invalid move format or occupied cell. Try again.\n";
                continue;
            }
        }
        else
        {
            // computer move
            RandomAIMove(board, current);
        }

        winner = CheckWinner(board);
        if (winner != EMPTY)
            gameOver = true;
        else if (IsBoardFull(board))
        {
            winner = EMPTY;
            gameOver = true;
        }
        else
            current = (current == X ? O : X);
    }

    return 0;
}
