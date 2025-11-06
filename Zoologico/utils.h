/************************************************
 * Nome do(a) estudante: Eduardo Pereira Xavier *
 * Trabalho Prático                             *
 * Disciplinas: Algoritmos e Programação II      *
 * Laboratório de Algoritmos e Programação II    *
 * Professores: Ivone e Ronaldo                 *
 * Data: 05/11/2025                             *
 * Versão: 1.0                                  *
 * Descrição: Cabeçalho do módulo Utils.        *
 * Define funções auxiliares e limpeza de       *
 * memória do sistema do zoológico.             *
 *************************************************/

#ifndef UTILS_H
#define UTILS_H

#include "habitat.h"
#include "fila.h"

/**
 * Libera toda a memória alocada (habitats, animais e fila).
 * Pré-condição: zoo e fila inicializados.
 * Pós-condição: toda memória liberada com segurança.
 */
void liberarMemoria(Habitat *zoo, FilaMed *fila);

/**
 * Converte um caractere de porte ('P', 'M', 'G') para valor numérico.
 * Retorna: 1, 2 ou 4.
 */
int converterPorte(char porteChar);

/**
 * Converte o valor numérico do porte para string textual.
 * Ex: 1 → "Pequeno"
 */
const char* porteToString(int porte);

/**
 * Função utilitária para limpar o buffer de entrada.
 */
void limparBuffer();

#endif
