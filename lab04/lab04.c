#include<stdio.h>
#include<stdlib.h>
/*Nome: Enzo Hideki Iwata RA:215394*/
typedef struct No {
  int info;
  struct No *prox;
} No;

//No *cria_lista();
No *insere(No *lista, int n);
int pertence(No *lista, int n);
No *remover(No *lista, int n);
No *uniao(No *vet1, No *vet2);
No *interseccao(No *vet1, No *vet2);
No *subtracao(No *vet1, No *vet2);
void printar(No *lista);
void print(No *vet1, No *vet2);
void liberar_lista(No *lista);

int main() {
  No *lista1, *lista2;
  int elemento, num;
  char comando = 'a';
  lista1 = NULL;
  lista2 = NULL;
  //lista1 = cria_lista();
  //lista2 = cria_lista();
  while (comando != 's'){
    scanf(" %c", &comando);
    if(comando == 'i'){
      scanf("%d %d", &elemento, &num);
      if (num == 1)
        lista1 = insere(lista1, elemento);
      else
        lista2 = insere(lista2, elemento);
    }
    else if(comando == 'p'){
      scanf(" %d %d", &elemento, &num);
      if (num == 1){
        if(pertence(lista1, elemento))
          printf("SIM\n");
        else
        printf("NAO\n");}
      if (num == 2){
        if(pertence(lista2, elemento))
          printf("SIM\n");
        else
        printf("NAO\n");}
    }
    else if(comando == 'r'){
      scanf(" %d %d", &elemento, &num);
      if (num == 1)
        lista1 = remover(lista1, elemento);
      else
        lista2 = remover(lista2, elemento);}
    else if(comando == 'u')
      lista1 = uniao(lista1, lista2);
    else if(comando == 'x')
      lista1 = interseccao(lista1, lista2);
    else if(comando == 'b')
      lista1 = subtracao(lista1, lista2);
    print(lista1, lista2);
  }
  liberar_lista(lista1);
  liberar_lista(lista2);
  return 0;
}

/*No *cria_lista(){
  No *novo;
  novo = (No*) malloc(sizeof(No));
  novo->prox = NULL;
  return (novo);
}*/

/*a funcao de inserir ja ordenara a lista*/
No *insere(No *lista, int n){
  /*para o caso de lista nula somente adiciono um novo elemento na lista*/
  if (lista == NULL){
    No *novo = (No*) malloc(sizeof(No));
    if (novo == NULL)
      exit(1);
    novo->info = n;
    novo->prox = NULL;
    return novo;
    /*se a lista nao eh nula olharei a informacao de dois espacos, a do atual e
    a do seguinte, se o elemnto estiver entre eles adiciono ele entre eles, se
    o elemento formaior que o atual e o prox for nulo entao ele adiona no if de cima
    , agora se o elemento for menor que o atual entao adiciono ele no comeco, ja
    que essa situacao so pode acontecer em primeiro caso, retornarei o que algum dos
    returns me enviou*/
  }else{
    if(lista->info == n){
      return(lista);
      /*para o caso de inserir um elemento que ja esta no conjunto*/
    }
    else if(lista->prox != NULL && lista->info < n
    && n < lista->prox->info){
      No *novo = (No*) malloc(sizeof(No));
      if (novo == NULL)
        exit(1);
      novo->info = n;
      novo->prox = lista->prox;
      lista->prox = novo;
      return(lista);}
    else if(lista->info > n){/*menor elemento da lista*/
      No *novo = (No*) malloc(sizeof(No));
      if (novo == NULL)
        exit(1);
      novo->info = n;
      novo->prox = lista;
      return(novo);
    }
    lista->prox = insere(lista->prox, n);
      return(lista);
    }
  }


int pertence(No *lista, int n){
  /*essa funcao ira percorrer cada lista, se a lista chegar em NULL quer dizer
  que o elemento nao pertence e retorn 0, se em alguma hora ele achar ele retorna
  1 se nao caber em nenhum dos casos chamo recursivamente a funcaoe retorno o que
  me foi retornado antes*/
  if(lista == NULL)
    return 0;
  if(lista->info == n)
    return (1);
  return(pertence(lista->prox, n));
}


No *remover(No *lista, int n){
  /*essa funcao sera feita para duas situacoes base, em geral tentarei pegar o
  no seguinte e verificar, e assim conectar o atual com o seguinte do seguinte
  mas antes irei verificar o atual(primeira situacao da recursao)*/
  No *aux;
  if(lista == NULL)
    return(lista);
  if (lista->info == n){
    aux = lista;
    lista = lista->prox;
    free(aux);
    return(lista);
  }else if(lista->prox && lista->prox->info == n){
      aux = lista->prox;
      lista->prox = lista->prox->prox;
      free(aux);
      return(lista);
    }
    lista->prox = remover(lista->prox, n);
  return(lista);
}

No *uniao(No *vet1, No *vet2){
  /*essa funcao basicamente pegara cada elemento no vet2 e colocara no vet1 pela
  funcao insere, ja que essa funcao previne elementos iguais e ja ordena*/
  if (vet2 == NULL)
    return(vet1);
  else{
  vet1 = insere(vet1, vet2->info);
  vet1 = uniao(vet1, vet2->prox);
  return(vet1);}
}

No *interseccao(No *vet1, No *vet2){
  /*essa funcao ira verificar a informacao do vet2 em relacao ao vet 1, entao
  para inicio, se o vet1 for nulo eu retorno ele memso pq nao preciso mecher em
  nada dele, se o vet2 for nulo eu comeco a remover todas as informacoes extras do
  vet 1, pq elas nao serao iguais ao vet1, se a informacao dos vets forem iguais
  ja chamo recursivamente a funcao com os proximos parametros, se o proximo do
  vet2 for nulo entao toso do vet1 que foram maiores que o atual do vet1 serao diferentes
   e portanto os removo,no caso do vet1 ser menor que vet2 eu removo esse menor pois
   nao havera um correspondente no vet2 e chamo os proximos do vet1, caso contrario,
   vet1 maior que vet2 vou chamando novos vet2 para ver se tem algum maior ou igual
   para se encaixar em algum dos ifs ateriores.*/
    if (vet1 == NULL)
      return(vet1);
    if (vet2 == NULL){
      vet1 = remover(vet1, vet1->info);
      vet1 = interseccao(vet1, vet2);
      return(vet1);
   }
    if (vet2->info == vet1->info){
      vet1->prox = interseccao(vet1->prox, vet2->prox);
      return(vet1);}
    if(vet2->prox == NULL && vet1->info > vet2->info){
      vet1 = remover(vet1, vet1->info);
      vet1 = interseccao(vet1, vet2);
      return(vet1);

    }
    if (vet1->info < vet2->info){
      vet1 = remover(vet1, vet1->info);
      vet1 = interseccao(vet1, vet2);
      return(vet1);}
    if (vet1->info > vet2->info){
      vet1 = interseccao(vet1, vet2->prox);
      return(vet1);
    }
    return(vet1);
}


No *subtracao(No *vet1, No *vet2){
  /*essa funcao ficara comparando as info seguindo um padrao muito parecido com
  a interseccao, entao quando a info do vet1 for menor q a info do vet2 passaremos
  para o proximo do vet1, quando o do vet2 for menor passaremos para o proximo do
  vet2 e quando for igual removeremos do vet1 */
  if(vet1 == NULL || vet2 == NULL)
    return(vet1);
  if(vet1->info == vet2->info){
    vet1 = remover(vet1, vet1->info);
    vet1 = subtracao(vet1, vet2->prox);
    return(vet1);
  }
  else if (vet1->info < vet2->info)
    vet1->prox = subtracao(vet1->prox, vet2);
  else if (vet2->info < vet1->info)
    vet1 = subtracao(vet1, vet2->prox);
  return(vet1);
}

void printar(No *lista){
  if(lista == NULL){
    printf("}\n");
  }else if(lista->prox == NULL){
    printf("%d", lista->info);
    printar(lista->prox);
  }else{
    printf("%d,", lista->info);
    printar(lista->prox);
  }
}
void print(No *vet1, No *vet2){
  printf("{");
  printar(vet1);
  printf("{");
  printar(vet2);
}
void liberar_lista(No *lista){
  if(lista != NULL){
    liberar_lista(lista->prox);
    free(lista);
  }
}
