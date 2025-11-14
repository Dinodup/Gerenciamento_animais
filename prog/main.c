/************************************************
 * Nome do estudante: Eduardo Pereira Xavier 	*
 * Trabalho Pratico                             *
 * Disciplinas: Algoritmos e Programação II     *
 * Laboratório de Algoritmos e Programação II   *
 * Professores: Ivone e Ronaldo                 *
 * Data: 26/10/2025                             *
 * Versão: 8.0                                  *
 * Descricao: Programa principal responsavel    *
 * pela interacao com o usuario e execucao das  *
 * operacoes do sistema de gerenciamento do     *
 * zoologico.                                   *
 ************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "animal.h"
#include "habitat.h"
#include "fila.h"
#include "arquivo.h"
#include "utils.h"

/*-------------------------------------------------------------
 * Funcao: menuMedico
 * Entrada:
 *      - zoo: ponteiro para a estrutura de habitats inicializada
 *      - fila: ponteiro para a fila de medicação inicializada
 * Pre-condicoes:
 *      - zoo != NULL
 *      - fila != NULL
 * Pos-condicao:
 *      - pode enfileirar, desenfileirar ou listar animais para medicacao
 * Complexidade:
 *      - O(1) para operacoes basicas da fila
 *      - O(n) para listagem completa
 -------------------------------------------------------------*/
void menuMedico(Habitat *zoo, FilaMed *fila) {

    int opcao;
    do {
        printf("\n========= MENU VETERINARIO ==========\n");
        printf("1. Enviar animal para medicacao\n");
        printf("2. Atender proximo animal\n");
        printf("3. Listar fila de medicacao\n");
        printf("4. Voltar ao menu principal\n");
        printf("=====================================\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch(opcao) {
            case 1:
                adicionarFilaMed(zoo, fila);
                break;
            case 2:
                atenderAnimalFila(fila);
                break;
            case 3:
                listarFilaMed(fila);
                break;
            case 4:
                printf("Retornando ao menu principal...\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }

        if (opcao != 4) {
            printf("\nPressione ENTER para continuar...");
            getchar();
        }

    } while(opcao != 4);
}

/*-------------------------------------------------------------
 * Funcao principal do programa
 * Pre-condições:
 *      - Todas as funções internas e TADs devem estar implementados
 * Pos-condições:
 *      - Inicializa e libera memoria dos habitats e da fila
 * Complexidade:
 *      - O(n) devido a buscas e operacoes iterativas
 -------------------------------------------------------------*/
int main() {
    Habitat *zoo;
    FilaMed *fila;
    int opcao;
    int sair = 0;

    zoo = inicializarHabitats();
    fila = criarFilaMed();

    printf("Zoologico inicializado com 20 habitats.\n");

    while (!sair) {
        printf("\n=====================================\n");
        printf("  SISTEMA DE GERENCIAMENTO DO ZOOLOGICO\n");
        printf("=====================================\n");
        printf("1. Cadastrar animal\n");
        printf("2. Buscar animal\n");
        printf("3. Alterar dados de um animal\n");
        printf("4. Medicar animal\n");
        printf("5. Transferir animal\n");
        printf("6. Salvar dados em arquivo\n");
        printf("7. Carregar dados do arquivo\n");
        printf("8. Sair\n");
        printf("=====================================\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);	/* limpa o buffer */

        switch (opcao) {
            case 1: {
                cadastrarAnimal(zoo);
                break;
            }

            case 2: {
                int id;
                Animal *a;

                printf("\nDigite o ID do animal a buscar: ");
                scanf("%d", &id);

                a = buscarAnimalRecursivo(zoo, id);
                if (a) {
        			exibirAnimal(a);
    			} else {
        			printf("Animal nao encontrado.\n");
    			}
                break;
            }

            case 3: {
                alterarAnimal(zoo);
                break;
            }

            case 4: {
                menuMedico(zoo, fila);
                break;
            }

            case 5: {
                transferirAnimal(zoo, 0);
                break;
            }

            case 6: {
                salvarDados(zoo);
                break;
            }

            case 7: {
                carregarDados(&zoo);
                break;
            }

            case 8: {
                sair = 1;
                break;
            }

            default:
                printf("Opção invalida. Tente novamente.\n");
                break;
        }
    }

    liberarMemoria(zoo, fila);
    return 0;
}
