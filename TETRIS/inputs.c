#include "IO.h"
extern void delay(int);

volatile int *button = (int *)0x040000e0; // adress of the GPIO

static int prevstate[4] = {0, 0, 0, 0};

int get_btn(int pin)
{
    int first = (*button >> pin) & 0x1;
    delay(5);
    int now = (*button >> pin) & 0x1;

    int pressed = (first == 1 && now == 1 && prevstate[pin] == 0);
    prevstate[pin] = now;

    return pressed;
}
