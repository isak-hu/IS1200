/*
 pointers.c
 By David Broman.
 Last modified: 2015-09-15
 This file is in the public domain.
*/


#include <stdio.h>
#include <stdlib.h>

void copycodes(char *a0, int *a1, int *a2);
void work();

int list1[20];
int list2[20];
int counter = 0;

char* text1 = "This is a string.";
char* text2 = "Yet another thing.";

void printlist(const int* lst){
  printf("ASCII codes and corresponding characters.\n");
  while(*lst != 0){
    printf("0x%03X '%c' ", *lst, (char)*lst);
    lst++;
  }
  printf("\n");
}

void endian_proof(const char* c){
  printf("\nEndian experiment: 0x%02x,0x%02x,0x%02x,0x%02x\n", 
         (int)*c,(int)*(c+1), (int)*(c+2), (int)*(c+3));
  
}

void work() {
    char* a0 = text1; // la a0, text1
    int* a1 = list1; // la a1, list1
    int* a2 = &counter; // la a2, counter
    copycodes(a0, a1, a2); // jal copycodes
    
    a0 = text2; // la a0, text2
    a1 = list2; // la a1, list2
    a2 = &counter; // la a2, counter
    copycodes(a0, a1, a2); // jal copycodes
}

void copycodes(char* a0, int* a1, int* a2) {
    char t0 = *a0;          // lb t0,0(a0)
    while (t0 != 0) {       // beqz t0,done 
        *a1 = t0;           // sw t0,0(a1)

        a0++;               // addi a0,a0,1
        a1++;               // addi a1,a1,4

        int t1 = *a2;       // lw t1,0(a2)
        t1++;               // addi t1,t1,1
        *a2 = t1;           // sw t1,0(a2)

        t0 = *a0;           // lb t0,0(a0) - load next char for next iteration
    }                       
}                            

int main(void){
 
    work();
    printf("\nlist1: ");
    printlist(list1);
    printf("\nlist2: ");
    printlist(list2);
    printf("\nCount = %d\n", counter);

    endian_proof((char*) &counter);
}
