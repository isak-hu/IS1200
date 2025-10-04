#include "IO.h"
#include "game.h"
#include "dtekv-lib.h"
#include "timer.h"
extern void delay(int);
volatile int lvl;

activepiece current;

void clearScreen(void)
{
  print("\x1B[2J"); // Clear entire screen
  print("\x1B[H");  // Move cursor to top-left
}

int main()
{

  int score = 0;

  print("Game starting...\n");
  timerinit();          // initialize timer
  generateseed();       // genrate seed
  clearboard();         // start with empty board
  spawnpiece(&current); // spawn first piece

  while (1)
  {
    // Handle button input
    if (get_btn(0))
      moveCurrent(&current, LEFT);
    if (get_btn(1))
      moveCurrent(&current, RIGHT);
    if (get_btn(2))
      moveCurrent(&current, DOWN);
    // smash down
    if (get_btn(3))
      rotate(&current);
    // smash down

    // gravity
    if (tick)
    {
      tick = 0;
      if (canMove(&current, DOWN))
      {
        current.row++; // move down by 1
      }
      else
      {
        settlePiece(&current);
        score += Lineclear(); // reached bottom or collided
        spawnpiece(&current);
        // spawn new piece
      }
    }

    // Refresh screen
    if (frames)
    {
      lvl = level();
      clearScreen();
      printboard(&current, score);
    }
  }
}
