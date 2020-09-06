#include<stdio.h>

int main(){
  unsigned char foo = 100;
  foo &= 0x8;
  printf("%d\n", foo);
}
