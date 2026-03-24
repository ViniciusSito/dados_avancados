/*
 * Cadastro de livros em Arvore Binaria de Busca (ABB).
 * Chave de ordenacao: nome do livro (ordem lexicografica com strcmp).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_NOME 256
#define BUF_AUTOR 256

typedef struct NoLivro {
    char* nome;
    char* autor;
    double preco;
    int quantidade;
    struct NoLivro* esquerda;
    struct NoLivro* direita;
} NoLivro;

static void trim_newline(char* s) {
    size_t n = strlen(s);
    if (n > 0 && s[n - 1] == '\n') {
        s[n - 1] = '\0';
    }
}

static char* duplicar_string(const char* s) {
    size_t n = strlen(s) + 1;
    char* p = (char*)malloc(n);
    if (p == NULL) {
        return NULL;
    }
    memcpy(p, s, n);
    return p;
}

static NoLivro* criar_no(const char* nome, const char* autor, double preco, int quantidade) {
    NoLivro* n = (NoLivro*)malloc(sizeof(NoLivro));
    if (n == NULL) {
        return NULL;
    }
    n->nome = duplicar_string(nome);
    n->autor = duplicar_string(autor);
    if (n->nome == NULL || n->autor == NULL) {
        free(n->nome);
        free(n->autor);
        free(n);
        return NULL;
    }
    n->preco = preco;
    n->quantidade = quantidade;
    n->esquerda = NULL;
    n->direita = NULL;
    return n;
}

static void liberar_no(NoLivro* n) {
    if (n == NULL) {
        return;
    }
    free(n->nome);
    free(n->autor);
    free(n);
}

void liberar_arvore(NoLivro* raiz) {
    if (raiz == NULL) {
        return;
    }
    liberar_arvore(raiz->esquerda);
    liberar_arvore(raiz->direita);
    liberar_no(raiz);
}

/* Insere livro; nomes duplicados nao sao aceitos (retorna raiz inalterada). */
NoLivro* inserir_livro(NoLivro* raiz, const char* nome, const char* autor, double preco, int quantidade) {
    if (raiz == NULL) {
        return criar_no(nome, autor, preco, quantidade);
    }
    int cmp = strcmp(nome, raiz->nome);
    if (cmp < 0) {
        raiz->esquerda = inserir_livro(raiz->esquerda, nome, autor, preco, quantidade);
    } else if (cmp > 0) {
        raiz->direita = inserir_livro(raiz->direita, nome, autor, preco, quantidade);
    }
    /* cmp == 0: ja existe livro com esse nome */
    return raiz;
}

NoLivro* buscar_livro(NoLivro* raiz, const char* nome) {
    if (raiz == NULL) {
        return NULL;
    }
    int cmp = strcmp(nome, raiz->nome);
    if (cmp < 0) {
        return buscar_livro(raiz->esquerda, nome);
    }
    if (cmp > 0) {
        return buscar_livro(raiz->direita, nome);
    }
    return raiz;
}

/* Percurso in-ordem: esquerda -> raiz -> direita (lista nomes em ordem alfabetica). */
void listar_em_ordem(NoLivro* raiz) {
    if (raiz == NULL) {
        return;
    }
    listar_em_ordem(raiz->esquerda);
    printf("  %-40s | %-24s | R$ %8.2f | %4d un.\n",
           raiz->nome, raiz->autor, raiz->preco, raiz->quantidade);
    listar_em_ordem(raiz->direita);
}

static int alterar_preco(NoLivro* raiz, const char* nome, double novo_preco) {
    NoLivro* n = buscar_livro(raiz, nome);
    if (n == NULL) {
        return 0;
    }
    n->preco = novo_preco;
    return 1;
}

static int alterar_quantidade(NoLivro* raiz, const char* nome, int nova_qtd) {
    NoLivro* n = buscar_livro(raiz, nome);
    if (n == NULL) {
        return 0;
    }
    n->quantidade = nova_qtd;
    return 1;
}

static void imprimir_cabecalho_lista(void) {
    printf("  %-40s | %-24s | %-10s | %s\n", "Nome", "Autor", "Preco", "Qtd");
    printf("  ");
    for (int i = 0; i < 100; i++) {
        putchar('-');
    }
    putchar('\n');
}

static void ler_linha(const char* prompt, char* buf, size_t cap) {
    printf("%s", prompt);
    if (fgets(buf, (int)cap, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }
    trim_newline(buf);
}

int main(void) {
    NoLivro* raiz = NULL;
    char buf_nome[BUF_NOME];
    char buf_autor[BUF_AUTOR];
    int opcao;

    printf("=== Cadastro de livros (ABB por nome) ===\n");

    for (;;) {
        printf("\n1) Inserir livro\n");
        printf("2) Buscar livro\n");
        printf("3) Alterar preco\n");
        printf("4) Alterar quantidade\n");
        printf("5) Listar livros (in-ordem)\n");
        printf("0) Sair\n");
        printf("Opcao: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida.\n");
            return 1;
        }
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
            /* descarta resto da linha apos o numero */
        }

        switch (opcao) {
        case 1:
            ler_linha("Nome do livro: ", buf_nome, sizeof buf_nome);
            if (buf_nome[0] == '\0') {
                printf("Nome vazio; insercao cancelada.\n");
                break;
            }
            ler_linha("Autor: ", buf_autor, sizeof buf_autor);
            {
                double preco;
                int qtd;
                printf("Preco (ex: 29.90): ");
                if (scanf("%lf", &preco) != 1) {
                    printf("Preco invalido.\n");
                    while (getchar() != '\n') {
                    }
                    break;
                }
                printf("Quantidade: ");
                if (scanf("%d", &qtd) != 1) {
                    printf("Quantidade invalida.\n");
                    while (getchar() != '\n') {
                    }
                    break;
                }
                while (getchar() != '\n') {
                }

                if (buscar_livro(raiz, buf_nome) != NULL) {
                    printf("Ja existe livro com o nome \"%s\".\n", buf_nome);
                    break;
                }
                NoLivro* nova_raiz = inserir_livro(raiz, buf_nome, buf_autor, preco, qtd);
                if (nova_raiz == NULL || buscar_livro(nova_raiz, buf_nome) == NULL) {
                    printf("Falha ao inserir (memoria insuficiente?).\n");
                    break;
                }
                raiz = nova_raiz;
                printf("Livro cadastrado.\n");
            }
            break;

        case 2:
            ler_linha("Nome do livro: ", buf_nome, sizeof buf_nome);
            {
                NoLivro* n = buscar_livro(raiz, buf_nome);
                if (n == NULL) {
                    printf("Livro nao encontrado.\n");
                } else {
                    printf("Encontrado:\n");
                    printf("  Nome:       %s\n", n->nome);
                    printf("  Autor:      %s\n", n->autor);
                    printf("  Preco:      R$ %.2f\n", n->preco);
                    printf("  Quantidade: %d\n", n->quantidade);
                }
            }
            break;

        case 3:
            ler_linha("Nome do livro: ", buf_nome, sizeof buf_nome);
            {
                double novo_preco;
                printf("Novo preco: ");
                if (scanf("%lf", &novo_preco) != 1) {
                    printf("Preco invalido.\n");
                    while (getchar() != '\n') {
                    }
                    break;
                }
                while (getchar() != '\n') {
                }
                if (!alterar_preco(raiz, buf_nome, novo_preco)) {
                    printf("Livro nao encontrado.\n");
                } else {
                    printf("Preco atualizado.\n");
                }
            }
            break;

        case 4:
            ler_linha("Nome do livro: ", buf_nome, sizeof buf_nome);
            {
                int nova_qtd;
                printf("Nova quantidade: ");
                if (scanf("%d", &nova_qtd) != 1) {
                    printf("Quantidade invalida.\n");
                    while (getchar() != '\n') {
                    }
                    break;
                }
                while (getchar() != '\n') {
                }
                if (!alterar_quantidade(raiz, buf_nome, nova_qtd)) {
                    printf("Livro nao encontrado.\n");
                } else {
                    printf("Quantidade atualizada.\n");
                }
            }
            break;

        case 5:
            if (raiz == NULL) {
                printf("Nenhum livro cadastrado.\n");
            } else {
                printf("\nListagem (in-ordem / ordem alfabetica do nome):\n");
                imprimir_cabecalho_lista();
                listar_em_ordem(raiz);
            }
            break;

        case 0:
            liberar_arvore(raiz);
            printf("Ate logo.\n");
            return 0;

        default:
            printf("Opcao invalida.\n");
            break;
        }
    }
}
