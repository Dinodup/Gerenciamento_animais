/************************************************
 * Nome do(a) estudante: Eduardo Pereira Xavier *
 * Trabalho Prático                             *
 * Disciplinas: Algoritmos e Programação II      *
 * Laboratório de Algoritmos e Programação II    *
 * Professores: Ivone e Ronaldo                 *
 * Data: 05/11/2025                             *
 * Versão: 1.0                                  *
 * Descrição: Cabeçalho do módulo Fila.         *
 * Define o TAD FilaMed e funções para controle *
 * de animais em medicação no zoológico.        *
 *************************************************/

#ifndef FILA_H
#define FILA_H

#include "animal.h"
#include "habitat.h"

#define MAX_FILA 50

typedef struct {
    struct animal* fila[MAX_FILA];
    int inicio;
    int fim;
    int total;
} FilaMed;

/* --- Protótipos --- */

/**
 * Cria e inicializa uma fila de medicação vazia.
 * Pós-condição: fila com total = 0, início = 0, fim = 0.
 */
FilaMed* criarFilaMed();

/**
 * Verifica se a fila está cheia.
 */
int filaCheia(FilaMed *f);

/**
 * Verifica se a fila está vazia.
 */
int filaVazia(FilaMed *f);

/**
 * Adiciona um animal doente na fila de medicação.
 * Pré-condição: zoo inicializado.
 * Pós-condição: animal adicionado à fila, estado 'M'.
 */
void adicionarFilaMed(Habitat *zoo, FilaMed *f);

/**
 * Atende o primeiro animal da fila.
 * Pós-condição: animal removido da fila, estado 'S'.
 */
void atenderAnimalFila(FilaMed *f);

/**
 * Lista todos os animais atualmente na fila de medicação.
 */
void listarFilaMed(FilaMed *f);

#endif
s