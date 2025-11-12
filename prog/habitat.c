/************************************************
 * Nome do(a) estudante: Eduardo Pereira Xavier *
 * Trabalho Prático                             *
 * Disciplinas: Algoritmos e Programação II     *
 * Professores: Ivone e Ronaldo                 *
 * Data: 05/11/2025                             *
 * Versão: 1.0                                  *
 * Descrição: Implementação do módulo Habitat.  *
 * Controla criação, inserção e transferência   *
 * de animais entre habitats.                   *
 *************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "habitat.h"
#include "animal.h"
#include "arquivo.h"

/* ----------------------------------------------------------
   Funções auxiliares locais
-----------------------------------------------------------*/

/* Remove espaços em branco iniciais e finais in-place */
static void trim_inplace(char *s) {
    char *start = s;
    char *end;
    size_t len;

    if (!s) return;

    /* pular espaços iniciais */
    while (*start && isspace((unsigned char)*start)) start++;

    if (start != s) memmove(s, start, strlen(start) + 1);

    len = strlen(s);
    if (len == 0) return;

    /* remover espaços finais */
    end = s + len - 1;
    while (end >= s && isspace((unsigned char)*end)) {
        *end = '\0';
        end--;
    }
}

/* Compara espécies de forma case-insensitive e ignorando espaços nas extremidades */
static int especiesIguais(const char *a, const char *b) {
    char ta[128], tb[128];
    size_t i;

    if (!a) a = "";
    if (!b) b = "";

    /* copiar para buffers locais e garantir terminação */
    strncpy(ta, a, sizeof(ta)-1); ta[sizeof(ta)-1] = '\0';
    strncpy(tb, b, sizeof(tb)-1); tb[sizeof(tb)-1] = '\0';

    /* trim in-place */
    trim_inplace(ta);
    trim_inplace(tb);

    /* transformar para maiúsculas */
    for (i = 0; ta[i]; ++i) ta[i] = (char) toupper((unsigned char)ta[i]);
    for (i = 0; tb[i]; ++i) tb[i] = (char) toupper((unsigned char)tb[i]);

    return strcmp(ta, tb) == 0;
}

/* ----------------------------------------------------------
   Cria e inicializa os 20 habitats do zoológico.
-----------------------------------------------------------*/
Habitat* inicializarHabitats() {
    Habitat *inicio = NULL, *anterior = NULL;

    int i;
    int id = 1;

    /* 7 pequenos (tamanho = 1) */
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

    /* 8 médios (tamanho = 10) */
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

    /* 5 grandes (tamanho = 20) */
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
   Escolhe um habitat disponível de acordo com o porte e espécie.
   Recebe o ponteiro para o animal recém-criado (struct animal *novo).
-----------------------------------------------------------*/
Habitat* escolherHabitat(Habitat *inicio, struct animal *novo) {
    Habitat *aux = inicio;
    int idEscolhido;

    if (!inicio || !novo) {
        printf("Zoológico ou animal não inicializado.\n");
        return NULL;
    }

    while (1) {
        printf("\n===== ESCOLHA DE HABITAT =====\n");
        listarHabitats(inicio);
        printf("Informe o ID do habitat desejado (0 p/ cancelar): ");
        if (scanf("%d", &idEscolhido) != 1) {
            int c;
            while ((c = getchar()) != EOF && c != '\n') {}
            printf("Entrada inválida. Tente novamente.\n");
            continue;
        }
        getchar(); /* limpar \n */

        if (idEscolhido == 0) {
            printf("Cadastro cancelado pelo usuário.\n");
            return NULL;
        }

        /* Busca o habitat pelo ID informado */
        aux = inicio;
        while (aux && aux->id != idEscolhido)
            aux = aux->prox;

        if (!aux) {
            printf("❌ Habitat inexistente. Tente novamente.\n");
            continue;
        }

        if ((aux->espaco_usado + novo->porte) > aux->tamanho) {
            printf("❌ O habitat %d não possui espaço suficiente. Escolha outro.\n", aux->id);
            continue;
        }

        /* Verifica a espécie do último animal inserido no habitat (cabeça da lista) */
		if (aux->animais != NULL) {
    		/* aux->animais é o último animal inserido (inserção no início) */
    		if (!especiesIguais(aux->animais->especie, novo->especie)) {
        		printf("❌ O habitat %d já possui animais de outra espécie. ", aux->id);
        		printf("Espécie presente (último inserido): %s. Escolha outro habitat.\n", aux->animais->especie);
        		continue;
    		}
		}

        /* Se passou em todas as verificações, escolha aceita */
        aux->espaco_usado += novo->porte;
        printf("✅ Habitat %d selecionado com sucesso!\n", aux->id);
        return aux;
    }
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
void transferirAnimal(Habitat *inicio, int forcarTipo) {
    struct animal *animal = NULL;
    struct animal *anterior = NULL;
    int id;
    int tipo = 0;
    int idDestino;
    Habitat *habOrigem = inicio;
    Habitat *destino = inicio;

    if (!inicio) {
        printf("Zoológico não inicializado.\n");
        return;
    }

    printf("\nDigite o ID do animal a transferir: ");
    scanf("%d", &id);

    /* Encontrar o animal e seu habitat de origem */
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

    if (forcarTipo == 0) {
    	/* Execução normal — usuário escolhe o tipo */
	    printf("\nTransferência:\n1. Interna (para outro habitat)\n2. Externa (retirar do zoológico)\nEscolha: ");
	    scanf("%d", &tipo);
	} else {
	    /* Chamada forçada — pula o menu e usa o tipo definido */
	    tipo = forcarTipo;
	}

    if (tipo == 1) {
        listarHabitats(inicio);
        printf("Digite o ID do habitat de destino: ");
        scanf("%d", &idDestino);

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

        /* Remover do habitat atual */
        if (anterior) anterior->prox = animal->prox;
        else habOrigem->animais = animal->prox;

        habOrigem->espaco_usado -= animal->porte;

        /* Inserir no destino */
        inserirAnimalNoHabitat(destino, animal);
        destino->espaco_usado += animal->porte;

        printf("\n✅ Animal transferido do habitat %d para o %d.\n", habOrigem->id, destino->id);
        registrarTransferencia(animal, habOrigem->id, destino->id, "interna");

    } else if (tipo == 2) {
        /* Remover definitivamente */
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
    Habitat *auxHab;
    Habitat *tmpHab;
    Animal *a;
    Animal *tmp;

    auxHab = inicio;

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

