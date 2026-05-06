#include <stdio.h>
#include <stdlib.h>

enum Color { RED, BLACK };

typedef struct Node {
    int data;
    enum Color color;
    struct Node *left, *right, *parent;
} Node;

Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->color = RED;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;
    return newNode;
}

// Rotação à esquerda
void leftRotate(Node** root, Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left != NULL)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == NULL)
        (*root) = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

// Rotação à direita
void rightRotate(Node** root, Node* y) {
    Node* x = y->left;
    y->left = x->right;
    if (x->right != NULL)
        x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == NULL)
        (*root) = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;
    x->right = y;
    y->parent = x;
}

// Função para corrigir violações das propriedades da árvore Rubro-Negra após inserção
void fixViolation(Node** root, Node* pt) {
    Node* parent_pt = NULL;
    Node* grand_parent_pt = NULL;

    while ((pt != *root) && (pt->color != BLACK) &&
           (pt->parent->color == RED)) {

        parent_pt = pt->parent;
        grand_parent_pt = pt->parent->parent;

        /* Caso A: Pais de pt é filho esquerdo do avô de pt */
        if (parent_pt == grand_parent_pt->left) {
            Node* uncle_pt = grand_parent_pt->right;

            /* Caso 1: O tio de pt também é vermelho
               Apenas recolorir */
            if (uncle_pt != NULL && uncle_pt->color == RED) {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
            }
            else {
                /* Caso 2: pt é filho direito do seu pai
                   Rotação à esquerda necessária */
                if (pt == parent_pt->right) {
                    leftRotate(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }

                /* Caso 3: pt é filho esquerdo do seu pai
                   Rotação à direita necessária */
                rightRotate(root, grand_parent_pt);
                enum Color temp = parent_pt->color;
                parent_pt->color = grand_parent_pt->color;
                grand_parent_pt->color = temp;
                pt = parent_pt;
            }
        }

        /* Caso B: Pais de pt é filho direito do avô de pt */
        else {
            Node* uncle_pt = grand_parent_pt->left;

            /* Caso 1: O tio de pt também é vermelho
               Apenas recolorir */
            if ((uncle_pt != NULL) && (uncle_pt->color == RED)) {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
            }
            else {
                /* Caso 2: pt é filho esquerdo do seu pai
                   Rotação à direita necessária */
                if (pt == parent_pt->left) {
                    rightRotate(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }

                /* Caso 3: pt é filho direito do seu pai
                   Rotação à esquerda necessária */
                leftRotate(root, grand_parent_pt);
                enum Color temp = parent_pt->color;
                parent_pt->color = grand_parent_pt->color;
                grand_parent_pt->color = temp;
                pt = parent_pt;
            }
        }
    }

    // A raiz deve ser sempre preta
    (*root)->color = BLACK;
}

// Função para inserir um novo dado na árvore
void insert(Node** root, int data) {
    Node* pt = createNode(data);

    // Se árvore está vazia, novo nó é a raiz
    if (*root == NULL) {
        pt->color = BLACK;
        *root = pt;
        return;
    }

    Node* parent = NULL;
    Node* current = *root;

    // Busca o local de inserção (como árvore de busca binária padrão)
    while (current != NULL) {
        parent = current;
        if (pt->data < current->data)
            current = current->left;
        else if (pt->data > current->data)
            current = current->right;
        else {
            // Valores duplicados não são permitidos nesta implementação
            free(pt);
            return;
        }
    }

    pt->parent = parent;
    if (pt->data < parent->data)
        parent->left = pt;
    else
        parent->right = pt;

    // Corrige as possíveis violações da árvore Rubro-Negra
    fixViolation(root, pt);
}

// Função auxiliar para impressão In-Order com as cores
void inorderHelper(Node* root) {
    if (root == NULL) return;
    inorderHelper(root->left);
    printf("%d(%s) ", root->data, root->color == RED ? "R" : "B");
    inorderHelper(root->right);
}

void inorder(Node* root) {
    inorderHelper(root);
    printf("\n");
}

// ─────────────────────────────────────────────────────────────
//  REMOÇÃO – funções auxiliares
// ─────────────────────────────────────────────────────────────

// Retorna o nó mínimo da subárvore (o mais à esquerda)
Node* minNode(Node* node) {
    while (node->left != NULL)
        node = node->left;
    return node;
}

// Substitui o nó u pelo nó v na árvore (transplante)
void transplant(Node** root, Node* u, Node* v) {
    if (u->parent == NULL)
        *root = v;                         // u era a raiz
    else if (u == u->parent->left)
        u->parent->left = v;               // u era filho esquerdo
    else
        u->parent->right = v;              // u era filho direito

    if (v != NULL)
        v->parent = u->parent;             // atualiza o pai de v
}

// ─────────────────────────────────────────────────────────────
//  Correção do duplo-preto após remoção de nó preto
//
//  Quando removemos um nó preto e o substituto também é preto
//  (ou NULL), criamos um "duplo-preto" que viola a propriedade
//  de altura negra. Existem 4 casos clássicos para corrigir:
//
//  Caso 1 – Irmão VERMELHO
//          Rotacionar o pai em direção ao duplo-preto e recolorir.
//          Isso transforma o caso em 2, 3 ou 4.
//
//  Caso 2 – Irmão NEGRO com 2 filhos NEGROS (ou NULL)
//          Recolorir o irmão de preto para vermelho e subir o
//          duplo-preto para o pai.
//
//  Caso 3 – Irmão NEGRO, filho ESQUERDO vermelho, direito preto
//          Rotacionar o irmão para o lado do duplo-preto e
//          recolorir. Transforma em Caso 4.
//
//  Caso 4 – Irmão NEGRO com filho DIREITO vermelho
//          Rotacionar o pai para o lado do duplo-preto, recolorir
//          o irmão com a cor do pai e o pai + filho direito de preto.
//          O duplo-preto é resolvido.
// ─────────────────────────────────────────────────────────────
void fixDelete(Node** root, Node* x, Node* x_parent) {
    while (x != *root && (x == NULL || x->color == BLACK)) {

        if (x == x_parent->left) {
            // ── Lado esquerdo: x é filho esquerdo do pai ──────────────

            Node* sibling = x_parent->right;  // irmão de x

            // GUARDA: em árvore válida o irmão nunca é NULL aqui,
            // mas protegemos contra estados inesperados.
            if (sibling == NULL) break;

            // Caso 1: irmão VERMELHO
            // Rotacionar pai para esquerda e recolorir para converter
            // o cenário em um dos casos 2, 3 ou 4.
            if (sibling->color == RED) {
                sibling->color = BLACK;
                x_parent->color = RED;
                leftRotate(root, x_parent);
                sibling = x_parent->right;    // novo irmão após rotação
                if (sibling == NULL) break;   // guarda extra após rotação
            }

            // Caso 2: irmão NEGRO com 2 filhos NEGROS (ou NULL)
            // O irmão fica vermelho e subimos o problema para o pai.
            if ((sibling->left == NULL  || sibling->left->color  == BLACK) &&
                (sibling->right == NULL || sibling->right->color == BLACK)) {
                sibling->color = RED;
                x        = x_parent;
                x_parent = x->parent;
            } else {
                // Caso 3: irmão NEGRO, filho esquerdo VERMELHO, direito PRETO
                // Rotacionar o irmão para a direita converte em Caso 4.
                if (sibling->right == NULL || sibling->right->color == BLACK) {
                    if (sibling->left != NULL)
                        sibling->left->color = BLACK;
                    sibling->color = RED;
                    rightRotate(root, sibling);
                    sibling = x_parent->right;  // novo irmão
                }

                // Caso 4: irmão NEGRO com filho DIREITO vermelho
                // Rotacionar o pai para a esquerda resolve o duplo-preto.
                sibling->color       = x_parent->color;
                x_parent->color      = BLACK;
                if (sibling->right != NULL)
                    sibling->right->color = BLACK;
                leftRotate(root, x_parent);
                x = *root;                  // encerra o loop
            }

        } else {
            // ── Lado direito: x é filho direito do pai (espelho) ──────

            Node* sibling = x_parent->left;   // irmão de x

            // GUARDA: em árvore válida o irmão nunca é NULL aqui.
            if (sibling == NULL) break;

            // Caso 1: irmão VERMELHO (espelho)
            if (sibling->color == RED) {
                sibling->color = BLACK;
                x_parent->color = RED;
                rightRotate(root, x_parent);
                sibling = x_parent->left;
                if (sibling == NULL) break;   // guarda extra após rotação
            }

            // Caso 2: irmão NEGRO com 2 filhos NEGROS (espelho)
            if ((sibling->right == NULL || sibling->right->color == BLACK) &&
                (sibling->left  == NULL || sibling->left->color  == BLACK)) {
                sibling->color = RED;
                x        = x_parent;
                x_parent = x->parent;
            } else {
                // Caso 3: irmão NEGRO, filho direito VERMELHO, esquerdo PRETO (espelho)
                if (sibling->left == NULL || sibling->left->color == BLACK) {
                    if (sibling->right != NULL)
                        sibling->right->color = BLACK;
                    sibling->color = RED;
                    leftRotate(root, sibling);
                    sibling = x_parent->left;
                }

                // Caso 4: irmão NEGRO com filho ESQUERDO vermelho (espelho)
                sibling->color      = x_parent->color;
                x_parent->color     = BLACK;
                if (sibling->left != NULL)
                    sibling->left->color = BLACK;
                rightRotate(root, x_parent);
                x = *root;
            }
        }
    }

    // Garante que x (ou a raiz) seja preto ao final
    if (x != NULL)
        x->color = BLACK;
}

// ─────────────────────────────────────────────────────────────
//  Função principal de remoção
//  (renomeada de 'delete' para evitar conflito com C++)
// ─────────────────────────────────────────────────────────────
void removeNode(Node** root, int data) {
    // 1. Encontrar o nó a remover
    Node* z = *root;
    while (z != NULL) {
        if      (data < z->data) z = z->left;
        else if (data > z->data) z = z->right;
        else break;
    }
    if (z == NULL) return;  // valor não encontrado na árvore

    Node* y = z;                        // nó que será efetivamente removido
    Node* x = NULL;                     // filho que substituirá y
    Node* x_parent = NULL;              // pai de x após a remoção
    enum Color y_original_color = y->color;

    if (z->left == NULL) {
        // Caso simples: sem filho esquerdo — x = filho direito (pode ser NULL)
        x = z->right;
        x_parent = z->parent;
        transplant(root, z, z->right);

    } else if (z->right == NULL) {
        // Caso simples: sem filho direito — x = filho esquerdo
        x = z->left;
        x_parent = z->parent;
        transplant(root, z, z->left);

    } else {
        // Dois filhos: substituímos z pelo sucessor in-order (mínimo à direita)
        y = minNode(z->right);
        y_original_color = y->color;
        x = y->right;

        if (y->parent == z) {
            // y é filho direto de z
            x_parent = y;
        } else {
            // Destacar y da sua posição atual
            x_parent = y->parent;
            transplant(root, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        // Colocar y no lugar de z
        transplant(root, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;  // y herda a cor de z
    }

    free(z);  // liberar memória do nó removido

    // 2. Se o nó removido era preto, corrigir possível violação
    if (y_original_color == BLACK)
        fixDelete(root, x, x_parent);
}

// ─────────────────────────────────────────────────────────────
//  Libera toda a memória da árvore (percurso pós-ordem)
// ─────────────────────────────────────────────────────────────
void freeTree(Node* node) {
    if (node == NULL) return;
    freeTree(node->left);
    freeTree(node->right);
    free(node);
}

// ─────────────────────────────────────────────────────────────
//  Verifica as 5 regras da árvore Rubro-Negra
//
//  Regra 1 – Todo nó é VERMELHO ou PRETO               (garantida pela struct)
//  Regra 2 – A RAIZ é sempre PRETA
//  Regra 3 – Nós NULL (folhas fictícias) são PRETOS     (convenção)
//  Regra 4 – Nó VERMELHO só tem filhos PRETOS
//  Regra 5 – Todo caminho da raiz até uma folha NULL
//             tem o mesmo número de nós PRETOS
// ─────────────────────────────────────────────────────────────

// Auxiliar: verifica regra 4 e 5; retorna a altura negra ou -1 se inválida
int checkRBT(Node* node) {
    if (node == NULL) return 1;  // nó NULL conta como 1 preto (folha fictícia)

    // Regra 4: nó vermelho não pode ter filho vermelho
    if (node->color == RED) {
        if ((node->left  != NULL && node->left->color  == RED) ||
            (node->right != NULL && node->right->color == RED)) {
            printf("  [ERRO] Regra 4 violada: no %d (V) tem filho vermelho!\n", node->data);
            return -1;
        }
    }

    int leftBH  = checkRBT(node->left);
    int rightBH = checkRBT(node->right);

    if (leftBH == -1 || rightBH == -1) return -1;

    // Regra 5: altura negra igual em ambos os lados
    if (leftBH != rightBH) {
        printf("  [ERRO] Regra 5 violada: no %d tem alturas negras diferentes (%d vs %d)!\n",
               node->data, leftBH, rightBH);
        return -1;
    }

    return leftBH + (node->color == BLACK ? 1 : 0);
}

void verifyRBT(Node* root) {
    printf("--- Verificacao das regras RBT ---\n");
    // Regra 2
    if (root != NULL && root->color != BLACK) {
        printf("  [ERRO] Regra 2 violada: raiz nao e preta!\n");
        return;
    }
    int bh = checkRBT(root);
    if (bh >= 0)
        printf("  [OK] Arvore valida! Altura negra = %d\n", bh);
    else
        printf("  [FALHA] Arvore invalida!\n");
    printf("----------------------------------\n");
}

int main() {
    Node* root = NULL;

    // ── Exemplo 1 ────────────────────────────────────────────
    // Árvore do enunciado: todos negros, equilibrada
    //         20(P)
    //        /     \
    //     10(P)   30(P)
    //    /    \   /   \
    //  5(P) 15(P) 25(P) 35(P)
    insert(&root, 20);
    insert(&root, 10);
    insert(&root, 30);
    insert(&root, 5);
    insert(&root, 15);
    insert(&root, 25);
    insert(&root, 35);

    printf("=== EXEMPLO 1 ===\n");
    printf("Arvore inicial (in-order):\n");
    inorder(root);

    verifyRBT(root);

    printf("Removendo 5 (caso 2: irmao negro com 2 filhos negros)...\n");
    removeNode(&root, 5);
    printf("Apos remocao do 5:\n");
    inorder(root);
    verifyRBT(root);

    // Libera a memória corretamente antes de recriar
    freeTree(root);
    root = NULL;

    // ── Exemplo 2 ────────────────────────────────────────────
    // Árvore com o nó 12 vermelho filho do 15
    //         20(P)
    //        /     \
    //     10(P)   30(P)
    //    /    \   /   \
    //  5(P) 15(P) 25(P) 35(P)
    //         /
    //       12(V)
    insert(&root, 20);
    insert(&root, 10);
    insert(&root, 30);
    insert(&root, 5);
    insert(&root, 15);
    insert(&root, 25);
    insert(&root, 35);
    insert(&root, 12);

    printf("\n=== EXEMPLO 2 ===\n");
    printf("Arvore inicial (in-order):\n");
    inorder(root);

    verifyRBT(root);

    printf("Removendo 5 (caso 3+4: irmao negro com filho vermelho)...\n");
    removeNode(&root, 5);
    printf("Apos remocao do 5:\n");
    inorder(root);
    verifyRBT(root);

    freeTree(root);

    return 0;
}
