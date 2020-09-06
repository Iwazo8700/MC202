#include<stdio.h>
int main() {
  char mat[2][2];
  for(int i = 0; i < 2;i++){
    for(int j = 0; j < 2; j++){
      scanf("%c ", &mat[i][j]);
    }
  }
  for (int i = 0; i < 2; i++){
    for(int j = 0; j < 2; j++){
      int num = mat[i][j];
      printf("%c ", num);
    }
    printf("\n");
  }
}
