#include <stdio.h>
#include <stdlib.h>

// Definindo a estrutura de um "No" (nodo) da arvore
typedef struct No {
    int valor;             // O dado que o nó armazena
    struct No* esquerda;   // Ponteiro para o filho da esquerda (subarvore esquerda)
    struct No* direita;    // Ponteiro para o filho da direita (subarvore direita)
} No;

// Funcao auxiliar para criar um novo no vazio na memoria
No* criarNo(int valor) {
    printf("LOG: Solicitando memoria para um novo No (tamanho: %zu bytes)...\n", sizeof(No));
    No* novoNo = (No*)malloc(sizeof(No));
    
    printf("LOG: Memoria alocada no endereco %p. Preenchendo com valor %d...\n", (void*)novoNo, valor);
    novoNo->valor = valor;
    
    printf("LOG: Garantindo que as setas da esquerda e direita nascam nulas (NULL).\n");
    novoNo->esquerda = NULL; // Comeca sem filhos
    novoNo->direita = NULL;
    
    printf("LOG: Novo No pronto! Retornando o endereco %p.\n\n", (void*)novoNo);
    return novoNo;
}

// ============== RECURSIVIDADE NA INSERCAO ==============
// Funcao para inserir um novo valor seguindo a regra da Arvore Binaria de Busca:
// Menores que a raiz vao pra esquerda, maiores vao pra direita
No* inserir(No* raiz, int valor) {
    // 1. CASO BASE: 
    // Se chegamos num espaco vazio (nulo), criamos o no e encerramos!
    if (raiz == NULL) {
        return criarNo(valor);
    }

    // 2. PASSO RECURSIVO:
    // Decide para qual lado vamos "viajar" e chamar a funcao novamente
    if (valor < raiz->valor) {
        // O valor eh menor, entao passa a responsabilidade para o filho da esquerda!
        // A raiz.esquerda recebe de volta a subarvore modificada
        raiz->esquerda = inserir(raiz->esquerda, valor);
    } else if (valor > raiz->valor) {
        // O valor eh maior, entao passa a responsabilidade para o filho da direita!
        raiz->direita = inserir(raiz->direita, valor);
    }

    // Sempre retorna a raiz atual intacta para ir "costurando" a arvore na volta da recursao
    return raiz;
}

// ============== RECURSIVIDADE NA IMPRESSAO ==============
// Funcao para imprimir os nos do menor para o maior (Caminhamento In-Order / Em Ordem)
// Regra: Visita Toda Esquerda -> Imprime o Valor Atual -> Visita Toda Direita
void imprimirEmOrdem(No* raiz) {
    // CASO BASE:
    // Se a raiz for diferente de nula, entao ha algo para visitar.
    // Se for nula, a recursao desse caminho simplesmente "acaba" e volta.
    if (raiz != NULL) {
        // 1. Vai ao fundo da esquerda primeiro (menores numeros)
        imprimirEmOrdem(raiz->esquerda);
        
        // 2. Imprime o no atual que esta visitando agora
        printf("%d ", raiz->valor);
        
        // 3. Depois vai ao fundo da direita (maiores numeros)
        imprimirEmOrdem(raiz->direita);
    }
}

int main() {
    // Uma arvore sempre deve comecar nula (vazia)
    No* raiz = NULL;

    printf("Inserindo valores na arvore: 50, 30, 70, 20, 40, 60, 80\n");

    // O primeiro elemento inserido vai definir quem eh a raiz principal
    raiz = inserir(raiz, 50);
    
    // Inserindo os outros: note como eh pratico. A gente sempre passa a raiz
    // e a recursividade da funcao "inserir" da um jeito de encontrar o lugar certo sozinho!
    inserir(raiz, 30);
    inserir(raiz, 70);
    inserir(raiz, 20);
    inserir(raiz, 40);
    inserir(raiz, 60);
    inserir(raiz, 80);

    /* Visao de como a arvore foi construida na memoria:
              50
            /    \
          30      70
         /  \    /  \
       20   40  60   80
    */

    printf("\nImprimindo a arvore usando a funcao recursiva (Em Ordem Crescente):\n");
    // Repare na simplicidade de chamar apenas uma funcao e ela rodar todo aquele galho de chamadas
    imprimirEmOrdem(raiz);
    printf("\n");

    return 0;
}
