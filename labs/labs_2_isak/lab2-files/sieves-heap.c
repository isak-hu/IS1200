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

void print_sieves(int n)
{
    char *prime = malloc((n + 8) * sizeof(char));

    prime[0] = 0;
    prime[1] = 0;

    for (int i = 2; i <= n; i++)
    {
        prime[i] = 1;
    }

    for (int p = 2; p <= n; p++)
    {
        if (prime[p] == 1)
        {

            print_number(p);

            for (int m = 2; m * p < n; m++)
            {
                prime[m * p] = 0;
            }
        }
    }
    printf("\n");
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
