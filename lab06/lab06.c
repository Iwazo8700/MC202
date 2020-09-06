#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*nome: Enzo Hideki Iwata RA:215394*/

/*defini Pilha, Fila e uma pilha para dados*/
typedef struct Pilha{
  int info;
  struct Pilha *prox, *ant;
} Pilha;

typedef struct Fila{
  int info;
  struct Fila *prox, *fim, *inicio;
} Fila;

typedef struct Dados{
  int info;
  char pessoa[52];
  struct Dados *prox;
} Dados;

Fila *inserir_Fila(Fila *f, int n);
Fila *remover_Fila(Fila *f, int *n);
Pilha *inserir_Pilha(Pilha *p, int n);
Pilha *remover_Pilha(Pilha *p);
Dados *inserir_Dados(Dados *d, int n, char *string);
char zerar_str(char string[], int b);
void printar_Fila(Fila *f);
void printar_Pilha(Pilha *p);
void printar_Dados(Dados *d);
Pilha *Liberar_pilha(Pilha *p);
Fila *Liberar_fila(Fila *f);
Dados *Liberar_dados(Dados *d);

int main() {
  Pilha *cadeira = NULL;
  Fila *lista = (Fila*) malloc(sizeof(Fila));
  lista->fim = NULL;
  lista->inicio = NULL;
  Dados *print = NULL;
  /*executa cada comando e joga para funcao certa*/

  char comando, lixo, line[52];
  int num, numero = 1, outro;
  scanf("%c", &comando);
  while(comando != 'D'){
    if (comando == 'Q'){
      scanf("%c", &comando);
      scanf("%d", &num);getchar();
      if (comando == 'I'){
        for(int i = 0; i < num; i++){
          scanf(" %[^\n]", line);
          lista = inserir_Fila(lista, numero);
          print = inserir_Dados(print, numero, line);
          numero++;
          zerar_str(line, 52);
        }
      }
      else if (comando == 'R'){
        for(int i = 0; i < num; i++){
          lista = remover_Fila(lista, &outro);
          cadeira = inserir_Pilha(cadeira, outro);
        }
      }
    }
    else if (comando == 'S'){
      scanf("%c", &lixo);
      scanf("%d", &num);getchar();
      for (int i = 0; i < num; i++) {
        cadeira = remover_Pilha(cadeira);
      }

    }
    else if (comando == 'P'){
      printar_Fila(lista);
      printar_Pilha(cadeira);}
    scanf("%c", &comando);
  }
  cadeira = Liberar_pilha(cadeira);
  printar_Dados(print);
  //while(lista != NULL){
  //  remover_Fila(lista, &num);
  //}
  lista = Liberar_fila(lista);
  cadeira = Liberar_pilha(cadeira);
  print = Liberar_dados(print);
  return 0;
}

/*insiro na fila*/
Fila *inserir_Fila(Fila *f, int n){
  Fila *novo = (Fila*)malloc(sizeof(Fila));
  if (novo == NULL)
    exit(1);
  novo->info = n;
  novo->prox = NULL;
  if(f->fim == NULL)
    f->inicio = novo;
  else
    f->fim->prox = novo;
  f->fim = novo;
  return (f);
}

/*removo da fila*/
Fila *remover_Fila(Fila *f, int *n){
  Fila *aux;
  if(f->inicio == NULL)
  return NULL;
  aux = f->inicio;
  *n = aux->info;
  f->inicio = aux->prox;
  free(aux);
  if (f->inicio == NULL)
    f->fim = NULL;
  return (f);
}
/*insiro na pilha*/
Pilha *inserir_Pilha(Pilha *p, int n){
  Pilha *novo = (Pilha*) malloc(sizeof(Pilha));
  if (novo != NULL){
  novo->info = n;
  novo->prox = p;
  novo->ant = NULL;
  if(p!=NULL){
    p->ant = novo;
  }
  return(novo);}
  else
  exit(1);
}
/*removo da pilha*/
Pilha *remover_Pilha(Pilha *p){
  Pilha *aux;
  aux = p;
  p = p->prox;
  if(p == NULL) return NULL;
  p->ant = NULL;
  free(aux);
  return p;
}
/*insiro na pilha de dados*/
Dados *inserir_Dados(Dados *d, int n, char *string){
  Dados *novo = (Dados*)malloc(sizeof(Dados));
  if (novo == NULL)
    exit(1);
  novo->info = n;
  strcpy(novo->pessoa, string);
  novo->prox = d;
  return (novo);
}
/*zerar string para nao dar erro de string*/
char zerar_str(char string[], int b){
  for(int a = 0; a < b; a++){
    string[a] = 0;
  }
  return *string;
}
/*printar a fila*/
void printar_Fila(Fila *f){
  Fila *aux;
  aux = f->inicio;
  printf("[Q]");
  while(aux != NULL){
    printf("%d,", aux->info);
    aux = aux->prox;
  }
  printf("\n");
  return ;
}
/*printar a pilja, primeiro vou da o primeiro elemento da plha, depois printo voltando*/
void printar_Pilha(Pilha *p){
  Pilha *aux;
  aux = p;
  if(aux == NULL){
    for(int i = 1; i <= 6; i++){
      printf("[S%d]\n", i);
    }return;
  }else{
    while(aux->prox != NULL)
      aux = aux->prox;
  for(int i = 1; i <= 6; i++){
    printf("[S%d]", i);
    int j = 0;
    while(j < 5 && aux != NULL){
      printf("%d,", aux->info);
      aux = aux->ant;
      j++;
    }
    printf("\n");}
    return ;}
}

/*printar dados*/
void printar_Dados(Dados *d){
  if(d == NULL)
    return ;
  printar_Dados(d->prox);
  printf("%d,%s\n", d->info, d->pessoa);
  return ;
}
/*liberar espaco caso necessario*/
Pilha* Liberar_pilha(Pilha *p){
  Pilha *aux;
  while(p != NULL){
    aux = p;
    p = p->prox;
    free(aux);
  }
  return NULL;
}
Dados *Liberar_dados(Dados *d){
  Dados *aux;
  while (d != NULL) {
    aux = d;
    d = d->prox;
    free(aux);
  }
  return NULL;
}
Fila *Liberar_fila(Fila *f){
  Fila *aux;
  while(f != NULL){
    aux = f;
    f = f->prox;
    free(aux);
  }
  return NULL;
}
