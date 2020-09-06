#include <stdio.h>
#include<stdlib.h>

typedef struct Pilha{
  int info;
  struct Pilha *prox;
} Pilha;

Pilha *inserir_Pilha(Pilha *p, int n){
  Pilha *novo = (Pilha*) malloc(sizeof(Pilha));
  if (novo != NULL){
  novo->info = n;
  novo->prox = p;
  return(novo);}
  else
  exit(1);
}
int main() {
  Pilha *pilha = NULL;
  pilha = inserir_Pilha(pilha, 1);
  return 0;
  free(pilha);
}
