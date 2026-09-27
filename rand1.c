#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main() {
    time_t now=time(NULL);
    printf("%ld,%s\n",now, ctime(&now));
    srand(time(NULL));
    int x = rand() % 100; // Generate a random number between 0 and 99
    printf("Random number: %d\n", x);

    const int N = 10;
    const int M = 20;
    x = rand() % (M - N + 1) + N;
    printf("Random number in [%d,%d]: %d\n", N,M,x);

    return 0;

}