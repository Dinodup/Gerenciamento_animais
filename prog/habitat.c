/************************************************
 * Nome do estudante: Eduardo Pereira Xavier 	*
 * Trabalho Pratico                             *
 * Disciplinas: Algoritmos e Programação II     *
 * Laboratório de Algoritmos e Programação II   *
 * Professores: Ivone e Ronaldo                 *
 * Data: 26/10/2025                             *
 * Versão: 8.0                                  *
 * Descricao: Implementacao do modulo Habitat.  *
 * Controla criacao, insercao e transferencia   *
 * de animais entre habitats do zoologico.      *
 *************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "habitat.h"
#include "animal.h"
#include "arquivo.h"

/*-------------------------------------------------------------
 * Nome: trim_inplace
 * Objetivo: Remover espacos iniciais e finais da string.
 * Entrada: s -> string modificavel.
 * Saida: nenhuma (operacao in-place).
 * Pre-condicoes:
 *      - s != NULL
 * Pos-condicoes:
 *      - string ajustada sem espacos no inicio e no fim.
 * Complexidade:
 *      - Tempo: O(n)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
static void trim_inplace(char *s) {
    char *start = s;
    char *end;
    size_t len;

    if (!s) return;

    while (*start && isspace((unsigned char)*start))
        start++;

    if (start != s)
        memmove(s, start, strlen(start) + 1);

    len = strlen(s);
    if (len == 0) return;

    end = s + len - 1;

    while (end >= s && isspace((unsigned char)*end)) {
        *end = '\0';
        end--;
    }
}

/*-------------------------------------------------------------
 * Nome: especiesIguais
 * Objetivo: Comparar duas especies ignorando caixa e espacos.
 * Entrada:
 *      - a: string da especie A
 *      - b: string da especie B
 * Saida: 1 se iguais, 0 caso contrario.
 * Pre-condicoes:
 *      - a != NULL
 *      - b != NULL
 * Pos-condicoes:
 *      - nenhuma
 * Complexidade:
 *      - Tempo: O(n)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
static int especiesIguais(const char *a, const char *b) {
    char ta[128], tb[128];
    size_t i;

    if (!a) a = "";
    if (!b) b = "";

    strncpy(ta, a, sizeof(ta) - 1);
    ta[sizeof(ta) - 1] = '\0';

    strncpy(tb, b, sizeof(tb) - 1);
    tb[sizeof(tb) - 1] = '\0';

    trim_inplace(ta);
    trim_inplace(tb);

    for (i = 0; ta[i]; i++)
        ta[i] = (char) toupper((unsigned char) ta[i]);

    for (i = 0; tb[i]; i++)
        tb[i] = (char) toupper((unsigned char) tb[i]);

    return strcmp(ta, tb) == 0;
}

/*-------------------------------------------------------------
 * Nome: inicializarHabitats
 * Objetivo: Instanciar e encadear os 20 habitats padrao.
 * Entrada: nenhuma
 * Saida: ponteiro para o primeiro habitat.
 * Pre-condicoes:
 *      - memoria suficiente disponivel
 * Pos-condicoes:
 *      - todos os habitats criados e encadeados corretamente
 * Complexidade:
 *      - Tempo: O(n)
 *      - Espaco: O(n)
 -------------------------------------------------------------*/
Habitat* inicializarHabitats() {
    Habitat *inicio = NULL, *anterior = NULL;
    int i, id = 1;

    /* 7 habitats pequenos */
    for (i = 0; i < 7; i++, id++) {
        Habitat *novo = (Habitat*) malloc(sizeof(Habitat));
        if (!novo) return inicio;

        novo->id = id;
        novo->tamanho = 1;
        novo->espaco_usado = 0;
        novo->animais = NULL;
        novo->prox = NULL;

        if (!inicio) inicio = novo;
        else anterior->prox = novo;

        anterior = novo;
    }

    /* 8 habitats medios */
    for (i = 0; i < 8; i++, id++) {
        Habitat *novo = (Habitat*) malloc(sizeof(Habitat));
        if (!novo) return inicio;

        novo->id = id;
        novo->tamanho = 10;
        novo->espaco_usado = 0;
        novo->animais = NULL;
        novo->prox = NULL;

        anterior->prox = novo;
        anterior = novo;
    }

    /* 5 habitats grandes */
    for (i = 0; i < 5; i++, id++) {
        Habitat *novo = (Habitat*) malloc(sizeof(Habitat));
        if (!novo) return inicio;

        novo->id = id;
        novo->tamanho = 20;
        novo->espaco_usado = 0;
        novo->animais = NULL;
        novo->prox = NULL;

        anterior->prox = novo;
        anterior = novo;
    }

    printf("Zoologico inicializado com 20 habitats.\n");
    return inicio;
}

/*-------------------------------------------------------------
 * Nome: listarHabitats
 * Objetivo: Exibir o status de todos os habitats.
 * Entrada: inicio -> primeiro habitat.
 * Saida: nenhuma
 * Pre-condicoes:
 *      - inicio != NULL
 * Pos-condicoes:
 *      - dados impressos na tela
 * Complexidade:
 *      - Tempo: O(n)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
void listarHabitats(Habitat *inicio) {
    Habitat *aux = inicio;

    printf("\n======= HABITATS DO ZOOLOGICO =======\n");
    while (aux) {
        printf("Habitat %d | Capacidade: %d | Ocupado: %d\n",
               aux->id, aux->tamanho, aux->espaco_usado);
        aux = aux->prox;
    }
    printf("=====================================\n");

}

/*-------------------------------------------------------------
 * Nome: escolherHabitat
 * Objetivo: Permitir que o usuario escolha um habitat valido
 *           para inserir um novo animal.
 * Entrada:
 *      - inicio: primeiro habitat
 *      - novo: animal recem-criado
 * Saida: habitat escolhido ou NULL se cancelado
 * Pre-condicoes:
 *      - inicio != NULL
 *      - novo != NULL
 * Pos-condicoes:
 *      - retorna habitat com espaco suficiente e especie valida
 * Complexidade:
 *      - Tempo: O(n)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
Habitat* escolherHabitat(Habitat *inicio, struct animal *novo) {
    Habitat *aux = inicio;
    int idEscolhido;

    if (!inicio || !novo) {
        printf("Zoologico ou animal nao inicializado.\n");
        return NULL;
    }

    while (1) {
        printf("\n======== ESCOLHA DE HABITAT =========\n");
        listarHabitats(inicio);
        printf("Informe o ID do habitat (0 para cancelar): ");

        if (scanf("%d", &idEscolhido) != 1) {
            int c;
            while ((c = getchar()) != EOF && c != '\n');
            printf("Entrada invalida.\n");
            continue;
        }
        getchar();

        if (idEscolhido == 0) {
            printf("Cadastro cancelado.\n");
            return NULL;
        }

        aux = inicio;
        while (aux && aux->id != idEscolhido)
            aux = aux->prox;

        if (!aux) {
            printf("Habitat inexistente.\n");
            continue;
        }

        if (aux->espaco_usado + novo->porte > aux->tamanho) {
            printf("Habitat sem espaco suficiente.\n");
            continue;
        }

        if (aux->animais != NULL) {
            if (!especiesIguais(aux->animais->especie, novo->especie)) {
                printf("Habitat %d possui especie diferente.\n", aux->id);
                continue;
            }
        }

        aux->espaco_usado += novo->porte;
        printf("Habitat %d selecionado.\n", aux->id);
        return aux;
    }
}

/*-------------------------------------------------------------
 * Nome: inserirAnimalNoHabitat
 * Objetivo: Inserir um animal no inicio da lista do habitat.
 * Entrada:
 *      - hab: habitat que recebera o animal
 *      - novo: animal a ser inserido
 * Saida: nenhuma
 * Pre-condicoes:
 *      - hab != NULL
 *      - novo != NULL
 * Pos-condicoes:
 *      - animal inserido na lista do habitat
 * Complexidade:
 *      - Tempo: O(1)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
void inserirAnimalNoHabitat(Habitat *hab, struct animal *novo) {
    if (!hab || !novo) return;

    novo->prox = hab->animais;
    hab->animais = novo;
    novo->h = hab;
}

/*-------------------------------------------------------------
 * Nome: transferirAnimal
 * Objetivo: Transferir um animal internamente ou externamente.
 * Entrada:
 *      - inicio: primeiro habitat
 *      - forcarTipo: modo de operacao (0 = normal)
 * Saida: nenhuma
 * Pre-condicoes:
 *      - inicio != NULL
 * Pos-condicoes:
 *      - animal movido ou removido conforme escolha
 * Complexidade:
 *      - Tempo: O(n)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
void transferirAnimal(Habitat *inicio, int forcarTipo) {
    struct animal *animal = NULL;
    struct animal *anterior = NULL;

    int id;
    int tipo = 0;
    int idDestino;

    Habitat *habOrigem = inicio;
    Habitat *destino = inicio;

    if (!inicio) {
        printf("Zoologico nao inicializado.\n");
        return;
    }

    printf("\nDigite o ID do animal: ");
    scanf("%d", &id);

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
        printf("Animal nao encontrado.\n");
        return;
    }

    if (forcarTipo == 0) {
        printf("\nTransferencia:\n1. Interna\n2. Externa\nEscolha: ");
        scanf("%d", &tipo);
    } else {
        tipo = forcarTipo;
    }

    if (tipo == 1) {
        listarHabitats(inicio);
        printf("ID do habitat destino: ");
        scanf("%d", &idDestino);

        while (destino && destino->id != idDestino)
            destino = destino->prox;

        if (!destino) {
            printf("Habitat destino inexistente.\n");
            return;
        }

        if (destino->espaco_usado + animal->porte > destino->tamanho) {
            printf("Habitat sem espaco.\n");
            return;
        }

        if (anterior) anterior->prox = animal->prox;
        else habOrigem->animais = animal->prox;

        habOrigem->espaco_usado -= animal->porte;

        inserirAnimalNoHabitat(destino, animal);
        destino->espaco_usado += animal->porte;

        printf("Animal transferido.\n");
        registrarTransferencia(animal, habOrigem->id, destino->id, "interna");

    } else if (tipo == 2) {

        if (anterior) anterior->prox = animal->prox;
        else habOrigem->animais = animal->prox;

        habOrigem->espaco_usado -= animal->porte;

        registrarTransferencia(animal, habOrigem->id, -1, "externa");
        free(animal);

        printf("Animal removido do zoologico.\n");

    } else {
        printf("Opcao invalida.\n");
    }
}

/*-------------------------------------------------------------
 * Nome: liberarHabitats
 * Objetivo: Liberar toda a memoria alocada dos habitats.
 * Entrada: inicio -> primeiro habitat
 * Saida: nenhuma
 * Pre-condicoes:
 *      - inicio != NULL
 * Pos-condicoes:
 *      - toda memoria de animais e habitats liberada
 * Complexidade:
 *      - Tempo: O(n + m)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
void liberarHabitats(Habitat *inicio) {
    Habitat *auxHab = inicio;
    Habitat *tmpHab;
    Animal *a;
    Animal *tmp;

    while (auxHab) {
        a = auxHab->animais;

        while (a) {
            tmp = a;
            a = a->prox;
            free(tmp);
        }

        tmpHab = auxHab;
        auxHab = auxHab->prox;
        free(tmpHab);
    }
}

