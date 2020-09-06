/*nome: Enzo Hideki Iwata     RA:215394*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define  MAX 10

//estrutura de no para arvore avl
typedef struct No{
  char palavra_chave[10];
  int altura;
  struct No *esq, *dir;
}No;

//estrutura para pilha, usada no print zigzag
typedef struct Pilha{
  No *info;
  struct Pilha *prox;
} Pilha;

void zerar_str(char *string, int a);
int procurar_palavra(char string[MAX]);
int altura_AVL(No *p);
int fator_balanceamento(No *p);
int max(int a, int b);
No *rotacao_esquerda(No *p);
No *rotacao_direita(No *p);
No *inserir_AVL(No *p, char word[]);
No *libera_arvore(No *arvore);
void print_in(No *arvore);
void print_zigzag(No *p);
Pilha *empilhar(Pilha *p, No *n);
Pilha *desempilhar(Pilha *p, No **n);


int main() {
  char base_0[MAX] = "\000\000\000\000\000\000\000\000\000";
  No *arvore_AVL = NULL;
  int contadora = 0;
  char palavra[MAX], letra;
  /*le letra por letra e vai colocando em uma string de acordo com os separadores*/
  while((letra = getchar()) != EOF){
    if (((letra >= 97 && letra <= 122) || (letra == 95) || (letra <= 90 && letra >= 65) || (letra>=48 && letra<=57)) && contadora <= 10){
      palavra[contadora++] = letra;
    }else{
      if(strcmp(palavra,base_0) != 0 && procurar_palavra(palavra) )
        arvore_AVL = inserir_AVL(arvore_AVL, palavra);
        //printf("%s\n", palavra);
      contadora = 0;
      zerar_str(palavra, MAX);
    }
  }
  //print_in(arvore_AVL);
  printf("O percurso da árvore é:\n");
  print_zigzag(arvore_AVL);
  arvore_AVL = libera_arvore(arvore_AVL);

  return 0;
}

//deixa uma string totalmente zerada
void zerar_str(char string[], int a){
  for(int i = 0; i < a; i++){
    string[i] = 0;
  }
}

//procura dentre as palavras dedicadas a entrar na arvore
int procurar_palavra(char string[MAX]){
  char lista_de_palavras[32][MAX] = {"auto","break","case","char","const","continue",
  "default","do","double","else","enum","extern","float","for","goto","if","int","long",
  "register","return","short","signed","sizeof","static","struct","switch","typedef",
  "union","unsigned","void","volatile","while"};
  for(int i = 0; i < 32; i++){
    if(strcmp(lista_de_palavras[i], string) == 0){
      zerar_str(lista_de_palavras[i], MAX);
      return 1;}
  }
  return 0;
}

//retorna a altura do no
int altura_AVL(No *p){
  if (p == NULL)
    return 0;
  return p->altura;
}

//faz o calculo do balanceamento
int fator_balanceamento(No *p){
  if(p == NULL)
    return 0;
  return altura_AVL(p->esq) - altura_AVL(p->dir);
}

//maximo entre dois inteiros
int max(int a, int b){
  if(a >= b)
    return a;
  return b;
}

//faz a rotacao para esquerda na arvore avl
No *rotacao_esquerda(No *p){
  No *a = p->dir;
  No *b = a->esq;

  a->esq = p;
  p->dir = b;

  p->altura = max(altura_AVL(p->esq), altura_AVL(p->dir)) + 1;
  a->altura = max(altura_AVL(a->esq), altura_AVL(a->dir)) + 1;

  return a;
}

//faz a rotacao para direita na arvore avl
No *rotacao_direita(No *p){
  No *a = p->esq;
  No *b = a->dir;

  a->dir = p;
  p->esq = b;

  p->altura = max(altura_AVL(p->esq), altura_AVL(p->dir)) + 1;
  a->altura = max(altura_AVL(a->esq), altura_AVL(a->dir)) + 1;

  return a;
}

//funcao para inserir elementos em uma arvore AVL, eh baseada no codigo do rpofessor
//a parte dos casos tbm esta enumerada como ele enumerou
No *inserir_AVL(No *p, char word[]){
  int balanceamento;
 //insercao normal em arvore binaria
  if(p == NULL){
    No *novo = (No*) malloc(sizeof(No));
    strcpy(novo->palavra_chave, word);
    novo->altura = 1;
    novo->dir = NULL;
    novo->esq = NULL;
    return novo;
  }

  if(strcmp(p->palavra_chave, word) < 0){
    p->dir = inserir_AVL(p->dir , word);
  }else if(strcmp(p->palavra_chave, word) > 0)
    p->esq = inserir_AVL(p->esq, word);
  else
    return p;

  // atualizar altura do no ancestral

  p->altura = max(altura_AVL(p->dir), altura_AVL(p->esq)) + 1;

  balanceamento = fator_balanceamento(p);

  /* 4 casos devem ser considerados quando um no se torna desbalanceado */

  /* caso  1.1:  rotacao  simples (direita) */
  if (balanceamento  > 1 && p->esq != NULL && strcmp(p->esq->palavra_chave, word) > 0)
    return  rotacao_direita(p);
  /* caso 2.1: rotacao simples (esquerda) */
  if (balanceamento < -1 && p->dir != NULL && strcmp(p->dir->palavra_chave, word) < 0)
    return rotacao_esquerda(p);
  /* caso 1.2: rotacao dupla (esquerda e direita) */
  if (balanceamento > 1 && p->esq != NULL && strcmp(p->esq->palavra_chave, word) < 0){
    p->esq = rotacao_esquerda(p->esq);
    return rotacao_direita(p);
  }
  /* caso 2.2: rotacao dupla (direita e esquerda) */
  if (balanceamento < -1 && p->dir != NULL && strcmp(p->dir->palavra_chave, word) > 0){
    p->dir = rotacao_direita(p->dir);
    return rotacao_esquerda(p);
  }
  return p;
}

//libera a arvore
No *libera_arvore(No *arvore){
 if (arvore != NULL){
   arvore->esq = libera_arvore(arvore->esq);
   arvore->dir = libera_arvore(arvore->dir);
   free(arvore);
 }
 return NULL;
}

void print_zigzag(No *p){
  No *aux;
  Pilha *dir_esq = NULL;
  Pilha *esq_dir = NULL;
  dir_esq = empilhar(dir_esq, p);
  while(1) {
    if(dir_esq != NULL){
      printf("[ ");
      while (dir_esq != NULL) {
        //dir_esq = desempilhar(dir_esq, &aux);
        dir_esq = desempilhar(dir_esq, &aux);
        printf("%s ", aux->palavra_chave);
        if(aux->dir != NULL)
          esq_dir = empilhar(esq_dir, aux->dir);
        if(aux->esq != NULL)
          esq_dir = empilhar(esq_dir, aux->esq);
      }
      printf("]\n");
    }
    if(esq_dir == NULL && dir_esq == NULL)
      break;
    if(esq_dir != NULL){
      printf("[ ");
      while (esq_dir != NULL) {
        //esq_dir = desempilhar(esq_dir, &aux);
        esq_dir = desempilhar(esq_dir, &aux);
        printf("%s ", aux->palavra_chave);
        if(aux->esq != NULL)
          dir_esq = empilhar(dir_esq, aux->esq);
        if(aux->dir != NULL)
          dir_esq = empilhar(dir_esq, aux->dir);
      }
      printf("]\n");
    }
    if(esq_dir == NULL && dir_esq == NULL)
      break;
  }
  return;
}

//funcao para empilhar nos em uma pilha
Pilha *empilhar(Pilha *p, No *n){
  Pilha *novo = (Pilha*) malloc(sizeof(Pilha));
  if(novo != NULL){
    novo->info = n;
    novo->prox = p;
  }
  return novo;
}

//funcao para desempilhar nos em uma pilha
Pilha *desempilhar(Pilha *p, No **n){
  Pilha *aux;
  aux = p;
  *n = p->info;
  p = p->prox;
  free(aux);
  return p;
}

/*No *desempilhar(Pilha *p, No *n){
  Pilha *aux;
  aux = p;
  n = p->info;
  p = p->prox;
  free(aux);
  return n;
}*/

void print_in(No *arvore){
  if(arvore != NULL){
    print_in(arvore->esq);
    printf("%s\n", arvore->palavra_chave);
    print_in(arvore->dir);
  }
}
