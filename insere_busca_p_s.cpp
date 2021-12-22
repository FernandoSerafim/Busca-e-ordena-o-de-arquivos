#include<stdio.h>
#include<conio.h>

int main() {
    FILE *fd;
    
    //////////////////////////////
    struct CliF {
        int CodCli;
        int CodF;
        char NomeCli[50];
        char NomeF[50];
        char Genero[50];
    } vet[4] = {{1, 1, "Joao da Silva", "Filme-1", "Aventura"},              
                {1, 2, "Joao da Silva", "Filme-2", "Aventura"},
				{1, 3, "Joao da Silva", "Filme-3", "Aventura"},
				
				{2, 3, "Pedro Silva e Silva", "Filme-3", "Aventura"},
				};
       
    fd = fopen("insere.bin", "w+b");
    fwrite(vet, sizeof(vet), 1, fd);
    fclose(fd);
    
    //////////////////////////////
    struct busca_p {
        int CodCli;
        int CodF;
    } vet_bp[2] = {{1,1},
                   {1,3},
                   };
    
    fd = fopen("busca_p.bin", "w+b");
    fwrite(vet_bp, sizeof(vet_bp), 1, fd);
    fclose(fd);
    
    //////////////////////////////
	const int quantidade = 4;
    char nomes[quantidade][50] = {"Filme-3", "Filme-X", "Filme-1", "Filme-3"};
       
    fd = fopen("busca_s.bin", "w+b");
    for (int i=0; i<quantidade; i++)
       fwrite(nomes[i], sizeof(char), 50, fd);
    fclose(fd);
    
    /*char buffer[50];
    fd = fopen("busca_s.bin", "r+b");
    for (int i=0; i<quantidade; i++)
       {
         fread(buffer, sizeof(buffer), 1, fd);
         printf("\n%s",buffer);
       }
    fclose(fd);
    getch();*/
}

