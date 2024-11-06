#ifndef CINEMA_H
#define CINEMA_H

typedef struct ingresso{
    float valor;
    int qtd_disponivel;
}Ingresso;

typedef struct filme{
    int id;
    char nome[50];
    int cartaz;
    int* horario_sessoes;
    Ingresso ingresso;
    int num_sessoes;
}Filme;

void menu();
Filme receber_dados();
void cadastrar_filme(const char *filename, Filme filme);
void atualizar_filmes(const char *filename);
void excluir_filmes(const char *filename, const char *nome);
void impressao(Filme *filmes, int index);
void consultar_filmes(const char *filename);

#endif 
