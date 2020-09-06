#include<stdio.h>
#include<string.h>
#define max 100
char realoca(int pos, char word[]){
  int len = strlen(word) + 1;
  while( pos < len){
    word[pos] = word[pos+1];
    pos += 1;
  }
  return *word;
}
int main(){
  char palavra[max];
  scanf("%s", palavra);
  int tam = strlen(palavra);
  for (int i = 0; i < tam; i++){
    if (palavra[i] == 'R')
      {if (palavra[i+1]=='R')
        {realoca(i+1, palavra);}
      palavra[i] = 'L';}
  }
  printf("%s\n", palavra);
}
