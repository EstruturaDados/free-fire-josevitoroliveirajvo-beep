/* ============================================================================
 *  SISTEMA DE INVENTARIO - MOCHILA DE LOOT (NIVEL MESTRE)
 * ----------------------------------------------------------------------------
 *  Simula a mochila de itens de um jogador em um jogo de sobrevivencia.
 *  Permite cadastrar, remover, listar, ORDENAR e BUSCAR itens.
 *
 *  DESTAQUES DESTE NIVEL:
 *   - Cada item agora possui um campo PRIORIDADE (1 a 5).
 *   - A mochila pode ser ORDENADA por nome, tipo ou prioridade usando o
 *     algoritmo Insertion Sort, com um CONTADOR DE COMPARACOES para analise
 *     de desempenho.
 *   - Foi adicionada a BUSCA BINARIA por nome, mais rapida que a sequencial,
 *     mas que exige a lista previamente ordenada por nome (validado por uma
 *     flag booleana de estado).
 *
 *  Conceitos praticados: struct, vetor de structs, enum (criterios de
 *  ordenacao), tipo bool, Insertion Sort, busca binaria, busca sequencial,
 *  strcmp e analise de desempenho por contagem de comparacoes.
 * ==========================================================================*/

#include <stdio.h>    // Entrada e saida padrao (printf, scanf, fgets)
#include <string.h>   // Manipulacao de strings (strcmp, strcspn)
#include <stdlib.h>   // Funcoes utilitarias da biblioteca padrao
#include <stdbool.h>  // Tipo booleano (bool, true, false)

/* ---------- Capacidade maxima da mochila ---------- */
#define MAX_ITENS 10

/* ============================================================================
 *  enum CriterioOrdenacao
 *  Enumera, de forma legivel, os criterios pelos quais a mochila pode ser
 *  ordenada. Usar enum no lugar de "numeros soltos" deixa o codigo mais claro.
 * ==========================================================================*/
typedef enum {
    ORDENAR_POR_NOME = 1,
    ORDENAR_POR_TIPO,
    ORDENAR_POR_PRIORIDADE
} CriterioOrdenacao;

/* ============================================================================
 *  Struct Item
 *  Agrupa as informacoes essenciais de cada objeto coletado pelo jogador.
 *  NOVIDADE: campo "prioridade" (1 = menos importante, 5 = mais importante).
 * ==========================================================================*/
typedef struct {
    char nome[30];    // Nome do item (ex: "Pistola", "Bandagem")
    char tipo[20];    // Tipo do item (ex: "arma", "municao", "cura")
    int  quantidade;  // Quantidade desse item na mochila
    int  prioridade;  // Prioridade estrategica do item (1 a 5)
} Item;

/* ---------- Prototipos das funcoes ---------- */
void inserirItem(Item mochila[], int* totalItens, bool* ordenadaPorNome);
void removerItem(Item mochila[], int* totalItens, bool* ordenadaPorNome);
void listarItens(Item mochila[], int totalItens);
void buscarSequencial(Item mochila[], int totalItens);
void ordenarItens(Item mochila[], int totalItens, bool* ordenadaPorNome);
void buscarBinaria(Item mochila[], int totalItens, bool ordenadaPorNome);
void lerTexto(char* destino, int tamanho);
void limparBufferEntrada(void);

/* ============================================================================
 *  lerTexto / limparBufferEntrada
 *  Funcoes auxiliares para leitura segura de strings e limpeza do buffer.
 * ==========================================================================*/
void lerTexto(char* destino, int tamanho) {
    fgets(destino, tamanho, stdin);
    destino[strcspn(destino, "\n")] = '\0';  // remove a quebra de linha final
}

void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* consome caracteres residuais */
    }
}

/* ============================================================================
 *  inserirItem
 *  Cadastra um novo item (agora tambem com prioridade de 1 a 5).
 *  Recebe POR REFERENCIA o total de itens e a flag "ordenadaPorNome":
 *  ao inserir, a ordenacao por nome se perde, entao a flag volta a false.
 * ==========================================================================*/
void inserirItem(Item mochila[], int* totalItens, bool* ordenadaPorNome) {
    if (*totalItens >= MAX_ITENS) {
        printf("\n[!] A mochila esta cheia (%d/%d itens).\n", *totalItens, MAX_ITENS);
        return;
    }

    Item novo;
    printf("\n--- Cadastro de novo item ---\n");

    printf("Nome do item: ");
    lerTexto(novo.nome, sizeof(novo.nome));

    printf("Tipo (ex: arma, municao, cura, ferramenta): ");
    lerTexto(novo.tipo, sizeof(novo.tipo));

    printf("Quantidade: ");
    while (scanf("%d", &novo.quantidade) != 1 || novo.quantidade < 0) {
        printf("Valor invalido. Digite um numero inteiro positivo: ");
        limparBufferEntrada();
    }

    printf("Prioridade (1 a 5): ");
    while (scanf("%d", &novo.prioridade) != 1 || novo.prioridade < 1 || novo.prioridade > 5) {
        printf("Prioridade invalida. Digite um numero de 1 a 5: ");
        limparBufferEntrada();
    }
    limparBufferEntrada();

    mochila[*totalItens] = novo;
    (*totalItens)++;
    *ordenadaPorNome = false;  // a insercao quebra a ordenacao por nome

    printf("[OK] Item \"%s\" adicionado a mochila!\n", novo.nome);
}

/* ============================================================================
 *  removerItem
 *  Remove um item pelo nome e desloca os demais para nao deixar buracos.
 *  Remover por nome mantem a ordenacao existente, mas por seguranca a flag
 *  e tratada pela funcao chamadora apenas em insercao/ordenacao.
 * ==========================================================================*/
void removerItem(Item mochila[], int* totalItens, bool* ordenadaPorNome) {
    if (*totalItens == 0) {
        printf("\n[!] A mochila esta vazia. Nao ha o que remover.\n");
        return;
    }

    char nomeBusca[30];
    printf("\nDigite o nome do item a remover: ");
    lerTexto(nomeBusca, sizeof(nomeBusca));

    int indice = -1;
    for (int i = 0; i < *totalItens; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("[!] Item \"%s\" nao encontrado na mochila.\n", nomeBusca);
        return;
    }

    for (int i = indice; i < *totalItens - 1; i++) {
        mochila[i] = mochila[i + 1];
    }
    (*totalItens)--;
    /* A remocao preserva a ordem relativa dos itens restantes;
       portanto, se ja estava ordenada por nome, continua ordenada. */
    (void)ordenadaPorNome;

    printf("[OK] Item \"%s\" removido da mochila!\n", nomeBusca);
}

/* ============================================================================
 *  listarItens
 *  Exibe todos os itens da mochila em formato de tabela, incluindo a
 *  prioridade. Chamada apos cada operacao relevante.
 * ==========================================================================*/
void listarItens(Item mochila[], int totalItens) {
    printf("\n========== ITENS NA MOCHILA (%d/%d) ==========\n", totalItens, MAX_ITENS);

    if (totalItens == 0) {
        printf("A mochila esta vazia.\n");
        printf("==================================================\n");
        return;
    }

    printf("%-3s %-20s %-12s %-5s %s\n", "#", "NOME", "TIPO", "QTD", "PRIOR");
    printf("--------------------------------------------------\n");
    for (int i = 0; i < totalItens; i++) {
        printf("%-3d %-20s %-12s %-5d %d\n",
               i + 1, mochila[i].nome, mochila[i].tipo,
               mochila[i].quantidade, mochila[i].prioridade);
    }
    printf("==================================================\n");
}

/* ============================================================================
 *  buscarSequencial
 *  BUSCA SEQUENCIAL por nome (do nivel anterior): percorre todo o vetor
 *  comparando com strcmp. Usa flag para indicar se encontrou. Funciona
 *  independentemente de a lista estar ou nao ordenada.
 * ==========================================================================*/
void buscarSequencial(Item mochila[], int totalItens) {
    if (totalItens == 0) {
        printf("\n[!] A mochila esta vazia. Nada para buscar.\n");
        return;
    }

    char nomeBusca[30];
    printf("\nDigite o nome do item a buscar (sequencial): ");
    lerTexto(nomeBusca, sizeof(nomeBusca));

    bool encontrado = false;
    int posicao = -1;

    for (int i = 0; i < totalItens; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            encontrado = true;
            posicao = i;
            break;
        }
    }

    if (encontrado) {
        printf("\n[ENCONTRADO] na posicao %d:\n", posicao + 1);
        printf("  Nome......: %s\n", mochila[posicao].nome);
        printf("  Tipo......: %s\n", mochila[posicao].tipo);
        printf("  Quantidade: %d\n", mochila[posicao].quantidade);
        printf("  Prioridade: %d\n", mochila[posicao].prioridade);
    } else {
        printf("\n[!] Item \"%s\" nao foi encontrado na mochila.\n", nomeBusca);
    }
}

/* ============================================================================
 *  ordenarItens
 *  Ordena a mochila pelo criterio escolhido (nome, tipo ou prioridade)
 *  usando o algoritmo INSERTION SORT.
 *
 *  Mantem um CONTADOR DE COMPARACOES para analise de desempenho e atualiza
 *  a flag booleana "ordenadaPorNome" (pre-requisito da busca binaria).
 * ==========================================================================*/
void ordenarItens(Item mochila[], int totalItens, bool* ordenadaPorNome) {
    if (totalItens < 2) {
        printf("\n[!] E preciso ter ao menos 2 itens para ordenar.\n");
        return;
    }

    printf("\n--- Ordenar mochila ---\n");
    printf("1 - Por nome\n");
    printf("2 - Por tipo\n");
    printf("3 - Por prioridade\n");
    printf("Escolha o criterio: ");

    int criterio;
    if (scanf("%d", &criterio) != 1) {
        limparBufferEntrada();
        printf("[!] Criterio invalido.\n");
        return;
    }
    limparBufferEntrada();

    if (criterio < ORDENAR_POR_NOME || criterio > ORDENAR_POR_PRIORIDADE) {
        printf("[!] Criterio inexistente. Escolha entre 1 e 3.\n");
        return;
    }

    long comparacoes = 0;  // contador de comparacoes realizadas

    /* Insertion Sort: a cada passo, insere o elemento atual na posicao
       correta dentro da parte ja ordenada do vetor (a esquerda). */
    for (int i = 1; i < totalItens; i++) {
        Item chave = mochila[i];
        int j = i - 1;

        /* Avanca para a esquerda enquanto o item anterior for "maior"
           que a chave, segundo o criterio escolhido. */
        while (j >= 0) {
            int maior = 0;  // 1 se mochila[j] deve vir depois da chave

            comparacoes++;  // conta cada comparacao entre dois itens
            if (criterio == ORDENAR_POR_NOME) {
                maior = strcmp(mochila[j].nome, chave.nome) > 0;
            } else if (criterio == ORDENAR_POR_TIPO) {
                maior = strcmp(mochila[j].tipo, chave.tipo) > 0;
            } else { /* ORDENAR_POR_PRIORIDADE (maior prioridade primeiro) */
                maior = mochila[j].prioridade < chave.prioridade;
            }

            if (!maior) {
                break;  // posicao correta encontrada
            }
            mochila[j + 1] = mochila[j];  // desloca o item para a direita
            j--;
        }
        mochila[j + 1] = chave;  // posiciona a chave no lugar certo
    }

    /* A busca binaria so vale quando a lista esta ordenada por NOME */
    *ordenadaPorNome = (criterio == ORDENAR_POR_NOME);

    const char* nomeCriterio =
        (criterio == ORDENAR_POR_NOME)       ? "nome" :
        (criterio == ORDENAR_POR_TIPO)       ? "tipo" : "prioridade";

    printf("[OK] Mochila ordenada por %s.\n", nomeCriterio);
    printf("[DESEMPENHO] Comparacoes realizadas: %ld\n", comparacoes);
}

/* ============================================================================
 *  buscarBinaria
 *  BUSCA BINARIA por nome. Muito mais rapida que a sequencial, mas exige que
 *  a lista esteja ordenada por nome. Por isso, recebe a flag "ordenadaPorNome"
 *  e so executa se o pre-requisito estiver satisfeito.
 *
 *  Algoritmo: compara a chave com o elemento do meio; se for menor, busca na
 *  metade da esquerda; se for maior, na metade da direita; ate achar ou
 *  esgotar o intervalo.
 * ==========================================================================*/
void buscarBinaria(Item mochila[], int totalItens, bool ordenadaPorNome) {
    if (totalItens == 0) {
        printf("\n[!] A mochila esta vazia. Nada para buscar.\n");
        return;
    }

    /* Validacao do pre-requisito usando a flag booleana */
    if (!ordenadaPorNome) {
        printf("\n[!] A busca binaria exige a mochila ORDENADA POR NOME.\n");
        printf("    Use a opcao de ordenacao (criterio: nome) antes de buscar.\n");
        return;
    }

    char nomeBusca[30];
    printf("\nDigite o nome do item a buscar (binaria): ");
    lerTexto(nomeBusca, sizeof(nomeBusca));

    int inicio = 0;
    int fim = totalItens - 1;
    int posicao = -1;
    long comparacoes = 0;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        comparacoes++;
        int cmp = strcmp(mochila[meio].nome, nomeBusca);

        if (cmp == 0) {
            posicao = meio;  // achou
            break;
        } else if (cmp < 0) {
            inicio = meio + 1;  // procura na metade da direita
        } else {
            fim = meio - 1;     // procura na metade da esquerda
        }
    }

    if (posicao != -1) {
        printf("\n[ENCONTRADO] na posicao %d (apos %ld comparacoes):\n",
               posicao + 1, comparacoes);
        printf("  Nome......: %s\n", mochila[posicao].nome);
        printf("  Tipo......: %s\n", mochila[posicao].tipo);
        printf("  Quantidade: %d\n", mochila[posicao].quantidade);
        printf("  Prioridade: %d\n", mochila[posicao].prioridade);
    } else {
        printf("\n[!] Item \"%s\" nao foi encontrado (apos %ld comparacoes).\n",
               nomeBusca, comparacoes);
    }
}

/* ============================================================================
 *  main
 *  Controla o fluxo do programa por meio de um menu interativo.
 *  A flag "ordenadaPorNome" guarda o estado da mochila para liberar ou nao
 *  a busca binaria.
 * ==========================================================================*/
int main(void) {
    Item mochila[MAX_ITENS];
    int  totalItens = 0;
    bool ordenadaPorNome = false;  // estado: a mochila esta ordenada por nome?
    int  opcao;

    printf("==================================================\n");
    printf("       MOCHILA DE LOOT - INVENTARIO (MESTRE)       \n");
    printf("==================================================\n");

    do {
        printf("\n----------------- MENU -----------------\n");
        printf("1 - Cadastrar item (com prioridade)\n");
        printf("2 - Remover item\n");
        printf("3 - Listar itens\n");
        printf("4 - Ordenar itens (nome/tipo/prioridade)\n");
        printf("5 - Buscar item (sequencial)\n");
        printf("6 - Buscar item (binaria, por nome)\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            limparBufferEntrada();
            printf("\n[!] Opcao invalida. Digite um numero.\n");
            opcao = -1;
            continue;
        }
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                inserirItem(mochila, &totalItens, &ordenadaPorNome);
                listarItens(mochila, totalItens);
                break;
            case 2:
                removerItem(mochila, &totalItens, &ordenadaPorNome);
                listarItens(mochila, totalItens);
                break;
            case 3:
                listarItens(mochila, totalItens);
                break;
            case 4:
                ordenarItens(mochila, totalItens, &ordenadaPorNome);
                listarItens(mochila, totalItens);
                break;
            case 5:
                buscarSequencial(mochila, totalItens);
                break;
            case 6:
                buscarBinaria(mochila, totalItens, ordenadaPorNome);
                break;
            case 0:
                printf("\nSaindo do inventario. Boa sorte na ilha!\n");
                break;
            default:
                printf("\n[!] Opcao inexistente. Escolha entre 0 e 6.\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}