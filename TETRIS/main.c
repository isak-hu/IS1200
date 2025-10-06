#include "IO.h"
#include "game.h"
#include "dtekv-lib.h"
#include "timer.h"
#include "output.h"
extern void delay(int);
extern volatile int softdrop_active;
volatile int lvl;

activepiece current;

void clearScreen(void)
{
  print("\x1B[2J"); // Clear entire screen
  print("\x1B[H");  // Move cursor to top-left
}

// int main(void)
// {
//   output_init();

//   int x = 0;
//   int dx = 2;
//   int tile_w = 20, tile_h = 20;

//   while (1)
//   {
//     clear_backbuffer(0);

//     draw_tile(x, 100, tile_w, tile_h, RED);

//     swap_buffers_blocking(); // make this frame visible (blocking)

//     x += dx;
//     if (x <= 0 || x >= (get_vga_width() - tile_w))
//       dx = -dx;

//     delay(200); /* tune to visible speed */
//   }
//   return 0;
// }

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
        score += ((lvl + 1) * Lineclear());
        spawnpiece(&current);
      }
    }

    // Refresh screen
    if (frames)
    {
      frames = 0;
      lvl = level();
      draw_board(&current, score);
    }
  }
}