#include "game.h"
#include "IO.h"
#include "dtekv-lib.h"
extern void delay(int);

int totallinescleard = 0;

char board[ROWS][COLLUM] = {0};

const shapetemplate pieces[7] = {
    {2, {{1, 1}, {1, 1}}},                                        // da sqaure
    {3, {{0, 1, 0}, {1, 1, 1}, {0, 0, 0}}},                       // da t thing
    {3, {{1, 1, 0}, {0, 1, 1}, {0, 0, 0}}},                       // da S
    {3, {{0, 1, 1}, {1, 1, 0}, {0, 0, 0}}},                       // da Z
    {3, {{1, 0, 0}, {1, 1, 1}, {0, 0, 0}}},                       // da anti L
    {3, {{0, 0, 1}, {1, 1, 1}, {0, 0, 0}}},                       // da l
    {4, {{1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}} // da long
};

static unsigned int seed;
void generateseed(void)
{
    seed = 0;
    print("Press any button to start for random seed...\n");

    // wait until any button is pressed, counting as "entropy"
    while (!get_btn(0) && !get_btn(1) && !get_btn(2) && !get_btn(3))
    {
        seed++;
        delay(1);
    }

    print("Seeded with: ");
    print_dec(seed);
    print("\n");
}

int rand(void)
{
    seed = seed * 1103515245 + 12345;
    return (seed >> 16) & 0x7FFF;
}

void clearboard(void) // resetar
{

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLLUM; j++)
        {
            board[i][j] = 0;
        }
    }
}

void printboard(activepiece *p, int score)
{
    print("score:");
    print_dec(score);
    print("     level:");
    print_dec(level());
    print("\n");

    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLLUM; col++)
        {
            int isBlock = board[row][col]; // kollar om ett permanet block finns där

            // räknar ut den relative position till den fallande biten
            int pieceRow = row - p->row;
            int pieceCol = col - p->col;

            // om den är där
            if (pieceRow >= 0 && pieceRow < p->chosenshape->width &&
                pieceCol >= 0 && pieceCol < p->chosenshape->width)
            {
                if (p->blocks[pieceRow][pieceCol])
                {
                    isBlock = 1; // overlay the falling piece
                }
            }

            if (isBlock)
                print(" #");
            else
                print(" .");
        }
        print("\n");
    }
}

void spawnpiece(activepiece *p) // säger till current viklen bit den är och var den befinner sig. row och coll däger var top vänsta hörnet av matrisen ligger
{
    int chose = rand() % 7;
    p->chosenshape = &pieces[chose];
    p->row = 0;
    p->col = (COLLUM - p->chosenshape->width) / 2;

    // kopiera template in i den den temnporara blocken som kan reteras seanre
    int w = p->chosenshape->width;
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < w; j++)
        {
            p->blocks[i][j] = p->chosenshape->blocks[i][j];
        }
    }
    // kollar om det är game over
    if (!canMove(p, DOWN) && !canMove(p, LEFT) && !canMove(p, RIGHT)) // kan inte röra sig nånstans.
    {
        print("\nGAME OVER!\n");
        print("Press Button 0 to Restart\n");

        // vänta på nått ska hända
        while (!get_btn(0))
        {
            // Optional: small delay to avoid busy loop
            delay(10);
        }

        // Reset
        clearboard();
        totallinescleard = 0; // reset level counter
        spawnpiece(p);        // spawn new piece
    }
}
int canMove(activepiece *p, int direction) // well can it ?
{
    int newRow = p->row; // var blocken befinniser sig innan input
    int newCol = p->col;

    // var blocken befinner sig efter vald input
    if (direction == LEFT)
        newCol--;
    if (direction == RIGHT)
        newCol++;
    if (direction == DOWN)
        newRow++;

    // kollar om om blocken klippar emot väggen
    for (int i = 0; i < p->chosenshape->width; i++)
    {
        for (int j = 0; j < p->chosenshape->width; j++)
        {
            if (p->blocks[i][j] == 0)
                continue;
            // bewstriver var bitten av figuren befinner sig på brädan
            int futurerow = newRow + i;
            int futurecol = newCol + j;

            // kolla om nån bit krockar med väggen
            if (futurerow < 0 || futurerow >= ROWS ||
                futurecol < 0 || futurecol >= COLLUM)
            {
                return 0;
            }

            // kollar om den krocar med befinliga bitar
            if (board[futurerow][futurecol] == 1)
            {
                return 0;
            }
        }
    }

    return 1;
}

void moveCurrent(activepiece *p, int direction) // rör på shapen
{
    if (canMove(p, direction)) // kollar oom moven ör valid eller inte innan det görs
    {
        if (direction == LEFT)
            p->col--;
        if (direction == RIGHT)
            p->col++;
        if (direction == DOWN)
        {
            while (canMove(p, DOWN))
            {
                p->row++;
            }
        }
    }
}

void settlePiece(activepiece *p)
{
    for (int i = 0; i < p->chosenshape->width; i++)
    {
        for (int j = 0; j < p->chosenshape->width; j++)
        {
            if (p->blocks[i][j] == 1)
            {
                int row = p->row + i; // beskriver bitens possion
                int col = p->col + j;
                // kollar om biten är i en valid plats
                if (row >= 0 && row < ROWS && col >= 0 && col < COLLUM)
                {
                    board[row][col] = 1; // permanetta på brädan
                }
            }
        }
    }
}

int Lineclear(void)
{
    int write = ROWS - 1; // börjar från botten skirver där
    int linescleard = 0;

    for (int i = ROWS - 1; i >= 0; i--) // kollar om varje line har en en full rad
    {
        int fullline = 1;
        for (int j = 0; j < COLLUM; j++)
        {
            if (board[i][j] == 0)
            {
                fullline = 0;
                break;
            }
        }

        // om den är inte full
        if (!fullline)
        {
            if (write != i) // och att den inte sig själv
            {
                for (int j = 0; j < COLLUM; j++)
                {
                    board[write][j] = board[i][j]; // skriv ner den på booten
                }
            }
            write--; // flyya upp ett steg och  nästa icke fulla rad
        }
        // om den är full skippas denna steg ^^ så den effektiv sätt raderas
        else
        {
            // Only increment linescleared for full lines
            linescleard++;
            totallinescleard++;
        }
    }

    // efter alla icke fulla rader har flyttas ner radera alla phantom blocken
    for (int i = write; i >= 0; i--)
    {
        for (int j = 0; j < COLLUM; j++)
        {
            board[i][j] = 0;
        }
    }
    int score = scoring(linescleard);
    return score;
}

void rotate(activepiece *p)
{
    int w = p->chosenshape->width;
    char transpose[4][4];
    char reverse[4][4];

    // Transponera matrisne
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < w; j++)
        {
            transpose[j][i] = p->blocks[i][j];
        }
    }

    // byt order
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < w; j++)
        {
            reverse[i][j] = transpose[i][w - 1 - j];
        }
    }

    // kolla om jag kan rotera blocken sp den inte klippar in i väggen
    int valid = 1;
    for (int i = 0; i < w && valid; i++)
    {
        for (int j = 0; j < w; j++)
        {
            if (reverse[i][j] == 0)
                continue;

            int r = p->row + i;
            int c = p->col + j;

            if (r < 0 || r >= ROWS || c < 0 || c >= COLLUM || board[r][c])
            {
                valid = 0;
                break;
            }
        }
    }

    // ucommit till rotationen endast omn den inteblippar.
    if (valid)
    {
        for (int i = 0; i < w; i++)
        {
            for (int j = 0; j < w; j++)
            {
                p->blocks[i][j] = reverse[i][j];
            }
        }
    }
}

int scoring(int linescleared)
{

    if (linescleared == 1)
        return 100;
    else if (linescleared == 2)
        return 400;
    else if (linescleared == 3)
        return 900;
    else if (linescleared == 4)
        return 2000;

    return 0;
}

int level(void)
{
    return totallinescleard / 10;
}