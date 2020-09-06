#include<stdio.h>
#include<stdlib.h>

typedef struct {/*struct necessaria para organizar cada pessoa*/
  long long int cpf;
  char nome[41];
  char email[31];
  int idade;
} pessoa;


int main(){
  pessoa *vetor, auxiliar;
  int n = 0;
  vetor = (pessoa *) malloc(sizeof(pessoa)*2);
  if (vetor==NULL){
    printf("ERROR de malloc\n");
  }
  /*le cada linha e coloca o valor para cada variavel da struct*/
    //while(scanf("%lu, %[^,]s, %[^,]s,%d", &vetor[n].cpf, vetor[n].nome, vetor[n].email, &vetor[n].idade)!= EOF){
    //printf("%011lu, %s, %s, %d \n", vetor[n].cpf, vetor[n].nome, vetor[n].email, vetor[n].idade);
    //printf("!");
    while((scanf(" %lli ", &vetor[n].cpf)!=EOF)&&
    (scanf(" ,%[^,]s", vetor[n].nome)!=EOF)&&
    (scanf(" ,%[^,]s", vetor[n].email)!=EOF)&&
    (scanf(" ,%d", &vetor[n].idade)!=EOF)){
    vetor = (pessoa *) realloc(vetor, sizeof(pessoa)*(n+2));
    n+=1;
  }
    //fprintf(arq_end, "%ld, %s %s %d\n", vetor[i].cpf, vetor[i].nome, vetor[i].email, vetor[i].idade);
  for(int i = 0; i < n;i++){/*selection sort*/
    for(int a = i+1; a < n; a++){
      if (vetor[a].idade < vetor[i].idade){
        auxiliar = vetor[a];
        vetor[a] = vetor[i];
        vetor[i] = auxiliar;
      }else if(vetor[a].idade == vetor[i].idade)
      {if (vetor[a].cpf < vetor[i].cpf)
       {auxiliar = vetor[a];
        vetor[a] = vetor[i];
        vetor[i] = auxiliar;}}
    }
    /*pensei em printar logo depois que acho o menor mas por algum motivo da erro*/
    //fprintf(arq_end, "%ld, %s %s %d\n", vetor[i].cpf, vetor[i].nome, vetor[i].email, vetor[i].idade);
  }
  /*por algum motivo printar logo depois que descubro o menor da algum problema, a primeira linha da saida fica cheio de quadradinhos
  se possivel gostaria de saber o porque do erro*/
  for(int i = 0; i < n;i++){/*printa*/
    if(vetor[i].cpf == 0 && vetor[i].idade==0 )/*erro constatado para um caso incomum de idade = 0, cpf = 0*/
      //printf("%s", vetor[i].nome);
      continue;
    printf("%011lli, %s, %s, %d \n", vetor[i].cpf, vetor[i].nome, vetor[i].email, vetor[i].idade);
  }

  //usar um selection sort para printar, se houver algum iguql comparaos cpfs
  free(vetor);
}
