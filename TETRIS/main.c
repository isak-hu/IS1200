#include "IO.h"
#include "game.h"
#include "dtekv-lib.h"
#include "timer.h"
extern void delay(int);
extern volatile int softdrop_active;
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
  generateseed();       // generate seed
  clearboard();         // start with empty board
  spawnpiece(&current); // spawn first piece

  while (1)
  {
    // Handle button input
    if (get_btn(0))
      moveCurrent(&current, LEFT);
    if (get_btn(1))
      moveCurrent(&current, RIGHT);
    if (get_btn(3))
      moveCurrent(&current, DOWN);
    if (get_btn(2))
      rotate(&current);

    softdrop_active = get_btn_held(4);

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
        score += Lineclear();
        spawnpiece(&current);
      }
    }

    // Refresh screen
    if (frames)
    {
      frames = 0;
      lvl = level();
      clearScreen();
      printboard(&current, score);
    }
  }
}