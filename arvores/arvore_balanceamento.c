/*
 * ============================================================================
 * ARVORE AVL — balanceamento automatico apos insercao/remocao
 * ============================================================================
 *
 * Baseado na aula sobre:
 *   - Fator de balanceamento FB = Ad - Ae (altura da subarvore DIREITA menos
 *     altura da subarvore ESQUERDA). Em AVL valido, FB so pode ser -1, 0 ou 1.
 *   - Se |FB| > 1, a arvore esta desbalanceada e aplicamos ROTACOES para
 *     restaurar a propriedade da ABB (menores a esquerda, maiores a direita)
 *     e o limite de altura.
 *
 * Convencao de ALTURA (folha = 0, como na aula):
 *   - No sem filhos: altura 0.
 *   - No NULL (subarvore vazia): tratamos como altura -1 para o calculo
 *     recursivo: assim a folha fica com altura max(-1,-1)+1 = 0.
 *
 * Os quatro casos classicos (nomes referem-se ao formato do desequilibrio):
 *   LL — peso a esquerda do filho a esquerda  -> rotacao simples a DIREITA
 *   RR — peso a direita do filho a direita   -> rotacao simples a ESQUERDA
 *   LR — "zigue-zague" a esquerda            -> dupla: esquerda no filho esq,
 *                                                depois direita na raiz
 *   RL — "zigue-zague" a direita             -> dupla: direita no filho dir,
 *                                                depois esquerda na raiz
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct No {
    int valor;
    struct No* esquerda;
    struct No* direita;
    /*
     * Guardamos a altura do no para nao recalcular a arvore inteira a cada
     * passo; apos inserir/remover/rotacionar, atualizamos so os nos afetados
     * no caminho de volta da recursao (O(log n) em arvore balanceada).
     */
    int altura;
} No;

/* Altura de subarvore vazia = -1 (permite folha com altura 0). */
#define ALTURA_SUB(N) ((N) == NULL ? -1 : (N)->altura)

static int max_int(int a, int b) {
    return (a > b) ? a : b;
}

/* Recalcula altura de 'n' a partir dos filhos (chame apos mudar filhos). */
static void atualizar_altura(No* n) {
    if (n == NULL) {
        return;
    }
    n->altura = 1 + max_int(ALTURA_SUB(n->esquerda), ALTURA_SUB(n->direita));
}

/*
 * Fator de balanceamento FB = Ad - Ae (aula):
 *   Ad = altura da subarvore a DIREITA do no
 *   Ae = altura da subarvore a ESQUERDA do no
 *
 * Interpretacao:
 *   FB = 0  -> subarvores com a mesma altura
 *   FB = 1  -> direita um nivel mais alta (ainda OK em AVL)
 *   FB = -1 -> esquerda um nivel mais alta (ainda OK)
 *   |FB| >= 2 -> DESBALANCEADO; e preciso rotacionar
 */
static int fator_balanceamento(No* n) {
    if (n == NULL) {
        return 0;
    }
    return ALTURA_SUB(n->direita) - ALTURA_SUB(n->esquerda);
}

static No* criar_no(int valor) {
    No* novo = (No*)malloc(sizeof(No));
    if (novo == NULL) {
        fprintf(stderr, "Erro: sem memoria.\n");
        exit(1);
    }
    novo->valor = valor;
    novo->esquerda = NULL;
    novo->direita = NULL;
    novo->altura = 0; /* folha */
    return novo;
}

/*
 * ---------------------------------------------------------------------------
 * ROTACAO SIMPLES A DIREITA (caso LL da aula — "pega o valor apos a raiz")
 * ---------------------------------------------------------------------------
 *
 * Antes (desbalanceio a esquerda do no 'y', estilo 8-6-5):
 *        y                         x
 *       / \                       / \
 *      x   C        --->         A   y
 *     / \                           / \
 *    A   B                         B   C
 *
 * 'x' sobe e vira raiz da subarvore; 'y' desce a direita de 'x'.
 * A subarvore B (filho direito de x) nao pode ficar pendurada em x depois
 * que y vai para baixo: ela vira filho ESQUERDO de y (ainda entre x e y na
 * ordenacao da ABB).
 */
static No* rotacao_direita(No* y) {
    No* x = y->esquerda;
    No* B = x->direita;

    x->direita = y;
    y->esquerda = B;

    /* Alturas mudam em y e x; atualizar de baixo para cima. */
    atualizar_altura(y);
    atualizar_altura(x);
    return x;
}

/*
 * ---------------------------------------------------------------------------
 * ROTACAO SIMPLES A ESQUERDA (caso RR — espelho do LL)
 * ---------------------------------------------------------------------------
 *
 *        x                         y
 *       / \                       / \
 *      A   y        --->         x   C
 *         / \                   / \
 *        B   C                 A   B
 */
static No* rotacao_esquerda(No* x) {
    No* y = x->direita;
    No* B = y->esquerda;

    y->esquerda = x;
    x->direita = B;

    atualizar_altura(x);
    atualizar_altura(y);
    return y;
}

/*
 * Apos uma insercao/remocao, o no 'raiz' pode ter |FB| = 2.
 * Olhamos o sinal do fator do FILHO na direcao do desequilibrio para decidir
 * se e rotacao simples ou dupla (LR / RL).
 */
static No* balancear(No* raiz) {
    if (raiz == NULL) {
        return NULL;
    }

    atualizar_altura(raiz);
    int fb = fator_balanceamento(raiz);

    /* Esquerda muito alta (FB negativo grande, ex.: -2): corrigir a esquerda. */
    if (fb < -1) {
        /*
         * Se o filho da esquerda esta "pendendo" para a DIREITA internamente
         * (fator > 0), o formato e LR — primeiro endireita o filho com
         * rotacao a esquerda, depois aplicamos LL (rotacao direita na raiz).
         */
        if (fator_balanceamento(raiz->esquerda) > 0) {
            raiz->esquerda = rotacao_esquerda(raiz->esquerda);
        }
        return rotacao_direita(raiz);
    }

    /* Direita muito alta (FB = +2): espelho do caso anterior. */
    if (fb > 1) {
        if (fator_balanceamento(raiz->direita) < 0) {
            raiz->direita = rotacao_direita(raiz->direita);
        }
        return rotacao_esquerda(raiz);
    }

    return raiz;
}

/*
 * Insercao igual a ABB: menor vai a esquerda, maior a direita.
 * Na volta da recursao, atualizamos altura e balanceamos cada ancestral.
 */
No* inserir_avl(No* raiz, int valor) {
    if (raiz == NULL) {
        return criar_no(valor);
    }

    if (valor < raiz->valor) {
        raiz->esquerda = inserir_avl(raiz->esquerda, valor);
    } else if (valor > raiz->valor) {
        raiz->direita = inserir_avl(raiz->direita, valor);
    } else {
        /* Duplicata: ignoramos para manter a arvore como conjunto de chaves. */
        return raiz;
    }

    return balancear(raiz);
}

/* Menor no da subarvore (sucessor pode ser util na remocao). */
static No* minimo_no(No* raiz) {
    while (raiz != NULL && raiz->esquerda != NULL) {
        raiz = raiz->esquerda;
    }
    return raiz;
}

/*
 * Remocao estilo ABB (folha / um filho / dois filhos com sucessor in-order),
 * depois balancear de volta no caminho — mesma ideia da insercao.
 */
No* remover_avl(No* raiz, int valor) {
    if (raiz == NULL) {
        return NULL;
    }

    if (valor < raiz->valor) {
        raiz->esquerda = remover_avl(raiz->esquerda, valor);
    } else if (valor > raiz->valor) {
        raiz->direita = remover_avl(raiz->direita, valor);
    } else {
        /* No encontrado */
        if (raiz->esquerda == NULL || raiz->direita == NULL) {
            No* velho = raiz;
            raiz = raiz->esquerda ? raiz->esquerda : raiz->direita;
            free(velho);
        } else {
            No* suc = minimo_no(raiz->direita);
            raiz->valor = suc->valor;
            raiz->direita = remover_avl(raiz->direita, suc->valor);
        }
    }

    if (raiz == NULL) {
        return NULL;
    }
    return balancear(raiz);
}

void imprimir_em_ordem(No* raiz) {
    if (raiz == NULL) {
        return;
    }
    imprimir_em_ordem(raiz->esquerda);
    printf("%d ", raiz->valor);
    imprimir_em_ordem(raiz->direita);
}

/* Imprime valor e FB de cada no (in-order) para conferir a aula no papel. */
static void imprimir_debug(No* raiz) {
    if (raiz == NULL) {
        return;
    }
    imprimir_debug(raiz->esquerda);
    printf("  valor=%d  FB=%d  (altura no=%d)\n", raiz->valor,
           fator_balanceamento(raiz), raiz->altura);
    imprimir_debug(raiz->direita);
}

static void liberar_arvore(No* raiz) {
    if (raiz == NULL) {
        return;
    }
    liberar_arvore(raiz->esquerda);
    liberar_arvore(raiz->direita);
    free(raiz);
}

int main(void) {
    No* raiz = NULL;

    printf("=== Exemplo da aula: inserir 8, depois 6, depois 5 ===\n");
    printf("Apos 8 e 6: raiz 8, filho 6 a esquerda. Com FB=Ad-Ae: Ae=0, Ad=-1 (vazio)\n");
    printf("  -> FB(8)=(-1)-0=-1 (ainda OK: |FB|<=1).\n");
    printf("Apos inserir 5 a esquerda de 6: a subarvore esquerda de 8 ganha altura;\n");
    printf("  no 8 fica Ae=1, Ad=-1 -> FB=-1-1=-2 (desbalanceado; rotacao LL).\n");
    printf("Rotacao simples a DIREITA (LL): 6 vira raiz, 5 a esquerda, 8 a direita.\n\n");

    raiz = inserir_avl(raiz, 8);
    raiz = inserir_avl(raiz, 6);
    raiz = inserir_avl(raiz, 5);

    printf("Em ordem (deve sair 5 6 8): ");
    imprimir_em_ordem(raiz);
    printf("\n\nDetalhe por no (FB = Ad - Ae):\n");
    imprimir_debug(raiz);

    printf("\n=== Mais insercoes para exercitar RR, LR, RL ===\n");
    int extras[] = { 10, 9, 11, 7, 12 };
    size_t i;
    for (i = 0; i < sizeof(extras) / sizeof(extras[0]); i++) {
        raiz = inserir_avl(raiz, extras[i]);
    }
    printf("Em ordem: ");
    imprimir_em_ordem(raiz);
    printf("\n");

    printf("\n=== Removendo 10 (teste de rebalanceamento) ===\n");
    raiz = remover_avl(raiz, 10);
    printf("Em ordem: ");
    imprimir_em_ordem(raiz);
    printf("\n");

    liberar_arvore(raiz);
    return 0;
}
