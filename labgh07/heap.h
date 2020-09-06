#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#define pai(i) ((i-1)/2)
#define filho_esq(i) ((2*i)+1)
#define filho_dir(i) ((2*i)+2)

typedef struct{
  char nome[100];
  int info;
} No;

typedef struct{
  No *v;
  int n;
} Prioridade;

void inserir_heapmax(Prioridade *vet, No *k);
void retirar_max(Prioridade *vet, No *iden);
void inserir_heapmin(Prioridade *vet, No *k);
void retirar_min(Prioridade *vet, No *iden);
#include "heap.c"
