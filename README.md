# dados_avancados
Matéria de Estruturas de Dados Avançadas

- aula 10-03

Problemas de recursividade:
```markdown
- escopo de variaveis;
- depuração;
- eficiência;
 ```
```markdown
### Árvores
Uma árvore é uma estrutura de dados não linear e hierárquica que consiste em nós conectados por arestas.

#### O que é?
É uma estrutura composta por um conjunto de elementos chamados nós, onde um nó principal é designado como **raiz**. Cada nó pode conter um valor e referências para outros nós (filhos), formando uma estrutura recursiva.

#### Como funciona?
- **Raiz:** O nó inicial no topo da hierarquia.
- **Nós Internos:** Nós que possuem pelo menos um filho.
- **Folhas:** Nós que não possuem filhos (extremidades).
- **Arestas:** As conexões entre os nós.
- **Altura:** O comprimento do caminho mais longo da raiz até uma folha.

### Ordem de Percurso (Caminhamentos em Árvores)
Os percursos definem a ordem em que os nós de uma árvore binária são visitados pela recursão. A principal diferença entre eles é **em que momento você executa a ação principal (como imprimir) no nó atual**.

#### 1. Pré-ordem (Pre-order): `[Raiz] -> [Esquerda] -> [Direita]`
- **Regra:** Primeiro você visita o nó atual. Só depois você visita os filhos da esquerda e da direita.
- **Uso comum:** Clonar uma árvore (você deve criar o pai primeiro, antes de criar os filhos).
```c
void preOrdem(No* raiz) {
    if (raiz != NULL) {
        printf("%d ", raiz->valor); // 1. Ação na raiz ("Pre")
        preOrdem(raiz->esquerda);   // 2. Vai para esquerda
        preOrdem(raiz->direita);    // 3. Vai para direita
    }
}
```

#### 2. Em-ordem (In-order): `[Esquerda] -> [Raiz] -> [Direita]`
- **Regra:** Vai tudo para a esquerda, executa a ação no nó atual, e depois vai para a direita.
- **Uso comum:** Imprimir uma Árvore Binária de Busca em ordem crescente do menor para o maior.
```c
void emOrdem(No* raiz) {
    if (raiz != NULL) {
        emOrdem(raiz->esquerda);    // 1. Vai para esquerda
        printf("%d ", raiz->valor); // 2. Ação na raiz ("In" / entre eles)
        emOrdem(raiz->direita);     // 3. Vai para direita
    }
}
```

#### 3. Pós-ordem (Post-order): `[Esquerda] -> [Direita] -> [Raiz]`
- **Regra:** Visita todos os filhos primeiro. Só depois que resolver ambos os lados, resolve a raiz.
- **Uso comum:** Deletar (liberar memória) de uma árvore. Não podemos deletar o pai antes de deletar as crianças, senão perdemos o acesso a elas!
```c
void posOrdem(No* raiz) {
    if (raiz != NULL) {
        posOrdem(raiz->esquerda);   // 1. Vai para esquerda
        posOrdem(raiz->direita);    // 2. Vai para direita
        printf("%d ", raiz->valor); // 3. Ação na raiz ("Pos" / depois de tudo)
    }
}
```
