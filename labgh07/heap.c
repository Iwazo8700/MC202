void Troca(No *a, No *b);
void subir_no_heapmax(Prioridade *vet, int num);
void descer_no_heapmax(Prioridade *vet, int num);
void subir_no_heapmin(Prioridade *vet, int num);
void descer_no_heapmin(Prioridade *vet, int num);

void inserir_heapmax(Prioridade *vet, No *k){
  vet->v[vet->n] = *k;
  vet->n++;
  subir_no_heapmax(vet, vet->n-1);
}

void subir_no_heapmax(Prioridade *vet, int num){
  while(num > 0 && vet->v[pai(num)].info < vet->v[num].info){
    Troca(&vet->v[pai(num)], &vet->v[num]);
    num = pai(num);
  }
}
/*desce no heap*/
void descer_no_heapmax(Prioridade *vet, int num){
  int maior;
  if(filho_esq(num) < vet->n){
  maior = filho_esq(num);
  if(filho_dir(num) < vet->n && vet->v[filho_esq(num)].info < vet->v[filho_dir(num)].info){
    maior = filho_dir(num);}
  if(vet->v[num].info < vet->v[maior].info){
    Troca(&vet->v[num], &vet->v[maior]);
    descer_no_heapmax(vet, maior);
    }
  return;
  }
}

/*retira o max do heap*/
void retirar_max(Prioridade *vet, No *iden){
  *iden = vet->v[0];
  Troca(&vet->v[0], &vet->v[vet->n - 1]);
  vet->n--;
  descer_no_heapmax(vet, 0);
}

void inserir_heapmin(Prioridade *vet, No *k){
  vet->v[vet->n] = *k;
  vet->n++;
  subir_no_heapmin(vet, vet->n-1);
}

void subir_no_heapmin(Prioridade *vet, int num){
  while(num > 0 && vet->v[pai(num)].info > vet->v[num].info){
    Troca(&vet->v[pai(num)], &vet->v[num]);
    num = pai(num);
  }
}

void descer_no_heapmin(Prioridade *vet, int num){
  int menor;
  if(filho_esq(num) < vet->n){
  menor = filho_esq(num);
  if(filho_dir(num) < vet->n && vet->v[filho_esq(num)].info > vet->v[filho_dir(num)].info){
    menor = filho_dir(num);}
  if(vet->v[num].info > vet->v[menor].info){
    Troca(&vet->v[num], &vet->v[menor]);
    descer_no_heapmin(vet, menor);
    }
  return;
  }
}

void retirar_min(Prioridade *vet, No *iden){
  *iden = vet->v[0];
  Troca(&vet->v[0], &vet->v[vet->n - 1]);
  vet->n--;
  descer_no_heapmin(vet, 0);
}

/*troca dois nos*/
void Troca(No *a, No *b){
  No aux;
  aux = *a;
  *a = *b;
  *b = aux;
}
