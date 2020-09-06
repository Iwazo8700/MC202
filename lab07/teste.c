#include <stdio.h>

typedef struct{
  float valor;
  int info;
} No;

void Troca(No *a, No *b){
  No aux;
  aux = *a;
  *a = *b;
  *b = aux;
}

int main() {
  /* code */
  No a,b;
  a.valor = 10;
  b.valor = 8;
  printf("%f %f\n", a.valor, b.valor);
  Troca(&a, &b);
  printf("%f %f\n", a.valor, b.valor);
  return 0;
}
