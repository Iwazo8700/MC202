#include<stdio.h>
#include<stdlib.h>

typedef struct No {
  int info;
  struct No *prox;
} No;


No *remove(No *lista, int n){
  /*essa funcao sera feita para duas situacoes base, em geral tentarei pegar o
  no seguinte e verificar, e assim conectar o atual com o seguinte do seguinte
  mas antes irei verificar o atual(primeira situacao da recursao)*/
  No *aux;
  if (lista->info == n){
    aux = lista;
    lista = lista->prox;
    free(aux);
    return(lista);
  }else if(lista->prox->info == n){
      aux = lista->prox;
      lista->prox = lista->prox->prox;
      free(aux);
      return(lista);
    }
  return(remove(lista->prox, n));
}
int main() {
  int n;
  No *lista1;
  remove(lista1, n);
  return 0;
}
