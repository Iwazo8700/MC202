#include <stdio.h>
#include <stdlib.h>

#define pai(i) ((i-1)/2)
#define filho_esq(i) ((2*i)+1)
#define filho_dir(i) ((2*i)+2)

typedef struct{
  float valor;
  int info;
} No;

typedef struct{
  No *v;
  int n, tamanho;
} Prioridade_MAX;

void inserir_heap(Prioridade_MAX *vet, No *k);
void subir_no_heap(Prioridade_MAX *vet, int num);
void descer_no_heap(Prioridade_MAX *vet, int num);
void retirar_max(Prioridade_MAX *vet, int *iden);
void retirar_outros(Prioridade_MAX *vet, int iden);
void Troca(No *a, No *b);
