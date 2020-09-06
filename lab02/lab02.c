#include<stdio.h>
#include<stdlib.h>

int ** forma_matriz(int linha, int coluna, int **matriz);
void le_matriz(int linha, int coluna, int **matriz);
void printa_vetor(int tamanho, char *frase);
void processa(int **matriz, int linha, int coluna, char *frase, int tamanho, int max);
void printa(int **matriz, int linha, int coluna){
  for(int i =0; i<linha; i++){
    for(int j=0; j<coluna; j++){
      printf("%d ", matriz[i][j]);
    }printf("\n");
  }
}
int main(){
  int x, y,max, **matriz, tamanho;
  char qlqr[2], *final;
  scanf("%s", qlqr);/*le P2*/
  scanf("%d %d",&y, &x );/*le linha e coluna respectivamente*/
  scanf("%d", &max);/*le maior pixel*/
  matriz = malloc( x * (sizeof(int *)));
  for(int i = 0; i < x; i++){
    matriz[i] = malloc(y * sizeof(int));}/*aloca a memoria necessaria para matriz*/
  le_matriz(x ,y, matriz);/*le a matriz*/
  //printa(matriz, x ,y);
  tamanho = (int)(x*y)/4;/*maior tamanho para frase*/
  final = malloc(tamanho * sizeof(char));
  processa(matriz, x, y, final, tamanho,max);
  //printa_vetor(tamanho , final);
  free(final);
  for(int i = 0; i < x; i++){
    free(matriz[i]);}
  free(matriz);
  }
int **forma_matriz(int linha, int coluna, int **matriz){/*funcao para alocar a memoria necessaria para matriz*/
  matriz = malloc( linha * (sizeof(int *)));
  for(int i = 0; i < linha; i++){
    matriz[i] = malloc(coluna * sizeof(int));}
  return matriz;
  }
void le_matriz(int linha, int coluna, int **matriz){/*funcao para ler a matriz*/
  for(int i = 0; i < linha; i++){
    for(int j = 0; j < coluna; j++){
      scanf("%d ", &matriz[i][j] );
    }
  }}
 void printa_vetor(int tamanho, char *frase){/*funcao para printar a frase*/
  for (int a = 0; a < tamanho; a++){
    //if(frase[a])
    printf("%c", frase[a]);
  }
  printf("\n");
 }
void processa(int **matriz, int linha, int coluna, char *frase, int tamanho, int max){
  int cont = 1, acu = 0, prim, seg, terc, quar, b =1;
  for(int i = 0; i < linha; i++){
    for(int j = 0; j < coluna; j++){
      if (acu < tamanho && matriz[i][j] <= max){
        if (cont==1){
          prim = (matriz[i][j]&3)<<6;
          cont+=1;}
        else if(cont==2){
          seg = (matriz[i][j]&3)<<4;
          cont+=1;}
        else if (cont==3){
          terc = (matriz[i][j]&3)<<2;
          cont+=1;}
        else if(cont==4){
          quar = (matriz[i][j]&3);
          frase[acu] = (char)prim+seg+terc+quar;
          if(frase[acu] == 0)
          b = 0;
          if(b)
          printf("%c", frase[acu]);
          cont = 1;
          acu+=1;
        }
      }
    }
  }}
