/************************************************
 * Nome do estudante: Eduardo Pereira Xavier 	*
 * Trabalho Pratico                             *
 * Disciplinas: Algoritmos e Programação II     *
 * Laboratório de Algoritmos e Programação II   *
 * Professores: Ivone e Ronaldo                 *
 * Data: 26/10/2025                             *
 * Versão: 8.0                                  *
 * Descricao: Implementacao do modulo Animal.   *
 * Contem funcoes para cadastro, busca,         *
 * alteracao e exibicao de animais.             *
 *************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "animal.h"
#include "habitat.h"
#include "utils.h"
#include "fila.h"

/*--------------------------------------------------------------
 * Funcao auxiliar: criarAnimalSemID
 * Descricao:
 *      Le dados do usuario e aloca um novo animal sem ID.
 * Pre-condicao:
 *      - Nenhuma
 * Pos-condicao:
 *      - Retorna ponteiro para Animal alocado em memoria
 * Complexidade:
 *      - O(1)
 --------------------------------------------------------------*/
static Animal* criarAnimalSemID() {
    Animal *novo;
    char porteChar;

    novo = (Animal*) malloc(sizeof(Animal));
    if (!novo) {
        printf("Erro de alocacao de memoria.\n");
        exit(1);
    }

    printf("Nome: ");
    fgets(novo->nome, 50, stdin);
    novo->nome[strcspn(novo->nome, "\n")] = '\0';

    printf("Especie: ");
    fgets(novo->especie, 50, stdin);
    novo->especie[strcspn(novo->especie, "\n")] = '\0';

    printf("Dieta (C - Carnivoro, H - Herbivoro, O - Onivoro): ");
    scanf(" %c", &novo->dieta);

    printf("Estado de saude (S - Saudavel, M - Medicacao): ");
    scanf(" %c", &novo->saude);

    printf("Porte (P - Pequeno, M - Medio, G - Grande): ");
    scanf(" %c", &porteChar);

    switch (porteChar) {
        case 'P': case 'p': novo->porte = 1; break;
        case 'M': case 'm': novo->porte = 2; break;
        case 'G': case 'g': novo->porte = 4; break;
        default: novo->porte = 1;
    }

    novo->prox = NULL;
    novo->h = NULL;

    getchar(); /* limpar buffer */

    return novo;
}

/*--------------------------------------------------------------
 * Funcao: idAnimalExiste
 * Descricao:
 *      Verifica se um ID ja existe em qualquer habitat.
 * Pre-condicao:
 *      - inicio != NULL
 * Pos-condicao:
 *      - Retorna 1 se existir, 0 caso contrario
 * Complexidade:
 *      - O(n)
 --------------------------------------------------------------*/
int idAnimalExiste(Habitat *inicio, int idProcurado) {
    Habitat *h = inicio;

    while (h != NULL) {
        Animal *a = h->animais;
        while (a != NULL) {
            if (a->id == idProcurado)
                return 1;
            a = a->prox;
        }
        h = h->prox;
    }
    return 0;
}

/*--------------------------------------------------------------
 * Funcao: cadastrarAnimal
 * Descricao:
 *      Cadastra um novo animal garantindo ausencia de IDs duplicados.
 * Pre-condicao:
 *      - zoo != NULL
 * Pos-condicao:
 *      - Animal criado, validado e inserido no habitat adequado
 * Complexidade:
 *      - O(n)
 --------------------------------------------------------------*/
void cadastrarAnimal(Habitat *zoo) {
    Animal *novo;
    Habitat *hab;
    int id;

    if (!zoo) {
        printf("Zoologico nao inicializado.\n");
        return;
    }

    printf("\n====== CADASTRO DE NOVO ANIMAL ======\n");

    /* leitura e verificacao de ID duplicado */
    while (1) {
        printf("ID do animal: ");
        scanf("%d", &id);
        getchar();

        if (idAnimalExiste(zoo, id)) {
            printf("Ja existe um animal com o ID %d.\n", id);
        } else {
            break;
        }
    }

    novo = criarAnimalSemID();
    novo->id = id;

    hab = escolherHabitat(zoo, novo);

    if (!hab) {
        printf("Nenhum habitat disponivel para este porte.\n");
        free(novo);
        return;
    }

    inserirAnimalNoHabitat(hab, novo);

    printf("Animal cadastrado com sucesso.\n");
}

/*--------------------------------------------------------------
 * Funcao auxiliar recursiva: buscarRec
 * Descricao:
 *      Procura animal por ID percorrendo recursivamente habitats.
 * Pre-condicao:
 *      - hab != NULL ou cadeia de habitats inicializada
 * Pos-condicao:
 *      - Retorna ponteiro para Animal ou NULL se nao encontrado
 * Complexidade:
 *      - O(n)
 --------------------------------------------------------------*/
static Animal* buscarRec(Habitat *hab, int id) {
    Animal *a;

    if (!hab) return NULL;

    a = hab->animais;
    while (a != NULL) {
        if (a->id == id)
            return a;
        a = a->prox;
    }

    return buscarRec(hab->prox, id);
}

/*--------------------------------------------------------------
 * Funcao: buscarAnimalRecursivo
 * Descricao:
 *      Encapsula a funcao recursiva auxiliar buscarRec.
 * Pre-condicao:
 *      - inicio != NULL
 * Pos-condicao:
 *      - Retorna o animal ou NULL
 * Complexidade:
 *      - O(n)
 --------------------------------------------------------------*/
Animal* buscarAnimalRecursivo(Habitat *inicio, int id) {
    return buscarRec(inicio, id);
}

/*--------------------------------------------------------------
 * Funcao: exibirAnimal
 * Descricao:
 *      Mostra informacoes completas de um animal.
 * Pre-condicao:
 *      - a != NULL
 * Pos-condicao:
 *      - Dados impressos no terminal
 * Complexidade:
 *      - O(1)
 --------------------------------------------------------------*/
void exibirAnimal(const Animal *a) {
    if (!a) return;

    printf("\n========== DADOS DO ANIMAL ==========\n");
    printf("ID: %d\n", a->id);
    printf("Nome: %s\n", a->nome);
    printf("Especie: %s\n", a->especie);
    printf("Dieta: %c\n", a->dieta);
    printf("Saude: %c\n", a->saude);
    printf("Porte: %d\n", a->porte);

    if (a->h)
        printf("Habitat atual: %d\n", a->h->id);
    else
        printf("Habitat atual: N/A\n");
    printf("=======================================\n");
}

/*--------------------------------------------------------------
 * Funcao: alterarAnimal
 * Descricao:
 *      Permite alterar dados de um animal existente.
 * Pre-condicao:
 *      - zoo != NULL
 * Pos-condicao:
 *      - Dados do animal podem ser modificados
 * Complexidade:
 *      - O(n) devido a busca
 --------------------------------------------------------------*/
void alterarAnimal(Habitat *zoo) {
    Animal *a;
    int op;
    int id;

    printf("\nDigite o ID do animal que deseja alterar: ");
    scanf("%d", &id);
    getchar();

    a = buscarAnimalRecursivo(zoo, id);
    if (!a) {
        printf("Animal nao encontrado.\n");
        return;
    }

    while (1) {
        printf("\n====== ALTERAR DADOS DO ANIMAL ======\n");
        printf("1. Nome\n");
        printf("2. Especie\n");
        printf("3. Dieta\n");
        printf("4. Saude\n");
        printf("5. Alterar Habitat\n");
        printf("6. Cancelar\n");
        printf("Escolha: ");
        scanf("%d", &op);
        getchar();

        switch (op) {
            case 1:
                printf("Novo nome: ");
                fgets(a->nome, 50, stdin);
                a->nome[strcspn(a->nome, "\n")] = '\0';
                printf("Nome atualizado.\n");
                break;

            case 2:
                printf("Nova especie: ");
                fgets(a->especie, 50, stdin);
                a->especie[strcspn(a->especie, "\n")] = '\0';
                printf("Especie atualizada.\n");
                break;

            case 3:
                printf("Nova dieta (C/H/O): ");
                scanf(" %c", &a->dieta);
                getchar();
                printf("Dieta atualizada.\n");
                break;

            case 4:
                printf("Novo estado de saude (S/M): ");
                scanf(" %c", &a->saude);
                getchar();
                printf("Saude atualizada.\n");
                break;

            case 5:
                printf("\n=== ALTERAR HABITAT ===\n");
                transferirAnimal(zoo, 1);
                break;

            case 6:
                printf("Alteracoes finalizadas.\n");
                return;

            default:
                printf("Opcao invalida.\n");
        }
    }
}

