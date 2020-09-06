/*nome: Enzo Hideki Iwata Ra: 215394*/
#include <stdio.h>
#include <stdlib.h>

typedef struct No{
  int info;
  struct No *esq, *dir;
}No;

No *insere_arvore(No *arvore, int n);
int Altura_arvore(No *arvore);
int Diametro(No *no);
int Arvore_inteira(No *arvore);
No *libera_arvore(No *arvore);
void print_pre(No *arvore);
void print_in(No *arvore);
void print_pos(No *arvore);

int main() {
  No *arvore_binaria;
  int numero_de_nos, luffy, maior_diametro;
  char letra, lixo;
  arvore_binaria = NULL;
/*o programa ficara em um loop infinito ate o usuario digitar o comando de saida, para identificar o
modo de print eu leio o char inicial e vou classificando o que quero fazer, preferi ler char a string
depois ele insere na arvore binaria cada elemento colocado, apos isso ele faz o calculo do diametro
que nada mais eh do que passar por cada no  da arvore e calcular a altura da direita e da esquerda +1,
depois ele faz o print exigido, cada funcao tem um nome meio autoexplicativo*/
  while(3) {//while 3 pq sim, poderia ser while 1, mas quis 3
    scanf("%d %c", &numero_de_nos, &letra);
    if((numero_de_nos == 0) && (letra == '0'))
      break;


    if(letra == 'p'){
      scanf("%c%c", &letra, &lixo);getchar();
      if (letra == 'r'){//pre ordem
        for(int i = 0; i < numero_de_nos-1; i++){
          scanf("%d ", &luffy);
          arvore_binaria = insere_arvore(arvore_binaria, luffy);
        }
        scanf("%d", &luffy);
        arvore_binaria = insere_arvore(arvore_binaria, luffy);
        maior_diametro = Arvore_inteira(arvore_binaria);
        printf("Diametro da arvore binaria: %d\n", maior_diametro);
        print_pre(arvore_binaria);
        printf("\n");


      }else if(letra == 'o'){//pos ordem
        for(int i = 0; i < numero_de_nos-1; i++){
          scanf("%d ", &luffy);
          arvore_binaria = insere_arvore(arvore_binaria, luffy);
        }
        scanf("%d", &luffy);getchar();
        arvore_binaria = insere_arvore(arvore_binaria, luffy);
        maior_diametro = Arvore_inteira(arvore_binaria);
        printf("Diametro da arvore binaria: %d\n", maior_diametro);
        print_pos(arvore_binaria);
        printf("\n");
      }
    }else


    if(letra == 'i'){//in ordem
      scanf("%c", &lixo);getchar();
      for(int i = 0; i < numero_de_nos-1; i++){
        scanf("%d ", &luffy);
        arvore_binaria = insere_arvore(arvore_binaria, luffy);
      }
      scanf("%d", &luffy);getchar();
      arvore_binaria = insere_arvore(arvore_binaria, luffy);
      maior_diametro = Arvore_inteira(arvore_binaria);
      printf("Diametro da arvore binaria: %d\n", maior_diametro);
      print_in(arvore_binaria);
      printf("\n");
    }


  arvore_binaria = libera_arvore(arvore_binaria);
  }

  return 0;
}

//insere na arvore
No *insere_arvore(No *arvore, int n){
  if(arvore == NULL){
    No *novo = (No*)malloc(sizeof(No));
    novo->esq = NULL;
    novo->dir = NULL;
    novo->info = n;
    return novo;
  }
  if(n < arvore->info){
    arvore->esq = insere_arvore(arvore->esq, n);
  }else{arvore->dir = insere_arvore(arvore->dir, n);}
  return arvore;
}

//calculo da altura
int Altura_arvore(No *arvore){
  int esq, dir;
  if (arvore == NULL){
    return 0;
  }
  else{
    esq = Altura_arvore(arvore->esq);
    dir = Altura_arvore(arvore->dir);
    return(1 + (esq > dir ? esq : dir));
  }
}

//calculo do diametro
int Diametro(No *no){
  if (no == NULL)
    return 0;
  return (Altura_arvore(no->esq) + Altura_arvore(no->dir) + 1);
  return 0;
}

//backtracking para ir em todos os nos e ver o diametro
int Arvore_inteira(No *arvore){
  int diametro_centro, diametro_esquerdo, diametro_direito;
  diametro_centro = Diametro(arvore);
  if (arvore->esq != NULL){
    diametro_esquerdo = Arvore_inteira(arvore->esq);}
    else{diametro_esquerdo = 0;}
  if (arvore->dir != NULL){
    diametro_direito = Arvore_inteira(arvore->dir);}
    else{diametro_direito = 0;}
  if(diametro_centro >= diametro_direito && diametro_centro >= diametro_esquerdo)
    {return diametro_centro;}else
  if(diametro_direito >= diametro_centro && diametro_direito >= diametro_esquerdo)
    {return diametro_direito;}else
  if(diametro_esquerdo >= diametro_centro && diametro_esquerdo >= diametro_direito)
    {return diametro_esquerdo;}
    return 0;
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

//print pre
void print_pre(No *arvore){
  if(arvore != NULL){
    printf("%d ", arvore->info);
    print_pre(arvore->esq);
    print_pre(arvore->dir);
  }
}

//print in
void print_in(No *arvore){
  if(arvore != NULL){
    print_in(arvore->esq);
    printf("%d ", arvore->info);
    print_in(arvore->dir);
  }
}

//print pos
void print_pos(No *arvore){
  if(arvore != NULL){
    print_pos(arvore->esq);
    print_pos(arvore->dir);
    printf("%d ", arvore->info);
  }
}
