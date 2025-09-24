/* main.c

   This file written 2024 by Artur Podobas and Pedro Antunes

   For copyright and licensing, see file COPYING */

/* Below functions are external and found in other files. */

extern void print(const char *);
extern void print_dec(unsigned int);
extern void display_string(char *);
extern void time2string(char *, int);
extern void tick(int *);
extern void delay(int);
extern int nextprime(int);

int mytime = 0x5957;
char textstring[] = "text, more text, and even more text!";

char decode[10] = {64, 121, 36, 48, 25, 18, 2, 120, 0, 16}; // decimal that reprenct the binaires thet displays the value on display

char display[6] = {0};
/* Below is the function that will be called when an interrupt is triggered. */
void handle_interrupt(unsigned cause)
{
}

/* Add your code here for initializing interrupts. */
void labinit(void)
{
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
  int hours = 0;
  int minutes = 0;
  int seconds = 0;
  int tickcount = 0;
  // Enter a forever loop
  while (1)
  {
    if (get_btn() == 1)
      break;

    delay(10);
    tickcount += 10;

    if (tickcount >= 1000)
    {
      tickcount = 0;

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
    }
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
int main()
{
  // Call labinit()

  labinit();

  for (int i = 0; i < 16; i++)
  {
    set_led(i);
    delay(1000);
  }

  clock();

  switches();

  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

  // time2string(textstring, mytime); // Converts mytime to string
  // display_string(textstring);      // Print out the string 'textstring'
  // delay(1000);                     // sec (adjust this value)
  // tick(&mytime);                   // Ticks the clock once
}
