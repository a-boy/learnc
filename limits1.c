#include <stdio.h>
#include <limits.h>
int main() {
    printf("%d,%d,%d\n", CHAR_MAX, UCHAR_MAX, CHAR_MIN);
    printf("%d,%d\n", INT_MAX, INT_MIN);
    printf("%ld,%lld\n", LONG_MAX, LLONG_MIN);
    return 0;
}