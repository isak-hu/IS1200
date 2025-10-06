#ifndef GAME_H
#define GAME_H

#include "output.h"

#define ROWS 20
#define COLLUM 10
#define LEFT 0
#define RIGHT 1
#define DOWN 2

extern char board[ROWS][COLLUM]; // deklaration; definieras i game.c

typedef struct // a piece bank to chose from to turn in to the falling piece
{
    int width;
    char blocks[4][4];
    int color;
    int bcolor;
} shapetemplate;

typedef struct // the structre for the pice that is curreclly falling. it hold the data of what it is, how it looks, and where it is.
{
    int row, col;
    const shapetemplate *chosenshape; // pekar på template
    char blocks[4][4];
    // kopierar template in i den så att den kan roteras.
} activepiece;

void clearboard(void);
void printboard(activepiece *p, int score);
int rand(void);
void spawnpiece(activepiece *p);
void moveCurrent(activepiece *p, int direction);
int canMove(activepiece *p, int direction);
void settlePiece(activepiece *p);
int Lineclear(void);
void rotate(activepiece *p);
int scoring(int linescleared);
int level(void);
void generateseed(void);
void draw_board(activepiece *p, int score);

#endif // GAME_H