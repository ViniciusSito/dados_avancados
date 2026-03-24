/*
 * A ideia e simples: em vez de guardar tudo numa lista e percorrer do comeco
 * ao fim na busca, a arvore "encaminha" voce para o lado certo (esquerda ou
 * direita) conforme a matricula — assim a busca tende a ser bem mais rapida
 * quando ha muitos cadastros.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_TEXTO 256

/*
 * Cada caixinha da arvore e um funcionario. Os ponteiros left e right sao
 * os dois filhos possiveis na ABP (conforme o enunciado do trabalho).
 */
typedef struct NoFuncionario {
    int matricula;
    char* nome;
    char* cargo;
    double salario;
    struct NoFuncionario* left;
    struct NoFuncionario* right;
} NoFuncionario;

/*
 * Quando lemos texto com fgets, o Enter do usuario fica guardado como '\n'
 * no final da string. Essa funcao tira esse caractere para nao aparecer
 * estranho na tela nem atrapalhar comparacoes.
 */
static void trim_newline(char* s) {
    size_t n = strlen(s);
    if (n > 0 && s[n - 1] == '\n') {
        s[n - 1] = '\0';
    }
}

/*
 * Os buffers do teclado sao reutilizados; por isso copiamos o texto para a
 * heap com malloc. Assim cada no "e dono" da sua propria string e nada se
 * perde quando o buffer e lido de novo.
 */
static char* duplicar_string(const char* s) {
    size_t n = strlen(s) + 1;
    char* p = (char*)malloc(n);
    if (p == NULL) {
        return NULL;
    }
    memcpy(p, s, n);
    return p;
}

/*
 * Monta um novo funcionario na memoria: aloca o no, copia nome e cargo,
 * preenche salario e deixa os dois filhos sem apontar para lugar nenhum
 * (arvore cresce a partir das folhas).
 */
static NoFuncionario* criar_no(int matricula, const char* nome, const char* cargo, double salario) {
    NoFuncionario* n = (NoFuncionario*)malloc(sizeof(NoFuncionario));
    if (n == NULL) {
        return NULL;
    }
    n->matricula = matricula;
    n->nome = duplicar_string(nome);
    n->cargo = duplicar_string(cargo);
    if (n->nome == NULL || n->cargo == NULL) {
        free(n->nome);
        free(n->cargo);
        free(n);
        return NULL;
    }
    n->salario = salario;
    n->left = NULL;
    n->right = NULL;
    return n;
}

/* Libera so um no e as strings que ele guarda (usado ao derrubar a arvore inteira). */
static void liberar_no(NoFuncionario* n) {
    if (n == NULL) {
        return;
    }
    free(n->nome);
    free(n->cargo);
    free(n);
}

/*
 * Importante: antes de fechar o programa, visitamos todos os nos (pos-ordem
 * implicita: primeiro os filhos, depois a raiz) para nao deixar memoria vazando.
 */
void liberar_arvore(NoFuncionario* raiz) {
    if (raiz == NULL) {
        return;
    }
    liberar_arvore(raiz->left);
    liberar_arvore(raiz->right);
    liberar_no(raiz);
}

/*
 * se a matricula e menor que a da raiz atual, descemos pela
 * esquerda; se for maior, pela direita. Quando achamos um "buraco" (NULL),
 * criamos o novo funcionario ali.
 *
 * Se a matricula for igual a de alguem que ja esta na arvore, nao fazemos
 * nada — matricula duplicada nao entra (senao a arvore perde o sentido).
 *
 * Retorno: sempre devolve o ponteiro que o pai deve usar (costuma ser a
 * mesma raiz, exceto quando a arvore estava vazia e o primeiro no vira raiz).
 */
NoFuncionario* inserir_funcionario(NoFuncionario* raiz, int matricula, const char* nome,
                                   const char* cargo, double salario) {
    if (raiz == NULL) {
        return criar_no(matricula, nome, cargo, salario);
    }
    if (matricula < raiz->matricula) {
        raiz->left = inserir_funcionario(raiz->left, matricula, nome, cargo, salario);
    } else if (matricula > raiz->matricula) {
        raiz->right = inserir_funcionario(raiz->right, matricula, nome, cargo, salario);
    }
    return raiz;
}

/*
 * Mesma logica da insercao, mas sem criar nada: a cada passo decidimos se
 * vamos para a esquerda ou direita ate achar a matricula ou bater em NULL
 * (quer dizer que nao existe cadastro com esse numero).
 */
NoFuncionario* buscar_funcionario(NoFuncionario* raiz, int matricula) {
    if (raiz == NULL) {
        return NULL;
    }
    if (matricula < raiz->matricula) {
        return buscar_funcionario(raiz->left, matricula);
    }
    if (matricula > raiz->matricula) {
        return buscar_funcionario(raiz->right, matricula);
    }
    return raiz;
}

/*
 * Primeiro achamos o funcionario pela matricula. A matricula em si nao muda
 * (e a "etiqueta" dele na arvore); atualizamos nome, cargo e salario.
 *
 * Criamos novas copias das strings antes de apagar as antigas — assim, se
 * der erro de memoria no meio, ainda temos os dados velhos inteiros.
 *
 * Retorna 1 se deu certo, 0 se nao achou a matricula ou faltou memoria.
 */
int atualizar_funcionario(NoFuncionario* raiz, int matricula, const char* novo_nome,
                          const char* novo_cargo, double novo_salario) {
    NoFuncionario* n = buscar_funcionario(raiz, matricula);
    if (n == NULL) {
        return 0;
    }
    char* nn = duplicar_string(novo_nome);
    char* nc = duplicar_string(novo_cargo);
    if (nn == NULL || nc == NULL) {
        free(nn);
        free(nc);
        return 0;
    }
    free(n->nome);
    free(n->cargo);
    n->nome = nn;
    n->cargo = nc;
    n->salario = novo_salario;
    return 1;
}

/*
 * Percurso "em ordem" (in-order): visita toda a subarvore esquerda, imprime
 * o no atual, depois a direita. Em uma ABP por matricula, isso mostra os
 * funcionarios do menor numero para o maior — uma listagem natural e legivel.
 */
void listar_em_ordem(NoFuncionario* raiz) {
    if (raiz == NULL) {
        return;
    }
    listar_em_ordem(raiz->left);
    printf("  %6d | %-28s | %-20s | R$ %10.2f\n",
           raiz->matricula, raiz->nome, raiz->cargo, raiz->salario);
    listar_em_ordem(raiz->right);
}

/* So deixa a tabela bonitinha na hora de listar todo mundo. */
static void imprimir_cabecalho_lista(void) {
    printf("  %6s | %-28s | %-20s | %s\n", "Matr.", "Nome", "Cargo", "Salario");
    printf("  ");
    for (int i = 0; i < 78; i++) {
        putchar('-');
    }
    putchar('\n');
}

/*
 * Le uma linha inteira com fgets (aceita espacos no meio do nome, ao contrario
 * de scanf com %s). O prompt e so o texto que aparece antes para o usuario.
 */
static void ler_linha(const char* prompt, char* buf, size_t cap) {
    printf("%s", prompt);
    if (fgets(buf, (int)cap, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }
    trim_newline(buf);
}

/* Mostra os dados de um funcionario ja encontrado (usado na busca). */
static void imprimir_funcionario(const NoFuncionario* f) {
    printf("  Matricula: %d\n", f->matricula);
    printf("  Nome:      %s\n", f->nome);
    printf("  Cargo:     %s\n", f->cargo);
    printf("  Salario:   R$ %.2f\n", f->salario);
}

/*
 * Loop de menu simples. Um detalhe chato mas necessario: depois de scanf com
 * numero, sobra o Enter no buffer; por isso "limpamos" com getchar ate o fim
 * da linha antes de ler nome e cargo com fgets — senao o fgets engole esse
 * Enter e parece que pulou a leitura.
 */
int main(void) {
    NoFuncionario* raiz = NULL;
    char buf_nome[BUF_TEXTO];
    char buf_cargo[BUF_TEXTO];
    int opcao;

    printf("=== Cadastro de funcionarios (ABP por matricula) ===\n");

    for (;;) {
        printf("\n1) Inserir funcionario\n");
        printf("2) Atualizar funcionario\n");
        printf("3) Buscar funcionario\n");
        printf("4) Listar funcionarios (em ordem)\n");
        printf("0) Sair\n");
        printf("Opcao: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida.\n");
            return 1;
        }
        /* Limpa o resto da linha apos o numero da opcao */
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
        }

        switch (opcao) {
        case 1: {
            int mat;
            double sal;
            printf("Matricula: ");
            if (scanf("%d", &mat) != 1) {
                printf("Matricula invalida.\n");
                while (getchar() != '\n') {
                }
                break;
            }
            while (getchar() != '\n') {
            }
            ler_linha("Nome: ", buf_nome, sizeof buf_nome);
            ler_linha("Cargo: ", buf_cargo, sizeof buf_cargo);
            printf("Salario (ex: 3500.00): ");
            if (scanf("%lf", &sal) != 1) {
                printf("Salario invalido.\n");
                while (getchar() != '\n') {
                }
                break;
            }
            while (getchar() != '\n') {
            }

            /* Evita duas pessoas com a mesma matricula (quebraria a chave da arvore) */
            if (buscar_funcionario(raiz, mat) != NULL) {
                printf("Ja existe funcionario com matricula %d.\n", mat);
                break;
            }
            NoFuncionario* nova_raiz = inserir_funcionario(raiz, mat, buf_nome, buf_cargo, sal);
            if (nova_raiz == NULL || buscar_funcionario(nova_raiz, mat) == NULL) {
                printf("Falha ao inserir (memoria insuficiente?).\n");
                break;
            }
            raiz = nova_raiz;
            printf("Funcionario cadastrado.\n");
            break;
        }

        case 2: {
            int mat;
            double sal;
            printf("Matricula do funcionario a atualizar: ");
            if (scanf("%d", &mat) != 1) {
                printf("Matricula invalida.\n");
                while (getchar() != '\n') {
                }
                break;
            }
            while (getchar() != '\n') {
            }
            ler_linha("Novo nome: ", buf_nome, sizeof buf_nome);
            ler_linha("Novo cargo: ", buf_cargo, sizeof buf_cargo);
            printf("Novo salario: ");
            if (scanf("%lf", &sal) != 1) {
                printf("Salario invalido.\n");
                while (getchar() != '\n') {
                }
                break;
            }
            while (getchar() != '\n') {
            }

            if (!atualizar_funcionario(raiz, mat, buf_nome, buf_cargo, sal)) {
                printf("Funcionario nao encontrado ou falha de memoria na atualizacao.\n");
            } else {
                printf("Dados atualizados.\n");
            }
            break;
        }

        case 3: {
            int mat;
            printf("Matricula: ");
            if (scanf("%d", &mat) != 1) {
                printf("Matricula invalida.\n");
                while (getchar() != '\n') {
                }
                break;
            }
            while (getchar() != '\n') {
            }
            NoFuncionario* f = buscar_funcionario(raiz, mat);
            if (f == NULL) {
                printf("Funcionario nao encontrado.\n");
            } else {
                printf("Encontrado:\n");
                imprimir_funcionario(f);
            }
            break;
        }

        case 4:
            if (raiz == NULL) {
                printf("Nenhum funcionario cadastrado.\n");
            } else {
                printf("\nFuncionarios (percurso em ordem / matricula crescente):\n");
                imprimir_cabecalho_lista();
                listar_em_ordem(raiz);
            }
            break;

        case 0:
            liberar_arvore(raiz);
            printf("Encerrando.\n");
            return 0;

        default:
            printf("Opcao invalida.\n");
            break;
        }
    }
}
