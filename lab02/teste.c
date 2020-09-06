#include<stdio.h>

int main(){
  int b,c;
  char a;
  b = (1&3)<<6;
  c = ((3&3)<<4);
  a = b + c;
  printf("%d %c\n", a, a);
}
