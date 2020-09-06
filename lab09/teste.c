#include <stdio.h>
#include <stdlib.h>
typedef struct Pilha{
  int info;
  struct Pilha *prox;
} Pilha;


Pilha *empilhar(Pilha *p, int n){
  Pilha *novo = (Pilha*) malloc(sizeof(Pilha));
  if(novo != NULL){
    novo->info = n;
    novo->prox = p;
  }
  return novo;
}


Pilha *desempilhar(Pilha *p, int *n){
  Pilha *aux;
  aux = p;
  *n = p->info;
  p = p->prox;
  free(aux);
  return p;
}

int main() {
  Pilha *pilha;
  pilha = NULL;
  int aux;
  pilha = empilhar(pilha, 1);
  pilha = desempilhar(pilha, &aux);
  return 0;
}
