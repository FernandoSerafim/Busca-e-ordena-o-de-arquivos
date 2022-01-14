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
    } vet[7] = {{1, 1, "Joao da Silva", "A era do gelo", "Aventura"},
                {1, 2, "Joao da Silva", "UP", "Aventura"},
				{1, 3, "Joao da Silva", "Paixao Avassaladora", "Romance"},
				{2, 3, "Pedro Silva e Silva", "Jair Bolsonaro: A lenda", "Acao"},
				{2, 4, "Guilherme zampronio", "Joao Bolsonarista", "Criminal"},
				{2, 5, "Fernando Serafim", "minions", "Desenho"},
				{2, 6, "Fernando Serafim", "A era do gelo", "Desenho"},
				};

    fd = fopen("insere.bin", "w+b");
    fwrite(vet, sizeof(vet), 1, fd);
    fclose(fd);

    //////////////////////////////
    struct busca_p {
        int CodCli;
        int CodF;
    } vet_bp[3] = {{1,1},
                   {1,3},
                   {2,9},
                   };

    fd = fopen("busca_p.bin", "w+b");
    fwrite(vet_bp, sizeof(vet_bp), 1, fd);
    fclose(fd);

    //////////////////////////////
	const int quantidade = 4;
    char nomes[quantidade][50] = {"A era do gelo",
    "Filme-X", "Joao Bolsonarista", "A era do gelo"};

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

