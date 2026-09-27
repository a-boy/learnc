#include <stdio.h>

int main() {
    long a = 0, b = 1, c = 1;
    while (c > 0 && c < 10000) { // 限制最大值避免爆屏
        printf("%5ld: ", c);
        for (int i = 0; i < c / 100; i++) {
            putchar('*');
        }
        putchar('\n');
        a = b + c;
        b = c + a;
        c = a + b;
    }
    return 0;
}
