#ifndef OUTPUT_H
#define OUTPUT_H

#include "game.h"
#define BLACK 0
#define RED 0b11100000 /* your earlier test used 232 => orange/red */
#define WHITE 0b11111111

/* Basic API */
void output_init(void);                                // init buffers (clear)
void clear_backbuffer(int color);                      // fill back buffer with color byte
void draw_pixel(int x, int y, int color);              // draw single pixel into back buffer
void draw_tile(int x, int y, int w, int h, int color); // filled rect
void swap_buffers_blocking(void);                      // request swap and wait until done

/* Query helpers */
int get_vga_bpp_bytes(void); // bytes-per-pixel (from status)
int get_vga_width(void);     // read resolution X
int get_vga_height(void);    // read resolution Y

void test_vga(void);

void draw_score_label(int x, int y, int color);
void draw_number(int num, int x, int y, int color);
void draw_digit(int digit, int x, int y, int color);
void draw_char(const unsigned char *chardata, int x, int y, int color);

#endif