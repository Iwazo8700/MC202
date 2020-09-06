#include<stdio.h>
int main(){
  int primeiro;
  float ope1, ope2, resultado;
  char operador;
  scanf("%d /n", &primeiro);
  for (int i = 0; i < primeiro; i+=1){
    scanf("%f %c %f", &ope1, &operador, &ope2);
    if(operador == '+'){resultado = ope1 + ope2;}else
    if(operador == '-'){resultado = ope1 - ope2;}else
    if(operador == '*'){resultado = ope1 * ope2;}else
    if(operador == '/'){resultado = ope1/ope2;}
    else{printf("operador invalido \n");}
    printf("%.1f \n", resultado);
  }
}
