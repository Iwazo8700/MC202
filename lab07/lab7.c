/*nome: Enzo Hideki Iwata RA:215394

header com as estruturas, bibliotecas declaradas, funcoe declaradas, e macros*/
#include "lab7.h"

int main() {
  int iteracao, maximo, carros_inseridos, carros_escolhidos, identificador, num = 0, aux;
  float ace_aux, contro_aux, vel_aux;
  Prioridade_MAX *aceleracao, *controle, *velocidade;
  No *no_aux;
  /*alocacao de memoria para os vetores e nos auxiliares*/
  aceleracao = (Prioridade_MAX*)malloc(sizeof(Prioridade_MAX));
  controle = (Prioridade_MAX*)malloc(sizeof(Prioridade_MAX));
  velocidade = (Prioridade_MAX*)malloc(sizeof(Prioridade_MAX));
  no_aux = (No*)malloc(sizeof(No));
  scanf("%d %d", &iteracao, &maximo);
  aceleracao->v = (No*)malloc((maximo)*sizeof(No));
  if(aceleracao->v == NULL)
    printf("1");
  controle->v = (No*)malloc((maximo)*sizeof(No));
  if(controle->v == NULL)
    printf("2");
  velocidade->v = (No*)malloc((maximo)*sizeof(No));
  if(velocidade->v == NULL)
    printf("3");

  /*execucao do programa, em geral as variaveis ja falam o q fazem, nesse programa fiz um heap para cada variavel(aceleracao, controle e velocidade)
  cada novo carro vem com um indicador para facilitar na retirada dos outros heaps, entao tiro o max de um e pego uma info que eh comum para
  os outros heaps nesse msm carro, apos isso retiro esse carro dos outros heaps, todas as funcoes de heap sao as basicas, em geral iterativas*/
  for(int i = 0; i < iteracao; i++){
    scanf("%d %d", &carros_inseridos, &carros_escolhidos);

    
    for(int a = 0; a < carros_inseridos; a++){
      /*inserir carros*/
      scanf("%f %f %f", &ace_aux, &contro_aux, &vel_aux);
      no_aux->info = num;
      no_aux->valor = ace_aux;
      inserir_heap(aceleracao, no_aux);
      no_aux->valor = contro_aux;
      inserir_heap(controle, no_aux);
      no_aux->valor = vel_aux;
      inserir_heap(velocidade, no_aux);
      /*printf("%.2f\n", aceleracao->v[0].valor);
      printf("%.2f\n", controle->v[0].valor);
      printf("%.2f\n", velocidade->v[0].valor);*/
      num++;
    }


    for(int a = 0; a < carros_escolhidos; a++){
      scanf("%d", &identificador);
      switch (identificador) {
        /*retirar carros*/
        case 0://aceleracao
          retirar_max(aceleracao, &aux);
          retirar_outros(velocidade, aux);
          retirar_outros(controle, aux);
          break;
        case 1:
          retirar_max(controle, &aux);
          retirar_outros(aceleracao, aux);
          retirar_outros(velocidade, aux);
          break;
        case 2:
          retirar_max(velocidade, &aux);
          retirar_outros(aceleracao, aux);
          retirar_outros(controle, aux);
          break;
      }
      printf("%.2f %.2f %.2f\n", aceleracao->v[aceleracao->n].valor, controle->v[controle->n].valor, velocidade->v[velocidade->n].valor);
    }

  }


  free(aceleracao->v);
  free(controle->v);
  free(velocidade->v);
  free(aceleracao);
  free(controle);
  free(velocidade);
  free(no_aux);
  /*libero a memoria alocada*/
  return 0;
}

/*em geral as funcoes ja falam o q fazem*/

/*insere no heap*/
void inserir_heap(Prioridade_MAX *vet, No *k){
  vet->v[vet->n] = *k;
  vet->n++;
  subir_no_heap(vet, vet->n-1);
}

/*sobe no heap*/
void subir_no_heap(Prioridade_MAX *vet, int num){
  while(num > 0 && vet->v[pai(num)].valor < vet->v[num].valor){
    Troca(&vet->v[pai(num)], &vet->v[num]);
    num = pai(num);
  }
}

/*desce no heap*/
void descer_no_heap(Prioridade_MAX *vet, int num){
  int maior;
  if(filho_esq(num) < vet->n){
  maior = filho_esq(num);
  if(filho_dir(num) < vet->n && vet->v[filho_esq(num)].valor < vet->v[filho_dir(num)].valor){
    maior = filho_dir(num);}
  if(vet->v[num].valor < vet->v[maior].valor){
    Troca(&vet->v[num], &vet->v[maior]);
    descer_no_heap(vet, maior);
    }
  return;
  }
}

/*retira o max do heap*/
void retirar_max(Prioridade_MAX *vet, int *iden){
  *iden = vet->v[0].info;
  Troca(&vet->v[0], &vet->v[vet->n - 1]);
  vet->n--;
  descer_no_heap(vet, 0);
}

/*retira outros do heap*/
void retirar_outros(Prioridade_MAX *vet, int iden){
  int luffy = 0;
  for(int i = 0; i < vet->n; i++){
    if(vet->v[i].info == iden){
      luffy = i;
      break;
    }
  }
  Troca(&vet->v[luffy], &vet->v[vet->n - 1]);
  vet->n--;
  descer_no_heap(vet, luffy);
  subir_no_heap(vet, luffy);

}

/*troca dois nos*/
void Troca(No *a, No *b){
  No aux;
  aux = *a;
  *a = *b;
  *b = aux;
}
