#include "game.h"
#include "IO.h"
#include "dtekv-lib.h"
extern void delay(int);

int totallinescleard = 0;

char board[ROWS][COLLUM] = {0};

const shapetemplate pieces[7] = {
    {2, {{1, 1}, {1, 1}}, 0b11111100, 0b11011000},                                        // da sqaure
    {3, {{0, 1, 0}, {1, 1, 1}, {0, 0, 0}}, 0b11100011, 0b11000010},                       // da t thing
    {3, {{1, 1, 0}, {0, 1, 1}, {0, 0, 0}}, 0b00011100, 0b00011000},                       // da S
    {3, {{0, 1, 1}, {1, 1, 0}, {0, 0, 0}}, 0b11100000, 0b1101110},                        // da Z
    {3, {{1, 0, 0}, {1, 1, 1}, {0, 0, 0}}, 0b00000011, 0b00000010},                       // da anti L
    {3, {{0, 0, 1}, {1, 1, 1}, {0, 0, 0}}, 232, 0b11010000},                              // da l
    {4, {{1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 0b00011111, 0b00011010} // da long
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

#define TILE_SIZE 10 // size of each block in pixels
#define BOARD_X 102  // x offset of the board on screen
#define BOARD_Y 20   // y offset of the board on screen

void draw_board(activepiece *p, int score)
{
    clear_backbuffer(0); // clear screen to black

    int boardWidth = COLLUM * TILE_SIZE;
    int boardHeight = ROWS * TILE_SIZE;

    // Top border
    draw_tile(BOARD_X - 1, BOARD_Y - 1, boardWidth + 2, 1, 0b11111111);
    // Bottom border
    draw_tile(BOARD_X - 1, BOARD_Y + boardHeight, boardWidth + 2, 1, 0b11111111);
    // Left border
    draw_tile(BOARD_X - 1, BOARD_Y, 1, boardHeight, 0b11111111);
    // Right border
    draw_tile(BOARD_X + boardWidth, BOARD_Y, 1, boardHeight, 0b11111111);

    for (int i = 10; i < 200; i += 10)
        draw_tile(BOARD_X, BOARD_Y + i, boardWidth, 1, 0x49);
    for (int i = 10; i < 100; i += 10)
        draw_tile(BOARD_X + i, BOARD_Y, 1, boardHeight, 0x49);

    // draw static blocks on board
    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLLUM; col++)
        {
            if (board[row][col])
            {
                int x = BOARD_X + col * TILE_SIZE;
                int y = BOARD_Y + row * TILE_SIZE;

                // Draw inner core (9x9) leaving 1-pixel space for top and left border
                draw_tile(x + 1, y + 1, 9, 9, board[row][col]);

                // Draw top and left border (1 pixel each)
                draw_tile(x, y, TILE_SIZE, 1, 0x49); // top
                draw_tile(x, y, 1, TILE_SIZE, 0x49); // left
            }
        }
    }

    // overlay falling piece
    int w = p->chosenshape->width;
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < w; j++)
        {
            if (p->blocks[i][j])
            {
                int px = p->col + j;
                int py = p->row + i;
                int x = BOARD_X + px * TILE_SIZE;
                int y = BOARD_Y + py * TILE_SIZE;

                // inner core 9x9
                draw_tile(x + 1, y + 1, 9, 9, p->chosenshape->color);

                // top and left border
                draw_tile(x, y, TILE_SIZE, 1, 0x49); // top
                draw_tile(x, y, 1, TILE_SIZE, 0x49); // left
            }
        }
    }

    // optional: draw score/level as rectangles or leave as later text
    // e.g., draw_tile(10, 10, 50, 10, 255);

    swap_buffers_blocking(); // show frame
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
            if (board[futurerow][futurecol] != 0)
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
    int w = p->chosenshape->width;
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < w; j++)
        {
            if (p->blocks[i][j] == 1)
            {
                int row = p->row + i;
                int col = p->col + j;
                if (row >= 0 && row < ROWS && col >= 0 && col < COLLUM)
                {
                    board[row][col] = p->chosenshape->color; // store piece color
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