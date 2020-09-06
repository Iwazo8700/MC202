#include<stdio.h>
float maior(float mat[], int n){
  float maior=0.0;
  for(int i = 0; i < n; i+=1){
    if (mat[i] > maior){
      maior = mat[i];
    }
  }
  return maior;
}
float media(float vet[], int n){
  float me = 0;
  for(int i = 0; i < n; i++){
    me += vet[i];
  }
  me /= 2;
  return me;
}
int main() {
  int n;
  float p1, p2, p3, mp, t1, t2, tm, max, final;
  scanf("%d", &n);
  float pratico[n], teorico[n];
  for(int i = 0; i < n; i+=1){
    scanf("%f %f %f", &p1, &p2, &p3);
    pratico[i] = (float)(1.1*((p1+p2+p3)/3));
  }
  for(int i = 0; i < n; i+=1){
    scanf("%f %f", &t1, &t2);
    teorico[i] = (float)((t1+t2)/2);
  }
  max = maior(teorico, n);
  for(int i = 0; i < n; i+=1){
    teorico[i] = (float)((teorico[i]*10)/max);
  }
  for (int i = 0; i < n; i++){
    final = (float)((pratico[i]+teorico[i])/2);
    printf("%.1f\n", final);
  }
  float maiort = maior(teorico, n);
  float maiorp = maior(pratico, n);
  float mediap = media(pratico, n);
  float mediat = media(teorico, n);
  printf("MAX P: %.1f\n", maiorp);
  printf("MAX T: %.1f\n", maiort);
  printf("media p: %.1f\n", mediap);
  printf("media T: %.1f\n", mediat);
}
