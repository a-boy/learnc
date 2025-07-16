#include <stdio.h>
#include <string.h>
#include <wchar.h>
int main(){
    char* s = "\u6625\u5929";
    printf("%s\n", s);
    char* s2="春天";
    printf("%s  %d\n", s2,strlen(s));

    wchar_t* s3 = L"春天";
    printf("%ls\n", s3);
    return 0;
}
