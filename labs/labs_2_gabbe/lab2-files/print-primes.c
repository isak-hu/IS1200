/*
 print-primes.c
 By David Broman.
 Last modified: 2015-09-15
 This file is in the public domain.
*/


#include <stdio.h>
#include <stdlib.h>

#define COLUMNS 6

int is_prime(int n) {
    if (n <= 1) {
        return 0; // inte primtal
    }

    // Kollar delbarhet från 2 till n-1
    for (int i = 2; i < n; i++) {
        if (n % i == 0) return 0; // n är delbart med i, inte primtal
    }

    return 1; // n är primtal
}

void print_number(int n) {
    static int count = 0; // Kollar vilken kolumn varje tal skrivs ut i
    printf("%10d", n);    // Skriv ut talet med mellanrum
    count++;

    if (count % COLUMNS == 0) {
        printf("\n"); // Byt rad när antal outputs = antal kolumner
    }
}

void print_primes(int n) {
  for (int i = 2; i <= n; i++) {
    if (is_prime(i)) {
      print_number(i);
    }
  }
  printf("\n"); // Avsluta med en ny rad
}

// 'argc' contains the number of program arguments, and
// 'argv' is an array of char pointers, where each
// char pointer points to a null-terminated string.
int main(int argc, char *argv[]){
    if(argc == 2)
    {
        print_primes(atoi(argv[1]));
    }
  else
    printf("Please state an integer number.\n");
  return 0;
}

 
