/************************************************
 * Nome do(a) estudante: Eduardo Pereira Xavier *
 * Trabalho Prático                             *
 * Disciplinas: Algoritmos e Programação II      *
 * Laboratório de Algoritmos e Programação II    *
 * Professores: Ivone e Ronaldo                 *
 * Data: 05/11/2025                             *
 * Versão: 1.0                                  *
 * Descrição: Implementação do módulo Animal.   *
 * Contém funções para cadastro, busca,         *
 * alteração e remoção de animais.              *
 *************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "animal.h"
#include "habitat.h"
#include "utils.h"

/* ----------------------------------------------------------
   Função auxiliar: cria um novo nó de animal na memória.
-----------------------------------------------------------*/
static Animal* criarAnimal() {
    Animal *novo = (Animal*) malloc(sizeof(Animal));
    if (!novo) {
        printf("Erro de alocação de memória.\n");
        exit(1);
    }

    printf("\n=== CADASTRO DE NOVO ANIMAL ===\n");
    printf("ID do animal: ");
    scanf("%d", &novo->id);
    getchar();

    printf("Nome: ");
    fgets(novo->nome, 50, stdin);
    novo->nome[strcspn(novo->nome, "\n")] = '\0';

    printf("Espécie: ");
    fgets(novo->especie, 50, stdin);
    novo->especie[strcspn(novo->especie, "\n")] = '\0';

    printf("Dieta (C - Carnívoro, H - Herbívoro, O - Onívoro): ");
    scanf(" %c", &novo->dieta);

    printf("Estado de saúde (S - Saudável, M - Medicação): ");
    scanf(" %c", &novo->saude);

    printf("Porte (P - Pequeno, M - Médio, G - Grande): ");
    char porteChar;
    scanf(" %c", &porteChar);

    switch (porteChar) {
        case 'P': case 'p': novo->porte = 1; break;
        case 'M': case 'm': novo->porte = 2; break;
        case 'G': case 'g': novo->porte = 4; break;
        default: novo->porte = 1;
    }

    novo->prox = NULL;
    novo->h = NULL;
    return novo;
}

/* ----------------------------------------------------------
   Cadastra um novo animal no zoológico.
-----------------------------------------------------------*/
void cadastrarAnimal(Habitat *zoo) {
    if (!zoo) {
        printf("Zoológico não inicializado.\n");
        return;
    }

    Animal *novo = criarAnimal();
    Habitat *hab = escolherHabitat(zoo, novo->porte);

    if (!hab) {
        printf("\n❌ Nenhum habitat disponível para esse porte.\n");
        free(novo);
        return;
    }

    inserirAnimalNoHabitat(hab, novo);
    printf("\n✅ Animal cadastrado com sucesso no habitat %d!\n", hab->id);
}

/* ----------------------------------------------------------
   Busca recursiva de animal por ID.
-----------------------------------------------------------*/
static Animal* buscarRec(Habitat *hab, int id) {
    if (!hab) return NULL;
    Animal *a = hab->animais;

    while (a != NULL) {
        if (a->id == id) return a;
        a = a->prox;
    }

    return buscarRec(hab->prox, id);
}

Animal* buscarAnimalRecursivo(Habitat *zoo) {
    if (!zoo) {
        printf("Zoológico não inicializado.\n");
        return NULL;
    }

    int id;
    printf("\nDigite o ID do animal para busca: ");
    scanf("%d", &id);

    Animal *encontrado = buscarRec(zoo, id);
    if (encontrado)
        exibirAnimal(encontrado);
    else
        printf("\n❌ Animal com ID %d não encontrado.\n", id);

    return encontrado;
}

/* ----------------------------------------------------------
   Exibe informações do animal.
-----------------------------------------------------------*/
void exibirAnimal(const Animal *a) {
    if (!a) return;
    printf("\n===== DADOS DO ANIMAL =====\n");
    printf("ID: %d\n", a->id);
    printf("Nome: %s\n", a->nome);
    printf("Espécie: %s\n", a->especie);
    printf("Dieta: %c\n", a->dieta);
    printf("Saúde: %c\n", a->saude);
    printf("Porte: %d\n", a->porte);
    if (a->h)
        printf("Habitat atual: %d\n", a->h->id);
    else
        printf("Habitat atual: N/A\n");
    printf("===========================\n");
}

/* ----------------------------------------------------------
   Altera informações de um animal.
-----------------------------------------------------------*/
void alterarAnimal(Habitat *zoo) {
    Animal *a = buscarAnimalRecursivo(zoo);
    if (!a) return;

    printf("\n=== ALTERAR DADOS DO ANIMAL ===\n");
    printf("1. Nome\n2. Espécie\n3. Dieta\n4. Saúde\n5. Cancelar\n");
    printf("Escolha: ");
    int op;
    scanf("%d", &op);
    getchar();

    switch(op) {
        case 1:
            printf("Novo nome: ");
            fgets(a->nome, 50, stdin);
            a->nome[strcspn(a->nome, "\n")] = '\0';
            break;
        case 2:
            printf("Nova espécie: ");
            fgets(a->especie, 50, stdin);
            a->especie[strcspn(a->especie, "\n")] = '\0';
            break;
        case 3:
            printf("Nova dieta (C/H/O): ");
            scanf(" %c", &a->dieta);
            break;
        case 4:
            printf("Novo estado de saúde (S/M): ");
            scanf(" %c", &a->saude);
            break;
        default:
            printf("Alteração cancelada.\n");
            return;
    }

    printf("\n✅ Dados atualizados com sucesso!\n");
}

/* ----------------------------------------------------------
   Remove animal do zoológico.
-----------------------------------------------------------*/
void removerAnimal(Habitat *zoo) {
    int id;
    printf("\nDigite o ID do animal a remover: ");
    scanf("%d", &id);

    Habitat *habAtual = zoo;
    while (habAtual) {
        Animal *a = habAtual->animais;
        Animal *ant = NULL;

        while (a) {
            if (a->id == id) {
                if (ant) ant->prox = a->prox;
                else habAtual->animais = a->prox;
                habAtual->espaco_usado -= a->porte;
                printf("\n✅ Animal removido do habitat %d.\n", habAtual->id);
                free(a);
                return;
            }
            ant = a;
            a = a->prox;
        }
        habAtual = habAtual->prox;
    }

    printf("\n❌ Animal não encontrado.\n");
}