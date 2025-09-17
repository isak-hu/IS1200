#include <stdio.h>
#include <stdlib.h>
#define COLUMNS 6


void print_number(int n) {
    static int count = 0; // Kollar vilken kolumn varje tal skrivs ut i
    printf("%10d", n);    // Skriv ut talet med mellanrum
    count++;

    if (count % COLUMNS == 0) {
        printf("\n"); // Byt rad när antal outputs = antal kolumner
    }
}

int print_sieves(int n){
    int *is_prime = malloc((n+1) * sizeof(int));
    // Initiera alla tal som primtal (1)
    for (int i = 0; i <= n; i++) {
        is_prime[i] = 1;
    }

    // För varje tal från 2 till sqrt(n), 
    // Om p är markerat som primtal, markera alla multiplar av p som icke-primtal (0)
    for (int i = 2; i*i <= n; i++) {
        if (is_prime[i]) {
            for (int j = i*i; j <= n; j += i) {
                is_prime[j] = 0;
            }
        }
    }

    // Printa alla tal som fortfarande är markerade som primtal
    for (int i = 2; i <= n; i++) {
        if (is_prime[i]) {
            print_number(i);
        }
    }
    free(is_prime);
    printf("\n"); // Avsluta med en ny rad
}

int main() {
    print_sieves(105);  
    return 0;
}