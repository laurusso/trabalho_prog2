#include "cinema.h"
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<stdlib.h>
#include<stddef.h>

#define BLOCO 10

void menu(){

    printf("\n\n== GERENCIAMENTO DE FILMES ==\n\n");
    printf("Menu:\n");
    printf("\t(1) Cadastrar filmes\n");
    printf("\t(2) Atualizar filmes\n");
    printf("\t(3) Excluir filmes\n");
    printf("\t(4) Consultar filmes\n");
    printf("\t(5) Sair\n");

}

Filme receber_dados(){

    Filme filme;

    printf("Insira o ID: ");
    scanf("%d", &filme.id);
    getchar();

    printf("\nNome do filme: ");
    fgets(filme.nome, 50, stdin);
    if(filme.nome[strlen(filme.nome)-1] == '\n'){
        filme.nome[strlen(filme.nome)-1] = '\0';
    }          

    printf("\nEsta em cartaz? (1 - sim; 0 - nao) ");
    scanf("%d", &filme.cartaz);
    getchar();

    while(filme.cartaz != 0 && filme.cartaz != 1){
        printf("Valor invalido! Redigite: ");
        scanf("%d", &filme.cartaz);
        getchar();
    }

    printf("\nQuantas sessoes esse filme tera? ");
    scanf("%d", &filme.num_sessoes);
    getchar();

    filme.horario_sessoes = (int*)malloc(filme.num_sessoes*sizeof(int));

    printf("Quais os horarios das sessoes?(HH) ");

    //utiliza for para a quantidade de horarios desejada pelo usuario
    for(int i=0;i<filme.num_sessoes;i++){
        printf("\nHorario %d: ", i+1);
        scanf("%d",&filme.horario_sessoes[i]);
        getchar();
    }

    printf("\nQual o valor do ingresso? (XX.XX) ");
    scanf("%f", &filme.ingresso.valor);
    getchar();
    printf("\nQuantos ingressos tem disponiveis? ");
    scanf("%d", &filme.ingresso.qtd_disponivel);
    getchar();

    return filme;

}

void cadastrar_filme(const char *filename, Filme filme){

    FILE *file = fopen(filename, "ab");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    //salva cada parte da struct separadamente para que a alocacao dinamica funcione
    fwrite(&filme.id, sizeof(int), 1, file);
    fwrite(filme.nome, sizeof(char), 50, file);
    fwrite(&filme.cartaz, sizeof(int), 1, file);
    fwrite(&filme.ingresso, sizeof(Ingresso), 1, file);
    fwrite(&filme.num_sessoes, sizeof(int), 1, file);
    fwrite(filme.horario_sessoes, sizeof(int), filme.num_sessoes, file);

    fclose(file);
    printf("Filme cadastrado com sucesso.\n");

}


void atualizar_filmes(const char *filename){

    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Filme *filmes = NULL;
    size_t num_filmes = 0;

    Filme filme;
    while (fread(&filme.id, sizeof(int), 1, file) > 0) {
        fread(filme.nome, sizeof(char), 50, file);
        char *newline_pos = strchr(filme.nome, '\n');

        if (newline_pos != NULL) {
            *newline_pos = '\0';
        }

        fread(&filme.cartaz, sizeof(int), 1, file);
        fread(&filme.ingresso, sizeof(Ingresso), 1, file);
        fread(&filme.num_sessoes, sizeof(int), 1, file);

        filme.horario_sessoes = malloc(filme.num_sessoes * sizeof(int));

        if (fread(filme.horario_sessoes, sizeof(int), filme.num_sessoes, file) != filme.num_sessoes) {
            printf("Erro ao ler os horarios das sessoes do filme.\n");
            free(filme.horario_sessoes);
            continue;
        }

        filmes = realloc(filmes, (num_filmes + 1) * sizeof(Filme));
        if (filmes == NULL) {
            printf("Erro ao realocar memoria.\n");
            fclose(file);
            return;
        }

        filmes[num_filmes] = filme;
        num_filmes++;

    }

    fclose(file);

    
    printf("Selecione o numero do filme que deseja atualizar:\n");
    for (size_t i = 0; i < num_filmes; i++) {
        printf("%zu: %s\n", i + 1, filmes[i].nome);
    }

    size_t escolha;
    printf("Digite o numero do filme: ");
    scanf("%zu", &escolha);

    if (escolha < 1 || escolha > num_filmes) {
        printf("Escolha invalida.\n");
        
        for (size_t i = 0; i < num_filmes; i++) {
            free(filmes[i].horario_sessoes);
        }
        free(filmes);
        return;
    }

    size_t index = escolha - 1;
    free(filmes[index].horario_sessoes);
    Filme atualizado = receber_dados();
    
    filmes[index] = atualizado;
    filmes[index].horario_sessoes = malloc(atualizado.num_sessoes * sizeof(int));
    memcpy(filmes[index].horario_sessoes, atualizado.horario_sessoes, atualizado.num_sessoes * sizeof(int));

    
    file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        
        for (size_t i = 0; i < num_filmes; i++) {
            free(filmes[i].horario_sessoes);
        }
        free(filmes);
        return;
    }

    for (size_t i = 0; i < num_filmes; i++) {
        fwrite(&filmes[i].id, sizeof(int), 1, file);
        fwrite(filmes[i].nome, sizeof(char), 50, file);
        fwrite(&filmes[i].cartaz, sizeof(int), 1, file);
        fwrite(&filmes[i].ingresso, sizeof(Ingresso), 1, file);
        fwrite(&filmes[i].num_sessoes, sizeof(int), 1, file);
        fwrite(filmes[i].horario_sessoes, sizeof(int), filmes[i].num_sessoes, file);
    }

    fclose(file);

    for (size_t i = 0; i < num_filmes; i++) {
        free(filmes[i].horario_sessoes);
    }
    free(filmes);

    printf("Filme '%s' foi atualizado!\n", atualizado.nome);

}

void excluir_filmes(const char *filename, const char *nome) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    FILE *temp = fopen("temp.bin", "wb");
    if (temp == NULL) {
        printf("Erro ao criar o arquivo temporario.\n");
        fclose(file);
        return;
    }

    Filme filme;
    bool found = false;

    while (fread(&filme.id, sizeof(int), 1, file) > 0) {
        fread(filme.nome, sizeof(char), 50, file);
        fread(&filme.cartaz, sizeof(int), 1, file);
        fread(&filme.ingresso, sizeof(Ingresso), 1, file);
        fread(&filme.num_sessoes, sizeof(int), 1, file);

        filme.horario_sessoes = malloc(filme.num_sessoes * sizeof(int));
        fread(filme.horario_sessoes, sizeof(int), filme.num_sessoes, file);

        if (strcmp(filme.nome, nome) == 0) {
            found = true;
            free(filme.horario_sessoes);  
            continue;
        }

        //atualiza um a um
        fwrite(&filme.id, sizeof(int), 1, temp);
        fwrite(filme.nome, sizeof(char), 50, temp);
        fwrite(&filme.cartaz, sizeof(int), 1, temp);
        fwrite(&filme.ingresso, sizeof(Ingresso), 1, temp);
        fwrite(&filme.num_sessoes, sizeof(int), 1, temp);
        fwrite(filme.horario_sessoes, sizeof(int), filme.num_sessoes, temp);

        free(filme.horario_sessoes);  
    }

    fclose(file);
    fclose(temp);

    
    if (found) {
        remove(filename);
        rename("temp.bin", filename);
        printf("Registro '%s' excluido fisicamente.\n", nome);
    }

    else {
        printf("Registro com nome '%s' nao encontrado.\n", nome);
        remove("temp.bin"); 
    }
}


void impressao(Filme *filmes, int index) {
    if (index < 0) {
        return;
    }

    printf("ID: %d\n", filmes[index].id);
    printf("Nome: %s\n", filmes[index].nome);
    if(filmes[index].cartaz){
        printf("Cartaz: sim\n");
    }
    else{
        printf("Cartaz: nao\n");
    }
    
    printf("Ingressos disponiveis: %d\n", filmes[index].ingresso.qtd_disponivel);
    printf("Valor do ingresso: R\x24%.2f\n", filmes[index].ingresso.valor); //\x24 para o simbolo do $
    printf("Numero de sessoes: %d\n", filmes[index].num_sessoes);
    for (int i = 0; i < filmes[index].num_sessoes; i++) {
        printf("Horario %d: %dh00\n", i + 1, filmes[index].horario_sessoes[i]);
    }
    printf("\n");

    impressao(filmes, index - 1); //utilizacao da recursao
}

void consultar_filmes(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Filme *filmes = NULL;
    size_t num_filmes = 0;

    Filme filme;
    while (fread(&filme.id, sizeof(int), 1, file) > 0) {
        fread(filme.nome, sizeof(char), 50, file);
        char *newline_pos = strchr(filme.nome, '\n');
        if (newline_pos != NULL) {
            *newline_pos = '\0';
        }
        fread(&filme.cartaz, sizeof(int), 1, file);
        fread(&filme.ingresso, sizeof(Ingresso), 1, file);
        fread(&filme.num_sessoes, sizeof(int), 1, file);

        filme.horario_sessoes = malloc(filme.num_sessoes * sizeof(int));
        if (fread(filme.horario_sessoes, sizeof(int), filme.num_sessoes, file) != filme.num_sessoes) {
            printf("Erro ao ler os horarios das sessoes do filme.\n");
            free(filme.horario_sessoes);
            continue;
        }

        filmes = realloc(filmes, (num_filmes + 1) * sizeof(Filme));
        if (filmes == NULL) {
            printf("Erro ao realocar memoria.\n");
            fclose(file);
            return;
        }

        filmes[num_filmes] = filme;
        num_filmes++;
    }
    fclose(file);

    //funcao recursiva de impressao
    if (num_filmes > 0) {
        impressao(filmes, num_filmes - 1);
    }

    for (size_t i = 0; i < num_filmes; i++) {
        free(filmes[i].horario_sessoes);
    }

    free(filmes);
}

