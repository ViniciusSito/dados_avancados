arvore vazia sem nós, apenas um

no raiz =

       30
        |
     10   40
     |	   |
     5	  15

se o valor do nó for maior vai para direito do nó pai e se for menor vai para esquerda do nó pai

\*verificar se o ponteiro para o nó a esquerda é nulo ou tem algum valor

no caso a situação exemplo:

- analisamos o nó raiz, se o valor for menor ele vai para a esquerda se for maior para a direita, no caso nossa raiz esta no valor de 30 ao inserir o valor 10 ele analisa se eh maior ou menor que o 30, e então como é menor ele vai para esquerda e então ele vai verificar se o ponteiro esta nulo ou não, como não esta ele preenche com o valor, no caso 5 é maior ou menor que 30, menor, então ele vai para a esquerda, porém o nó do 30 esta ocupado, então ele faz outra verificação, 5 é maior ou menor que o 10, no caso menor então ele vai para a raiz da esquerda, assim por diante ...

Caso de remoção de nó com dois filhos:

- maior à esquerda
- menor à direita

  30
  | |
  10 40
  | | | |
  5 15 35 50
  | |
  45 55

no caso querendo remover o nó 40 a gente pega o menor numero a direita, analisando a arvore, é o 45 o menor numero, então trocando pelo menor numero, a arvore ainda mantem a logica, pois caso queira remover o 45 escolhemos o 50 e analisando novamente a arvore, o 55 continua sendo maior e ainda na parte da direita.

      30
      |   |
    10    45

| | | |
5 15 35 50
|
55

sempre que trocar o nó, eliminar para não houver duplicata

Aula 31-03

Balanceamento de arvores:

- a medida que inserimos ou removemos nós estamos balanceando ou não
  . a cada inserção ou remoção vai ter que recalcular a arvore inteira

- uma arvore balanceada é considerada com 0, 1 ou -1, acima de 1 ou -1 esta desbalanceada
  0 A
  | |
  1 B E
  | | | |
  2 C D F G
  |
  3 H
  | |
  4 I J

Ad= 2, Ae= 4

Ad - Ae = 4 - 2 = 2 desbalanceada

Para cada nó na arvore:
AD - Altura do nó à direita
AE - Altura do nó a esquerda
FB - Diferença da altura entre subarvores direita e esquerda

Nó folha normalmente é 0.

para verificarmos os valores de balanceamento devemos fazer apartir dos nós

           A = 2
          |  |
     0 = B    E = 2
        | |  | |
        C D  F G
               |
               H  = 0
              | |
              I J

Desbalanceada

- Caso uma arvore não tenha um nó de um dos lados é degenerada

             A
              |
               E
                |
                i
                 |
                 O
                  |
                  U

  Ae= 0 Ad = 4

0 - 4 = -4

Balanceamento=

- Depende de como ela esta desbalanceada.

Arvore AVL

- Metodo sovietico de rotação de arvore.

- Rotação simples à direita (LL)

      8

  |
  6

- inciamos uma arvore com uma raiz e um nó, por enquanto ela esta balanceada (1 - 0 = 1), no caso queremos inserir o valor de 5. Como 5 é menor que o 8, menor que o 6 ele vai para baixo a esquerda do 6.

      8

  |
  6
  |
  5

- No caso após a inserção ela esta desbalanceada (2 - 0 = 2), devemos fazer uma rotação, pegamos o valor após a raiz, então o 6 se torna a raiz.

      6

  | |
  5 8

- mantendo a integridade da logica (menor a esquerda e a maior a direita comparado com a raiz) e conseguimos balancear a arvore (1 - 1 = 0)

- vamos ver com um exemplo com mais nós

  8
  |
  6
  | |
  5 7

- se o nó que acabou de se tornar raiz tiver um filho à direta, esse nó vai se tornar o filho à direita da antiga raiz.

  6
  | |
  5 8
  |
  7

-colocamos o nó 7 para a esquerda do nó 8, para manter a consistencia

Rotação para esquerda

4
|
6
|
8

- é praticamente um espelho

  6
  | |
  4 8

- o filho da direita vira a nova raiz
- a raiz original se torna filho da esquerda

      4
       |
       6
      | |
      5 8

- se o filho à direita, que á a nova raiz, tiver filho à esquerda, esse filho se torna um filho à direita da raiz original

  6
  | |
  4 8
  |
  5

Rotação Dupla

- quando o desbalanceamento da arvore ocorreu gerando um zigue-zague na estrutura da arvore

  15
  |
  8
  |
  12

- invertemos a esquerda os nós

  15
  |
  12
  |
  8

- ainda esta desbalanceada, entçao nos fazemos a rotação a direita

  12
  | |
  8 15

aula 07-04

- raiz começa o balanceamento, filho a direita do nó raiz, vai mudar para ser o filho a esquerda do nó raiz e o filho a esquerda de B se torna o A
  A "raiz"
  |
  B "no"
  | |
  C D

"no_esq" "no_dir"

     B
    |  |
    A  D
     |
     C

- Arvore Rubro-Negra:
- ela possui regras para a colocação das cores dos nós
  - não existe nó com duas cores ou sem cor (preto ou vermelho)
  - o nó raiz sempre sera preto
  - quando não temos nó folha nulo, fazendo um calculo com nó nulo será preto
  - se um nó for vermelho, não pode ter filho vermelho e devem ter seus filhos pretos
  - todos os caminhos da raiz até uma folha deve ter o mesmo número de nós pretos
  - o caminho mais longo da raiz ate uma folha não pode ser maior que o dobro do caminho mais curto ou seja, a quantidade de nós pretos tem que ser iguais em cada caminho

    P
    | |
    V V
    |
    P

- nós nulos não tem valores, são nós ficticios

Inserção de nós

- utilizamos a mesma regra da arvore de busca
- para cada novo nó deve ser vermelho
- o rebalanceamento deve ser realizado dependendo da cor do pai e do tio (pai é conexão direta com o nó, e o tio é do lado do nó pai)

Caso de balanceamento:

num caso onde o nó avô é preto, o pai, tio e o neto são vermelhos deve ocorrer de recoloração, primeiro vemos a cor do tio, para alterar e verificar se esta afetando a propriedade

caso o tio seja vermelho, deve ser recoloração, caso seja preto, deve ser uma rotação

sendo recoloração, deve trocar as cores do pai, tio e avô

sendo a rotação temos que ver qual rotação deve ser feita

- caso um filho esteja a direita e o tio tambem, necessario fazer rotação

Aula 05-05

remoção do nó (nó negro com filho vermelho)
como funciona a remoção do nó preto:

- Quando precisamos remover um nó preto que possui apenas um único filho e este filho é vermelho, o processo é relativamente simples.
- Em uma árvore rubro-negra, se um nó possui apenas um filho não nulo, pelas propriedades da árvore, obrigatoriamente o nó a ser removido é preto e seu único filho é vermelho (se o filho fosse preto, os caminhos pela esquerda e pela direita teriam quantidades diferentes de nós pretos, violando as regras).
- Para remover este nó preto, nós o substituímos pelo seu filho vermelho.
- No entanto, a remoção do nó preto altera a quantidade de nós pretos no caminho até a folha (altura negra), violando uma das regras.
- Para corrigir isso e rebalancear a árvore, nós simplesmente mudamos a cor do filho que o substituiu de vermelho para preto.
- Dessa forma, a árvore mantém todas as suas propriedades intactas sem a necessidade de rotações.
  exemplo:

         10 (P)
        /    \
      5 (V)   15 (P)

- ao remover o 10, o 5 toma o lugar do 10, porem mantendo a cor preta

  10(P)
  / \
  5(V) 15(P)

- nó preto com 1 filho preto, vai ficar vermelho

- nó vermelho com 1 filho preto, fica vermelho

          10(V)
         /   \
       5(V)   15(V)
      /

  2(V)
  /
  1(P)

- 1(P) → 2(V) → 5(V) → 10(V)

- 1(P) → 2(V) → 5(V) → 10(P)
  \ /
  4(V) 8(V)

Corrigir remoção:
casos classicos:

- irmão vermelho -> rotação + recoloração
- irmão negro com 2 filhos negros -> recoloração e sobe
- irmão negro, filho esquerdo vermelho e direito negro -> rotação no irmão
- irmão negro, filho a direita vermelho -> rotação do pai

exemplo1:

       20(P)
      /    \
    10(P)   30(P)

/ \ / \  
 5(P) 15(P) 25(P) 35(P)

- a arvore esta okay, sem vermelho, mas ela esta equilibrada e com a quantidade de nós negros correta
- Querendo remover o no 5, a gente vai recolorir o nó 15, o irmão dele.
- no caso 2 como é o 30 com os filhos dele, no caso devemos trocar a cor dele para vermelho.

      20(P)

  / \  
   10(P) 30(V)
  \ / \  
   15(V) 25(P) 35(P)

exemplo2:

       20(P)
      /    \
    10(P)   30(P)

/ \ / \  
 5(P) 15(P) 25(P) 35(P)
\
 12(V)

- A arvore esta equilibrada, mesmo com o 12 sendo vermelho.
- caso a gente queira remover o 5 a arvore fica desequilibrada.
- utilizamos o caso 3: o irmão negro e filho vermelho vão fazer rotação e recolocaração

      20(P)

  / \  
   10(P) 30(P)
  \ / \  
   12(P) 25(P) 35(P)
  \
   15(V)

- após realizarmos o caso 3, analisando novamente a gente percebe que ele continua desequilibrada, agora vamos fazer o passo 4.
- rotação do pai com o irmão para a esquerda

        20(P)
       /     \

  12(P) 30(P)
  / \ / \  
  10(P) \ 25(P) 35(P)
  15(P)

---

Grafos:

- Um grafo é uma estrutura de dados composta por nós e arestas

Representação de grafos:

- lista de adjacência
- matriz de adjacência

Representações:
G(V, E)
V é o conjunto de vertices, E é o conjunto de arestas
G é uma função que mapeia cada vertice para um conjunto de vertices adjacentes a ele

    1 -- 2
    |  / |
    3 -- 4

V {1,2,3,4,5}
E {{a1,a2,a3,a4,a5,a6}}

- no caso a aresta é a ligação entre os vertices

exemplo:

a1: 1 -> 2
a2: 1 -> 2
a3: 2 -> 2
a4: 2 -> 3
a5: 1 -> 3
a6: 3 -> 4

Tipos de Grafos:

- Grafo simples: não tem arestas duplas nem laços (arestas de um vértice para ele mesmo)
- Grafo direcionado: arestas com direção
- Grafo ponderado: arestas com peso
- Grafo conexo: existe um caminho entre quaisquer dois vértices
- Arvore: grafo conexo e sem ciclos
