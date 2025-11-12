/************************************************
 * Nome do(a) estudante: Eduardo Pereira Xavier *
 * Trabalho Prático                             *
 * Disciplinas: Algoritmos e Programação II      *
 * Laboratório de Algoritmos e Programação II    *
 * Professores: Ivone e Ronaldo                 *
 * Data: 05/11/2025                             *
 * Versão: 1.0                                  *
 * Descrição: Cabeçalho do módulo Habitat.      *
 * Define o TAD Habitat e funções de controle   *
 * e inserção de animais em habitats.           *
 *************************************************/

#ifndef HABITAT_H
#define HABITAT_H

#include <stdio.h>
#include <stdlib.h>

/* Estrutura de Habitat */
typedef struct habitat {
    int id;                  /* identificador do habitat */
    int tamanho;             /* capacidade total (1, 10 ou 20) */
    int espaco_usado;        /* espaço atual ocupado */
    struct animal *animais;  /* lista encadeada de animais */
    struct habitat *prox;    /* próximo habitat */
} Habitat;

/* --- Protótipos --- */

/**
 * Inicializa a lista encadeada de habitats.
 * Cria 7 pequenos, 8 médios e 5 grandes (total 20).
 * Pré-condição: nenhuma.
 * Pós-condição: lista de habitats criada.
 */
Habitat* inicializarHabitats();

/**
 * Exibe um resumo dos habitats disponíveis.
 */
void listarHabitats(Habitat *inicio);

/**
 * Escolhe o habitat mais adequado para um animal,
 * de acordo com o porte e espaço disponível.
 * Retorna ponteiro para o habitat escolhido ou NULL.
 */
Habitat* escolherHabitat(Habitat *inicio, struct animal *novo);

/**
 * Insere um animal em um habitat específico.
 * Pré-condição: habitat válido e espaço suficiente.
 * Pós-condição: animal adicionado à lista do habitat.
 */
void inserirAnimalNoHabitat(Habitat *hab, struct animal *novo);

/**
 * Transfere um animal de um habitat para outro ou para fora do zoológico.
 * Pré-condição: habitats inicializados e animal existente.
 * Pós-condição: transferência realizada e registrada.
 */
void transferirAnimal(Habitat *inicio, int forcarTipo);

/**
 * Libera toda a memória alocada pelos habitats e animais.
 */
void liberarHabitats(Habitat *inicio);

#endif
