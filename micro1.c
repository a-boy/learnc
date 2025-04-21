#include <stdio.h>
#define DEBUG 1
#define OW "C programming language is invented \
in 1970s."

#define XNAME(n) "x"#n
#define MK_ID(n) i##n
#define X(...) #__VA_ARGS__

//#undef OW

/* #if __STDC_VERSION__ != 201112L
  #error Not C11
#endif */

// 使用 C99 标准
#pragma c9x on

int main(void) {
	int i=-14;
	printf("%+d,%+x\n",i,i);
	
#if DEBUG
	printf("%s\n",OW);
#endif	
	// 输出 x4
	printf("%s\n", XNAME(4));
	
	int MK_ID(1), MK_ID(2), MK_ID(3);
	
	printf("%s\n", X(1,2,3));  // Prints "1, 2, 3"
	
	printf("%s,%s,%s,%s\n",__DATE__,__TIME__,__FILE__,__func__);
	printf("This line: %d\n", __LINE__);
	printf("C Version: %ld\n", __STDC_VERSION__);
	
	return 0;
}

/* gcc -Iinclude/ -o code code.c 
gcc -DDEBUG=1 micro1.c
*/