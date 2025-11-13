/************************************************
 * Nome do(a) estudante: Eduardo Pereira Xavier *
 * Trabalho Prático                             *
 * Disciplinas: Algoritmos e Programação II      *
 * Laboratório de Algoritmos e Programação II    *
 * Professores: Ivone e Ronaldo                 *
 * Data: 05/11/2025                             *
 * Versão: 1.0                                  *
 * Descrição: Implementação do módulo Fila.     *
 * Controla a fila sequencial de animais em     *
 * medicação no zoológico.                      *
 *************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "fila.h"
#include "animal.h"
#include "habitat.h"

/* ----------------------------------------------------------
   Cria uma nova fila de medicação.
-----------------------------------------------------------*/
FilaMed* criarFilaMed() {
    FilaMed *f = (FilaMed*) malloc(sizeof(FilaMed));
    if (!f) {
        printf("Erro ao criar fila de medicação.\n");
        exit(1);
    }
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
    return f;
}

/* ----------------------------------------------------------
   Verifica se a fila está cheia.
-----------------------------------------------------------*/
int filaCheia(FilaMed *f) {
    return f->total == MAX_FILA;
}

/* ----------------------------------------------------------
   Verifica se a fila está vazia.
-----------------------------------------------------------*/
int filaVazia(FilaMed *f) {
    return f->total == 0;
}

/* ----------------------------------------------------------
   Adiciona um animal doente à fila de medicação.
-----------------------------------------------------------*/
void adicionarFilaMed(Habitat *zoo, FilaMed *f) {
    struct animal *animal = NULL;
    struct animal *a;
    Habitat *h;
    int id;
    int r;

    if (!zoo) {
        printf("Zoológico não inicializado.\n");
        return;
    }
    if (!f) {
        printf("Fila de medicação não inicializada.\n");
        return;
    }

    if (filaCheia(f)) {
        printf("❌ A fila de medicação está cheia.\n");
        return;
    }

    printf("\nDigite o ID do animal que precisa de medicação: ");

    r = scanf("%d", &id);
    /* limpar resto da linha para evitar problemas com getchar mais adiante */
    while (getchar() != '\n') { }

    if (r != 1) {
        printf("Entrada inválida. ID deve ser um número.\n");
        return;
    }
    if (id <= 0) {
        printf("ID inválido. Use um ID positivo.\n");
        return;
    }

    /* Busca o animal no zoológico */
    animal = NULL;
    h = zoo;
    while (h && !animal) {
        a = h->animais;
        while (a) {
            if (a->id == id) {
                animal = a;
                break;
            }
            a = a->prox;
        }
        h = h->prox;
    }

    if (!animal) {
        printf("❌ Animal não encontrado (ID %d).\n", id);
        return;
    }

    if (animal->saude == 'M') {
        printf("⚠️  Animal já está em tratamento.\n");
        return;
    }

    /* Enfileirar animal */
    f->fila[f->fim] = animal;
    f->fim = (f->fim + 1) % MAX_FILA;
    f->total++;
    animal->saude = 'M';

    printf("✅ Animal '%s' (ID %d) adicionado à fila de medicação.\n", animal->nome, animal->id);
}

/* ----------------------------------------------------------
   Atende o primeiro animal da fila (dequeue).
-----------------------------------------------------------*/
void atenderAnimalFila(FilaMed *f) {
    struct animal *a;

    if (filaVazia(f)) {
        printf("❌ Nenhum animal na fila de medicação.\n");
        return;
    }

    a = f->fila[f->inicio];
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->total--;

    a->saude = 'S';
    printf("✅ Animal '%s' (ID %d) foi atendido e está saudável novamente.\n",
           a->nome, a->id);
}

/* ----------------------------------------------------------
   Lista todos os animais na fila.
-----------------------------------------------------------*/
void listarFilaMed(FilaMed *f) {
    int i, pos;
    
    if (filaVazia(f)) {
        printf("\n📭 Nenhum animal na fila de medicação.\n");
        return;
    }

    printf("\n===== FILA DE MEDICAÇÃO =====\n");
    
    pos = f->inicio;
    for (i = 0; i < f->total; i++) {
        struct animal *a = f->fila[pos];
        if (a) {
    		printf("Posição %d -> ID %d | Nome: %s | Espécie: %s | Habitat: %d\n",
           	i + 1, a->id, a->nome, a->especie, a->h ? a->h->id : -1);
		}
        pos = (pos + 1) % MAX_FILA;
    }
    printf("==============================\n");
}

/* ----------------------------------------------------------
   Enfileira animais recem chegados na fila.
-----------------------------------------------------------*/

void enfileirarAnimal(FilaMed *f, Animal *a) {
    int i, pos;
    if (!f || !a) return;
    if (filaCheia(f)) {
        printf("⚠️  Fila de medicação cheia; não foi possível enfileirar ID %d.\n", a->id);
        return;
    }
    /* evita duplicação por ID */
    pos = f->inicio;
    for (i = 0; i < f->total; i++) {
        if (f->fila[pos] && f->fila[pos]->id == a->id) {
            /* já está enfileirado */
            return;
        }
        pos = (pos + 1) % MAX_FILA;
    }
    f->fila[f->fim] = a;
    f->fim = (f->fim + 1) % MAX_FILA;
    f->total++;
}

