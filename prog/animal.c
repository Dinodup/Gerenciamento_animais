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

/*-------------------------------------------------------------
 * Nome: criarAnimalSemID
 * Objetivo:
 *      Ler os dados fornecidos pelo usuário e criar um novo
 *      registro de Animal ainda sem ID atribuído.
 * Entrada:
 *      - nenhuma (leitura via teclado)
 * Saida:
 *      - ponteiro para Animal alocado dinamicamente
 * Pre-condicoes:
 *      - nenhuma
 * Pos-condicoes:
 *      - Animal criado com atributos preenchidos e ponteiros
 *        inicializados
 * Complexidade:
 *      - Tempo: O(1)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
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

/*-------------------------------------------------------------
 * Nome: idAnimalExiste
 * Objetivo:
 *      Verificar se um determinado ID já está presente em
 *      algum dos habitats do zoológico.
 * Entrada:
 *      - inicio: ponteiro para a lista de habitats
 *      - idProcurado: ID a ser verificado
 * Saida:
 *      - 1 se o ID existe
 *      - 0 caso contrário
 * Pre-condicoes:
 *      - inicio != NULL
 * Pos-condicoes:
 *      - nenhuma
 * Complexidade:
 *      - Tempo: O(n) percorrendo todos os animais
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
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

/*-------------------------------------------------------------
 * Nome: cadastrarAnimal
 * Objetivo:
 *      Realizar o cadastro de um novo animal, garantindo ID
 *      único e inserção no habitat compatível com seu porte.
 * Entrada:
 *      - zoo: ponteiro para o primeiro habitat
 * Saida:
 *      - nenhuma
 * Pre-condicoes:
 *      - zoo != NULL
 * Pos-condicoes:
 *      - novo animal criado, validado e inserido no habitat
 *        adequado
 * Complexidade:
 *      - Tempo: O(n) devido à busca por ID e seleção de habitat
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
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

/*-------------------------------------------------------------
 * Nome: buscarRec
 * Objetivo:
 *      Buscar recursivamente um animal por ID percorrendo
 *      habitats e suas listas internas.
 * Entrada:
 *      - hab: habitat atual da recursão
 *      - id: ID procurado
 * Saida:
 *      - ponteiro para Animal se encontrado
 *      - NULL caso contrário
 * Pre-condicoes:
 *      - cadeia de habitats inicializada
 * Pos-condicoes:
 *      - nenhuma
 * Complexidade:
 *      - Tempo: O(n)
 *      - Espaco: O(n) devido à recursão
 -------------------------------------------------------------*/
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

/*-------------------------------------------------------------
 * Nome: buscarAnimalRecursivo
 * Objetivo:
 *      Encapsular e iniciar a busca recursiva por um animal
 *      através da função auxiliar buscarRec.
 * Entrada:
 *      - inicio: primeiro habitat
 *      - id: ID procurado
 * Saida:
 *      - ponteiro para Animal encontrado ou NULL
 * Pre-condicoes:
 *      - inicio != NULL
 * Pos-condicoes:
 *      - nenhuma
 * Complexidade:
 *      - Tempo: O(n)
 *      - Espaco: O(n) pela recursão
 -------------------------------------------------------------*/
Animal* buscarAnimalRecursivo(Habitat *inicio, int id) {
    return buscarRec(inicio, id);
}

/*-------------------------------------------------------------
 * Nome: exibirAnimal
 * Objetivo:
 *      Exibir no terminal todos os dados armazenados de
 *      um animal.
 * Entrada:
 *      - a: ponteiro para Animal válido
 * Saida:
 *      - nenhuma (apenas impressão)
 * Pre-condicoes:
 *      - a != NULL
 * Pos-condicoes:
 *      - informações exibidas ao usuário
 * Complexidade:
 *      - Tempo: O(1)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
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

/*-------------------------------------------------------------
 * Nome: alterarAnimal
 * Objetivo:
 *      Permitir ao usuário modificar dados cadastrados de um
 *      animal já existente, incluindo mudança de habitat.
 * Entrada:
 *      - zoo: ponteiro para a lista de habitats
 * Saida:
 *      - nenhuma
 * Pre-condicoes:
 *      - zoo != NULL
 * Pos-condicoes:
 *      - dados do animal podem ser atualizados
 * Complexidade:
 *      - Tempo: O(n) devido à busca do animal
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
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


