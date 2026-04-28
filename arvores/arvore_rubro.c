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

int main() {
    Node* root = NULL;

    insert(&root, 7);
    insert(&root, 6);
    insert(&root, 5);
    insert(&root, 4);
    insert(&root, 3);
    insert(&root, 2);
    insert(&root, 1);

    printf("Travessia In-Order da Arvore Rubro-Negra Criada (Valor e Cor):\n");
    inorder(root);

    return 0;
}
