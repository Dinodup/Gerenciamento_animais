/************************************************
 * Nome do(a) estudante: Eduardo Pereira Xavier *
 * Trabalho Prático                             *
 * Disciplinas: Algoritmos e Programação II      *
 * Laboratório de Algoritmos e Programação II    *
 * Professores: Ivone e Ronaldo                 *
 * Data: 05/11/2025                             *
 * Versão: 1.0                                  *
 * Descrição: Cabeçalho do módulo Animal.       *
 * Define o TAD Animal e as funções de          *
 * gerenciamento de animais do zoológico.       *
 *************************************************/

#ifndef ANIMAL_H
#define ANIMAL_H

#include "habitat.h"

typedef struct animal {
    int id;
    char nome[50];
    char especie[50];
    char dieta;   /* 'C' = carnívoro, 'H' = herbívoro, 'O' = onívoro */
    char saude;   /* 'S' = saudável, 'M' = em medicação */
    int porte;    /* 'P' = 1, 'M' = 2, 'G' = 4 */
    struct habitat *h; /* ponteiro para o habitat */
    struct animal *prox;
} Animal;

/* --- Protótipos das funções públicas --- */

/**
 * Cria e cadastra um novo animal em um habitat.
 * Pré-condição: zoo inicializado.
 * Pós-condição: animal adicionado à lista do habitat.
 */
void cadastrarAnimal(Habitat *zoo);

/**
 * Busca um animal de forma recursiva pelo ID.
 * Pré-condição: zoo inicializado.
 * Pós-condição: exibe os dados do animal, se encontrado.
 */
Animal* buscarAnimalRecursivo(Habitat *inicio, int id);

/**
 * Altera os dados de um animal existente.
 * Pré-condição: zoo e animal existentes.
 * Pós-condição: dados atualizados conforme entrada do usuário.
 */
void alterarAnimal(Habitat *zoo);

/**
 * Exibe as informações de um animal.
 */
void exibirAnimal(const Animal *a);

#endif
