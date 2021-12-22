#include<stdio.h>
#include<stdlib.h>
#define TAM 1000
/*um passo importante a entender � que n�o necessariamente preciso implementar a �rvore bin�ria.
Posso, mas n�o necessariamente devo. Heapsort n�o � focado em divis�o e conquista, apenas enxerga o vetor com a
aplicabilidade da �rvore bin�ria mantendo ela em um vetor*/

void constroiHeap(int *v, int tam){
/*
    Aqui, precisamos atender a ideia do algoritmo
    -> para todo i, v[i] >= v[2i + 1] && v[i] >= v[2i + 2];
    v[2i + 1] <= v[2i + 2] ---> N�O PRECISA O MAIOR ESTAR � DIREITA E O MENOR � ESQUERDA
    O HEAP N�O � UMA �RVORE BIN�RIA DE PESQUISA. S� PRECISA QUE O MAIOR ELEMENTO DA SUB�RVORE
    ESTEJA NA RA�Z DESSA SUB�RVORE
*/
   int i = tam / 2, pai, filho, t;
   while(1) {
      if (i > 0) {
          i--; //(tam-1)/2
          t = v[i]; //t = v[(tam-1)/2]
      } else {
          tam--;
          if (tam <= 0) return;
          t = v[tam];
          v[tam] = v[0];
      }
      pai = i; //ra�z que estamos analisando
      filho = i * 2 + 1; //filho da esquerda
      while (filho < tam) { //tam == 9, no caso
if ((filho + 1 < tam)  &&  (v[filho + 1] > v[filho])) //se est� dentro do vetor e dir > esq
              filho++; //filho passa a ser o da direita (escolhi o maior filho)
          if (v[filho] > t) { //t = v[i] --> se o filho maior � maior que o pai
             v[pai] = v[filho]; //o pai passa a ser esse filho maior
             pai = filho;
             filho = pai * 2 + 1;
          } else {
             break;
          }
      } //FIM WHILE
      v[pai] = t;
   }

}

int main(){

    int vetor[TAM], k = 0, tam = TAM;
    for(k = 0; k < TAM; k++){vetor[k] = tam; tam--;} //pior caso
    constroiHeap(vetor, TAM);
    for(k = 0; k < TAM; k++) printf("%d ", vetor[k]);

   return 0;
}
