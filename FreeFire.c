#/* ============================================================================
 *  SISTEMA DE INVENTARIO - MOCHILA DE LOOT INICIAL
 * ----------------------------------------------------------------------------
 *  Simula a mochila de itens de um jogador em um jogo de sobrevivencia.
 *  Permite cadastrar, remover, listar e buscar itens (armas, municoes,
 *  kits medicos, ferramentas etc.) usando uma struct e um vetor sequencial.
 *
 *  Conceitos praticados: struct como agregador de dados, vetor de structs
 *  como lista sequencial, modularizacao em funcoes e busca sequencial.
 * ==========================================================================*/

#include <stdio.h>   // Entrada e saida padrao (printf, scanf, fgets)
#include <string.h>  // Manipulacao de strings (strcmp, strcspn)
#include <stdlib.h>  // Funcoes utilitarias da biblioteca padrao

/* ---------- Capacidade maxima da mochila ---------- */
#define MAX_ITENS 10

/* ============================================================================
 *  Struct Item
 *  Agrupa, em um unico registro, as informacoes essenciais de cada objeto
 *  coletado pelo jogador.
 * ==========================================================================*/
typedef struct {
    char nome[30];    // Nome do item (ex: "Pistola", "Bandagem")
    char tipo[20];    // Tipo do item (ex: "arma", "municao", "cura")
    int  quantidade;  // Quantidade desse item na mochila
} Item;

/* ---------- Prototipos das funcoes obrigatorias e auxiliares ---------- */
void inserirItem(Item mochila[], int* totalItens);
void removerItem(Item mochila[], int* totalItens);
void listarItens(Item mochila[], int totalItens);
void buscarItem(Item mochila[], int totalItens);
void lerTexto(char* destino, int tamanho);
void limparBufferEntrada(void);

/* ============================================================================
 *  lerTexto
 *  Le uma linha de texto com seguranca usando fgets e remove o '\n' final.
 *  Evita problemas de strings com espacos e estouro de buffer.
 * ==========================================================================*/
void lerTexto(char* destino, int tamanho) {
    fgets(destino, tamanho, stdin);
    destino[strcspn(destino, "\n")] = '\0';  // troca a quebra de linha por fim de string
}

/* ============================================================================
 *  limparBufferEntrada
 *  Descarta caracteres residuais (inclusive o '\n') deixados no buffer pelo
 *  scanf, evitando que eles atrapalhem a proxima leitura de texto.
 * ==========================================================================*/
void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* apenas consome os caracteres restantes */
    }
}

/* ============================================================================
 *  inserirItem
 *  Cadastra um novo item na mochila, caso ainda haja espaco.
 *  Recebe o total de itens POR REFERENCIA (ponteiro), pois precisa atualiza-lo.
 * ==========================================================================*/
void inserirItem(Item mochila[], int* totalItens) {
    if (*totalItens >= MAX_ITENS) {
        printf("\n[!] A mochila esta cheia (%d/%d itens). Remova algo antes de adicionar.\n",
               *totalItens, MAX_ITENS);
        return;
    }

    Item novo;  // item temporario que sera preenchido e adicionado

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
    limparBufferEntrada();  // limpa o '\n' deixado pelo scanf

    mochila[*totalItens] = novo;  // armazena o item na proxima posicao livre
    (*totalItens)++;              // atualiza a quantidade de itens cadastrados

    printf("[OK] Item \"%s\" adicionado a mochila!\n", novo.nome);
}

/* ============================================================================
 *  removerItem
 *  Remove um item da mochila pelo nome. Ao encontra-lo, desloca os itens
 *  seguintes uma posicao para tras, mantendo o vetor sequencial sem "buracos".
 *  Recebe o total de itens POR REFERENCIA para poder atualiza-lo.
 * ==========================================================================*/
void removerItem(Item mochila[], int* totalItens) {
    if (*totalItens == 0) {
        printf("\n[!] A mochila esta vazia. Nao ha o que remover.\n");
        return;
    }

    char nomeBusca[30];
    printf("\nDigite o nome do item a remover: ");
    lerTexto(nomeBusca, sizeof(nomeBusca));

    int indice = -1;  // posicao do item encontrado (-1 = nao encontrado)
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

    /* Desloca os itens posteriores para preencher o espaco do item removido */
    for (int i = indice; i < *totalItens - 1; i++) {
        mochila[i] = mochila[i + 1];
    }
    (*totalItens)--;  // reduz a contagem de itens

    printf("[OK] Item \"%s\" removido da mochila!\n", nomeBusca);
}

/* ============================================================================
 *  listarItens
 *  Exibe todos os itens cadastrados na mochila em formato de tabela.
 *  Chamada apos cada operacao para o jogador acompanhar o estado da mochila.
 * ==========================================================================*/
void listarItens(Item mochila[], int totalItens) {
    printf("\n========== ITENS NA MOCHILA (%d/%d) ==========\n", totalItens, MAX_ITENS);

    if (totalItens == 0) {
        printf("A mochila esta vazia.\n");
        printf("=============================================\n");
        return;
    }

    printf("%-3s %-20s %-12s %s\n", "#", "NOME", "TIPO", "QTD");
    printf("---------------------------------------------\n");
    for (int i = 0; i < totalItens; i++) {
        printf("%-3d %-20s %-12s %d\n",
               i + 1, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
    printf("=============================================\n");
}

/* ============================================================================
 *  buscarItem
 *  Realiza uma BUSCA SEQUENCIAL: percorre o vetor do inicio ao fim comparando
 *  o nome de cada item com o nome procurado. Ao encontrar, exibe seus dados.
 * ==========================================================================*/
void buscarItem(Item mochila[], int totalItens) {
    if (totalItens == 0) {
        printf("\n[!] A mochila esta vazia. Nada para buscar.\n");
        return;
    }

    char nomeBusca[30];
    printf("\nDigite o nome do item a buscar: ");
    lerTexto(nomeBusca, sizeof(nomeBusca));

    /* Busca sequencial: verifica posicao por posicao */
    for (int i = 0; i < totalItens; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            printf("\n[ENCONTRADO] na posicao %d:\n", i + 1);
            printf("  Nome......: %s\n", mochila[i].nome);
            printf("  Tipo......: %s\n", mochila[i].tipo);
            printf("  Quantidade: %d\n", mochila[i].quantidade);
            return;  // encerra apos encontrar
        }
    }

    printf("[!] Item \"%s\" nao foi encontrado na mochila.\n", nomeBusca);
}

/* ============================================================================
 *  main
 *  Controla o fluxo do programa por meio de um menu interativo, repetido em
 *  laco ate que o jogador escolha sair.
 * ==========================================================================*/
int main(void) {
    Item mochila[MAX_ITENS];  // vetor de structs (lista sequencial de itens)
    int totalItens = 0;       // quantidade de itens atualmente cadastrados
    int opcao;                // opcao escolhida no menu

    printf("=============================================\n");
    printf("     MOCHILA DE LOOT - INVENTARIO INICIAL     \n");
    printf("=============================================\n");

    do {
        /* Exibicao do menu de opcoes */
        printf("\n--------------- MENU ---------------\n");
        printf("1 - Cadastrar item\n");
        printf("2 - Remover item\n");
        printf("3 - Listar itens\n");
        printf("4 - Buscar item\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");

        /* Leitura segura da opcao do menu */
        if (scanf("%d", &opcao) != 1) {
            limparBufferEntrada();
            printf("\n[!] Opcao invalida. Digite um numero.\n");
            opcao = -1;  // forca o laco a continuar
            continue;
        }
        limparBufferEntrada();  // limpa o '\n' apos a leitura do numero

        switch (opcao) {
            case 1:
                inserirItem(mochila, &totalItens);
                listarItens(mochila, totalItens);  // lista apos a operacao
                break;
            case 2:
                removerItem(mochila, &totalItens);
                listarItens(mochila, totalItens);  // lista apos a operacao
                break;
            case 3:
                listarItens(mochila, totalItens);
                break;
            case 4:
                buscarItem(mochila, totalItens);
                break;
            case 0:
                printf("\nSaindo do inventario. Boa sorte na partida!\n");
                break;
            default:
                printf("\n[!] Opcao inexistente. Escolha entre 0 e 4.\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}