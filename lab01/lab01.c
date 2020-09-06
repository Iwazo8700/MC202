#include <math.h>   /*Biblioteca com a funcao de raiz quadrada, sqrt()*/
#include <stdio.h>  /*Biblioteca para input e output*/
/*Nome: Enzo Hideki Iwata RA:215394*/
#define COL 128
#define LIN 96
int main() {
  int col = 130, lin = 98, x, y, x_quadrado, y_quadrado;
  int matriz[lin][col], final[LIN][COL];
  for(int i=0; i<col; i++){  /*for para inserir zeros das linhas extremas*/
    matriz[0][i] = 0;
    matriz[97][i] = 0;}
  for(int i=0; i<lin; i++){/*for para inserir zeros nas colunas extremas*/
    matriz[i][0] = 0;
    matriz[i][129] = 0;}
  for (int a = 1; a<97; a++){/*for para ler a matriz*/
    for(int b =1; b < 129; b++)
    {scanf("%d ", &matriz[a][b]);}
    scanf("\n");}
  for(int i = 1; i <97; i++){/*for para executar a operacao em cada elemento e guardar em uma outra matriz*/
    for(int j = 1; j < 129; j++){
      x = matriz[i-1][j-1]+2*matriz[i][j-1]+matriz[i+1][j-1]-matriz[i-1][j+1]-2*matriz[i][j+1]-matriz[i+1][j+1];
      y = matriz[i-1][j-1]+2*matriz[i-1][j]+matriz[i-1][j+1]-matriz[i+1][j-1]-2*matriz[i+1][j]-matriz[i+1][j+1];
      x_quadrado = x*x;
      y_quadrado = y*y;
      final[i-1][j-1] = sqrt(x_quadrado + y_quadrado);
    }
  }
    for (int a = 0; a<LIN; a++){/*printar a nova matriz*/
      for(int b =0; b < COL; b++)
      {printf("%d ", final[a][b]);}
      printf("\n");}

  return 0;
}
