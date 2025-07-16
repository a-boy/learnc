#include <stdio.h>
#include <assert.h>
int main() {
    static_assert(sizeof(int) == 4, "64-bit code generation is not supported.");
    
    int x=2, y=3, z;
    z = x * x - y * y;
    assert(z >= 0);
    
    return 0;
}