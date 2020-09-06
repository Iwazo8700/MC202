#include<stdio.h>
#include<string.h>

void print(int lin, int col, char mat[lin][col]){
  for(int i = 0; i < lin; i++){
    for(int j = 0; j < col; j++)
      {printf("%c ", mat[i][j]);}
    printf("\n");}}
char igual(int lin, int col, char array, char vetor){
  for(int i = 0, i < lin, i++){
    for(int j = 0, j < col, j++){
      array[i][j] = vetor[i][j]
    }
  }
  return vetor
}
int main(){
  int linha, coluna, numero;
  scanf("%d %d", &linha, &coluna);
  char matriz[linha][coluna], letra, vet[linha][coluna];
  for(int i = 0; i < linha; i++){
    for(int a = 0; a < coluna; a++)
    {scanf(" %c", &letra);
    matriz[i][a] = letra;}
  }
  vet = igual(linha, coluna, matriz, vet)
  /*scanf("%d", &numero);
  char palavras[numero];
  for(int a = 0; a < numero; a++){
    palavras[a]
  }*/
}
