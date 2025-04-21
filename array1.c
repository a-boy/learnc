#include <stdio.h>

int main(void) {
	int a[15] = {1, [5] = 10, 11, [10] = 20, 21};
	int b[2][2] = {1, 0, 0, 2};
	printf("%-5d",a[6]);
	return 0;
}
