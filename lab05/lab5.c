#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct No {
  char info[21];
  struct No *prox;
  struct No *ant;
} No;

int contadora = 0;

char zerar_str(char string, int b);
void troca_No(No *no1, No *no2);
void print_No(No *lista);
No* insere_No(char nome, No *lista, int posicao);

int main() {
  No *lista = (No*) malloc(sizeof(No));
  lista->prox = lista;
  lista->ant = lista;
  char string[21], nome1[21], nome2[21], aux;
  int i = 0;//le a primeira linha de nomes da lista
  scanf("%c", &aux);
  while(aux != '\n'){
    if(aux == ' '){
      contadora += 1;
      lista = insere_No(string, lista, 1);
      i = 0;
      string = zerar_str(string, 21);
    } else {
      string[i++] = aux;
    }
    scanf("%c", &aux);
  }
  lista = insere_No(string, lista, 1);
  print_No(lista);
  return 0;
}


char zerar_str(char string[], int b){
  for(int a = 0; a < b; a++){
    string[a] = 0;
  }
  return string;
}


No* insere_No(char nome, No *lista, int posicao){
  int bol = 1;
  No *cabeca = lista;
  No *aux = lista;
  for(int i = 0; i < contadora; i++){
    if(aux->info == nome){
      bol = 0;
      break;}
    aux = aux->prox;}
  for (int i = 0; i < posicao; i++){
    lista = lista->prox;
  }
  if (bol){//nao pertence a lista, entao cria novo no
    No *aux = (No*) malloc(sizeof(No));
    aux->prox = lista->ant->prox;
    aux->ant = lista->ant;
    lista->ant->prox = aux;
    lista->ant = aux;
  }else
    troca_No(aux, lista);
  return(aux);
}

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

void print_No(No *lista){
  No *outro = lista;
  lista = lista->prox;
  while(outro->info != lista->info){
    printf("%s ", lista->info);
  }
  printf("\n");
}
