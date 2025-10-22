/************************************************
 * Nome do(a) estudante: Eduardo Pereira Xavier
 * Trabalho Prático - Versão 1 (Protótipo)
 * Disciplinas: Algoritmos e Programação II
 * Professores: Ivone e Ronaldo
 * Data: 2025-10-xx
 * Versão: 1.0
 * Descrição: Gerenciamento simples de animais em um zoológico.
 *************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------- Configurações e constantes ---------- */
#define MAX_HABITATS 20
#define MAX_NAME 50
#define MAX_SPECIE 50
#define MAX_DIETA 20
#define MAX_SAUDE 20
#define FILE_NAME "animais.txt"

/* Vet queue capacity (enough for prototyping). */
#define VET_QUEUE_CAP 100

/* ---------- Tipos de dados ---------- */
typedef struct {
    int id;
    char nome[MAX_NAME];
    char especie[MAX_SPECIE];
    char dieta[MAX_DIETA];    /* "Carnivoro", "Herbivoro", "Onivoro" */
    char saude[MAX_SAUDE];    /* "Saudavel", "Quarentena", "Medicação" */
    char porte;               /* 'P','M','G' */
    int tamanho_ocupado;      /* 1 para P, 2 para M, 4 para G (configurável) */
    int habitat_id;           /* -1 se estiver na fila veterinaria ou removido */
} Animal;

typedef struct AnimalNode {
    Animal animal;
    struct AnimalNode *next;
} AnimalNode;

typedef struct {
    int id;                   /* identificador do habitat (0..n-1) */
    int capacidade;           /* capacidade total em "unidades" (ex: 1,10,20) */
    int ocupado;              /* unidades atualmente ocupadas */
    AnimalNode *head;         /* lista encadeada de animais neste habitat */
} Habitat;

/* Fila sequencial circular para veterinário */
typedef struct {
    Animal *items[VET_QUEUE_CAP];
    int front;
    int rear;
    int count;
} VetQueue;

typedef struct {
    Habitat habitats[MAX_HABITATS];
    int num_habitats;
    int next_animal_id;
    VetQueue vet_queue;
} Zoo;

/* ---------- Protótipos ---------- */

/* Zoo / habitat */
void zoo_init(Zoo *z);
int zoo_add_habitat(Zoo *z, int capacidade);
Habitat *zoo_get_habitat(Zoo *z, int hid);

/* Animal list */
AnimalNode *create_animal_node(const Animal *a);
void free_animal_node(AnimalNode *n);
int add_animal_to_habitat(Habitat *h, Animal a);
AnimalNode *remove_animal_from_habitat_by_id(Habitat *h, int id, Animal *out);
AnimalNode *find_animal_node_by_id(Habitat *h, int id);

/* Vet queue */
void vet_queue_init(VetQueue *q);
int vet_enqueue(VetQueue *q, Animal *a);
Animal *vet_dequeue(VetQueue *q);
int vet_is_empty(VetQueue *q);

/* High-level operations */
int inserir_novo_animal(Zoo *z, int habitat_id, const char *nome, const char *especie,
                         const char *dieta, const char *saude, char porte);
int transferir_animal_interno(Zoo *z, int animal_id, int destino_habitat);
int transferir_animal_externo(Zoo *z, int animal_id); /* remove do zoo */
int medicar_animal(Zoo *z, int animal_id, const char *novo_estado); /* coloca na fila */
void listar_tudo(Zoo *z);
AnimalNode *buscar_animal_por_id(Zoo *z, int id, int *hab_id_out);

/* Arquivo */
int salvar_para_arquivo(Zoo *z, const char *filename);
int carregar_de_arquivo(Zoo *z, const char *filename);

/* Util */
int porte_to_size(char porte);

/* UI simples */
void menu_loop(Zoo *z);

/* ---------- Implementações ---------- */

void zoo_init(Zoo *z) {
    int i;
    for (i = 0; i < MAX_HABITATS; ++i) {
        z->habitats[i].id = -1;
        z->habitats[i].capacidade = 0;
        z->habitats[i].ocupado = 0;
        z->habitats[i].head = NULL;
    }
    z->num_habitats = 0;
    z->next_animal_id = 1;
    vet_queue_init(&z->vet_queue);
}

/* Adiciona habitat e retorna id (index) ou -1 se cheio */
int zoo_add_habitat(Zoo *z, int capacidade) {
    if (z->num_habitats >= MAX_HABITATS) return -1;
    int id = z->num_habitats;
    z->habitats[id].id = id;
    z->habitats[id].capacidade = capacidade;
    z->habitats[id].ocupado = 0;
    z->habitats[id].head = NULL;
    z->num_habitats++;
    return id;
}

Habitat *zoo_get_habitat(Zoo *z, int hid) {
    if (hid < 0 || hid >= z->num_habitats) return NULL;
    return &z->habitats[hid];
}

/* Lista encadeada e operações */
AnimalNode *create_animal_node(const Animal *a) {
    AnimalNode *n = (AnimalNode*) malloc(sizeof(AnimalNode));
    if (!n) return NULL;
    n->animal = *a;
    n->next = NULL;
    return n;
}

void free_animal_node(AnimalNode *n) {
    if (n) free(n);
}

/* Adiciona no início (O(1)) */
int add_animal_to_habitat(Habitat *h, Animal a) {
    if (!h) return 0;
    if (h->ocupado + a.tamanho_ocupado > h->capacidade) return 0;
    AnimalNode *n = create_animal_node(&a);
    if (!n) return 0;
    n->next = h->head;
    h->head = n;
    h->ocupado += a.tamanho_ocupado;
    return 1;
}

/* Remove animal por id; retorna nó retirada (quem chamou deve free ou reusar) */
AnimalNode *remove_animal_from_habitat_by_id(Habitat *h, int id, Animal *out) {
    if (!h) return NULL;
    AnimalNode *prev = NULL, *cur = h->head;
    while (cur) {
        if (cur->animal.id == id) {
            if (prev) prev->next = cur->next;
            else h->head = cur->next;
            h->ocupado -= cur->animal.tamanho_ocupado;
            if (out) *out = cur->animal;
            cur->next = NULL;
            return cur;
        }
        prev = cur;
        cur = cur->next;
    }
    return NULL;
}

AnimalNode *find_animal_node_by_id(Habitat *h, int id) {
    AnimalNode *cur = h->head;
    while (cur) {
        if (cur->animal.id == id) return cur;
        cur = cur->next;
    }
    return NULL;
}

/* Vet queue circular */
void vet_queue_init(VetQueue *q) {
    int i;
    for (i = 0; i < VET_QUEUE_CAP; ++i) q->items[i] = NULL;
    q->front = 0; q->rear = -1; q->count = 0;
}

int vet_enqueue(VetQueue *q, Animal *a) {
    if (q->count >= VET_QUEUE_CAP) return 0;
    q->rear = (q->rear + 1) % VET_QUEUE_CAP;
    q->items[q->rear] = a;
    q->count++;
    return 1;
}

Animal *vet_dequeue(VetQueue *q) {
    if (q->count == 0) return NULL;
    Animal *a = q->items[q->front];
    q->items[q->front] = NULL;
    q->front = (q->front + 1) % VET_QUEUE_CAP;
    q->count--;
    return a;
}

int vet_is_empty(VetQueue *q) { return q->count == 0; }

/* ---------- Operações de alto nível ---------- */

int porte_to_size(char porte) {
    if (porte == 'P' || porte == 'p') return 1;
    if (porte == 'M' || porte == 'm') return 2;
    if (porte == 'G' || porte == 'g') return 4;
    return 1;
}

int inserir_novo_animal(Zoo *z, int habitat_id, const char *nome, const char *especie,
                         const char *dieta, const char *saude, char porte) {
    Habitat *h = zoo_get_habitat(z, habitat_id);
    if (!h) return 0;
    Animal a;
    a.id = z->next_animal_id++;
    strncpy(a.nome, nome, MAX_NAME-1); a.nome[MAX_NAME-1]=0;
    strncpy(a.especie, especie, MAX_SPECIE-1); a.especie[MAX_SPECIE-1]=0;
    strncpy(a.dieta, dieta, MAX_DIETA-1); a.dieta[MAX_DIETA-1]=0;
    strncpy(a.saude, saude, MAX_SAUDE-1); a.saude[MAX_SAUDE-1]=0;
    a.porte = porte;
    a.tamanho_ocupado = porte_to_size(porte);
    a.habitat_id = habitat_id;
    if (!add_animal_to_habitat(h, a)) {
        return 0;
    }
    return 1;
}

/* Busca animal em todos habitats; retorna nó e hab id (não remove) */
AnimalNode *buscar_animal_por_id(Zoo *z, int id, int *hab_id_out) {
    int i;
    for (i = 0; i < z->num_habitats; ++i) {
        AnimalNode *n = find_animal_node_by_id(&z->habitats[i], id);
        if (n) {
            if (hab_id_out) *hab_id_out = i;
            return n;
        }
    }
    return NULL;
}

/* Transferir internamente: remove de origem e insere no destino (verifica capacidade) */
int transferir_animal_interno(Zoo *z, int animal_id, int destino_habitat) {
    int i;
    Animal tmp;
    for (i = 0; i < z->num_habitats; ++i) {
        AnimalNode *n = remove_animal_from_habitat_by_id(&z->habitats[i], animal_id, &tmp);
        if (n) {
            Habitat *dest = zoo_get_habitat(z, destino_habitat);
            if (!dest) {
                /* devolve para origem */
                add_animal_to_habitat(&z->habitats[i], tmp);
                free_animal_node(n);
                return 0;
            }
            if (dest->ocupado + tmp.tamanho_ocupado > dest->capacidade) {
                /* sem espaço: devolve */
                add_animal_to_habitat(&z->habitats[i], tmp);
                free_animal_node(n);
                return 0;
            }
            tmp.habitat_id = destino_habitat;
            add_animal_to_habitat(dest, tmp);
            free_animal_node(n);
            return 1;
        }
    }
    return 0;
}

/* Transferir externamente (remover do zoo): remove da lista */
int transferir_animal_externo(Zoo *z, int animal_id) {
    int i;
    Animal tmp;
    for (i = 0; i < z->num_habitats; ++i) {
        AnimalNode *n = remove_animal_from_habitat_by_id(&z->habitats[i], animal_id, &tmp);
        if (n) {
            free_animal_node(n);
            return 1;
        }
    }
    return 0;
}

/* Medicar: colocar na fila veterinária.
   We'll remove from habitat and put pointer into queue. Need to preserve the Animal memory.
   Strategy: allocate a new Animal on heap, copy the data, enqueue pointer.
*/
int medicar_animal(Zoo *z, int animal_id, const char *novo_estado) {
    int i;
    Animal tmp;
    for (i = 0; i < z->num_habitats; ++i) {
        AnimalNode *n = remove_animal_from_habitat_by_id(&z->habitats[i], animal_id, &tmp);
        if (n) {
            /* aloca animal em heap para fila */
            Animal *heap_a = (Animal*) malloc(sizeof(Animal));
            if (!heap_a) {
                /* devolve para habitat */
                add_animal_to_habitat(&z->habitats[i], tmp);
                free_animal_node(n);
                return 0;
            }
            if (novo_estado) strncpy(tmp.saude, novo_estado, MAX_SAUDE-1);
            tmp.habitat_id = -1;
            *heap_a = tmp;
            if (!vet_enqueue(&z->vet_queue, heap_a)) {
                /* fila cheia: devolver e liberar */
                add_animal_to_habitat(&z->habitats[i], tmp);
                free(heap_a);
                free_animal_node(n);
                return 0;
            }
            free_animal_node(n);
            return 1;
        }
    }
    return 0;
}

/* Quando o veterinário terminar, desenfileira e tenta recolocar no habitat indicado (ou em outro livre) */
int finalizar_atendimento(Zoo *z, int destino_habitat) {
    Animal *a = vet_dequeue(&z->vet_queue);
    if (!a) return 0;
    Habitat *h = zoo_get_habitat(z, destino_habitat);
    if (!h) {
        free(a);
        return 0;
    }
    if (h->ocupado + a->tamanho_ocupado > h->capacidade) {
        /* se não couber, tenta colocar no primeiro habitat que couber */
        int i;
        int placed = 0;
        for (i = 0; i < z->num_habitats && !placed; ++i) {
            if (z->habitats[i].ocupado + a->tamanho_ocupado <= z->habitats[i].capacidade) {
                a->habitat_id = i;
                add_animal_to_habitat(&z->habitats[i], *a);
                placed = 1;
            }
        }
        if (!placed) {
            /* sem lugar - remove do sistema */
            free(a);
            return 0;
        } else {
            free(a);
            return 1;
        }
    } else {
        a->habitat_id = destino_habitat;
        add_animal_to_habitat(h, *a);
        free(a);
        return 1;
    }
}

/* Listagens */
void listar_tudo(Zoo *z) {
    int i;
    printf("=== Lista de Habitats e Animais ===\n");
    for (i = 0; i < z->num_habitats; ++i) {
        Habitat *h = &z->habitats[i];
        printf("Habitat %d: capacidade=%d ocupado=%d\n", h->id, h->capacidade, h->ocupado);
        AnimalNode *cur = h->head;
        while (cur) {
            Animal *a = &cur->animal;
            printf("  ID:%d Nome:%s Especie:%s Porte:%c Tam:%d Saude:%s\n",
                   a->id, a->nome, a->especie, a->porte, a->tamanho_ocupado, a->saude);
            cur = cur->next;
        }
    }
    /* fila veterinaria */
    printf("=== Fila Veterinaria (%d) ===\n", z->vet_queue.count);
    if (z->vet_queue.count == 0) printf(" (vazia)\n");
    else {
        int i2, idx;
        for (i2 = 0; i2 < z->vet_queue.count; ++i2) {
            idx = (z->vet_queue.front + i2) % VET_QUEUE_CAP;
            Animal *a = z->vet_queue.items[idx];
            if (a) printf("  ID:%d Nome:%s Saude:%s\n", a->id, a->nome, a->saude);
        }
    }
}

/* Salvar em arquivo: um CSV simples. Animais que estão em fila terão habitat = -1 */
int salvar_para_arquivo(Zoo *z, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) return 0;
    /* Percorrer habitats */
    int i;
    for (i = 0; i < z->num_habitats; ++i) {
        AnimalNode *cur = z->habitats[i].head;
        while (cur) {
            Animal *a = &cur->animal;
            /* Formato: id;nome;especie;dieta;saude;porte;tamanho;habitat */
            fprintf(f, "%d;%s;%s;%s;%s;%c;%d;%d\n",
                    a->id, a->nome, a->especie, a->dieta, a->saude, a->porte, a->tamanho_ocupado, a->habitat_id);
            cur = cur->next;
        }
    }
    /* Também salvar animais na fila veterinaria */
    int i2, idx;
    for (i2 = 0; i2 < z->vet_queue.count; ++i2) {
        idx = (z->vet_queue.front + i2) % VET_QUEUE_CAP;
        Animal *a = z->vet_queue.items[idx];
        if (a) {
            fprintf(f, "%d;%s;%s;%s;%s;%c;%d;%d\n",
                    a->id, a->nome, a->especie, a->dieta, a->saude, a->porte, a->tamanho_ocupado, -1);
        }
    }
    fclose(f);
    return 1;
}

/* Carregar: limpa zoo atual e carrega do arquivo; estratégias simples:
   - Se habitat id >=0 e válido: insere naquele habitat
   - Se habitat id == -1: coloca na fila veterinaria (heap allocated)
*/
int carregar_de_arquivo(Zoo *z, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return 0;
    /* Primeiro: liberar memória atual */
    int i;
    for (i = 0; i < z->num_habitats; ++i) {
        AnimalNode *cur = z->habitats[i].head;
        while (cur) {
            AnimalNode *tmp = cur;
            cur = cur->next;
            free_animal_node(tmp);
        }
        z->habitats[i].head = NULL;
        z->habitats[i].ocupado = 0;
    }
    /* liberar fila veterinaria */
    while (!vet_is_empty(&z->vet_queue)) {
        Animal *a = vet_dequeue(&z->vet_queue);
        if (a) free(a);
    }
    /* agora ler linhas */
    char line[512];
    int max_id_seen = 0;
    while (fgets(line, sizeof(line), f)) {
        /* parse CSV simples */
        Animal a;
        int habitat_id;
        char *p;
        /* remove newline */
        p = strchr(line, '\n'); if (p) *p = 0;
        /* sscanf is okay here but be careful with strings contaning ';' - presumo não haver */
        if (sscanf(line, "%d;%49[^;];%49[^;];%19[^;];%19[^;];%c;%d;%d",
                   &a.id, a.nome, a.especie, a.dieta, a.saude, &a.porte, &a.tamanho_ocupado, &habitat_id) >= 8) {
            if (a.id > max_id_seen) max_id_seen = a.id;
            a.habitat_id = habitat_id;
            if (habitat_id >= 0 && habitat_id < z->num_habitats) {
                add_animal_to_habitat(&z->habitats[habitat_id], a);
            } else {
                /* coloca em fila veterinaria (aloca no heap) */
                Animal *heap_a = (Animal*) malloc(sizeof(Animal));
                if (heap_a) {
                    *heap_a = a;
                    vet_enqueue(&z->vet_queue, heap_a);
                }
            }
        }
    }
    fclose(f);
    z->next_animal_id = max_id_seen + 1;
    return 1;
}

/* ---------- Interface simples para testes ---------- */

void flush_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void menu_loop(Zoo *z) {
    int opt = 0;
    while (1) {
        printf("\n===== ZOO MENU =====\n");
        printf("1 Inserir novo animal\n");
        printf("2 Listar tudo\n");
        printf("3 Transferir animal internamente\n");
        printf("4 Transferir animal para fora (externo)\n");
        printf("5 Medicar (enfileirar) animal\n");
        printf("6 Finalizar atendimento (desenfileirar e recolocar)\n");
        printf("7 Salvar dados\n");
        printf("8 Carregar dados\n");
        printf("9 Sair\n");
        printf("Escolha: ");
        if (scanf("%d", &opt) != 1) { flush_stdin(); continue; }
        if (opt == 1) {
            char nome[MAX_NAME], especie[MAX_SPECIE], dieta[MAX_DIETA], saude[MAX_SAUDE];
            char porte;
            int hid;
            printf("Nome: "); flush_stdin(); fgets(nome, MAX_NAME, stdin); nome[strcspn(nome, "\n")] = 0;
            printf("Especie: "); fgets(especie, MAX_SPECIE, stdin); especie[strcspn(especie, "\n")] = 0;
            printf("Dieta: "); fgets(dieta, MAX_DIETA, stdin); dieta[strcspn(dieta, "\n")] = 0;
            printf("Saude: "); fgets(saude, MAX_SAUDE, stdin); saude[strcspn(saude, "\n")] = 0;
            printf("Porte (P/M/G): "); flush_stdin(); scanf(" %c", &porte);
            printf("Habitat id (0..%d): ", z->num_habitats - 1); scanf("%d", &hid);
            if (inserir_novo_animal(z, hid, nome, especie, dieta, saude, porte)) {
                printf("Animal inserido com sucesso.\n");
            } else {
                printf("Falha ao inserir: possivelmente sem espaco ou habitat invalido.\n");
            }
        } else if (opt == 2) {
            listar_tudo(z);
        } else if (opt == 3) {
            int aid, hid;
            printf("ID do animal: "); scanf("%d", &aid);
            printf("ID do habitat destino: "); scanf("%d", &hid);
            if (transferir_animal_interno(z, aid, hid)) printf("Transferencia OK\n");
            else printf("Falha na transferencia\n");
        } else if (opt == 4) {
            int aid;
            printf("ID do animal a remover externamente: "); scanf("%d", &aid);
            if (transferir_animal_externo(z, aid)) printf("Animal removido do zoo\n");
            else printf("Animal nao encontrado\n");
        } else if (opt == 5) {
            int aid; char novo_estado[MAX_SAUDE];
            printf("ID do animal a medicar (colocar em fila): "); scanf("%d", &aid);
            printf("Novo estado de saude (ex: Quarentena): "); flush_stdin(); fgets(novo_estado, MAX_SAUDE, stdin); novo_estado[strcspn(novo_estado, "\n")] = 0;
            if (medicar_animal(z, aid, novo_estado)) printf("Animal enfileirado para veterinario\n");
            else printf("Falha ao enfileirar (talvez animal nao exista)\n");
        } else if (opt == 6) {
            int hid;
            printf("Habitat destino para recolocar animal: "); scanf("%d", &hid);
            if (finalizar_atendimento(z, hid)) printf("Atendimento finalizado e animal recolocado (se houve espaco)\n");
            else printf("Nenhum animal atendido ou falha ao recolocar\n");
        } else if (opt == 7) {
            if (salvar_para_arquivo(z, FILE_NAME)) printf("Dados salvos em %s\n", FILE_NAME);
            else printf("Erro ao salvar\n");
        } else if (opt == 8) {
            if (carregar_de_arquivo(z, FILE_NAME)) printf("Dados carregados de %s\n", FILE_NAME);
            else printf("Erro ao carregar (arquivo ausente?)\n");
        } else if (opt == 9) {
            printf("Saindo...\n");
            break;
        } else {
            printf("Opcao invalida\n");
        }
    }
}

/* ---------- Exemplo de inicializacao com seus habitats ---------- */
int main(void) {
    Zoo z;
    zoo_init(&z);

    /* Inicialização sugerida conforme sua proposta:
       7 habitats pequenos (capacidade 1), 8 médios (cap 10), 5 grandes (cap 20)
    */
    int i;
    for (i = 0; i < 7; ++i) zoo_add_habitat(&z, 1);
    for (i = 0; i < 8; ++i) zoo_add_habitat(&z, 10);
    for (i = 0; i < 5; ++i) zoo_add_habitat(&z, 20);

    printf("Zoo inicializado com %d habitats.\n", z.num_habitats);

    /* Opcional: inserir alguns animais de teste */
    inserir_novo_animal(&z, 7, "Leao", "Panthera leo", "Carnivoro", "Saudavel", 'G');
    inserir_novo_animal(&z, 8, "Zebra", "Equus quagga", "Herbivoro", "Saudavel", 'M');
    inserir_novo_animal(&z, 1, "Coelho", "Oryctolagus", "Herbivoro", "Saudavel", 'P');

    /* Entrar no menu de testes */
    menu_loop(&z);

    /* Antes de sair: salvar e liberar memória */
    salvar_para_arquivo(&z, FILE_NAME);

    /* Liberar memória das listas e fila */
    for (i = 0; i < z.num_habitats; ++i) {
        AnimalNode *cur = z.habitats[i].head;
        while (cur) {
            AnimalNode *tmp = cur;
            cur = cur->next;
            free_animal_node(tmp);
        }
    }
    while (!vet_is_empty(&z.vet_queue)) {
        Animal *a = vet_dequeue(&z.vet_queue);
        if (a) free(a);
    }
    return 0;
}
