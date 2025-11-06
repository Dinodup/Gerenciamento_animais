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
    FILE *fp = fopen("animais.txt", "w");
    if (!fp) {
        printf("❌ Erro ao abrir arquivo para escrita.\n");
        return;
    }

    Habitat *hab = inicio;
    while (hab) {
        fprintf(fp, "HABITAT %d TAMANHO %d OCUPADO %d\n",
                hab->id, hab->tamanho, hab->espaco_usado);

        struct animal *a = hab->animais;
        while (a) {
            fprintf(fp, "%d;%s;%s;%c;%c;%d;%d\n",
                    a->id, a->nome, a->especie, a->dieta,
                    a->saude, a->porte, hab->id);
            a = a->prox;
        }

        fprintf(fp, "FIM_HABITAT\n");
        hab = hab->prox;
    }

    fclose(fp);
    printf("\n💾 Dados salvos em 'animais.txt'.\n");
}

/* ----------------------------------------------------------
   Carrega habitats e animais a partir de 'animais.txt'
-----------------------------------------------------------*/
void carregarDados(Habitat **inicio) {
    FILE *fp = fopen("animais.txt", "r");
    if (!fp) {
        printf("❌ Arquivo 'animais.txt' não encontrado.\n");
        return;
    }

    Habitat *zoo = *inicio;
    if (!zoo) {
        printf("⚠️  Inicializando habitats antes da leitura...\n");
        zoo = inicializarHabitats();
        *inicio = zoo;
    }

    char linha[256];
    while (fgets(linha, sizeof(linha), fp)) {
        if (strncmp(linha, "HABITAT", 7) == 0) continue;
        if (strncmp(linha, "FIM_HABITAT", 11) == 0) continue;

        struct animal *novo = (struct animal*) malloc(sizeof(struct animal));
        if (!novo) {
            printf("Erro de alocação ao carregar animal.\n");
            fclose(fp);
            return;
        }

        sscanf(linha, "%d;%49[^;];%49[^;];%c;%c;%d;%d",
               &novo->id, novo->nome, novo->especie, &novo->dieta,
               &novo->saude, &novo->porte, &novo->h);

        // Localizar habitat correspondente
        int idHab = 0;
        sscanf(linha, "%*d;%*[^;];%*[^;];%*c;%*c;%*d;%d", &idHab);

        Habitat *hab = zoo;
        while (hab && hab->id != idHab) hab = hab->prox;

        if (hab) {
            novo->h = hab;
            novo->prox = hab->animais;
            hab->animais = novo;
            hab->espaco_usado += novo->porte;
        } else {
            free(novo);
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
