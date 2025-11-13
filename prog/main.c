/************************************************
 * Nome do(a) estudante: Eduardo Pereira Xavier *
 * Trabalho Prático                             *
 * Disciplinas: Algoritmos e Programação II      *
 * Laboratório de Algoritmos e Programação II    *
 * Professores: Ivone e Ronaldo                 *
 * Data: 05/11/2025                             *
 * Versão: 1.0                                  *
 * Descrição: Programa principal do sistema de  *
 * gerenciamento de animais em um zoológico.    *
 *************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "animal.h"
#include "habitat.h"
#include "fila.h"
#include "arquivo.h"
#include "utils.h"

void menuPrincipal();
void menuMedico();
void limparTela();

/* Função principal */
int main() {
    Habitat *zoo;
    FilaMed *fila;
    int opcao;
    int sair = 0;

    zoo = inicializarHabitats();
    fila = criarFilaMed();

    printf("✅ Zoológico inicializado com 20 habitats.\n");

    while (!sair) {
        printf("\n=====================================\n");
        printf("  SISTEMA DE GERENCIAMENTO DO ZOOLÓGICO\n");
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
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

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
        			printf("❌ Animal não encontrado.\n");
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
                printf("⚠️  Opção inválida. Tente novamente.\n");
                break;
        }
    }

    liberarMemoria(zoo, fila);
    return 0;
}

/* Menu do módulo veterinário (fila de medicação) */
void menuMedico(Habitat *zoo, FilaMed *fila) {
    int opcao;
    do {
        printf("\n===== MENU VETERINÁRIO =====\n");
        printf("1. Enviar animal para medicação\n");
        printf("2. Atender próximo animal\n");
        printf("3. Listar fila de medicação\n");
        printf("4. Voltar ao menu principal\n");
        printf("=============================\n");
        printf("Escolha uma opção: ");
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
                printf("Opção inválida.\n");
        }

        if (opcao != 4) {
            printf("\nPressione ENTER para continuar...");
            getchar();
        }

    } while(opcao != 4);
}

