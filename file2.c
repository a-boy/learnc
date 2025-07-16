#include <stdio.h>
#include <stdlib.h>


int main() {
    rename("hi.txt", "bar.txt");
    // remove("bar.txt");
    char* val = getenv("PATH");
    printf("getenv PATH: \n%s\n", val);
}