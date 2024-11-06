/*Projeto do grupo 06 - Sistema de ingressos de cinema
Autoria: 
    Laura Rodrigues Russo 235826
    Lucas de Oliveira Lopes Cardoso 269538
    Maria Clara Marsola Paulini 219443
    Raissa Toassa Martinelli 184404
*/


#include "cinema.h" //inclusao do header
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<stdlib.h>
#include<stddef.h>

int main(){
    
    const char *filename = "filmes.bin";
    Filme filme;
    int opcao;
    char nome_atualizar[50];
    
    do{
        menu();
        scanf("%d", &opcao);
        getchar();
        
        switch (opcao){
            case 1:
                printf("\nCadastro de filmes\n");
                cadastrar_filme(filename, receber_dados());
                break;
            case 2:
                printf("\nAtualizar filmes\n");
                printf("Nome do filme: ");
                atualizar_filmes(filename);

                break;
            case 3:
                printf("\nExcluir filmes\n");
                printf("Nome do filme: ");
                fgets(nome_atualizar, 50, stdin);
                if(nome_atualizar[strlen(nome_atualizar)-1] == '\n'){
                    nome_atualizar[strlen(nome_atualizar)-1] = '\0';
                }
                excluir_filmes(filename, nome_atualizar);
                break;
            case 4:
                printf("\nConsultar filmes\n");
                consultar_filmes(filename);
                break;
            default:
                if(opcao != 5){
                    printf("Opcao invalida\n");
                }
                break;
        }

        if(opcao == 5){
            break;
        }
    }while(1);
    
    return 0;
}
