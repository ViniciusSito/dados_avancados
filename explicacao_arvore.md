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
    |     |
    10    45
   |  |  |  |
   5  15 35 50
        	 |
             55



sempre que trocar o nó, eliminar para não houver duplicata





