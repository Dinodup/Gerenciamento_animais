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
   Função auxiliar: cria um novo nó de animal na memória (sem ID)
-----------------------------------------------------------*/
static Animal* criarAnimalSemID() {
    Animal *novo;
    char porteChar;

    novo = (Animal*) malloc(sizeof(Animal));
    if (!novo) {
        printf("Erro de alocação de memória.\n");
        exit(1);
    }

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
   Verifica se já existe um animal com o mesmo ID em qualquer habitat
-----------------------------------------------------------*/
int idAnimalExiste(Habitat *inicio, int idProcurado) {
    Habitat *h = inicio;
    while (h != NULL) {
        Animal *a = h->animais;
        while (a != NULL) {
            if (a->id == idProcurado)
                return 1; /* ID duplicado encontrado */
            a = a->prox;
        }
        h = h->prox;
    }
    return 0; /* ID não encontrado */
}

/* ----------------------------------------------------------
   Cadastra um novo animal no zoológico, impedindo ID duplicado
-----------------------------------------------------------*/
void cadastrarAnimal(Habitat *zoo) {
    Animal *novo;
    Habitat *hab;
    int id;

    if (!zoo) {
        printf("Zoológico não inicializado.\n");
        return;
    }

    printf("\n=== CADASTRO DE NOVO ANIMAL ===\n");

    /* Leitura e verificação de ID duplicado */
    while (1) {
        printf("ID do animal: ");
        scanf("%d", &id);
        getchar();

        if (idAnimalExiste(zoo, id)) {
            printf("❌ Já existe um animal com o ID %d!\n", id);
        } else {
            break;  /* ID válido, prosseguir */
        }
    }

    /* Criar e preencher os outros dados */
    novo = criarAnimalSemID();
    novo->id = id;

    hab = escolherHabitat(zoo, novo);

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
    Animal *a;

    if (!hab) return NULL;
    a = hab->animais;

    while (a != NULL) {
        if (a->id == id) return a;
        a = a->prox;
    }

    return buscarRec(hab->prox, id);
}

Animal* buscarAnimalRecursivo(Habitat *inicio, int id) {
    return buscarRec(inicio, id); /* Ligação entre as duas */
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
    Animal *a;
    int op;
    int id;

    printf("\nDigite o ID do animal que deseja alterar: ");
    scanf("%d", &id);

    a = buscarAnimalRecursivo(zoo, id);
    if (!a) {
        printf("❌ Animal não encontrado.\n");
        return;
    }

    while (1) {
        printf("\n=== ALTERAR DADOS DO ANIMAL ===\n");
        printf("1. Nome\n");
        printf("2. Espécie\n");
        printf("3. Dieta\n");
        printf("4. Saúde\n");
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
                printf("✅ Nome atualizado!\n");
                break;
            case 2:
                printf("Nova espécie: ");
                fgets(a->especie, 50, stdin);
                a->especie[strcspn(a->especie, "\n")] = '\0';
                printf("✅ Espécie atualizada!\n");
                break;
            case 3:
                printf("Nova dieta (C/H/O): ");
                scanf(" %c", &a->dieta);
                printf("✅ Dieta atualizada!\n");
                break;
            case 4:
                printf("Novo estado de saúde (S/M): ");
                scanf(" %c", &a->saude);
                printf("✅ Estado de saúde atualizado!\n");
                break;
            case 5:
			    printf("\n=== ALTERAR HABITAT ===\n");
			    transferirAnimal(zoo, 1);
			    break;
            case 6:
                printf("🔙 Alterações finalizadas.\n");
                return;  /* Sai da função */
            default:
                printf("❌ Opção inválida.\n");
        }
    }
}

/* ----------------------------------------------------------
   Remove animal do zoológico.
-----------------------------------------------------------*/
void removerAnimal(Habitat *zoo) {
    int id;
    Habitat *habAtual;
    Animal *a;
    Animal *ant;

    printf("\nDigite o ID do animal a remover: ");
    scanf("%d", &id);

    habAtual = zoo;
    while (habAtual) {
        a = habAtual->animais;
        ant = NULL;

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
