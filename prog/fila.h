/************************************************
 * Nome do estudante: Eduardo Pereira Xavier 	*
 * Trabalho Pratico                             *
 * Disciplinas: Algoritmos e Programação II     *
 * Laboratório de Algoritmos e Programação II   *
 * Professores: Ivone e Ronaldo                 *
 * Data: 26/10/2025                             *
 * Versão: 8.0                                  *
 * Descricao: Cabecalho do modulo Fila.         *
 * Define o TAD FilaMed e funcoes para controle *
 * da fila de medicacao de animais.             *
 ************************************************/

#ifndef FILA_H
#define FILA_H

#include "animal.h"
#include "habitat.h"

#define MAX_FILA 50

/*-------------------------------------------------------------
 * Nome: Estrutura FilaMed
 * Objetivo: Representar uma fila circular contendo ponteiros
 *           para animais em processo de medicacao.
 * Campos:
 *      - fila: arranjo de ponteiros para animais
 *      - inicio: indice do primeiro elemento
 *      - fim: indice do proximo espaco livre
 *      - total: quantidade atual de elementos
 -------------------------------------------------------------*/
typedef struct {
    struct animal *fila[MAX_FILA];
    int inicio;
    int fim;
    int total;
} FilaMed;

/*-------------------------------------------------------------
 * Nome: criarFilaMed
 * Objetivo: Criar e inicializar uma fila circular vazia.
 * Entrada: nenhuma
 * Saida: ponteiro para a fila criada
 * Pre-condicoes:
 *      - memoria suficiente para alocacao
 * Pos-condicoes:
 *      - fila inicializada (inicio = 0, fim = 0, total = 0)
 * Complexidade:
 *      - Tempo: O(1)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
FilaMed* criarFilaMed();

/*-------------------------------------------------------------
 * Nome: filaCheia
 * Objetivo: Verificar se a fila atingiu sua capacidade maxima.
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
int filaCheia(FilaMed *f);

/*-------------------------------------------------------------
 * Nome: filaVazia
 * Objetivo: Verificar se a fila esta vazia.
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
int filaVazia(FilaMed *f);

/*-------------------------------------------------------------
 * Nome: adicionarFilaMed
 * Objetivo: Inserir um animal que precise de tratamento na fila.
 * Entrada:
 *      - zoo: primeiro habitat
 *      - f: fila de medicacao
 * Saida: nenhuma
 * Pre-condicoes:
 *      - zoo != NULL
 *      - f != NULL
 * Pos-condicoes:
 *      - animal passa para estado 'M'
 *      - elemento adicionado na fila circular
 * Complexidade:
 *      - Tempo: O(n) (busca do animal)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
void adicionarFilaMed(Habitat *zoo, FilaMed *f);

/*-------------------------------------------------------------
 * Nome: atenderAnimalFila
 * Objetivo: Remover o primeiro animal da fila, marcando-o como
 *           tratado.
 * Entrada:
 *      - f: fila
 * Saida: nenhuma
 * Pre-condicoes:
 *      - f != NULL
 * Pos-condicoes:
 *      - fila reduzida
 *      - animal passa para estado 'S'
 * Complexidade:
 *      - Tempo: O(1)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
void atenderAnimalFila(FilaMed *f);

/*-------------------------------------------------------------
 * Nome: listarFilaMed
 * Objetivo: Exibir todos os animais na fila de medicacao.
 * Entrada:
 *      - f: fila
 * Saida: nenhuma
 * Pre-condicoes:
 *      - f != NULL
 * Pos-condicoes:
 *      - informacoes exibidas na tela
 * Complexidade:
 *      - Tempo: O(n)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
void listarFilaMed(FilaMed *f);

/*-------------------------------------------------------------
 * Nome: enfileirarAnimal
 * Objetivo: Inserir um animal na fila sem permitir duplicacao
 *           por ID.
 * Entrada:
 *      - f: fila
 *      - a: animal
 * Saida: nenhuma
 * Pre-condicoes:
 *      - f != NULL
 *      - a != NULL
 * Pos-condicoes:
 *      - animal inserido caso haja espaco e nao esteja duplicado
 * Complexidade:
 *      - Tempo: O(n)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
void enfileirarAnimal(FilaMed *f, Animal *a);

#endif

