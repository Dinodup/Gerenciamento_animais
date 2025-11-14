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
 * Nome: menuMedico
 * Objetivo:
 *      Exibir o menu veterinário e permitir operações sobre
 *      a fila de medicação, como inserir, atender e listar
 *      animais que necessitam de cuidados médicos.
 * Entrada:
 *      - zoo: ponteiro para a lista encadeada de habitats
 *      - fila: ponteiro para a fila de medicação
 * Saida:
 *      - nenhuma
 * Pre-condicoes:
 *      - zoo != NULL
 *      - fila != NULL
 * Pos-condicoes:
 *      - Animais podem ser inseridos ou removidos da fila
 *        de acordo com as escolhas do usuário
 * Complexidade:
 *      - Tempo:
 *          O(1) para enfileirar ou desenfileirar
 *          O(n) para listar a fila completa
 *      - Espaco: O(1)
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
 * Nome: main
 * Objetivo:
 *      Controlar o fluxo principal do programa, exibir o menu
 *      geral ao usuário e acionar as funções responsáveis por
 *      cadastro, busca, alteração, transferência, medicação e
 *      manipulação de arquivos.
 * Entrada:
 *      - nenhuma (apenas entrada do usuário em tempo de execução)
 * Saida:
 *      - retorna 0 ao final da execução
 * Pre-condicoes:
 *      - Todos os TADs e funções internas devem estar corretamente
 *        implementados e compilados
 * Pos-condicoes:
 *      - Memória alocada para habitats e fila é liberada antes
 *        do encerramento do programa
 * Complexidade:
 *      - Tempo: O(n) devido às operações iterativas e buscas
 *      - Espaco: O(1) além das estruturas já alocadas
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

