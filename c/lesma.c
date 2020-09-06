#include<stdio.h>
int main(){
  int primeiro, n, maior, maior_n, aux;
  maior = 0;
  scanf("%d\n", &primeiro);
  for(int i = 0; i < primeiro; i += 1){
    scanf("%d\n", &n);
    if(n <= 10){aux = n*3;} else
    if(n > 10 && n <= 20){ aux = n*2;}else
    if(n > 20){aux = n;}
    if(aux > maior){maior_n = n;maior = aux;}}
  printf("%d\n", maior_n);
}
