/************************************************
 * Nome do estudante: Eduardo Pereira Xavier 	*
 * Trabalho Pratico                             *
 * Disciplinas: Algoritmos e Programação II     *
 * Laboratório de Algoritmos e Programação II   *
 * Professores: Ivone e Ronaldo                 *
 * Data: 26/10/2025                             *
 * Versão: 8.0                                  *
 * Descricao: Cabecalho do modulo Habitat.      *
 * Define o TAD Habitat e funcoes de controle   *
 * sobre a distribuicao e gerenciamento de      *
 * animais no zoologico.                        *
 ************************************************/

#ifndef HABITAT_H
#define HABITAT_H

#include <stdio.h>
#include <stdlib.h>

/*-------------------------------------------------------------
 * Nome: Estrutura Habitat
 * Objetivo: Representar um habitat do zoologico contendo
 *           capacidade, ocupacao e lista de animais.
 * Campos:
 *      - id: identificador do habitat
 *      - tamanho: capacidade total (1, 10 ou 20)
 *      - espaco_usado: soma dos portes dos animais
 *      - animais: lista encadeada de animais do habitat
 *      - prox: ponteiro para o proximo habitat
 -------------------------------------------------------------*/
typedef struct habitat {
    int id;
    int tamanho;
    int espaco_usado;
    struct animal *animais;
    struct habitat *prox;
} Habitat;

/*-------------------------------------------------------------
 * Nome: inicializarHabitats
 * Objetivo: Criar e encadear os 20 habitats padrao.
 * Entrada: nenhuma
 * Saida: ponteiro para o primeiro habitat criado
 * Pre-condicoes:
 *      - memoria suficiente para alocacao
 * Pos-condicoes:
 *      - 7 habitats pequenos, 8 medios e 5 grandes criados
 * Complexidade:
 *      - Tempo: O(n)
 *      - Espaco: O(n)
 -------------------------------------------------------------*/
Habitat* inicializarHabitats();

/*-------------------------------------------------------------
 * Nome: listarHabitats
 * Objetivo: Exibir uma lista resumida dos habitats do zoologico.
 * Entrada: inicio -> primeiro habitat
 * Saida: nenhuma
 * Pre-condicoes:
 *      - inicio != NULL
 * Pos-condicoes:
 *      - dados exibidos na tela
 * Complexidade:
 *      - Tempo: O(n)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
void listarHabitats(Habitat *inicio);

/*-------------------------------------------------------------
 * Nome: escolherHabitat
 * Objetivo: Selecionar um habitat valido para um novo animal.
 * Entrada:
 *      - inicio: primeiro habitat da lista
 *      - novo: animal recem-criado
 * Saida: ponteiro para o habitat escolhido ou NULL
 * Pre-condicoes:
 *      - inicio != NULL
 *      - novo != NULL
 * Pos-condicoes:
 *      - retorna habitat com espaco e especie compativel
 * Complexidade:
 *      - Tempo: O(n)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
Habitat* escolherHabitat(Habitat *inicio, struct animal *novo);

/*-------------------------------------------------------------
 * Nome: inserirAnimalNoHabitat
 * Objetivo: Inserir um animal no inicio da lista do habitat.
 * Entrada:
 *      - hab: habitat destino
 *      - novo: animal a ser inserido
 * Saida: nenhuma
 * Pre-condicoes:
 *      - hab != NULL
 *      - novo != NULL
 * Pos-condicoes:
 *      - animal inserido no habitat
 * Complexidade:
 *      - Tempo: O(1)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
void inserirAnimalNoHabitat(Habitat *hab, struct animal *novo);

/*-------------------------------------------------------------
 * Nome: transferirAnimal
 * Objetivo: Transferir um animal entre habitats ou removelo
 *           definitivamente do zoologico.
 * Entrada:
 *      - inicio: primeiro habitat
 *      - forcarTipo: 0 = modo interativo
 * Saida: nenhuma
 * Pre-condicoes:
 *      - habitats inicializados
 *      - animal existente
 * Pos-condicoes:
 *      - transferencia concluida conforme escolha do usuario
 * Complexidade:
 *      - Tempo: O(n)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
void transferirAnimal(Habitat *inicio, int forcarTipo);

/*-------------------------------------------------------------
 * Nome: liberarHabitats
 * Objetivo: Liberar toda a memoria alocada pelos habitats
 *           e seus animais.
 * Entrada: inicio -> primeiro habitat
 * Saida: nenhuma
 * Pre-condicoes:
 *      - inicio != NULL
 * Pos-condicoes:
 *      - toda a memoria liberada
 * Complexidade:
 *      - Tempo: O(n + m)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
void liberarHabitats(Habitat *inicio);

#endif

