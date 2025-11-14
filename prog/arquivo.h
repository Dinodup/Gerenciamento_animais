/************************************************
 * Nome do estudante: Eduardo Pereira Xavier 	*
 * Trabalho Pratico                             *
 * Disciplinas: Algoritmos e Programação II     *
 * Laboratório de Algoritmos e Programação II   *
 * Professores: Ivone e Ronaldo                 *
 * Data: 26/10/2025                             *
 * Versão: 8.0                                  *
 * Descricao: Cabecalho do modulo Arquivo.      *
 * Define funcoes para leitura, escrita e       *
 * registro de eventos em arquivos externos.    *
 ************************************************/

#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "habitat.h"
#include "animal.h"

/*-------------------------------------------------------------
 * Nome: salvarDados
 * Objetivo: Gravar em "animais.txt" todas as informacoes dos
 *           habitats e seus animais.
 * Entrada:
 *      - inicio: ponteiro para o primeiro habitat
 * Saida: nenhuma
 * Pre-condicoes:
 *      - inicio != NULL
 * Pos-condicoes:
 *      - arquivo "animais.txt" gerado ou sobrescrito
 * Complexidade:
 *      - Tempo: O(n + m)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
void salvarDados(Habitat *inicio);

/*-------------------------------------------------------------
 * Nome: carregarDados
 * Objetivo: Ler o arquivo "animais.txt" e reconstruir a
 *           distribuicao dos animais nos habitats.
 * Entrada:
 *      - inicio: endereco do ponteiro para o primeiro habitat
 * Saida: nenhuma
 * Pre-condicoes:
 *      - arquivo "animais.txt" existente
 * Pos-condicoes:
 *      - estruturas de animais recriadas
 *      - habitats atualizados com novos animais
 * Complexidade:
 *      - Tempo: O(n + m)
 *      - Espaco: O(m)
 -------------------------------------------------------------*/
void carregarDados(Habitat **inicio);

/*-------------------------------------------------------------
 * Nome: registrarTransferencia
 * Objetivo: Registrar uma transferencia interna ou externa
 *           no arquivo "transferencias.txt".
 * Entrada:
 *      - a: animal envolvido
 *      - origem: id do habitat original
 *      - destino: id do habitat destino ou -1
 *      - tipo: "interna" ou "externa"
 * Saida: nenhuma
 * Pre-condicoes:
 *      - a != NULL
 * Pos-condicoes:
 *      - nova linha adicionada em "transferencias.txt"
 * Complexidade:
 *      - Tempo: O(1)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
void registrarTransferencia(struct animal *a, int origem, int destino, const char *tipo);

#endif

