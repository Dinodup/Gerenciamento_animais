/************************************************
 * Nome do estudante: Eduardo Pereira Xavier 	*
 * Trabalho Pratico                             *
 * Disciplinas: Algoritmos e Programação II     *
 * Laboratório de Algoritmos e Programação II   *
 * Professores: Ivone e Ronaldo                 *
 * Data: 26/10/2025                             *
 * Versão: 8.0                                  *
 * Descricao: Implementacao do modulo Utils.    *
 * Contem funcoes auxiliares e rotina de        *
 * liberacao de memoria do sistema.             *
 ************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "animal.h"
#include "habitat.h"
#include "fila.h"

/*-------------------------------------------------------------
 * Nome: liberarMemoria
 * Objetivo: Liberar todos os espacos alocados pelo zoologico,
 *           incluindo habitats, animais e fila de medicacao.
 * Entrada:
 *      - zoo: primeiro habitat
 *      - fila: fila de medicacao
 * Saida: nenhuma
 * Pre-condicoes:
 *      - zoo pode ser NULL
 *      - fila pode ser NULL
 * Pos-condicoes:
 *      - toda a memoria dinamica eh liberada
 * Complexidade:
 *      - Tempo: O(n + m)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
void liberarMemoria(Habitat *zoo, FilaMed *fila) {
    Habitat *h;
    Habitat *tmpH;
    Animal *a;
    Animal *tmp;

    if (fila) {
        free(fila);
    }

    h = zoo;
    while (h) {
        a = h->animais;

        while (a) {
            tmp = a;
            a = a->prox;
            free(tmp);
        }

        tmpH = h;
        h = h->prox;
        free(tmpH);
    }

    printf("Memoria liberada com sucesso.\n");
}

/*-------------------------------------------------------------
 * Nome: converterPorte
 * Objetivo: Converter um caractere representando o porte de
 *           um animal em um valor numerico.
 * Entrada:
 *      - porteChar: 'P', 'M' ou 'G'
 * Saida: inteiro correspondente ao porte
 * Pre-condicoes:
 *      - nenhuma
 * Pos-condicoes:
 *      - retorna 1, 2 ou 4 (default = 1)
 * Complexidade:
 *      - Tempo: O(1)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
int converterPorte(char porteChar) {
    switch (porteChar) {
        case 'P': case 'p': return 1;
        case 'M': case 'm': return 2;
        case 'G': case 'g': return 4;
        default: return 1;
    }
}

/*-------------------------------------------------------------
 * Nome: porteToString
 * Objetivo: Converter o valor numerico do porte em uma string.
 * Entrada:
 *      - porte: 1, 2 ou 4
 * Saida: string estatica representando o porte
 * Pre-condicoes:
 *      - porte deve ser um valor valido
 * Pos-condicoes:
 *      - retorna texto correspondente ao porte
 * Complexidade:
 *      - Tempo: O(1)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
const char* porteToString(int porte) {
    switch (porte) {
        case 1: return "Pequeno";
        case 2: return "Medio";
        case 4: return "Grande";
        default: return "Desconhecido";
    }
}

/*-------------------------------------------------------------
 * Nome: limparBuffer
 * Objetivo: Esvaziar o buffer de entrada padrao para evitar
 *           conflitos com funcoes scanf ou fgets.
 * Entrada: nenhuma
 * Saida: nenhuma
 * Pre-condicoes:
 *      - nenhuma
 * Pos-condicoes:
 *      - buffer de entrada limpo
 * Complexidade:
 *      - Tempo: O(n) (n caracteres restantes no buffer)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

