/************************************************
 * Nome do estudante: Eduardo Pereira Xavier 	*
 * Trabalho Pratico                             *
 * Disciplinas: Algoritmos e Programação II     *
 * Laboratório de Algoritmos e Programação II   *
 * Professores: Ivone e Ronaldo                 *
 * Data: 26/10/2025                             *
 * Versão: 8.0                                  *
 * Descricao: Cabecalho do modulo Animal.       *
 * Define o TAD Animal e as funcoes de          *
 * gerenciamento dos animais do zoologico.      *
 ************************************************/

#ifndef ANIMAL_H
#define ANIMAL_H

#include "habitat.h"

/*-------------------------------------------------------------
 * TAD Animal
 * id        -> identificacao unica
 * nome      -> nome do animal
 * especie   -> especie do animal
 * dieta     -> 'C' carnivoro, 'H' herbivoro, 'O' onivoro
 * saude     -> 'S' saudavel, 'M' em medicacao
 * porte     -> 1 pequeno, 2 medio, 4 grande
 * h         -> ponteiro para habitat do animal
 * prox      -> proximo elemento na lista encadeada
 -------------------------------------------------------------*/
typedef struct animal {
    int id;
    char nome[50];
    char especie[50];
    char dieta;
    char saude;
    int porte;
    struct habitat *h;
    struct animal *prox;
} Animal;

/*-------------------------------------------------------------
 * Nome: cadastrarAnimal
 * Objetivo:
 *      Realizar o cadastro de um novo animal, garantindo que
 *      o ID seja unico e que o animal seja inserido no habitat
 *      adequado ao seu porte.
 * Entrada:
 *      - zoo: ponteiro para a lista de habitats inicializada
 * Saida:
 *      - nenhuma
 * Pre-condicoes:
 *      - zoo != NULL
 * Pos-condicoes:
 *      - novo animal criado e inserido no habitat apropriado
 * Complexidade:
 *      - Tempo: O(n) para verificar IDs e escolher habitat
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
void cadastrarAnimal(Habitat *zoo);

/*-------------------------------------------------------------
 * Nome: buscarAnimalRecursivo
 * Objetivo:
 *      Localizar um animal pelo ID utilizando busca recursiva
 *      nos habitats encadeados.
 * Entrada:
 *      - inicio: primeiro habitat da lista
 *      - id: identificacao do animal procurado
 * Saida:
 *      - ponteiro para Animal se encontrado
 *      - NULL caso contrario
 * Pre-condicoes:
 *      - inicio != NULL
 * Pos-condicoes:
 *      - nenhuma (nao modifica estruturas)
 * Complexidade:
 *      - Tempo: O(n)
 *      - Espaco: O(n) devido a recursao
 -------------------------------------------------------------*/
Animal* buscarAnimalRecursivo(Habitat *inicio, int id);

/*-------------------------------------------------------------
 * Nome: alterarAnimal
 * Objetivo:
 *      Permitir alteracao dos dados de um animal existente,
 *      incluindo sua realocacao para outro habitat.
 * Entrada:
 *      - zoo: ponteiro para o primeiro habitat
 * Saida:
 *      - nenhuma
 * Pre-condicoes:
 *      - zoo != NULL
 * Pos-condicoes:
 *      - campos do animal podem ter sido modificados
 * Complexidade:
 *      - Tempo: O(n) para localizar e atualizar o animal
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
void alterarAnimal(Habitat *zoo);

/*-------------------------------------------------------------
 * Nome: exibirAnimal
 * Objetivo:
 *      Exibir todas as informacoes do animal informado.
 * Entrada:
 *      - a: ponteiro para Animal
 * Saida:
 *      - nenhuma (somente impressao)
 * Pre-condicoes:
 *      - a != NULL
 * Pos-condicoes:
 *      - informacoes impressas na tela
 * Complexidade:
 *      - Tempo: O(1)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
void exibirAnimal(const Animal *a);

#endif

