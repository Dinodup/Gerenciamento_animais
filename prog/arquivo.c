/************************************************
 * Nome do estudante: Eduardo Pereira Xavier 	*
 * Trabalho Pratico                             *
 * Disciplinas: Algoritmos e Programação II     *
 * Laboratório de Algoritmos e Programação II   *
 * Professores: Ivone e Ronaldo                 *
 * Data: 26/10/2025                             *
 * Versão: 8.0                                  *
 * Descricao: Implementacao do modulo Arquivo.  *
 * Responsavel pelo armazenamento, leitura e    *
 * registro de operacoes envolvendo animais e   *
 * habitats do zoologico.                       *
 ************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arquivo.h"
#include "habitat.h"
#include "animal.h"

/*-------------------------------------------------------------
 * Nome: salvarDados
 * Objetivo: Registrar todos os habitats e animais no arquivo
 *           "animais.txt" utilizando formato simples de texto.
 * Entrada:
 *      - inicio: ponteiro para o primeiro habitat
 * Saida: nenhuma
 * Pre-condicoes:
 *      - inicio != NULL
 * Pos-condicoes:
 *      - arquivo "animais.txt" sobrescrito com o estado atual
 * Complexidade:
 *      - Tempo: O(n + m)    (n habitats, m animais)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
void salvarDados(Habitat *inicio) {
    FILE *fp;
    Habitat *hab;
    struct animal *a;

    fp = fopen("animais.txt", "w");
    if (!fp) {
        printf("Erro ao abrir arquivo para escrita.\n");
        return;
    }

    hab = inicio;
    while (hab) {
        fprintf(fp, "HABITAT %d | TAMANHO %d | OCUPADO %d\n",
                hab->id, hab->tamanho, hab->espaco_usado);

        a = hab->animais;
        if (!a) {
            fprintf(fp, "  (Sem animais neste habitat)\n");
        }

        while (a) {
            fprintf(fp, "ID %d, NOME: %s, ESPECIE: %s, DIETA: %c\n",
                    a->id, a->nome, a->especie, a->dieta);
            a = a->prox;
        }

        fprintf(fp, "\n");
        hab = hab->prox;
    }

    fclose(fp);
    printf("Dados salvos em 'animais.txt'.\n");
}

/*-------------------------------------------------------------
 * Nome: carregarDados
 * Objetivo: Ler todos os habitats e animais do arquivo
 *           "animais.txt" e reconstruir a lista de estruturas.
 * Entrada:
 *      - inicio: endereco do ponteiro para o primeiro habitat
 * Saida: nenhuma
 * Pre-condicoes:
 *      - arquivo "animais.txt" existente
 * Pos-condicoes:
 *      - habitats preservados
 *      - animais recriados e inseridos nos habitats corretos
 * Complexidade:
 *      - Tempo: O(n + m)
 *      - Espaco: O(m)
 -------------------------------------------------------------*/
void carregarDados(Habitat **inicio) {
    FILE *fp;
    Habitat *zoo, *h, *hab = NULL;
    struct animal *novo;
    char linha[256];
    int idHabAtual = -1;

    fp = fopen("animais.txt", "r");
    if (!fp) {
        printf("Arquivo 'animais.txt' nao encontrado.\n");
        return;
    }

    zoo = *inicio;
    if (!zoo) {
        printf("Inicializando habitats para leitura...\n");
        zoo = inicializarHabitats();
        *inicio = zoo;
    }

    /* limpar animais antigos */
    h = zoo;
    while (h) {
        struct animal *a = h->animais;
        while (a) {
            struct animal *temp = a->prox;
            free(a);
            a = temp;
        }
        h->animais = NULL;
        h->espaco_usado = 0;
        h = h->prox;
    }

    /* leitura linha-por-linha */
    while (fgets(linha, sizeof(linha), fp)) {
        if (strncmp(linha, "HABITAT", 7) == 0) {
            sscanf(linha, "HABITAT %d", &idHabAtual);

            hab = zoo;
            while (hab && hab->id != idHabAtual)
                hab = hab->prox;
        }
        else if (strncmp(linha, "ID", 2) == 0 && hab) {
            novo = (struct animal*) malloc(sizeof(struct animal));
            if (!novo) continue;

            sscanf(linha,
                "ID %d, NOME: %49[^,], ESPECIE: %49[^,], DIETA: %c",
                &novo->id, novo->nome, novo->especie, &novo->dieta
            );

            novo->saude = 'S';
            novo->porte = 1;
            novo->h = hab;

            novo->prox = hab->animais;
            hab->animais = novo;
        }
    }

    fclose(fp);
    printf("Dados carregados de 'animais.txt'.\n");
}

/*-------------------------------------------------------------
 * Nome: registrarTransferencia
 * Objetivo: Registrar operacoes de transferencia interna ou
 *           externa de animais em arquivo de log.
 * Entrada:
 *      - a: animal transferido
 *      - origem: id do habitat de origem
 *      - destino: id do habitat destino ou -1
 *      - tipo: "interna" ou "externa"
 * Saida: nenhuma
 * Pre-condicoes:
 *      - a != NULL
 *      - origem >= 1
 * Pos-condicoes:
 *      - registro adicionado ao final de "transferencias.txt"
 * Complexidade:
 *      - Tempo: O(1)
 *      - Espaco: O(1)
 -------------------------------------------------------------*/
void registrarTransferencia(struct animal *a, int origem, int destino, const char *tipo) {
    FILE *fp = fopen("transferencias.txt", "a");
    if (!fp) {
        printf("Erro ao abrir 'transferencias.txt'.\n");
        return;
    }

    if (strcmp(tipo, "interna") == 0) {
        fprintf(fp,
            "Transferencia interna | ID %d | Nome: %s | Origem: %d | Destino: %d\n",
            a->id, a->nome, origem, destino
        );
    }
    else {
        fprintf(fp,
            "Transferencia externa | ID %d | Nome: %s | Origem: %d | Removido do zoologico\n",
            a->id, a->nome, origem
        );
    }

    fclose(fp);
}

