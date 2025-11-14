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
 * Funcao: cadastrarAnimal
 * Pre-condicoes:
 *      - zoo != NULL (habitats inicializados)
 * Pos-condicoes:
 *      - cria um novo animal e o insere no habitat apropriado
 * Complexidade:
 *      - O(n), dependendo da busca pelo habitat correto
 -------------------------------------------------------------*/
void cadastrarAnimal(Habitat *zoo);

/*-------------------------------------------------------------
 * Funcao: buscarAnimalRecursivo
 * Entrada:
 *      - inicio: primeiro habitat
 *      - id: identificacao do animal buscado
 * Pre-condicoes:
 *      - inicio != NULL
 * Pos-condicoes:
 *      - retorna ponteiro para Animal se encontrado
 *      - retorna NULL caso contrario
 * Complexidade:
 *      - O(n + m) usando recursao
 -------------------------------------------------------------*/
Animal* buscarAnimalRecursivo(Habitat *inicio, int id);

/*-------------------------------------------------------------
 * Funcao: alterarAnimal
 * Pre-condicoes:
 *      - zoo != NULL
 * Pos-condicoes:
 *      - modifica campos do animal escolhido pelo usuario
 * Complexidade:
 *      - O(n) para localizar o animal e atualizar dados
 -------------------------------------------------------------*/
void alterarAnimal(Habitat *zoo);

/*-------------------------------------------------------------
 * Funcao: exibirAnimal
 * Pre-condicoes:
 *      - a != NULL
 * Pos-condicoes:
 *      - imprime informacoes do animal
 * Complexidade: O(1)
 -------------------------------------------------------------*/
void exibirAnimal(const Animal *a);

#endif
