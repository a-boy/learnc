#include <stdio.h>
main(m,k){for(;m%k--?:(k=m++);k^1?:printf("%i|",m));}
// 性能不如 primes_60B.c