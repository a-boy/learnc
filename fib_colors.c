#include <stdio.h>
int main() {
    long a = 0, b = 1, c = 1;
    while (c > 0) {
        //printf("\x1b[91m%5ld \x1b[92m%5ld \x1b[94m%5ld\n", a, b, c);
        //printf("\x1b[91m%5ld \x1b[93m%5ld \x1b[96m%5ld\n", a, b, c);
        //printf("\x1b[94m%5ld \x1b[96m%5ld \x1b[97m%5ld\n", a, b, c);
        printf("\x1b[95m%5ld \x1b[92m%5ld \x1b[94m%5ld\n", a, b, c);
        //printf("\x1b[95m%5ld \x1b[92m%5ld \x1b[103m\x1b[94m%5ld\x1b[0m\n", a, b, c);
        a = b + c, b = c + a, c = a + b;
    }
    return 0;
}
