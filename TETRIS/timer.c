#include "IO.h"
#include "timer.h"
extern void enable_interrupt(void);

#define STATUS 0x04000020
#define CONTROL 0x04000024
#define PERIODL 0x04000028
#define PERIODH 0x0400002C

volatile int *periodl = (int *)PERIODL;
volatile int *periodh = (int *)PERIODH;
volatile int *status = (int *)STATUS;
volatile int *control = (int *)CONTROL;

volatile int tick = 0;
volatile int frames = 0;

extern volatile int lvl;
int timeoutcount = 0;
int framescountt = 0;

volatile int basetick = 48;

// makes it fall faster
volatile int softdrop_multiplier = 8;
volatile int softdrop_active = 0;

void timerinit(void)
{
    *periodl = 0x7530; // set to 30000 cycles  = 1 ms
    *periodh = 0x0;    //
    *status = 0x1;
    *control = 0x7;
    enable_interrupt();
    return;
}

void handle_interrupt(unsigned cause)
{
    if (cause == 16)
    {
        framescountt++;

        *status &= ~1;

        // determine current interval based on soft drop
        int current_interval = basetick;
        if (softdrop_active)
            current_interval = basetick / softdrop_multiplier;

        if (timeoutcount >= (current_interval - 5 * lvl))
        {
            timeoutcount = 0;
            tick = 1; // trigger gravity movement
        }

        if (framescountt == 16) // 1 frame
        {
            framescountt = 0;
            frames = 1;
            timeoutcount++;
        }
    }
}
