#include<stdio.h>
#include<stdlib.h>

int main()
{
    int *p, counter = 0;
  
    p = malloc(2 * sizeof(int));
    
    p[0] = 1;
    p[1] = 2;

    printf("P antes do realloc: %d  %d\n\n", p[0], p[1]);

    p = realloc(p, 4*sizeof(int));

    p[2] = 4;
    p[3] = 5;

    for(counter = 0; counter < 4; counter++){
        printf("%d ", p[counter]);
    }

    free(p);
    return 0;
}