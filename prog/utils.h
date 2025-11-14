/************************************************
 * Nome do estudante: Eduardo Pereira Xavier 	*
 * Trabalho Pratico                             *
 * Disciplinas: Algoritmos e Programação II     *
 * Laboratório de Algoritmos e Programação II   *
 * Professores: Ivone e Ronaldo                 *
 * Data: 26/10/2025                             *
 * Versão: 8.0                                  *
 * Descricao: Cabecalho do modulo Utils.        *
 * Define funcoes auxiliares utilizadas pelos   *
 * demais modulos do sistema.                   *
 ************************************************/

#ifndef UTILS_H
#define UTILS_H

#include "habitat.h"
#include "fila.h"

/*-------------------------------------------------------------
 * Nome: liberarMemoria
 * Objetivo: Desalocar toda a memoria utilizada pelo zoologico,
 *           incluindo habitats, animais e fila de medicacao.
 * Entrada:
 *      - zoo: primeiro habitat
 *      - fila: fila de medicacao
 * Saida: nenhuma
 * Pre-condicoes:
 *      - estruturas podem ser NULL
 * Pos-condicoes:
 *      - toda a memoria dinamica liberada
 * Complexidade:
 *      - Tempo: O(n + m)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
void liberarMemoria(Habitat *zoo, FilaMed *fila);

/*-------------------------------------------------------------
 * Nome: converterPorte
 * Objetivo: Converter caractere de porte ('P','M','G') em um
 *           valor numerico equivalente.
 * Entrada:
 *      - porteChar: caracter representando o porte
 * Saida: inteiro (1, 2 ou 4)
 * Pre-condicoes:
 *      - nenhuma
 * Pos-condicoes:
 *      - retorna porte numerico valido
 * Complexidade:
 *      - Tempo: O(1)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
int converterPorte(char porteChar);

/*-------------------------------------------------------------
 * Nome: porteToString
 * Objetivo: Converter porte numerico em uma representacao de
 *           texto estatica.
 * Entrada:
 *      - porte: 1, 2 ou 4
 * Saida: string constante
 * Pre-condicoes:
 *      - nenhuma
 * Pos-condicoes:
 *      - retorna texto correspondente ou "Desconhecido"
 * Complexidade:
 *      - Tempo: O(1)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
const char* porteToString(int porte);

/*-------------------------------------------------------------
 * Nome: limparBuffer
 * Objetivo: Limpar o buffer de entrada padrao para evitar
 *           inconsistencias com operacoes scanf ou fgets.
 * Entrada: nenhuma
 * Saida: nenhuma
 * Pre-condicoes:
 *      - nenhuma
 * Pos-condicoes:
 *      - buffer de entrada limpo
 * Complexidade:
 *      - Tempo: O(n)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
void limparBuffer();

#endif

