#ifndef OUTPUT_H
#define OUTPUT_H

#include "game.h"
#define BLACK 0
#define RED 0b11100000 /* your earlier test used 232 => orange/red */
#define WHITE 0b11111111

/* Basic API */
void output_init(void);                     // init buffers (clear)
void clear_backbuffer(int color);           // fill back buffer with color byte
void draw_pixel(int x, int y, int color);  // draw single pixel into back buffer
void draw_tile(int x, int y, int w, int h, int color); // filled rect
void swap_buffers_blocking(void);           // request swap and wait until done

/* Query helpers */
int get_vga_bpp_bytes(void);                // bytes-per-pixel (from status)
int get_vga_width(void);                    // read resolution X
int get_vga_height(void);                   // read resolution Y

void test_vga(void);

#endif