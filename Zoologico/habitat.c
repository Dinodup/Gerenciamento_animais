/************************************************
 * Nome do(a) estudante: Eduardo Pereira Xavier *
 * Trabalho Prático                             *
 * Disciplinas: Algoritmos e Programação II      *
 * Laboratório de Algoritmos e Programação II    *
 * Professores: Ivone e Ronaldo                 *
 * Data: 05/11/2025                             *
 * Versão: 1.0                                  *
 * Descrição: Implementação do módulo Habitat.  *
 * Controla criação, inserção e transferência   *
 * de animais entre habitats.                   *
 *************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "habitat.h"
#include "animal.h"
#include "arquivo.h"

/* ----------------------------------------------------------
   Cria e inicializa os 20 habitats do zoológico.
-----------------------------------------------------------*/
Habitat* inicializarHabitats() {
    Habitat *inicio = NULL, *anterior = NULL;

    int i;
    int id = 1;

    // 7 pequenos (tamanho = 1)
    for (i = 0; i < 7; i++, id++) {
        Habitat *novo = (Habitat*) malloc(sizeof(Habitat));
        novo->id = id;
        novo->tamanho = 1;
        novo->espaco_usado = 0;
        novo->animais = NULL;
        novo->prox = NULL;

        if (!inicio)
            inicio = novo;
        else
            anterior->prox = novo;

        anterior = novo;
    }

    // 8 médios (tamanho = 10)
    for (i = 0; i < 8; i++, id++) {
        Habitat *novo = (Habitat*) malloc(sizeof(Habitat));
        novo->id = id;
        novo->tamanho = 10;
        novo->espaco_usado = 0;
        novo->animais = NULL;
        novo->prox = NULL;
        anterior->prox = novo;
        anterior = novo;
    }

    // 5 grandes (tamanho = 20)
    for (i = 0; i < 5; i++, id++) {
        Habitat *novo = (Habitat*) malloc(sizeof(Habitat));
        novo->id = id;
        novo->tamanho = 20;
        novo->espaco_usado = 0;
        novo->animais = NULL;
        novo->prox = NULL;
        anterior->prox = novo;
        anterior = novo;
    }

    printf("\n✅ Zoológico inicializado com 20 habitats.\n");
    return inicio;
}

/* ----------------------------------------------------------
   Exibe todos os habitats e seu status de ocupação.
-----------------------------------------------------------*/
void listarHabitats(Habitat *inicio) {
    Habitat *aux = inicio;
    printf("\n===== HABITATS DO ZOOLÓGICO =====\n");
    while (aux) {
        printf("Habitat %d | Capacidade: %d | Ocupado: %d\n",
               aux->id, aux->tamanho, aux->espaco_usado);
        aux = aux->prox;
    }
    printf("=================================\n");
}

/* ----------------------------------------------------------
   Escolhe um habitat disponível de acordo com o porte.
-----------------------------------------------------------*/
Habitat* escolherHabitat(Habitat *inicio, int porte) {
    Habitat *aux = inicio;

    while (aux) {
        if ((aux->espaco_usado + porte) <= aux->tamanho) {
            aux->espaco_usado += porte;
            return aux;
        }
        aux = aux->prox;
    }
    return NULL;
}

/* ----------------------------------------------------------
   Insere animal no habitat (início da lista).
-----------------------------------------------------------*/
void inserirAnimalNoHabitat(Habitat *hab, struct animal *novo) {
    if (!hab || !novo) return;

    novo->prox = hab->animais;
    hab->animais = novo;
    novo->h = hab;
}

/* ----------------------------------------------------------
   Transfere animal internamente ou externamente.
-----------------------------------------------------------*/
void transferirAnimal(Habitat *inicio) {
    if (!inicio) {
        printf("Zoológico não inicializado.\n");
        return;
    }

    int id;
    printf("\nDigite o ID do animal a transferir: ");
    scanf("%d", &id);

    Habitat *habOrigem = inicio;
    struct animal *animal = NULL;
    struct animal *anterior = NULL;

    // Encontrar o animal e seu habitat de origem
    while (habOrigem) {
        struct animal *a = habOrigem->animais;
        anterior = NULL;

        while (a) {
            if (a->id == id) {
                animal = a;
                break;
            }
            anterior = a;
            a = a->prox;
        }

        if (animal) break;
        habOrigem = habOrigem->prox;
    }

    if (!animal) {
        printf("❌ Animal não encontrado.\n");
        return;
    }

    printf("\nTransferência:\n1. Interna (para outro habitat)\n2. Externa (retirar do zoológico)\nEscolha: ");
    int tipo;
    scanf("%d", &tipo);

    if (tipo == 1) {
        listarHabitats(inicio);
        printf("Digite o ID do habitat de destino: ");
        int idDestino;
        scanf("%d", &idDestino);

        Habitat *destino = inicio;
        while (destino && destino->id != idDestino)
            destino = destino->prox;

        if (!destino) {
            printf("❌ Habitat de destino inexistente.\n");
            return;
        }

        if ((destino->espaco_usado + animal->porte) > destino->tamanho) {
            printf("❌ Habitat sem espaço disponível.\n");
            return;
        }

        // Remover do habitat atual
        if (anterior) anterior->prox = animal->prox;
        else habOrigem->animais = animal->prox;

        habOrigem->espaco_usado -= animal->porte;

        // Inserir no destino
        inserirAnimalNoHabitat(destino, animal);
        destino->espaco_usado += animal->porte;

        printf("\n✅ Animal transferido do habitat %d para o %d.\n", habOrigem->id, destino->id);
        registrarTransferencia(animal, habOrigem->id, destino->id, "interna");

    } else if (tipo == 2) {
        // Remover definitivamente
        if (anterior) anterior->prox = animal->prox;
        else habOrigem->animais = animal->prox;
        habOrigem->espaco_usado -= animal->porte;

        registrarTransferencia(animal, habOrigem->id, -1, "externa");
        free(animal);
        printf("\n✅ Animal removido do zoológico (transferência externa).\n");

    } else {
        printf("❌ Opção inválida.\n");
    }
}

/* ----------------------------------------------------------
   Libera toda a memória dos habitats e seus animais.
-----------------------------------------------------------*/
void liberarHabitats(Habitat *inicio) {
    Habitat *auxHab = inicio;
    while (auxHab) {
        struct animal *a = auxHab->animais;
        while (a) {
            struct animal *tmp = a;
            a = a->prox;
            free(tmp);
        }
        Habitat *tmpHab = auxHab;
        auxHab = auxHab->prox;
        free(tmpHab);
    }
}
