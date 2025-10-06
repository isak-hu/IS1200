
#include "output.h"

#define WIDTH 320
#define HEIGHT 240
#define PIXELS (WIDTH * HEIGHT) // 76800

#define FB0 0x08000000     // front buffer base
#define FB1 (FB0 + PIXELS) // second buffer base

#define CTRL 0x04000100
#define BUFFER ((volatile int *)(CTRL + 0x0))  // write to swap
#define BF_REG ((volatile int *)(CTRL + 0x4))  // address to display next
#define RES_REG ((volatile int *)(CTRL + 0x8)) // read-only resolution
#define STATUS ((volatile int *)(CTRL + 0xC))  // status bits

/* Framebuffer pointers we use in software */
static volatile char *front_buffer = (volatile char *)FB0;
static volatile char *back_buffer = (volatile char *)FB1;

/* Example palette bytes (you can choose any 0..255 values) */

/* -------------------- API implementations -------------------- */

void output_init(void)
{
    // clearar allt i början
    int i;
    volatile char *fb;
    fb = (volatile char *)FB0;
    for (i = 0; i < PIXELS; i++)
        fb[i] = BLACK;
    fb = (volatile char *)FB1;
    for (i = 0; i < PIXELS; i++)
        fb[i] = BLACK;
}

void clear_backbuffer(int color)
{
    int i;
    for (i = 0; i < PIXELS; i++)
        back_buffer[i] = (char)color;
}

void draw_pixel(int x, int y, int color)
{
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) // safty check
        return;
    back_buffer[y * WIDTH + x] = (char)color;
}

void draw_tile(int x, int y, int w, int h, int color)
{
    int j, i;
    // safty check
    // if (x >= WIDTH || y >= HEIGHT)
    //     return;
    // if (x + w <= 0 || y + h <= 0)
    //     return;

    for (j = 0; j < h; j++)
    {
        int py = y + j;
        // if (py < 0 || py >= HEIGHT)
        //     continue;
        int base = py * WIDTH;
        for (i = 0; i < w; i++)
        {
            int px = x + i;
            // if (px < 0 || px >= WIDTH)
            //     continue;
            back_buffer[base + px] = (char)color;
        }
    }
}

/* Blocking swap: tell controller which backbuffer to use, request swap,
   then wait until status.S becomes 0 (swap finished). After swap finishes
   we update our software front/back pointers. */
void swap_buffers_blocking(void)
{
    // back buffer location
    *BF_REG = (int)back_buffer;

    // DO the swap
    *BUFFER = 0;

    // vänta tills controller clearas
    while ((*STATUS & 0x1) != 0)
    {
    }

    {
        volatile char *tmp = front_buffer;
        front_buffer = back_buffer;
        back_buffer = tmp;
    }
}

/* -------------------- Query helpers -------------------- */

/* Returns bytes-per-pixel (1,2,3,4) by reading bits [7:4] of status */
int get_vga_bpp_bytes(void)
{
    int s = *STATUS;
    int b = (s >> 4) & 0xF;
    return b;
}

/* Read resolution (X = low 16 bits, Y = high 16 bits of res reg) */
int get_vga_width(void)
{
    int r = *RES_REG;
    return r & 0xFFFF;
}
int get_vga_height(void)
{
    int r = *RES_REG;
    return (r >> 16) & 0xFFFF;
}
