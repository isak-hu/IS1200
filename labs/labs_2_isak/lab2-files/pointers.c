/*
 pointers.c
 By David Broman.
 Last modified: 2015-09-15
 This file is in the public domain.
*/

#include <stdio.h>
#include <stdlib.h>

void copy_codes(char *texta0, int *lista1, int *counta2);
void work(void);

char *text1 = "This is a string.";
char *text2 = "Yet another thing.";

int list1[20];
int list2[20];

int counter = 0;

void printlist(const int *lst)
{
  printf("ASCII codes and corresponding characters.\n");
  while (*lst != 0)
  {
    printf("0x%03X '%c' ", *lst, (char)*lst);
    lst++;
  }
  printf("\n");
}

void endian_proof(const char *c)
{
  printf("\nEndian experiment: 0x%02x,0x%02x,0x%02x,0x%02x\n",
         (int)*c, (int)*(c + 1), (int)*(c + 2), (int)*(c + 3));
}
void work()
{

  char *texta0 = text1;
  int *lista1 = list1;
  int *counta2 = &counter;

  copy_codes(texta0, lista1, counta2);

  texta0 = text2;
  lista1 = list2;

  copy_codes(texta0, lista1, counta2);
}

void copy_codes(char *texta0, int *lista1, int *counta2)
{
  while (*texta0 != 0)
  {
    *lista1 = (int)*texta0;
    texta0++;
    lista1++;
    (*counta2)++;
  }
  *lista1 = 0;
}

int main(void)
{

  work();
  printf("\nlist1: ");
  printlist(list1);
  printf("\nlist2: ");
  printlist(list2);
  printf("\nCount = %d\n", counter);

  endian_proof((char *)&counter);
}
