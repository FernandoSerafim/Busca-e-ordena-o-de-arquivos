#include<stdio.h>
#include<stdlib.h>

typedef struct clif{
    int numero;
}Cliente;

int main()
{
    int *p, contador = 0, counter = 0;
    Cliente* PS;

    PS = malloc(2*sizeof(Cliente));
    //p = malloc(sizeof(int));
    //*p = 1;
    //p[1] = 2;
    PS[0].numero = 1;
    PS[1].numero = 2;
    //PS[1]->numero = 0;
    printf("%d  %d\n\n",PS[0].numero, PS[1].numero);
    while(PS[counter].numero != NULL) counter++;
    printf("O tamanho do ponteiro e: %d\n", counter/sizeof(Cliente));
     //while(*(p++)) contador++;
    
    //printf("O tamanho do ponteiro p e: %d\n\n", contador/sizeof(int));

    //for(i=0; i<2; i++) printf("%d\n", p[i]);

    //printf("%d %d", p[0], p[1]);
    free(PS);
    free(p);
    return 0;
}