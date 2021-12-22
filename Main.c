#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*aqui é necessário criar uma struct do tipo certo que acompanhará a struct do início ao fim
Podemos aplciar o realloc no vetor da struct toda vez que for realizada uma inserção
Podemos abrir e fechar os índices somente no momento em que fecharmos a aplicação
Utilizar eles abertos durante toda a aplicação dá mais suscetibilidade aos erros*/

typedef struct CliF {
    int CodCli;
    int CodF;
    char NomeCli[50];
    char NomeF[50];
    char Genero[50];
}CliF;

typedef struct Busca_P {
    int CodCli;
    int CodF;   //ESSA STRUCT CARREGA O ÍNDICE PRIMÁRIO E TAMBÉM O LIP2
}Busca_P;

typedef struct Busca_S{
    char Nome_Filme[50]; //ESSA STRUCT CARREGA O LIP1
    int Bos_LIP2;
}Busca_S;

void Menu_Opcoes();
void Designa_Abertura(FILE** principal, FILE** insere, FILE** busca_p, FILE** busca_s);
void Designa_Abertura_Indices(FILE** indice_p, FILE** indice_LIP1, FILE** indice_LIP2);
void Designa_Fechamento(FILE** principal, FILE** insere, FILE** busca_p, FILE** busca_s);
void Designa_Fechamento_Indices(FILE** indice_p, FILE** indice_LIP1, FILE** indice_LIP2);
void Insercao(FILE** principal, FILE** insere, Busca_P** primaria, Busca_S** secundaria);
void Ordena_Dados();
int Carrega_Indices(Busca_P** primaria, Busca_S** secundaria);
int Direciona_Ponteiro(Busca_P** primaria); //usar dentro da inserção

int main(){
    FILE *principal, *insere, *busca_p, *busca_s, *indice_p, *indice_LIP1, *indice_LIP2;
    Busca_P* primaria;
    Busca_S* secundaria;
    Designa_Abertura(&principal, &insere, &busca_p, &busca_s); //o nosso principal e os três da professora
    //Designa_Abertura_Indices(&indice_p, &indice_LIP1, &indice_LIP2); //o índice primário e os da lista invertida
    //Carrega_Indices(&primaria, &secundaria);

    int opcao = 1;
    while(opcao != 0){
        Menu_Opcoes();
        scanf("%d", &opcao);
        switch(opcao){
            case 1:{
                Insercao(&principal, &insere, &primaria, &secundaria);                
                break;
            }
        case 2:{
            printf("Bom dia\n");
            //Pesquisa_primaria();
            break;
        }
        case 3:{
            printf("Bom dia\n");
            //Pesquisa_Secundaria();
            break;
        }
        case 0:{
             Designa_Fechamento(&principal, &insere, &busca_p, &busca_s);
             //Designa_Fechamento_Indices(&indice_p, &indice_LIP1, &indice_LIP2);
             printf("Encerrando...\n");
             break;
        }
        default:{
                printf("Entre com uma opcao valida!\n");
            }
        }
    }

    return 0;
}

void Insercao(FILE** principal, FILE** insere, Busca_P** primaria, Busca_S** secundaria){
   int tamanho =  Direciona_Ponteiro(primaria);  
    
    if((sizeof(CliF) * tamanho) == ftell(*insere)){
        printf("Nao ha mais registros a serem inseridos...\n\n");
        return;
    } 

    fseek(*insere, sizeof(CliF) * tamanho, SEEK_SET);//direcionei o ponteiro para o próximo registro
    fseek(*principal, 0, SEEK_END); //movi o arquivo de dados para o final

    CliF cliente;
    char buffer[sizeof(CliF)];

    fread(&cliente, sizeof(CliF), 1, *insere); //LEIO A POSIÇÃO "BOS" DO INSERE
        sprintf(buffer,"%d#%d#%s#%s#%s#",cliente.CodCli, cliente.CodF, cliente.Genero, cliente.NomeCli, cliente.NomeF);
            tamanho = strlen(buffer);
        fwrite(&tamanho, sizeof(int), 1, *principal);
    fwrite(buffer, sizeof(char), tamanho, *principal);
    //Agora que já inseri os dados no arquivo, preciso inserir os dados na struct

    printf("Registro inserido com sucesso...\n");
}

int Direciona_Ponteiro(Busca_P** primaria){//lembre que o primaria vai ser um vetor de ponteiros
    int i = 0, contador = 0;

    while(primaria[i] != NULL) contador++;

    return contador;
}
void Designa_Abertura(FILE** principal, FILE** insere, FILE** busca_p, FILE** busca_s){
//vai direcionar os ponteiros dentro da função de inserção
    if((*principal =  fopen("principal.bin", "a+b")) ==  NULL){
        printf("Erro na abertura do arquivo\n");
        exit(1);
    }
    fseek(*principal, 0, SEEK_END);

    if(ftell(*principal) > 0){
        fclose(*principal);
        if((*principal = fopen("principal.bin", "r+b")) == NULL){
            printf("Erro na abertura do arquivo\n");
            exit(1);
        }
    }

    if((*insere = fopen("insere.bin", "r+b")) == NULL){
        printf("Erro na abertura do arquivo\n");
        exit(1);
    }
    if((*busca_p = fopen("busca_p.bin", "r+b")) == NULL){
        printf("Erro na abertura do arquivo\n");
        exit(1);
    }
    if((*busca_s = fopen("busca_s.bin", "r+b")) == NULL){
        printf("Erro na abertura do arquivo\n");
        exit(1);
    }
}
void Designa_Abertura_Indices(FILE** indice_p, FILE** indice_LIP1, FILE** indice_LIP2){
    
    if((*indice_p = fopen("indice_p.bin", "a+b")) == NULL){
        printf("Erro na abertura do arquivo\n");
        exit(1);
    }
    fseek(*indice_p, 0, SEEK_END);

    if(ftell(*indice_p) > 0){
        fclose(*indice_p);
        if((*indice_p = fopen("indice_p.bin", "r+b")) == NULL){
            printf("Erro na abertura do arquivo\n");
            exit(1);
        }
    }

    if((*indice_LIP1 = fopen("indice_LIP1.bin", "a+b")) == NULL){
        printf("Erro na abertura do arquivo\n");
        exit(1);
    }
    
    fseek(*indice_LIP1, 0, SEEK_END);

    if(ftell(*indice_LIP1) > 0){
        fclose(*indice_LIP1);
        if((*indice_LIP1 = fopen("indice_LIP1.bin", "r+b")) == NULL){
            printf("Erro na abertura do arquivo\n");
            exit(1);
        }
    }

    if((*indice_LIP2 = fopen("indice_LIP2.bin", "a+b")) == NULL){
        printf("Erro na abertura do arquivo\n");
        exit(1);
    }

    fseek(*indice_LIP2, 0, SEEK_END);

    if(ftell(*indice_LIP2) > 0){
        fclose(*indice_LIP2);
        if((*indice_LIP2 = fopen("indice_LIP2.bin", "r+b")) == NULL){
            printf("Erro na abertura do arquivo\n");
            exit(1);
        }
    }
}
void Designa_Fechamento(FILE** principal, FILE** insere, FILE** busca_p, FILE** busca_s){
    fclose(*principal);     fclose(*busca_p);   fclose(*busca_s);   fclose(*insere);
}
void Designa_Fechamento_Indices(FILE** indice_p, FILE** indice_LIP1, FILE** indice_LIP2){
    fclose(*indice_p);     fclose(*indice_LIP1);    fclose(*indice_LIP2);
}