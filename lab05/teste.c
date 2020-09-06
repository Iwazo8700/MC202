#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct No {
  int info;
  struct No *prox;
  struct No *ant;
} No;
void troca_No(No *no1, No *no2){
  No *aux;
  *aux = *no1->prox->ant;
  *no1->prox->ant = *no2->prox->ant;
  *no2->prox->ant = *aux;
  *aux = *no1->prox;
  *no1->prox = *no2->prox;
  *no2->prox = *aux;
  *aux = *no1->ant->prox;
  *no1->ant->prox = *no2->ant->prox;
  *no2->ant->prox = *aux;
  *aux = *no1->ant;
  *no1->ant = *no2->ant;
  *no2->ant = *aux;
}

int main() {
  No *lista = (No*) malloc(sizeof(No));
  No *novo = (No*) malloc(sizeof(No));
  No *novo1 = (No*) malloc(sizeof(No));
  lista->prox = novo;
  novo->prox = novo1;
  novo1->prox = lista;
  lista->ant = novo1;
  novo1->ant = novo;
  novo->ant = lista;
  novo->info = 1;
  novo1->info = 2;
  troca_No(novo1, novo);
  for(int i = 0; i < 2; i++){
    printf("%d\n", lista->info);
    lista = lista->prox;
  }
  return 0;}
