#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define bool int
#define TRUE 1
#define FALSE 0

typedef struct CliF {
    int CodCli;
    int CodF;
    char NomeCli[50];   //ESSA STRUCT USAREMOS PARA RECUPERAR E INSERIR OS DADOS NO ARQUIVO
    char NomeF[50];
    char Genero[50];
}CliF;

typedef struct Busca_SP { //Pode ser usada tanto no índice primário quanto no das chaves primárias da lista invertida
    int CodCli;
    int CodF;   //ESSA STRUCT CARREGA O ÍNDICE PRIMÁRIO E TAMBÉM O LIP2
    int BOS_Prox;
}Busca_SP;

typedef struct Busca_Secundaria{
    char Nome_Filme[50]; //ESSA STRUCT CARREGA O LIP1
    int Bos_LIP2;
}Busca_Secundaria;

typedef struct busca_p{
    int CodCli;
    int CodF;
}Primaria_Buscar;

void Menu_Opcoes(); //ok
void Designa_Abertura(FILE** principal, FILE** insere, FILE** busca_p, FILE** busca_s); //ok
void Designa_Abertura_Indices(FILE** indice_p, FILE** indice_LIP1, FILE** indice_LIP2, FILE** Size_Structs);//ok

void Descarrega_Dados(FILE** indice_p, FILE** indice_LIP1, FILE** indice_LIP2, Busca_SP** primaria,
Busca_Secundaria** S1, Busca_SP** S2, FILE** Size_Structs); //AQUI MANDO AS STRUCTS E DESCARREGO NOS ÍNDICES

void Limpa_Sistema(Busca_SP** primaria, Busca_Secundaria** S1, Busca_SP** S2); //ok
void Recria_Indices(FILE*** principal, FILE*** indice_p, FILE*** indice_LIP1, FILE*** indice_LIP2);
void Ordena_Primaria(Busca_SP** ponteiro, int tam);

void Carrega_Indices(FILE** principal, FILE** indice_p, FILE** indice_LIP1, FILE** indice_LIP2, Busca_SP** primaria,
Busca_Secundaria** S1, Busca_SP** S2, FILE* Size_Structs); //ok
void Pesquisa_primaria(FILE** principal, Busca_SP** primaria, FILE** busca_p, int Tamanho);
void Pesquisa_Secundaria(FILE** principal, FILE** Size_Structs, Busca_SP** primaria,
Busca_Secundaria** S1, Busca_SP** S2, FILE** busca_s, int* direcionamento);

void Alterna_Para_R(FILE** Size_Structs, FILE** principal);
bool Direciona_Ponteiro(FILE** primaria); //ok
bool Verifica_Existencia_Nome(Busca_Secundaria** S1, char* cliente, int Size_S); //ok

void Insercao(FILE** principal, FILE** insere, FILE** Size_Structs, Busca_SP ** primaria, Busca_Secundaria** S1,
Busca_SP** S2);
bool Tamanho_Indice_Primario(FILE** Size_Structs);
bool direciona_busca(int bytes_busca);

bool Abre_Auxiliar_PS(FILE** PSBytes);
void DescarregaEFechaPS(FILE** PSBytes, int direcionamento);
void Aloca_S1(Busca_Secundaria** S1, FILE* Size_Structs){

    int alocar = 0;

     fseek(Size_Structs, -4, SEEK_END);
     fread(&alocar, sizeof(int), 1, Size_Structs);

     *S1 = malloc(alocar * sizeof(Busca_Secundaria));

}

int main(){

    FILE *principal = NULL, *insere = NULL, *busca_s = NULL;
    FILE *indice_p = NULL, *indice_LIP1 = NULL, *indice_LIP2 = NULL;
    FILE* Size_Structs = NULL, *busca_p = NULL, *PSBytes = NULL; //Pesquisa Secundária bytes

    Busca_SP* primaria = NULL;
    Busca_SP *S2 = NULL;

    Busca_Secundaria* S1 = NULL;

    int opcao = 1, direcionamento = 0;

    direcionamento = Abre_Auxiliar_PS(&PSBytes);

    Designa_Abertura(&principal, &insere, &busca_p, &busca_s);
    Designa_Abertura_Indices(&indice_p, &indice_LIP1, &indice_LIP2, &Size_Structs);
    Aloca_S1(&S1, Size_Structs);
    Carrega_Indices(&principal, &indice_p, &indice_LIP1, &indice_LIP2, &primaria, &S1, &S2, Size_Structs);

    while(opcao){

        Menu_Opcoes();

        scanf("%d", &opcao);

        switch(opcao){

            case 1:{
                Insercao(&principal, &insere, &Size_Structs, &primaria, &S1, &S2);
                Alterna_Para_R(&Size_Structs, &principal);
                break;
            }

            case 2:{

                int tamanho = Tamanho_Indice_Primario(&Size_Structs);
                Pesquisa_primaria(&principal, &primaria, &busca_p, tamanho);
                break;
            }

            case 3:{
                Pesquisa_Secundaria(&principal, &Size_Structs, &primaria, &S1, &S2, &busca_s, &direcionamento);
                break;
            }

            case 0:{
                Descarrega_Dados(&indice_p, &indice_LIP1, &indice_LIP2, &primaria, &S1, &S2, &Size_Structs);
                DescarregaEFechaPS(&PSBytes, direcionamento);
                printf("Encerrando...\n");
                break;
            }

            default:{
                    printf("Opcao invalida...\n");
                    break;
                }
            }
    }

    fclose(principal);      fclose(insere);
    fclose(busca_p);        fclose(busca_s);
    fclose(indice_p);       fclose(indice_LIP1);
    fclose(indice_LIP2);    fclose(Size_Structs);

    free(primaria);         primaria = NULL;
    free(S1);               S1 = NULL;
    free(S2);               S2 = NULL;

    return 0;
}

int Abre_Auxiliar_PS(FILE** PSBytes)
{

    if((*PSBytes =  fopen("Pesquisa_Secundaria.bin", "a+b")) == NULL){
        printf("Erro na abertura do arquivo\n\n");
        exit(1);
    }

    fclose(*PSBytes);

    if((*PSBytes =  fopen("Pesquisa_Secundaria.bin", "r+b")) == NULL){
        printf("Erro na abertura do arquivo\n\n");
        exit(1);
    }

    fseek(*PSBytes, 0, 2);

    if(ftell(*PSBytes) == 0) return 0;

    fseek(*PSBytes, 0, 0);

    int leitura;

    fread(&leitura, sizeof(int), 1, *PSBytes);

    return leitura;
}

void DescarregaEFechaPS(FILE** PSBytes, int direcionamento)
{
    int tamanho  = direcionamento;

    rewind(*PSBytes);
    fwrite(&tamanho, sizeof(int), 1, *PSBytes);

    fclose(*PSBytes);
}

void Insercao(FILE** principal, FILE** insere, FILE** Size_Structs, Busca_SP** primaria,
Busca_Secundaria** S1, Busca_SP** S2)
{
   CliF cliente;
   long int deslocamento = 0;
   unsigned bytes_insere = 0;
   int posicao = 0;
   char* argumento = NULL, buffer[sizeof(CliF)];;
   int Vet_Tam[2];

   int tamanho =  Direciona_Ponteiro(Size_Structs);

    fseek(*insere, 0, SEEK_END);
    bytes_insere = ftell(*insere);

    if((sizeof(CliF) * tamanho) == bytes_insere){
        printf("Nao ha mais registros a serem inseridos...\n\n");
        return;
    }

    fseek(*insere, sizeof(CliF) * tamanho, SEEK_SET);
    fseek(*principal, 0, SEEK_END);

    fread(&cliente, sizeof(CliF), 1, *insere);
    sprintf(buffer,"%d#%d#%s#%s#%s#", cliente.CodCli, cliente.CodF, cliente.NomeCli,
            cliente.NomeF, cliente.Genero);

    tamanho = strlen(buffer);
    fwrite(&tamanho, sizeof(int), 1, *principal);
    fwrite(buffer, sizeof(char), tamanho, *principal);

    fseek(*Size_Structs, 0, SEEK_END);
    deslocamento = ftell(*Size_Structs);

    if(deslocamento == 0){
            Vet_Tam[0] = 1; Vet_Tam[1] = 1;
            fwrite(Vet_Tam, sizeof(Vet_Tam), 1, *Size_Structs); //Já escrevi a primeira inserção nele
    }
    else{
        fseek(*Size_Structs, 0, SEEK_SET);
        fread(Vet_Tam, sizeof(Vet_Tam), 1, *Size_Structs);
        Vet_Tam[0] = Vet_Tam[0] + 1;
    }

    if(deslocamento == 0){ //significa que eu ainda não adicionei nada

        *primaria = malloc(sizeof(Busca_SP));
        *S1 = malloc(sizeof(Busca_Secundaria));
        *S2 = malloc(sizeof(Busca_SP));

        (*primaria + 0)->CodCli = cliente.CodCli;
        (*primaria + 0)->CodF = cliente.CodF;
        (*primaria + 0)->BOS_Prox = ftell(*principal) - strlen(buffer) - sizeof(int); //ESSE BOS TÁ CERTO?

        (*S2 + 0)->BOS_Prox = -1;
        (*S2 + 0)->CodCli = cliente.CodCli;
        (*S2 + 0)->CodF = cliente.CodF;

        strcpy((*S1 + 0)->Nome_Filme, cliente.NomeF);
        (*S1 + 0)->Bos_LIP2 = 0;

    }else{

         int index = Vet_Tam[0] - 1;
        *primaria = realloc(*primaria, Vet_Tam[0] * sizeof(Busca_SP));
        *S2 = realloc(*S2, Vet_Tam[0] * sizeof(Busca_SP));

         /*--------------CONSTRUINDO O ÍNDICE PRIMÁRIO---------------*/
         (*primaria)[index].BOS_Prox = ftell(*principal) - strlen(buffer) - sizeof(int);
         (*primaria)[index].CodCli = cliente.CodCli;
         (*primaria)[index].CodF = cliente.CodF;

    /*--------------CONSTRUINDO A LISTA INVERTIDA---------------*/
        posicao = strlen(cliente.NomeF);
        argumento = malloc(posicao * sizeof(char));

        strcpy(argumento, cliente.NomeF);

        posicao = Verifica_Existencia_Nome(S1, argumento, Vet_Tam[1]);

        if(posicao > -1){
                //Não precisa realocar memória, neste caso
                (*S2)[index].CodCli = cliente.CodCli;
                (*S2)[index].CodF = cliente.CodF;
                (*S2)[index].BOS_Prox = (*S1)[posicao].Bos_LIP2;

                (*S1)[posicao].Bos_LIP2 = index * sizeof(Busca_SP);
        }else{
                //Aqui eu atualizo somente o vet_tam[1]
                Vet_Tam[1] = Vet_Tam[1] + 1;
                *S1 = realloc(*S1, Vet_Tam[1] * sizeof(Busca_Secundaria));

                 (*S2)[index].CodCli =  cliente.CodCli;
                 (*S2)[index].CodF =  cliente.CodF;
                 (*S2)[index].BOS_Prox = -1; //o nome ainda não existe, o Bos do próximo é -1

                int Tam = Vet_Tam[1]-1;

                (*S1)[Tam].Bos_LIP2 = index * sizeof(Busca_SP);
                strcpy((*S1)[Tam].Nome_Filme, cliente.NomeF);
        }

    }

    Ordena_Primaria(primaria, Vet_Tam[0]);

    /*printf("INDICE PRIMARIO:\n");
    for(int i = 0; i < Vet_Tam[0]; i++){
        printf("%d %d %d\n\n", (*primaria)[i].CodCli, (*primaria)[i].CodF, (*primaria)[i].BOS_Prox);
    }
    printf("\n");
    printf("LIP2:\n");
    for(int i = 0; i < Vet_Tam[0]; i++){
        printf("%d %d %d\n\n", (*S2)[i].CodCli, (*S2)[i].CodF, (*S2)[i].BOS_Prox);
    }
    printf("\n");
    printf("LIP1:\n");
    for(int i = 0; i < Vet_Tam[1]; i++){
        printf("%s %d\n\n", (*S1)[i].Nome_Filme, (*S1)[i].Bos_LIP2);
    }
    printf("\n\n\n");
    */

    if(deslocamento > 0){
        fseek(*Size_Structs, 0, SEEK_SET);
        fwrite(Vet_Tam, sizeof(Vet_Tam), 1, *Size_Structs);
    }

    free(argumento);
    argumento = NULL;

   printf("Registro inserido com sucesso...\n\n\n");
}

void Ordena_Primaria(Busca_SP** v, int tam){
    //IF CODCLI * 10 + COF [I] > CODCLI * 10 + CODF [I+1]
    int i, j, menor;
    Busca_SP temp; //struct auxiliar

  for (i = 0; i < tam - 1; i++)
  {
    menor = i;

    for (j = i + 1 ; j < tam; j++){
        // 1 1      1 2     2 1     2 2
        //if ( (*v)[j].CodCli <= (*v)[menor].CodCli && (*v)[j].CodF < (*v)[menor].CodF) menor = j;
        //if ( (*v)[j].CodCli <= (*v)[menor].CodCli && (*v)[j].CodF > (*v)[menor].CodF) menor = j;
        if ((*v)[j].CodCli + (*v)[j].CodF < (*v)[menor].CodCli + (*v)[menor].CodF) menor = j;

    }

    temp = (*v)[menor]; //o novo menor
    (*v)[menor] = (*v)[i]; //deslocando o maior para a posição do menor
    (*v)[i] = temp; //colocando o menor na posição de i
  }

}

bool Direciona_Ponteiro(FILE** Size_Structs){//lembre que o primaria vai ser um vetor de ponteiros
    int insercoes;

    fseek(*Size_Structs, 0, SEEK_END);
    if(ftell(*Size_Structs) == 0) return 0;

    fseek(*Size_Structs, 0, SEEK_SET);
    fread(&insercoes, sizeof(int), 1, *Size_Structs); //quando eu leio o size do IP, já sei quantas inserções eu fiz

    return insercoes;
}

bool Verifica_Existencia_Nome(Busca_Secundaria** S1, char* cliente, int Size_S){
	int i = 0;

    for(i = 0; i < Size_S; i++){ //Pois Size_S é o tamanho atual, e eu ainda nem inserir nada na nova posição
        if(strcmp((*S1)[i].Nome_Filme, cliente) == 0) return i;
    }

    return -1; //Não existe distância negativa
}

void Menu_Opcoes(){
    printf("Digite a opcao que deseja realizar:\n\n");
    printf("[1]Insercao\n\n[2]Pesquisa com indice primario\n\n[3]Pesquisa com indice secundario\n\n");
    printf("[0]Sair\n\n");
}

void Carrega_Indices(FILE** principal, FILE** indice_p, FILE** indice_LIP1, FILE** indice_LIP2,
Busca_SP** primaria, Busca_Secundaria** S1, Busca_SP** S2, FILE* Size_Structs)
{
    char Atualizacao = ' ';
    int counter = 0;
    Busca_Secundaria AuxSecundaria;
    Busca_SP AuxSP;

    fseek(*indice_LIP2, 0, SEEK_END); //troquei o índice aqui
    fseek(*principal, 0, SEEK_END);
    fseek(Size_Structs, 0, SEEK_SET);

    if(ftell(*principal) == 0) return; //Simboliza 0 inserções

    fseek(*indice_p, 0, SEEK_SET);
    fread(&Atualizacao, sizeof(char), 1, *indice_p); //Qual a situação no header

    /*ESSA LINHA DIFERENCIA "ZERO INSERÇÕES" DE UMA "PANE DEPOIS DE VÁRIAS INSERÇÕES", UMA VEZ QUE CASO EU FAÇA N INSERÇÕES E O SISTEMA APAGUE,
    O PROGRAMA NÃO CHEGA ATÉ A FUNÇÃO DE DESCARREGAR OS DADOS, E CONSEQUENTEMENTE OS ÍNDICES FICAM com 0kb. PORÉM, O ARQUIVO E DADOS PRINCIPAL
    JÁ POSSUI TODOS OS DADOS INSERIDOS...*/
    if( Atualizacao == 'N' || ( ftell(*principal) > 0 && ftell(*indice_LIP2)  == 0) )
    Recria_Indices(&principal, &indice_p, &indice_LIP1, &indice_LIP2); //Vou deletar o antigo e escrever tudo do zero

    fseek(*indice_p, sizeof(char), SEEK_SET); //Esse tem a flag
    fseek(*indice_LIP1, 0, SEEK_SET);
    fseek(*indice_LIP2, 0, SEEK_SET);

    int alocar = 0;
    int Bytes;

    fread(&alocar, sizeof(int), 1, Size_Structs);
    Bytes = alocar;

    *primaria = malloc(alocar * sizeof(Busca_SP));
    *S2 = malloc(alocar * sizeof(Busca_SP));

    Busca_SP Auxiliar[alocar];

    for(int i = 0; i < alocar; i++){
        (*S2)[i].CodCli = 0;
        (*S2)[i].CodF = 0;
        (*S2)[i].BOS_Prox = 0;
    }

    fseek(*indice_p, sizeof(char), SEEK_SET);
    fseek(*indice_LIP1, 0, SEEK_SET);
    fseek(*indice_LIP2, 0, SEEK_SET);

    counter = 0;

    while(fread(&AuxSP, sizeof(Busca_SP), 1, *indice_p)){
        (*primaria)[counter].CodCli = AuxSP.CodCli;
        (*primaria)[counter].CodF = AuxSP.CodF;
        (*primaria)[counter].BOS_Prox = AuxSP.BOS_Prox;
        //printf("%d %d %d\n", (*primaria)[i].CodCli, (*primaria)[i].CodF, (*primaria)[i].BOS_Prox);
        counter++;
    }

    counter = 0;

    int deslocamento = 0;
    char buffer[50];

    while(fread(buffer, sizeof(buffer), 1, *indice_LIP1)){

        fread(&deslocamento, sizeof(int), 1, *indice_LIP1);

        strcpy( (*S1)[counter].Nome_Filme, buffer );
        (*S1)[counter].Bos_LIP2 = deslocamento;
        //printf("%s %d\n", (*S1)[counter].Nome_Filme, (*S1)[counter].Bos_LIP2);
        counter++;
    }

    counter = 0;

    while(fread(&Auxiliar[counter], sizeof(Busca_SP), 1, *indice_LIP2)){
        //printf("ftell LIP2: %ld\n", ftell(*indice_LIP2));
        counter++;
    }

    for(int i = 0; i < Bytes; i++)
    {
        (*S2)[i].CodCli = Auxiliar[i].CodCli;
        (*S2)[i].CodF = Auxiliar[i].CodF;
        (*S2)[i].BOS_Prox = Auxiliar[i].BOS_Prox;
        //printf("%d %d %d\n", (*S2)[i].CodCli, (*S2)[i].CodF, (*S2)[i].BOS_Prox);
    }

    fseek(*indice_p, 0, SEEK_SET);
    Atualizacao = 'N';
    fwrite(&Atualizacao, sizeof(char), 1, *indice_p);

}

void Recria_Indices(FILE*** principal, FILE*** indice_p, FILE*** indice_LIP1, FILE*** indice_LIP2){

      //Busca_Secundaria S1[50];
    Busca_SP* IP = NULL, *S2 = NULL;
    Busca_Secundaria* S1 = NULL;
    char buffer[sizeof(CliF)], *ponteiro = NULL;
    int contador = 0, tamanho = 0, entremeio = 0, retorno = 0, controle = 0;
    int valor;

    fseek(**principal, 0, SEEK_SET);

    IP = malloc(sizeof(Busca_SP));
    S1 = malloc(sizeof(Busca_Secundaria));
    S2 = malloc(sizeof(Busca_SP));

    while(fread(&tamanho, sizeof(int), 1, **principal)){

        fread(buffer, sizeof(char), tamanho, **principal);

        if(!contador){ // "PRIMEIRA INSERÇÃO"; Cuidado com lixo na próxima iteração; BOS do princiapal
                        //Acredito que o lixo não será maior problema porque o ponteiro vai até a quinta cerquilha, somente
            //primary index
            ponteiro = strtok(buffer, "#");
            entremeio = atoi(ponteiro);
            IP[contador].CodCli = entremeio;

            ponteiro = strtok(NULL, "#");
            entremeio = atoi(ponteiro);
            IP[contador].CodF = entremeio;
            IP[contador].BOS_Prox = ftell(**principal) - tamanho - sizeof(int);

            //inverted list
            ponteiro = strtok(NULL, "#"); //skipping customer name
            ponteiro = strtok(NULL, "#"); //Movie name

            S1[contador].Bos_LIP2 = contador * sizeof(Busca_SP);
            strcpy(S1[contador].Nome_Filme, ponteiro);
            controle++;

            S2[contador].CodCli = IP[contador].CodCli;
            S2[contador].CodF = IP[contador].CodF;
            S2[contador].BOS_Prox = -1;

        }else{

            IP = realloc(IP, (contador + 1) * sizeof(Busca_SP));
            S2 = realloc(S2, (contador + 1) * sizeof(Busca_SP));

            //Primary index
            ponteiro = strtok(buffer, "#");
            entremeio = atoi(ponteiro);
            IP[contador].CodCli = entremeio;

            ponteiro = strtok(NULL, "#");
            entremeio = atoi(ponteiro);
            IP[contador].CodF = entremeio;
            IP[contador].BOS_Prox = ftell(**principal) - tamanho - sizeof(int);

            //Inverted list
            ponteiro = strtok(NULL, "#"); //skipping customer name
            ponteiro = strtok(NULL, "#"); //Movie names

            if((retorno = Verifica_Existencia_Nome(&S1, ponteiro, controle)) > -1){

                S2[contador].CodCli = IP[contador].CodCli;
                S2[contador].CodF = IP[contador].CodF;
                S2[contador].BOS_Prox = S1[retorno].Bos_LIP2;

                S1[retorno].Bos_LIP2 = contador * sizeof(Busca_SP); //BOS do registro inserido no S2

            }else{

                controle++;
                //S1 = realloc(S1, controle);//Como você quer alocar somente "controle bytes?"
                S1 = realloc(S1, controle * sizeof(Busca_Secundaria));

                S2[contador].CodCli = IP[contador].CodCli;
                S2[contador].CodF = IP[contador].CodF;
                S2[contador].BOS_Prox = -1;

                S1[controle - 1].Bos_LIP2 = contador * sizeof(Busca_SP); //BOS do registro inserido no S2
                strcpy(S1[controle - 1].Nome_Filme, ponteiro);

            }

        }
        contador = contador + 1; //RRN of IP and SN
    }

    int counter = 0;
    char nome_filme[50];
    Busca_SP Aux;
    Busca_Secundaria Aux_S;

    fseek(**indice_p, sizeof(char), SEEK_SET);
    fseek(**indice_LIP1, 0, SEEK_SET);
    fseek(**indice_LIP2, 0, SEEK_SET);

    Ordena_Primaria(&IP, contador);

    /*PREENCHENDO O ÍNDICE PRIMÁRIO*/
    int i = 0;
    for(i = 0; i < contador; i++){

        fwrite(&IP[i].CodCli, sizeof(int), 1, **indice_p);
        fwrite(&IP[i].CodF, sizeof(int), 1, **indice_p);
        fwrite(&IP[i].BOS_Prox, sizeof(int), 1, **indice_p);
    }

    /*PREENCHENDO O S2*/
    for(i = 0; i < contador; i++){

        fwrite(&S2[i].CodCli, sizeof(int), 1, **indice_LIP2);
        fwrite(&S2[i].CodF, sizeof(int), 1, **indice_LIP2);
        fwrite(&S2[i].BOS_Prox, sizeof(int), 1, **indice_LIP2);
    }

    char nome[50];
    int recebe;

    /*PREENCHENDO O S1*/
    for(i = 0; i < controle; i++){

        strcpy(nome, S1[i].Nome_Filme);
        fwrite(nome, sizeof(char), 50, **indice_LIP1);

        recebe = S1[i].Bos_LIP2;
        fwrite(&recebe, sizeof(int), 1, **indice_LIP1);
    }

/*
    ESSE TRECHO É SÓ PRA LER E VER SE O CONTEÚDO FOI GRAVADO CORRETAMENTE NO ÍNDICE
    fseek(**indice_p, sizeof(char), SEEK_SET);
    while(fread(&Aux, sizeof(Busca_SP), 1, **indice_p)){

        printf("%d %d %d\n", Aux.CodCli, Aux.CodF, Aux.BOS_Prox);
        counter++;

    }

    printf("\n\n");
    counter = 0;

    fseek(**indice_LIP2, 0, SEEK_SET);
    while(fread(&Aux, sizeof(Busca_SP), 1, **indice_LIP2)){

        printf("%d %d %d\n", Aux.CodCli, Aux.CodF, Aux.BOS_Prox);
        counter++;

    }

    printf("\n\n");

    counter = 0;

    int numero;

    fseek(**indice_LIP1, 0, SEEK_SET);
    while(fread(nome_filme, sizeof(nome_filme), 1, **indice_LIP1)){

        fread(&numero, sizeof(int), 1, **indice_LIP1);

        //printf("%s %d\n", nome_filme, numero);
        counter++;

    }
*/
    free(IP);
    free(S1);
    free(S2);
    IP = NULL;
    S1 = NULL;
    S2 = NULL;
}

void Pesquisa_primaria(FILE** principal, Busca_SP** primaria, FILE** busca_p, int Tamanho)
{
    //Verificar se o filme existe
    Primaria_Buscar Aux_Leitura;
    int i = 0;

    fseek(*busca_p, 0, SEEK_END);

    unsigned deslocamento;

    if( (deslocamento =  direciona_busca(ftell(*busca_p)) ) == -1){
        printf("Nao existem mais chaves a serem pesquisadas\n\n");
        return;
    }

    fseek(*busca_p, deslocamento, SEEK_SET);
    fread(&Aux_Leitura, sizeof(Primaria_Buscar), 1, *busca_p);

    //printf("Chaves que estao sendo pesquisadas: %d %d\n\n\n", Aux_Leitura.CodCli, Aux_Leitura.CodF);

    while(1)
    {
        if( (*primaria)[i].CodCli == Aux_Leitura.CodCli && (*primaria)[i].CodF == Aux_Leitura.CodF ){

            deslocamento = (*primaria)[i].BOS_Prox;
            fseek(*principal, deslocamento, SEEK_SET);
            break;

        }

        i++;

        if(i  == Tamanho){
            printf("A chave solicitada nao existe...\n\n");
            return;
        }

    }

    char buffer[sizeof(CliF)];
    int Tam_Reg;

    fread(&Tam_Reg, sizeof(int), 1, *principal);
    fread(buffer, sizeof(char), Tam_Reg, *principal);

    char *p = NULL;

    p = strtok(buffer, "#");
    printf("Codigo do cliente: %c\n", *p);

    p = strtok(NULL, "#");
    printf("Codigo do filme:   %c\n", *p);

    p = strtok(NULL, "#");
    printf("Nome do cliente:   %s\n", p);

    p = strtok(NULL, "#");
    printf("Nome do filme:     %s\n", p);

    p = strtok(NULL, "#");
    printf("Genero do filme:   %s\n", p);

    printf("\n\n");
}

bool direciona_busca(int bytes_busca){

    FILE* Ponteiro_Buscar;
    char simbolo = '+';

    if((Ponteiro_Buscar =  fopen("Pesquisa_Primaria.bin", "a+b")) == NULL){
        printf("Erro na abertura do arquivo\n\n");
        exit(1);
    }

    fseek(Ponteiro_Buscar, 0, SEEK_END);

    if(ftell(Ponteiro_Buscar) == 0) {

        fwrite(&simbolo, sizeof(char), 1, Ponteiro_Buscar);
        fclose(Ponteiro_Buscar);
        return 0;

    }
    else{

        unsigned RRN;

        RRN = ftell(Ponteiro_Buscar); //Temos um byte para cara RRN

        if(RRN * sizeof(Primaria_Buscar) == bytes_busca) return -1;

        fwrite(&simbolo, sizeof(char), 1, Ponteiro_Buscar);
        fclose(Ponteiro_Buscar);

        return RRN * sizeof(Primaria_Buscar);
    }
}

bool Tamanho_Indice_Primario(FILE** Size_Structs){

    int leitura;

    fseek(*Size_Structs, 0, SEEK_SET);

    fread(&leitura, sizeof(int), 1, *Size_Structs);

    return leitura;

}

void Pesquisa_Secundaria(FILE** principal, FILE** Size_Structs, Busca_SP** primaria,
Busca_Secundaria** S1, Busca_SP** S2, FILE** busca_s, int* direcionamento)
{

    int tamanho_IP = 0, tamanho_Sec = 0;

/*-------------------------DIRECIONANDO O ARQUIVO DE RECUPERAÇÃO DE DADOS-------------------------*/
    fseek(*busca_s, 0, SEEK_END);

    if( (*direcionamento) * 50 == ftell(*busca_s))
    {
        printf("Nao existem mais chaves a serem pesquisadas\n\n");//chegou no fim
        return;
    }

    fseek(*busca_s, (*direcionamento) * 50, SEEK_SET); //já direcionei o arquivo com os nomes dos filmes

    fseek(*Size_Structs, 0, SEEK_END);

    if(ftell(*Size_Structs) == 0){
        printf("Nao existem dados a serem pesquisados\n\n"); //Não foram inseridos dados ainda
        return;
    }

    fseek(*Size_Structs, 0, SEEK_SET);//DEPOIS EU COLOCO O MÓDULO DE NÃO TER NADA PARA PESQUISAR AINDA;
    fread(&tamanho_IP, sizeof(int), 1, *Size_Structs); //tamanho do índice primário
    fread(&tamanho_Sec, sizeof(int), 1, *Size_Structs); //tamanho do índice secundário

/*-----------------------------------------------------------------------------------------------*/

    int BOS, counter = 0;
    char buffer[50];

    fread(buffer, sizeof(buffer), 1, *busca_s);

    //primeiro eu devo tentar encontrar o filme
    int i = 0;
    for(i = 0; i < tamanho_Sec; i++)
    {

        if( strcmp(buffer, (*S1)[i].Nome_Filme) == 0)
        {
             BOS = (*S1)[i].Bos_LIP2; //deslocamento no LIP2
             counter++;
             //Por enquanto, eu sei que ele tá chegando no BOS 0 antes daqui, por algum motivo
        }

    }

    if(!counter)
    {
        printf("A chave solicitada nao existe\n\n");
        *direcionamento = *direcionamento + 1;
        return;
    }

    int RRN = 0, TAM = 0, posicao = 0;
    char *ponteiro;

    char Data[sizeof(CliF)];

    while(1)
    {

        RRN = BOS/sizeof(Busca_SP); //posição referenciada pelo LIP1 do LIP2 com a chaves associadas ao filme
		int i = 0;
        for(i = 0; i< tamanho_IP; i++)
        {

            if( (*primaria)[i].CodCli == (*S2)[RRN].CodCli && (*primaria)[i].CodF == (*S2)[RRN].CodF )
            {

                posicao = (*primaria)[i].BOS_Prox;
                fseek(*principal, posicao, SEEK_SET);
                break;

            }

        }

        fread(&TAM, sizeof(int), 1, *principal);
        fread(Data, TAM, 1, *principal);

        ponteiro = strtok(Data, "#");
        printf("Codigo do cliente: %c\n", *ponteiro);

        ponteiro = strtok(NULL, "#");
        printf("Codigo do filme:   %c\n", *ponteiro);

        ponteiro = strtok(NULL, "#");
        printf("Nome do cliente:   %s\n", ponteiro);

        ponteiro = strtok(NULL, "#");
        printf("Nome do filme:     %s\n", ponteiro);

        ponteiro = strtok(NULL, "#");
        printf("Genero do filme:   %s\n", ponteiro);

        printf("\n\n");

        BOS = (*S2)[RRN].BOS_Prox;

        if(BOS == -1) break;

    }

    printf("\n\n");

    *direcionamento = *direcionamento + 1;
}

void Designa_Abertura(FILE** principal, FILE** insere, FILE** busca_p, FILE** busca_s){

    if((*principal =  fopen("principal.bin", "a+b")) ==  NULL){
        printf("Erro na abertura do arquivo\n");
        exit(1);
    }

    fclose(*principal);
    if((*principal = fopen("principal.bin", "r+b")) == NULL){
        printf("Erro na abertura do arquivo\n");
        exit(1);
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

void Designa_Abertura_Indices(FILE** indice_p, FILE** indice_LIP1, FILE** indice_LIP2, FILE** Size_Structs){

    if((*indice_p = fopen("indice_p.bin", "a+b")) == NULL){
        printf("Erro na abertura do arquivo\n");
        exit(1);
    }

    fclose(*indice_p);
    if((*indice_p = fopen("indice_p.bin", "r+b")) == NULL){
        printf("Erro na abertura do arquivo\n");
        exit(1);
    }

    if((*indice_LIP1 = fopen("indice_LIP1.bin", "a+b")) == NULL){
        printf("Erro na abertura do arquivo\n");
        exit(1);
    }

    fclose(*indice_LIP1);
    if((*indice_LIP1 = fopen("indice_LIP1.bin", "r+b")) == NULL){
        printf("Erro na abertura do arquivo\n");
        exit(1);
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

    if((*Size_Structs = fopen("Size_Structs.bin", "a+b")) == NULL){
        printf("Erro na abertura do arquivo\n\n");
        exit(1);
    }

    fseek(*Size_Structs, 0, SEEK_END);

    fclose(*Size_Structs);

    if((*Size_Structs = fopen("Size_Structs.bin", "r+b")) == NULL){
        printf("Erro ao alternar o modo\n\n");
        exit(1);
    }

    fseek(*Size_Structs, 0, SEEK_END);
    fseek(*indice_LIP2, 0, SEEK_END);

    /*PROTEÇÃO CONTRA APAGÃO ANTES DE INSERIR QUALQUER DADO NOS ÍNDICES*/
    if(ftell(*Size_Structs) > 0 && ftell(*indice_LIP2) == 0){

         fclose(*Size_Structs);
         fclose(*indice_LIP2);
         fclose(*indice_LIP1);
         fclose(*indice_p);

        if((*Size_Structs = fopen("Size_Structs.bin", "r+b")) == NULL){
        printf("Erro ao alternar o modo\n\n");
        exit(1);
        }

        if((*indice_LIP2 = fopen("indice_LIP2.bin", "r+b")) == NULL){
            printf("Erro na abertura do arquivo\n");
            exit(1);
        }

        if((*indice_LIP1 = fopen("indice_LIP1.bin", "r+b")) == NULL){
            printf("Erro na abertura do arquivo\n");
            exit(1);
        }

        if((*indice_p = fopen("indice_p.bin", "r+b")) == NULL){
            printf("Erro na abertura do arquivo\n");
            exit(1);
        }
    }

}

void Alterna_Para_R(FILE** Size_Structs, FILE** principal){

    fclose(*Size_Structs);
     if((*Size_Structs = fopen("Size_Structs.bin", "r+b")) == NULL){
         printf("Erro ao alternar o modo\n\n");
        exit(1);
     }

    fclose(*principal);
    if((*principal = fopen("principal.bin", "r+b")) == NULL){
        printf("Erro ao alternar o modo\n\n");
        exit(1);
    }

}

void Descarrega_Dados(FILE** indice_p, FILE** indice_LIP1, FILE** indice_LIP2, Busca_SP** primaria,
Busca_Secundaria** S1, Busca_SP** S2, FILE** Size_Structs)
{
    char Atualizacao = 'S';
    int i = 0, guarda_tamanho = 0, recebe = 0;

    if(*primaria == NULL){
        printf("Nao existem dados a serem descarregados\n\n");
        return;
    }

    fseek(*Size_Structs, 0, SEEK_SET);
    fseek(*indice_p, 0, SEEK_SET);
    fseek(*indice_LIP1, 0, SEEK_SET);
    fseek(*indice_LIP2, 0, SEEK_SET);
    fwrite(&Atualizacao, sizeof(char), 1, *indice_p);//Aqui eu faço diferente pra poder escrever o caracter

    fread(&guarda_tamanho, sizeof(int), 1, *Size_Structs);

    for(i=0; i < guarda_tamanho; i++){
        recebe = (*primaria)[i].CodCli;
        fwrite(&recebe, sizeof(int), 1, *indice_p);

        recebe = (*primaria)[i].CodF;
        fwrite(&recebe , sizeof(int), 1, *indice_p);

        recebe = (*primaria)[i].BOS_Prox;
        fwrite(&recebe, sizeof(int), 1, *indice_p);

        //printf("%d %d %d\n", (*primaria)[i].CodCli, (*primaria)[i].CodF, (*primaria)[i].BOS_Prox);
    }

    for(i=0; i < guarda_tamanho; i++){
        recebe = (*S2)[i].CodCli;
        fwrite(&recebe, sizeof(int), 1, *indice_LIP2);

        recebe = (*S2)[i].CodF;
        fwrite(&recebe , sizeof(int), 1, *indice_LIP2);

        recebe = (*S2)[i].BOS_Prox;
        fwrite(&recebe, sizeof(int), 1, *indice_LIP2);
        //printf("%d %d %d\n", (*S2)[i].CodCli, (*S2)[i].CodF, (*S2)[i].BOS_Prox);
    }

    //printf("\n\n");

    fread(&guarda_tamanho, sizeof(int), 1, *Size_Structs);

    char nome[50];

    for(i=0; i < guarda_tamanho; i++){

        strcpy(nome, (*S1)[i].Nome_Filme);
        fwrite(nome, sizeof(char), 50, *indice_LIP1);

        recebe = (*S1)[i].Bos_LIP2;
        fwrite(&recebe, sizeof(int), 1, *indice_LIP1);
        //printf("%s %d\n\n", (*S1)[i].Nome_Filme, (*S1)[i].Bos_LIP2);
    }

}

