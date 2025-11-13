/************************************************
 * Nome do(a) estudante: Eduardo Pereira Xavier *
 * Trabalho Prático                             *
 * Disciplinas: Algoritmos e Programação II      *
 * Laboratório de Algoritmos e Programação II    *
 * Professores: Ivone e Ronaldo                 *
 * Data: 05/11/2025                             *
 * Versão: 1.0                                  *
 * Descrição: Implementação do módulo Arquivo.  *
 * Responsável por salvar, carregar e registrar *
 * dados do zoológico em arquivos de texto.     *
 *************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivo.h"
#include "habitat.h"
#include "animal.h"

/* ----------------------------------------------------------
   Salva todos os habitats e animais no arquivo 'animais.txt'
-----------------------------------------------------------*/
void salvarDados(Habitat *inicio) {
    FILE *fp;
    Habitat *hab;
    struct animal *a;

    /* Modo 'w' substitui o conteúdo — mantém o estado atual */
    fp = fopen("animais.txt", "w");
    if (!fp) {
        printf("❌ Erro ao abrir arquivo para escrita.\n");
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
            fprintf(fp, "ID %d, NOME: %s, ESPÉCIE: %s, DIETA: %c\n",
                    a->id, a->nome, a->especie, a->dieta);
            a = a->prox;
        }

        fprintf(fp, "\n");
        hab = hab->prox;
    }

    fclose(fp);
    printf("\n💾 Dados salvos em 'animais.txt' no formato de tabela.\n");
    printf("⚠️ Atenção: dados anteriores foram substituídos pelo estado atual.\n");
}

/* ----------------------------------------------------------
   Carrega habitats e animais a partir de 'animais.txt'
-----------------------------------------------------------*/
void carregarDados(Habitat **inicio) {
    FILE *fp;
    Habitat *zoo, *h, *hab = NULL;
    struct animal *novo;
    char linha[256];
    int idHabAtual = -1;

    fp = fopen("animais.txt", "r");
    if (!fp) {
        printf("❌ Arquivo 'animais.txt' não encontrado.\n");
        return;
    }

    zoo = *inicio;
    if (!zoo) {
        printf("⚠️ Inicializando habitats antes da leitura...\n");
        zoo = inicializarHabitats();
        *inicio = zoo;
    }

    /* Limpar lista atual antes de carregar */
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

    while (fgets(linha, sizeof(linha), fp)) {
        if (strncmp(linha, "HABITAT", 7) == 0) {
            /* Pega o ID do habitat atual */
            sscanf(linha, "HABITAT %d", &idHabAtual);
            hab = zoo;
            while (hab && hab->id != idHabAtual)
                hab = hab->prox;
        } else if (strncmp(linha, "ID", 2) == 0 && hab) {
            /* Novo animal */
            novo = (struct animal*) malloc(sizeof(struct animal));
            if (!novo) continue;

            sscanf(linha, "ID %d, NOME: %49[^,], ESPÉCIE: %49[^,], DIETA: %c",
                   &novo->id, novo->nome, novo->especie, &novo->dieta);

            novo->saude = 'S';
            novo->porte = 1;
            novo->h = hab;
            novo->prox = hab->animais;
            hab->animais = novo;
        }
    }

    fclose(fp);
    printf("\n📂 Dados carregados de 'animais.txt'.\n");
}

/* ----------------------------------------------------------
   Registra transferência em 'transferencias.txt'
-----------------------------------------------------------*/
void registrarTransferencia(struct animal *a, int origem, int destino, const char *tipo) {
    FILE *fp = fopen("transferencias.txt", "a");
    if (!fp) {
        printf("❌ Erro ao abrir 'transferencias.txt'.\n");
        return;
    }

    if (strcmp(tipo, "interna") == 0) {
        fprintf(fp, "Transferência interna | ID %d | Nome: %s | Origem: %d | Destino: %d\n",
                a->id, a->nome, origem, destino);
    } else {
        fprintf(fp, "Transferência externa | ID %d | Nome: %s | Origem: %d | Removido do zoológico\n",
                a->id, a->nome, origem);
    }

    fclose(fp);
}
