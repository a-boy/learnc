#include <stdio.h>

int main(void) {
  FILE* fp;
  char c;

  fp = fopen("hello.txt", "r");
  if (fp == NULL) {
    return -1;
  }

  char str[100];
  c = fgetc(fp);
  fgets(str, sizeof(str), fp);
  printf("%s\n", str);

  fclose(fp);

  return 0;
}