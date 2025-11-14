/************************************************
 * Nome do estudante: Eduardo Pereira Xavier 	*
 * Trabalho Pratico                             *
 * Disciplinas: Algoritmos e Programação II     *
 * Laboratório de Algoritmos e Programação II   *
 * Professores: Ivone e Ronaldo                 *
 * Data: 26/10/2025                             *
 * Versão: 8.0                                  *
 * Descricao: Implementacao do modulo Fila.     *
 * Controla a fila de medicacao de animais      *
 * utilizando estrutura sequencial circular.    *
 ************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "fila.h"
#include "animal.h"
#include "habitat.h"

/*-------------------------------------------------------------
 * Nome: criarFilaMed
 * Objetivo: Inicializar uma estrutura de fila circular para
 *           controle de animais em tratamento.
 * Entrada: nenhuma
 * Saida: ponteiro para a fila criada
 * Pre-condicoes:
 *      - memoria suficiente para alocacao
 * Pos-condicoes:
 *      - fila com indices zerados e total = 0
 * Complexidade:
 *      - Tempo: O(1)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
FilaMed* criarFilaMed() {
    FilaMed *f = (FilaMed*) malloc(sizeof(FilaMed));
    if (!f) {
        printf("Erro ao criar fila de medicacao.\n");
        exit(1);
    }
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
    return f;
}

/*-------------------------------------------------------------
 * Nome: filaCheia
 * Objetivo: Verificar se a fila atingiu seu limite de espacos.
 * Entrada:
 *      - f: fila
 * Saida: 1 se cheia, 0 caso contrario
 * Pre-condicoes:
 *      - f != NULL
 * Pos-condicoes:
 *      - nenhuma
 * Complexidade:
 *      - Tempo: O(1)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
int filaCheia(FilaMed *f) {
    return f->total == MAX_FILA;
}

/*-------------------------------------------------------------
 * Nome: filaVazia
 * Objetivo: Verificar se a fila esta sem elementos.
 * Entrada:
 *      - f: fila
 * Saida: 1 se vazia, 0 caso contrario
 * Pre-condicoes:
 *      - f != NULL
 * Pos-condicoes:
 *      - nenhuma
 * Complexidade:
 *      - Tempo: O(1)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
int filaVazia(FilaMed *f) {
    return f->total == 0;
}

/*-------------------------------------------------------------
 * Nome: adicionarFilaMed
 * Objetivo: Inserir na fila um animal que precise de medicacao.
 * Entrada:
 *      - zoo: primeiro habitat do zoologico
 *      - f: fila de medicacao
 * Saida: nenhuma
 * Pre-condicoes:
 *      - zoo != NULL
 *      - f != NULL
 * Pos-condicoes:
 *      - animal com saude = 'M'
 *      - animal inserido na fila circular
 * Complexidade:
 *      - Tempo: O(n) (busca do animal)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
void adicionarFilaMed(Habitat *zoo, FilaMed *f) {
    struct animal *animal = NULL;
    struct animal *a;
    Habitat *h;
    int id;
    int r;

    if (!zoo) {
        printf("Zoologico nao inicializado.\n");
        return;
    }
    if (!f) {
        printf("Fila de medicacao nao inicializada.\n");
        return;
    }

    if (filaCheia(f)) {
        printf("Fila de medicacao cheia.\n");
        return;
    }

    printf("Digite o ID do animal que precisa de medicacao: ");
    r = scanf("%d", &id);

    while (getchar() != '\n') { }

    if (r != 1) {
        printf("Entrada invalida. ID deve ser numerico.\n");
        return;
    }
    if (id <= 0) {
        printf("ID invalido. Use um ID positivo.\n");
        return;
    }

    /* busca do animal */
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
        printf("Animal nao encontrado (ID %d).\n", id);
        return;
    }

    if (animal->saude == 'M') {
        printf("Animal ja esta em tratamento.\n");
        return;
    }

    f->fila[f->fim] = animal;
    f->fim = (f->fim + 1) % MAX_FILA;
    f->total++;
    animal->saude = 'M';

    printf("Animal '%s' (ID %d) adicionado a fila.\n", animal->nome, animal->id);
}

/*-------------------------------------------------------------
 * Nome: atenderAnimalFila
 * Objetivo: Remover o primeiro animal da fila, marcando-o
 *           como tratado.
 * Entrada:
 *      - f: fila de medicacao
 * Saida: nenhuma
 * Pre-condicoes:
 *      - f != NULL
 * Pos-condicoes:
 *      - saude do animal = 'S'
 *      - fila reduzida em um elemento
 * Complexidade:
 *      - Tempo: O(1)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
void atenderAnimalFila(FilaMed *f) {
    struct animal *a;

    if (filaVazia(f)) {
        printf("Nenhum animal na fila de medicacao.\n");
        return;
    }

    a = f->fila[f->inicio];
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->total--;

    a->saude = 'S';
    printf("Animal '%s' (ID %d) tratado com sucesso.\n", a->nome, a->id);
}

/*-------------------------------------------------------------
 * Nome: listarFilaMed
 * Objetivo: Exibir todos os animais presentes na fila.
 * Entrada:
 *      - f: fila de medicacao
 * Saida: nenhuma
 * Pre-condicoes:
 *      - f != NULL
 * Pos-condicoes:
 *      - dados exibidos na tela
 * Complexidade:
 *      - Tempo: O(n)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
void listarFilaMed(FilaMed *f) {
    int i, pos;

    if (filaVazia(f)) {
        printf("Nenhum animal na fila de medicacao.\n");
        return;
    }
    printf("========= FILA DE MEDICACAO =========\n");

    pos = f->inicio;
    for (i = 0; i < f->total; i++) {
        struct animal *a = f->fila[pos];
        if (a) {
            printf("Posicao %d -> ID %d | Nome: %s | Especie: %s | Habitat: %d\n",
                   i + 1, a->id, a->nome, a->especie, a->h ? a->h->id : -1);
        }
        pos = (pos + 1) % MAX_FILA;
    }

    printf("=====================================\n");
}

/*-------------------------------------------------------------
 * Nome: enfileirarAnimal
 * Objetivo: Inserir um animal diretamente na fila, evitando
 *           duplicacao por ID.
 * Entrada:
 *      - f: fila
 *      - a: animal a ser inserido
 * Saida: nenhuma
 * Pre-condicoes:
 *      - f != NULL
 *      - a != NULL
 * Pos-condicoes:
 *      - animal enfileirado se houver espaco e nao estiver duplicado
 * Complexidade:
 *      - Tempo: O(n)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
void enfileirarAnimal(FilaMed *f, Animal *a) {
    int i, pos;

    if (!f || !a) return;

    if (filaCheia(f)) {
        printf("Fila cheia; nao foi possivel enfileirar ID %d.\n", a->id);
        return;
    }

    pos = f->inicio;
    for (i = 0; i < f->total; i++) {
        if (f->fila[pos] && f->fila[pos]->id == a->id) {
            return;
        }
        pos = (pos + 1) % MAX_FILA;
    }

    f->fila[f->fim] = a;
    f->fim = (f->fim + 1) % MAX_FILA;
    f->total++;
}

