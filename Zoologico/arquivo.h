/************************************************
 * Nome do(a) estudante: Eduardo Pereira Xavier *
 * Trabalho Prático                             *
 * Disciplinas: Algoritmos e Programação II      *
 * Laboratório de Algoritmos e Programação II    *
 * Professores: Ivone e Ronaldo                 *
 * Data: 05/11/2025                             *
 * Versão: 1.0                                  *
 * Descrição: Cabeçalho do módulo Arquivo.      *
 * Define funções para salvar, carregar e       *
 * registrar informações em arquivos externos.  *
 *************************************************/

#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "habitat.h"
#include "animal.h"

/**
 * Salva todos os animais e habitats em um arquivo de texto.
 * Pós-condição: gera 'animais.txt' com dados do zoológico.
 */
void salvarDados(Habitat *inicio);

/**
 * Carrega os dados de animais e habitats a partir de arquivo.
 * Pós-condição: estrutura do zoológico restaurada.
 */
void carregarDados(Habitat **inicio);

/**
 * Registra uma transferência (interna ou externa) em arquivo.
 * Pós-condição: adiciona registro ao arquivo 'transferencias.txt'.
 */
void registrarTransferencia(struct animal *a, int origem, int destino, const char *tipo);

#endif
