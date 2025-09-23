/*
 print-primes.c
 By David Broman.
 Last modified: 2015-09-15
 This file is in the public domain.
*/

#include <stdio.h>
#include <stdlib.h>

#define COLUMNS 6

void print_number(int n);
int is_prime(int n);

int* array;


void print_sieves(int n)
{
    int size = n*n+1;

    int* array = malloc(size * sizeof(int));

    array[1] = 2;


    char prime[n*n + 8];

    for (int i = 2; i <= n*n; i++)
    {
        prime[i] = 1;
    }

    for (int p = 2,i = 1; p <= n*n; p++)
    {
        if (prime[p] == 1)
        { 
            array[i] = p;
            i++;

            for (int m = 2; m * p < n*n; m++)
            {
                prime[m * p] = 0;
            }
        }
    }
    printf("%d\n", array[n]);
}

void print_number(int n)
{
    static int counter = 0;

    printf("%4d", n);
    counter++;

    if (counter == COLUMNS)
    {
        printf("\n");
        counter = 0;
    }
}

// 'argc' contains the number of program arguments, and
// 'argv' is an array of char pointers, where each
// char pointer points to a null-terminated string.
int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        print_sieves(atoi(argv[1]));
    }
    else
        printf("Please state an integer number.\n");
    return 0;
}
