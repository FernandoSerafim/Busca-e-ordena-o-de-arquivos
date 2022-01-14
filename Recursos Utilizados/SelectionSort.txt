#include <stdio.h>
#include <time.h>
#include <stdlib.h>

typedef struct registro registro;

struct registro {
	int chave;
	//além de outros campos de informação do registro
};

void selectionsort (registro* v, int n)
{ 
  int i, j, menor;
  registro temp;
  for (i = 0; i < n - 1; i++)
  { 
    menor = i;
    
    for (j = i + 1 ; j < n; j++)
      if ( v[j].chave < v[menor].chave) 
        menor = j;
        
    temp = v[menor];
    v[menor] = v[i];
    v[i] = temp;
  }
}

int main()
{
	registro v[10];
	
	srand(time(NULL)); 
    
    printf("Vetor gerado\n");
	for (int i = 0; i < 10; i++) {
		v[i].chave = rand() % 100;
		printf("%d ", v[i].chave);
	}
	
	selectionsort(v, 10);
	
	printf("\n\nVetor ordenado\n");
	for (int i = 0; i < 10; i++)
		printf("%d ", v[i].chave);
	
	return 0;
}