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


* maior à esquerda 
* menor à direita


	  30
     |     |
    10    40
   |  |  |  |
   5  15 35 50
         |	 |
        45  55

no caso querendo remover o nó 40 a gente pega o menor numero a direita, analisando a arvore, é o 45 o menor numero, então trocando pelo menor numero, a arvore ainda mantem a logica, pois caso queira remover o 45 escolhemos o 50 e analisando novamente a arvore, o 55 continua sendo maior e ainda na parte   da direita.



	  30
      |   |
    10    45
   |  |  |  |
   5  15 35 50
        	 |
             55



sempre que trocar o nó, eliminar para não houver duplicata



Aula 31-03

Balanceamento de arvores:
- a medida que inserimos ou removemos nós estamos balanceando ou não 
. a cada inserção ou remoção vai ter que recalcular a arvore inteira

- uma arvore balanceada é considerada com 0, 1 ou -1, acima de 1 ou -1 esta desbalanceada
     
  0         A
          |   |
  1       B   E
         | | | |
  2      C D F G
               |
  3            H
              | |
  4           I J

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
Ae= 0  Ad = 4 

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
  |  |
  5  8
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
    |  |
    4  8
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
 |  |
 8  15
