/************************************************
 * Nome do(a) estudante: Eduardo Pereira Xavier *
 * Trabalho Prático                             *
 * Disciplinas: Algoritmos e Programação II      *
 * Laboratório de Algoritmos e Programação II    *
 * Professores: Ivone e Ronaldo                 *
 * Data: 05/11/2025                             *
 * Versão: 1.0                                  *
 * Descrição: Implementação do módulo Utils.    *
 * Contém funções auxiliares de uso geral e     *
 * rotina de liberação de memória.              *
 *************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "animal.h"
#include "habitat.h"
#include "fila.h"

/* ----------------------------------------------------------
   Libera toda a memória alocada pelos habitats, animais e fila.
-----------------------------------------------------------*/
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

    printf("\n🧹 Memória liberada com sucesso.\n");
}

/* ----------------------------------------------------------
   Converte o caractere do porte para número.
-----------------------------------------------------------*/
int converterPorte(char porteChar) {
    switch (porteChar) {
        case 'P': case 'p': return 1;
        case 'M': case 'm': return 2;
        case 'G': case 'g': return 4;
        default: return 1;
    }
}

/* ----------------------------------------------------------
   Retorna texto correspondente ao porte.
-----------------------------------------------------------*/
const char* porteToString(int porte) {
    switch (porte) {
        case 1: return "Pequeno";
        case 2: return "Médio";
        case 4: return "Grande";
        default: return "Desconhecido";
    }
}

/* ----------------------------------------------------------
   Limpa o buffer de entrada para evitar erros com scanf/fgets.
-----------------------------------------------------------*/
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
