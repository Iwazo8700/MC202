#include "heap.h"

int main() {
  /*logica basica do programa se da por dois heaps, um heap max e um min, decido
  que o primeiro vai ser a base da mediana, entao adiono ele no max, depois para cada nova
  entrada se ela for maior que a mediana eu coloco no heap max, se for maior coloco no heap min,
  entao teremos dois heap um max e um min, e um dos topos sera a mediana(senao ambos), quando a
  diferenca entre o tamanaho do min com o max da maior que 1 ou meor q -1 quer deizer que um dos heap tem mais
  elementos que devia, assim eu jogo o topo desse heap para o outro, assim o tamanho entre eles fica igual
  e tenho claramente uma quantidade igual(ou com 1 de dif)em cada heap, assim se um dos heap eh
  uma vez maior quer dizer q seu topo eh a mediana, no caso dos heaps terrem o msm tamanho dizemos
  que temos uma lista de numero par e o topo dos dois fazem parte da mediana e arbitraroiamente nomeio
  um deles como nova mediana so para ter uma base*/
  int numero, i, mediana, balanca;
  Prioridade *maximo = (Prioridade*)malloc(sizeof(Prioridade));
  Prioridade *minimo = (Prioridade*)malloc(sizeof(Prioridade));
  No *novo = (No*)malloc(sizeof(No));
  scanf("%d", &numero);
  scanf("%s%d", novo->nome, &novo->info);
  mediana = novo->info;
  maximo->v = (No*)malloc(numero*sizeof(No));
  minimo->v = (No*)malloc(numero*sizeof(No));
  inserir_heapmax(maximo, novo);
  printf("%s: %d\n", novo->nome, novo->info);
  for(i = 0; i < numero-1; i++){
    scanf("%s%d", novo->nome, &novo->info);
    if(novo->info > mediana)
      inserir_heapmin(minimo, novo);
    else
      inserir_heapmax(maximo, novo);
    balanca = minimo->n - maximo->n;
    if(balanca == 2){
      retirar_min(minimo, novo);
      inserir_heapmax(maximo, novo);
    }else if(balanca == -2){
      retirar_max(maximo, novo);
      inserir_heapmin(minimo, novo);
    }
    balanca = minimo->n - maximo->n;
    if(balanca == 0){
      printf("%s: %d\n", maximo->v[0].nome, maximo->v[0].info);
      printf("%s: %d\n", minimo->v[0].nome, minimo->v[0].info);
      mediana = maximo->v[0].info;
    }else if(balanca == 1){
      printf("%s: %d\n", minimo->v[0].nome, minimo->v[0].info);
      mediana = minimo->v[0].info;
    }else if(balanca == -1){
      printf("%s: %d\n", maximo->v[0].nome, maximo->v[0].info);
      mediana = maximo->v[0].info;
    }

  }
  free(novo);
  free(maximo->v);
  free(minimo->v);
  free(maximo);
  free(minimo);
  return 0;
}
