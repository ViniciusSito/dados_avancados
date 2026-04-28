#include <stdio.h>
#include <stdlib.h>

// Definindo a estrutura de um No
typedef struct No{
    int valor; // Valor armazenado no no, valor do nó
    int altura; // Armazena a altura do no, a altura não é necessaria, mas declarando vai diminuir logica 
    struct No *esq; // Ponteiro para o filho da esquerda
    struct No *dir; // Ponteiro para o filho da direita
}No; 

// Função para calcular a altura de um nó
int altura_No(struct No *no){
    if(no == NULL){ // Se o nó for nulo, a altura é 0
        return 0;
    }
    return no->altura; // Retorna a altura do nó
}

int maior(int a, int b){// Retorna o maior de dois inteiros
    if(a > b){
        return a;// Se a for maior que b, retorna a
    }
    return b;// Se b for maior que a, retorna b
}

// Função para criar um novo nó
struct No *novo_no(int valor){ // Cria um novo nó
    struct No *no = (struct No*)malloc(sizeof(struct No)); // Aloca memória para o nó
    no->valor = valor; // Atribui o valor ao nó
    no->altura = 1; // A altura inicial é 1
    no->esq = NULL; // O filho da esquerda é nulo
    no->dir = NULL; // O filho da direita é nulo
    return no;
}
//função de rotação a direita (Caso LL)
struct No* rotacao_direita(struct No *raiz){
    struct No *no = raiz->esq; // Rotação p/ direita sobe o filho da ESQUERDA
    struct No *temp = no->dir;

    no->dir = raiz; 
    raiz->esq = temp;

    raiz->altura = maior(altura_No(raiz->esq), altura_No(raiz->dir)) + 1;
    no->altura = maior(altura_No(no->esq), altura_No(no->dir)) + 1;
    
    return no;
}

//função de rotação a esquerda (Caso RR)
struct No* rotacao_esquerda(struct No *raiz){
    struct No *no = raiz->dir; // Rotação p/ esquerda sobe o filho da DIREITA
    struct No *temp = no->esq;

    no->esq = raiz;
    raiz->dir = temp;

    raiz->altura = maior(altura_No(raiz->esq), altura_No(raiz->dir)) + 1;
    no->altura = maior(altura_No(no->esq), altura_No(no->dir)) + 1;
    
    return no;
}

//rotacao dupla a direita (Caso Esquerda-Direita / LR)
struct No *rotacao_dupla_direita(struct No *raiz){
    raiz->esq = rotacao_esquerda(raiz->esq); // Rotação ESQUERDA no nó da esquerda
    return rotacao_direita(raiz); // Rotação DIREITA na raiz
}

//rotacao dupla a esquerda (Caso Direita-Esquerda / RL)
struct No *rotacao_dupla_esquerda(struct No *raiz){
    raiz->dir = rotacao_direita(raiz->dir); // Rotação DIREITA no nó da direita
    return rotacao_esquerda(raiz); // Rotação ESQUERDA na raiz
}

// Função para obter o fator de balanceamento de um nó
int fator_balanceamento(struct No *no){
    if(no == NULL){
        return 0;
    }
    return altura_No(no->esq) - altura_No(no->dir);
}

// Função para inserir um nó na árvore AVL e fazer o balanceamento
struct No* inserir(struct No *no, int valor){
    // 1. O passo normal de inserir em uma Árvore Binária de Busca
    if (no == NULL)
        return novo_no(valor);

    if (valor < no->valor)
        no->esq = inserir(no->esq, valor);
    else if (valor > no->valor)
        no->dir = inserir(no->dir, valor);
    else // Valores iguais geralmente não são permitidos em uma BST simples
        return no;

    // 2. Atualiza a altura do nó pai
    no->altura = 1 + maior(altura_No(no->esq), altura_No(no->dir));

    // 3. Pega o fator de balanceamento deste nó para checar se desbalanceou
    int fb = fator_balanceamento(no);

    // 4. Casos de Desbalanceamento

    // Caso Esquerda Esquerda (LL)
    if (fb > 1 && valor < no->esq->valor)
        return rotacao_direita(no);

    // Caso Direita Direita (RR)
    if (fb < -1 && valor > no->dir->valor)
        return rotacao_esquerda(no);

    // Caso Esquerda Direita (LR)
    if (fb > 1 && valor > no->esq->valor)
        return rotacao_dupla_direita(no);

    // Caso Direita Esquerda (RL)
    if (fb < -1 && valor < no->dir->valor)
        return rotacao_dupla_esquerda(no);

    // Retorna o ponteiro do nó (sem alterações)
    return no;
}

// ========= FUNCOES DE REMOCAO =========

// Função auxiliar para encontrar o nó com o menor valor (usado na remoção)
struct No* menor_no(struct No* no) {
    struct No* atual = no;
    while (atual->esq != NULL)
        atual = atual->esq;
    return atual;
}

// Função de remoção de nó na árvore AVL e rebalanceamento
struct No* remover(struct No* raiz, int valor) {
    if (raiz == NULL)
        return raiz; // Nó não encontrado

    // 1. Remoção padrão de Árvore Binária de Busca
    if (valor < raiz->valor)
        raiz->esq = remover(raiz->esq, valor);
    else if (valor > raiz->valor)
        raiz->dir = remover(raiz->dir, valor);
    else {
        // Encontramos o nó a ser removido
        
        // Caso 1 (Nenhum filho) ou Caso 2 (Apenas um filho)
        if ((raiz->esq == NULL) || (raiz->dir == NULL)) { 
            struct No *temp = raiz->esq ? raiz->esq : raiz->dir;

            if (temp == NULL) { // Sem filhos
                temp = raiz;
                raiz = NULL;
            } else { // 1 filho
                *raiz = *temp; // O conteúdo do filho é copiado
            }
            free(temp);
        } else {
            // Caso 3 (Dois filhos): Pegar o sucessor (menor nó da subárvore direita)
            struct No* temp = menor_no(raiz->dir);
            raiz->valor = temp->valor;
            raiz->dir = remover(raiz->dir, temp->valor); // Remove o sucessor
        }
    }

    if (raiz == NULL)
        return raiz; // Se a árvore tinha só 1 nó e ficou vazia

    // 2. Atualiza a altura do nó pai
    raiz->altura = 1 + maior(altura_No(raiz->esq), altura_No(raiz->dir));

    // 3. Fator de Balanceamento
    int fb = fator_balanceamento(raiz);

    // 4. Casos de Desbalanceamento na Remoção

    // Caso LL (Esquerda Esquerda)
    if (fb > 1 && fator_balanceamento(raiz->esq) >= 0)
        return rotacao_direita(raiz);

    // Caso LR (Esquerda Direita)
    if (fb > 1 && fator_balanceamento(raiz->esq) < 0)
        return rotacao_dupla_direita(raiz);

    // Caso RR (Direita Direita)
    if (fb < -1 && fator_balanceamento(raiz->dir) <= 0)
        return rotacao_esquerda(raiz);

    // Caso RL (Direita Esquerda)
    if (fb < -1 && fator_balanceamento(raiz->dir) > 0)
        return rotacao_dupla_esquerda(raiz);

    return raiz;
}

// ========= FUNCOES DE IMPRESSAO E MAIN =========

// Função para imprimir a árvore em in-order (Esquerda, Raiz, Direita) p/ visualizar
void imprimir_in_order(struct No *raiz) {
    if(raiz != NULL) {
        imprimir_in_order(raiz->esq);
        printf("Valor: %d (Altura: %d)\n", raiz->valor, raiz->altura);
        imprimir_in_order(raiz->dir);
    }
}

// Função Principal
int main() {
    struct No *raiz = NULL;

    printf("--- Inserindo Elementos (Testando o Balancemanto) ---\n");
    raiz = inserir(raiz, 10);
    raiz = inserir(raiz, 20);
    raiz = inserir(raiz, 30); // Vai causar uma rotação à esquerda (RR) na raiz
    raiz = inserir(raiz, 40);
    raiz = inserir(raiz, 50); // Vai causar outra rotação à esquerda (RR) no 30
    raiz = inserir(raiz, 25); // Vai causar rotação dupla à direita (RL)
    
    printf("\nArvore preenchida e balanceada (Ordem Crescente):\n");
    imprimir_in_order(raiz);

    printf("\n--- Removendo o Elemento 30 ---\n");
    raiz = remover(raiz, 30); // Remoção de um nó com 2 filhos e rebalanceamento
    
    printf("\nArvore apos remocao e rebalanceamente (Ordem Crescente):\n");
    imprimir_in_order(raiz);

    return 0;
}