/* main.c

   This file written 2024 by Artur Podobas and Pedro Antunes

   For copyright and licensing, see file COPYING */

/* Below functions are external and found in other files. */

#define STATUS 0x04000020
#define CONTROL 0x04000024
#define PERIODL 0x04000028
#define PERIODH 0x0400002C
#define SNAPL 0x04000030
#define SNAPH 0x04000034

extern void print(const char *);
extern void print_dec(unsigned int);
extern void display_string(char *);
extern void time2string(char *, int);
extern void tick(int *);
extern void delay(int);
extern int nextprime(int);
extern void enable_interrupt(void);

int mytime = 0x5957;

char textstring[] = "text, more text, and even more text!";
char decode[10] = {64, 121, 36, 48, 25, 18, 2, 120, 0, 16};
char display[6] = {0};

volatile int *periodl = (int *)PERIODL;
volatile int *periodh = (int *)PERIODH;
volatile int *status = (int *)STATUS;
volatile int *control = (int *)CONTROL;

volatile int *led_ptr = (int *)0x04000000;

int timeoutcount = 0;
int prime = 1234567;

void clock(void);

void handle_interrupt(unsigned cause)
{
  if (cause == 16)
  {
    timeoutcount++;
    *status &= ~1;
    if (timeoutcount == 10)
    {
      timeoutcount = 0;
      clock();
      tick(&mytime);
    }
  }
}

/* Add your code here for initializing interrupts. */
void labinit(void)
{
  *periodl = 0xC6C0;
  *periodh = 0x2D;
  *status = 0x1;
  *control = 0x7;
  enable_interrupt();
}

int get_btn(void)
{
  volatile int *sw_ptr = (int *)0x040000d0;
  return *sw_ptr & 0x1;
}

int get_sw(void)
{
  volatile int *sw_ptr = (int *)0x04000010;
  return *sw_ptr & 0x3FF;
}

void set_displays(int display_number, int value)
{

  int val = decode[value];

  volatile int *ssd_ptr = (int *)(0x04000050 + (display_number * 0x10));
  *ssd_ptr = val;
}

void set_led(int led_mask)
{
  volatile int *led_ptr = (int *)0x04000000;
  *led_ptr = led_mask & 0x3FF;
}

void clock(void)
{
  static int hours = 0;
  static int minutes = 0;
  static int seconds = 0;

  // Enter a forever loop

  display[5] = hours / 10;
  display[4] = hours % 10;

  display[3] = minutes / 10;
  display[2] = minutes % 10;

  display[1] = seconds / 10;
  display[0] = seconds % 10;

  seconds++;

  if (seconds == 60)
  {
    seconds = 0;
    minutes++;
  }

  if (minutes == 60)
  {
    minutes = 0;
    hours++;
  }

  if (hours == 24)
    hours = 0;

  for (int i = 0; i < 6; i++)
  {
    set_displays(i, (int)display[i]);
  }
  return;
}

void switches(void)
{

  int left_state = (get_sw() & 0b1100000000) >> 8;
  int right_state = (get_sw() & 0b0000111111);

  for (int i = 0; i < 6; i += 2)
  {
    display[i + 1] = right_state / 10;
    display[i] = right_state % 10;
  }

  if (left_state == 0b01)
  {
    for (int i = 0; i < 2; i++)
    {
      set_displays(i, display[i]);
    }
  }

  if (left_state == 0b10)
  {
    for (int i = 2; i < 4; i++)
    {
      set_displays(i, display[i]);
    }
  }

  if (left_state == 0b11)
  {
    for (int i = 4; i < 6; i++)
    {
      set_displays(i, display[i]);
    }
  }
}

/* Your code goes into main as well as any needed functions. */
int main(void)
{
  labinit(); // initialize timer + enable interrupts ONCE

  while (1)
  {
    print("Prime : ");
    prime = nextprime(prime);
    print_dec(prime);
    print("\n");
  }
}