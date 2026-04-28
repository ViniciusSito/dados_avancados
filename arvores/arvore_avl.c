#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 1. Estrutura de Dados
typedef struct {
    int id;             // Um identificador único do produto
    char nome[100];     // O nome do produto
    float preco;        // O preço do produto
    int quantidade;     // A quantidade disponível em estoque
} Produto;

typedef struct No {
    Produto produto;    // Dados do produto
    int altura;         // Armazena a altura do nó
    struct No *esq;     // Ponteiro para o filho da esquerda
    struct No *dir;     // Ponteiro para o filho da direita
} No;

// Retorna o maior de dois inteiros
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Retorna a altura do nó
int altura(No *N) {
    if (N == NULL)
        return 0;
    return N->altura;
}

// Cria um novo nó da árvore
No* novo_no(int id, const char* nome, float preco, int quantidade) {
    No* no = (No*)malloc(sizeof(No));
    no->produto.id = id;
    strncpy(no->produto.nome, nome, 99);
    no->produto.nome[99] = '\0';
    no->produto.preco = preco;
    no->produto.quantidade = quantidade;
    no->esq = NULL;
    no->dir = NULL;
    no->altura = 1; // Nó novo entra como folha
    return no;
}

// Rotação para a direita
No* rotacao_direita(No *y) {
    No *x = y->esq;
    No *T2 = x->dir;

    // Realiza a rotação
    x->dir = y;
    y->esq = T2;

    // Atualiza as alturas
    y->altura = max(altura(y->esq), altura(y->dir)) + 1;
    x->altura = max(altura(x->esq), altura(x->dir)) + 1;

    // Retorna a nova raiz
    return x;
}

// Rotação para a esquerda
No* rotacao_esquerda(No *x) {
    No *y = x->dir;
    No *T2 = y->esq;

    // Realiza a rotação
    y->esq = x;
    x->dir = T2;

    // Atualiza as alturas
    x->altura = max(altura(x->esq), altura(x->dir)) + 1;
    y->altura = max(altura(y->esq), altura(y->dir)) + 1;

    // Retorna a nova raiz
    return y;
}

// Obtém o fator de balanceamento do nó
int fator_balanceamento(No *N) {
    if (N == NULL)
        return 0;
    return altura(N->esq) - altura(N->dir);
}

// Compara dois produtos usando ID e Nome (desempate)
int comparar_produtos(int id1, const char* nome1, int id2, const char* nome2) {
    if (id1 < id2) return -1;
    if (id1 > id2) return 1;
    return strcmp(nome1, nome2); // Em caso de mesmo ID, usa o nome
}

// 2. Operações Suportadas
// 2.1 Inserção de Produto
No* inserir_produto(No* no, int id, const char* nome, float preco, int quantidade) {
    // Passo 1: Inserção normal de BST usando ID e depois Nome
    if (no == NULL)
        return novo_no(id, nome, preco, quantidade);

    int cmp = comparar_produtos(id, nome, no->produto.id, no->produto.nome);

    if (cmp < 0)
        no->esq = inserir_produto(no->esq, id, nome, preco, quantidade);
    else if (cmp > 0)
        no->dir = inserir_produto(no->dir, id, nome, preco, quantidade);
    else
        return no; // Produtos exatamente iguais não são duplicados

    // Passo 2: Atualizar a altura do ancestral
    no->altura = 1 + max(altura(no->esq), altura(no->dir));

    // Passo 3: Obter o fator de balanceamento
    int fb = fator_balanceamento(no);

    // Se estiver desbalanceado, testamos os 4 casos:
    // Caso Esquerda Esquerda
    if (fb > 1 && comparar_produtos(id, nome, no->esq->produto.id, no->esq->produto.nome) < 0)
        return rotacao_direita(no);

    // Caso Direita Direita
    if (fb < -1 && comparar_produtos(id, nome, no->dir->produto.id, no->dir->produto.nome) > 0)
        return rotacao_esquerda(no);

    // Caso Esquerda Direita
    if (fb > 1 && comparar_produtos(id, nome, no->esq->produto.id, no->esq->produto.nome) > 0) {
        no->esq = rotacao_esquerda(no->esq);
        return rotacao_direita(no);
    }

    // Caso Direita Esquerda
    if (fb < -1 && comparar_produtos(id, nome, no->dir->produto.id, no->dir->produto.nome) < 0) {
        no->dir = rotacao_direita(no->dir);
        return rotacao_esquerda(no);
    }

    // Retorna o nó não alterado
    return no;
}

// 2.2 Busca de Produto
// Embora possa haver ids iguais e ordenados por nome, 
// a busca por ID encontra o primeiro que for correspondente
void buscar_produto(No* raiz, int id) {
    No* atual = raiz;
    while (atual != NULL) {
        if (id < atual->produto.id)
            atual = atual->esq;
        else if (id > atual->produto.id)
            atual = atual->dir;
        else {
            // ID Igual - Encontrado
            printf("Produto encontrado! ID: %d | Nome: %s | Preco: %.2f | Quantidade: %d\n",
                   atual->produto.id, atual->produto.nome, atual->produto.preco, atual->produto.quantidade);
            return;
        }
    }
    // Não encontrou
    printf("Produto com ID %d nao existe no estoque.\n", id);
}

// Encontra o nó com o menor valor na árvore (usado para o sucessor in-order)
No* menor_no(No* no) {
    No* atual = no;
    while (atual->esq != NULL)
        atual = atual->esq;
    return atual;
}

// 2.3 Remoção de Produto
No* remover_produto(No* raiz, int id, const char* nome) {
    // Passo 1: Remoção BST padrão
    if (raiz == NULL)
        return raiz;

    // Comparamos usando id e nome
    int cmp = comparar_produtos(id, nome, raiz->produto.id, raiz->produto.nome);

    if (cmp < 0)
        raiz->esq = remover_produto(raiz->esq, id, nome);
    else if (cmp > 0)
        raiz->dir = remover_produto(raiz->dir, id, nome);
    else {
        // Encontramos o nó a ser removido
        // Casos 1 (Sem filhos) ou 2 (Apenas 1 filho)
        if ((raiz->esq == NULL) || (raiz->dir == NULL)) {
            No *temp = raiz->esq ? raiz->esq : raiz->dir;

            // Caso 1: Sem filhos
            if (temp == NULL) {
                temp = raiz;
                raiz = NULL;
            } else {
                // Caso 2: Um filho
                *raiz = *temp; // O conteúdo do filho é copiado
            }
            free(temp);
        } else {
            // Caso 3: Dois filhos. Pegamos o sucessor in-order (menor na direita)
            No* temp = menor_no(raiz->dir);

            // Copia os dados do sucessor para o nó atual
            raiz->produto = temp->produto;

            // Remove o sucessor na árvore da direita
            raiz->dir = remover_produto(raiz->dir, temp->produto.id, temp->produto.nome);
        }
    }

    // Se após remoção a árvore ficou vazia, retorna nulo
    if (raiz == NULL)
        return raiz;

    // Passo 2: Atualizar altura do nó atual
    raiz->altura = 1 + max(altura(raiz->esq), altura(raiz->dir));

    // Passo 3: Pegar o fator de balanceamento para reorganizar AVL
    int fb = fator_balanceamento(raiz);

    // Caso Esquerda Esquerda
    if (fb > 1 && fator_balanceamento(raiz->esq) >= 0)
        return rotacao_direita(raiz);

    // Caso Esquerda Direita
    if (fb > 1 && fator_balanceamento(raiz->esq) < 0) {
        raiz->esq = rotacao_esquerda(raiz->esq);
        return rotacao_direita(raiz);
    }

    // Caso Direita Direita
    if (fb < -1 && fator_balanceamento(raiz->dir) <= 0)
        return rotacao_esquerda(raiz);

    // Caso Direita Esquerda
    if (fb < -1 && fator_balanceamento(raiz->dir) > 0) {
        raiz->dir = rotacao_direita(raiz->dir);
        return rotacao_esquerda(raiz);
    }

    return raiz;
}

// Função principal de teste - Demonstração de uso
int main() {
    No *raiz = NULL;
    
    printf("=== Inserindo Produtos ===\n");
    raiz = inserir_produto(raiz, 30, "Monitor", 1200.50, 10);
    raiz = inserir_produto(raiz, 20, "Teclado", 150.00, 50);
    raiz = inserir_produto(raiz, 40, "Mouse", 80.00, 100);
    // Inserindo com mesmo ID, organizado pelo nome (cenário solicitado)
    raiz = inserir_produto(raiz, 20, "Teclado Mecanico", 300.00, 10); 
    raiz = inserir_produto(raiz, 10, "Mousepad", 40.00, 70); 
    printf("Produtos inseridos!\n\n");

    printf("=== Buscando Produtos ===\n");
    buscar_produto(raiz, 40); // Existe
    buscar_produto(raiz, 20); // Retornará o primeiro produto com ID 20 que encontrar
    buscar_produto(raiz, 99); // Nao existe
    
    printf("\n=== Removendo Produtos ===\n");
    
    // Removendo folha (Caso 1)
    raiz = remover_produto(raiz, 10, "Mousepad");
    printf("[1] Removido Mousepad (ID: 10) - Buscando para verificar:\n");
    buscar_produto(raiz, 10);
    
    // Removendo nó com dois filhos (Caso 3) -> raiz (ID 30)
    raiz = remover_produto(raiz, 30, "Monitor");
    printf("\n[2] Removido Monitor (ID: 30) - Buscando para verificar:\n");
    buscar_produto(raiz, 30);
    
    return 0;
}
