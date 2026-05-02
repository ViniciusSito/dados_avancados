#include "eventos.h"

// Variável global para contar o total de rotações da árvore AVL durante a
// execução
int total_rotacoes = 0;

// Função auxiliar simples para retornar o maior entre dois números.
// Ajuda muito na hora de calcular a altura da árvore.
int max(int a, int b) { return (a > b) ? a : b; }

// Retorna a altura de um nó. Se o nó for nulo (árvore vazia naquele lado), a
// altura é 0.
int getAltura(NoAVL *no) {
  if (no == NULL)
    return 0;
  return no->altura;
}

// Calcula o fator de balanceamento (Altura da Esquerda - Altura da Direita).
// Se o resultado for maior que 1 ou menor que -1, a árvore precisará de
// rotação.
int getFatorBalanceamento(NoAVL *no) {
  if (no == NULL)
    return 0;
  return getAltura(no->esq) - getAltura(no->dir);
}

// Cria um novo nó na memória para a árvore, já preenchendo com os dados do
// evento.
NoAVL *criarNo(int id, TipoEvento tipo, int severidade, DataHora data_hora,
               const char *regiao) {
  NoAVL *novo = (NoAVL *)malloc(sizeof(NoAVL));
  if (!novo) {
    printf("Erro de alocacao de memoria!\n");
    exit(1);
  }
  novo->id = id;
  novo->tipo = tipo;
  novo->severidade = severidade;
  novo->data_hora = data_hora;

  // Copia a string da região garantindo que não ultrapasse 49 caracteres e
  // finaliza com '\0'
  strncpy(novo->regiao, regiao, 49);
  novo->regiao[49] = '\0';

  // Todo evento recém-criado começa com o status ATIVO por padrão
  novo->status = ATIVO;

  novo->altura =
      1; // Um nó novo sempre entra nas folhas, então sua altura inicial é 1
  novo->esq = NULL;
  novo->dir = NULL;

  return novo;
}

// Rotação para a Direita (usada quando a subárvore esquerda fica muito pesada)
NoAVL *rotacaoDireita(NoAVL *y) {
  NoAVL *x = y->esq;
  NoAVL *T2 = x->dir;

  // Faz a troca de ponteiros: x sobe, y desce para a direita de x
  x->dir = y;
  y->esq = T2;

  // Depois de rodar, as alturas de x e y precisam ser recalculadas
  y->altura = max(getAltura(y->esq), getAltura(y->dir)) + 1;
  x->altura = max(getAltura(x->esq), getAltura(x->dir)) + 1;

  // Aumenta o nosso contador global de rotações
  total_rotacoes++;

  // Retorna a nova raiz dessa subárvore
  return x;
}

// Rotação para a Esquerda (usada quando a subárvore direita fica muito pesada)
NoAVL *rotacaoEsquerda(NoAVL *x) {
  NoAVL *y = x->dir;
  NoAVL *T2 = y->esq;

  // Faz a troca de ponteiros: y sobe, x desce para a esquerda de y
  y->esq = x;
  x->dir = T2;

  // Recalcula as alturas
  x->altura = max(getAltura(x->esq), getAltura(x->dir)) + 1;
  y->altura = max(getAltura(y->esq), getAltura(y->dir)) + 1;

  total_rotacoes++;

  return y;
}

// Insere um novo evento mantendo as regras da árvore AVL
NoAVL *inserirEvento(NoAVL *no, int id, TipoEvento tipo, int severidade,
                     DataHora data_hora, const char *regiao) {
  // 1. Inserção normal de uma Árvore Binária de Busca (BST)
  if (no == NULL)
    return criarNo(id, tipo, severidade, data_hora, regiao);

  // Se o ID for menor, vai pra esquerda. Se for maior, vai pra direita.
  if (id < no->id)
    no->esq = inserirEvento(no->esq, id, tipo, severidade, data_hora, regiao);
  else if (id > no->id)
    no->dir = inserirEvento(no->dir, id, tipo, severidade, data_hora, regiao);
  else {
    // Como o ID é a chave primária, não faz sentido ter dois eventos com o
    // mesmo ID
    printf("Erro: ID do evento %d ja existe!\n", id);
    return no;
  }

  // 2. Atualiza a altura do nó atual após a inserção nos filhos
  no->altura = 1 + max(getAltura(no->esq), getAltura(no->dir));

  // 3. Pega o fator de balanceamento para verificar se a árvore desbalanceou
  int balance = getFatorBalanceamento(no);

  // 4. Analisa os 4 casos possíveis de desbalanceamento e aplica as rotações

  // Caso Esquerda-Esquerda: O peso está na esquerda do filho esquerdo
  if (balance > 1 && id < no->esq->id)
    return rotacaoDireita(no);

  // Caso Direita-Direita: O peso está na direita do filho direito
  if (balance < -1 && id > no->dir->id)
    return rotacaoEsquerda(no);

  // Caso Esquerda-Direita: O peso está na direita do filho esquerdo (precisa de
  // rotação dupla)
  if (balance > 1 && id > no->esq->id) {
    no->esq = rotacaoEsquerda(no->esq);
    return rotacaoDireita(no);
  }

  // Caso Direita-Esquerda: O peso está na esquerda do filho direito (rotação
  // dupla)
  if (balance < -1 && id < no->dir->id) {
    no->dir = rotacaoDireita(no->dir);
    return rotacaoEsquerda(no);
  }

  // Se chegou até aqui, a árvore já está balanceada. Apenas retorna o nó.
  return no;
}

// Desce até o final da esquerda para achar o nó com o menor valor (útil na
// remoção)
NoAVL *noMinimo(NoAVL *no) {
  NoAVL *atual = no;
  while (atual->esq != NULL)
    atual = atual->esq;
  return atual;
}

// Remove um evento da árvore se ele já estiver resolvido
NoAVL *removerEvento(NoAVL *raiz, int id, int *removido) {
  // Caso base: árvore vazia
  if (raiz == NULL) {
    return raiz;
  }

  // Procura o nó na esquerda ou na direita
  if (id < raiz->id) {
    raiz->esq = removerEvento(raiz->esq, id, removido);
  } else if (id > raiz->id) {
    raiz->dir = removerEvento(raiz->dir, id, removido);
  } else {
    // Encontrou o nó que precisa ser removido!

    // Regra de negócio: só pode apagar se o status for RESOLVIDO
    if (raiz->status != RESOLVIDO) {
      printf("Erro: O evento ID %d nao pode ser removido pois nao esta "
             "RESOLVIDO.\n",
             id);
      *removido = 0; // Sinaliza que falhou
      return raiz;
    }

    *removido = 1; // Sinaliza que deu certo

    // Caso 1 ou 2: O nó tem só um filho ou nenhum filho
    if ((raiz->esq == NULL) || (raiz->dir == NULL)) {
      NoAVL *temp = raiz->esq ? raiz->esq : raiz->dir;

      if (temp == NULL) { // Sem filhos: apenas remove o próprio nó
        temp = raiz;
        raiz = NULL;
      } else { // Um filho: copia os dados do filho para o nó atual e remove o
               // filho
        *raiz = *temp;
      }
      free(temp);
    } else {
      // Caso 3: O nó tem dois filhos
      // Precisamos buscar o sucessor (o menor valor da subárvore direita)
      NoAVL *temp = noMinimo(raiz->dir);

      // Substituímos os dados do nó atual pelos dados do sucessor
      raiz->id = temp->id;
      raiz->tipo = temp->tipo;
      raiz->severidade = temp->severidade;
      raiz->data_hora = temp->data_hora;
      strncpy(raiz->regiao, temp->regiao, 50);
      raiz->status = temp->status;

      // Agora chamamos a remoção lá em baixo para apagar o sucessor duplicado
      int lixo = 0;
      raiz->dir = removerEvento(raiz->dir, temp->id, &lixo);
    }
  }

  // Se a árvore só tinha aquele nó e ele foi removido, retorna nulo
  if (raiz == NULL)
    return raiz;

  // Atualiza a altura do nó atual
  raiz->altura = 1 + max(getAltura(raiz->esq), getAltura(raiz->dir));

  // Pega o fator de balanceamento para checar se a remoção desbalanceou
  int balance = getFatorBalanceamento(raiz);

  // Verifica os 4 casos de desbalanceamento e arruma com rotações

  // Desbalanceou pra esquerda
  if (balance > 1 && getFatorBalanceamento(raiz->esq) >= 0)
    return rotacaoDireita(raiz);

  if (balance > 1 && getFatorBalanceamento(raiz->esq) < 0) {
    raiz->esq = rotacaoEsquerda(raiz->esq);
    return rotacaoDireita(raiz);
  }

  // Desbalanceou pra direita
  if (balance < -1 && getFatorBalanceamento(raiz->dir) <= 0)
    return rotacaoEsquerda(raiz);

  if (balance < -1 && getFatorBalanceamento(raiz->dir) > 0) {
    raiz->dir = rotacaoDireita(raiz->dir);
    return rotacaoEsquerda(raiz);
  }

  return raiz;
}

// Busca simples por um ID na árvore
NoAVL *buscarEvento(NoAVL *raiz, int id) {
  if (raiz == NULL || raiz->id == id)
    return raiz;

  if (id < raiz->id)
    return buscarEvento(raiz->esq, id);

  return buscarEvento(raiz->dir, id);
}

// Altera o status do evento (ex: de ATIVO para RESOLVIDO)
void alterarStatus(NoAVL *raiz, int id, StatusEvento novo_status) {
  NoAVL *evento = buscarEvento(raiz, id);
  if (evento != NULL) {
    evento->status = novo_status;
    printf("Status do evento %d alterado com sucesso.\n", id);
  } else {
    printf("Evento %d nao encontrado.\n", id);
  }
}

// Muda a severidade, mas só permite se o evento ainda estiver ATIVO
void atualizarSeveridade(NoAVL *raiz, int id, int nova_severidade) {
  if (nova_severidade < 1 || nova_severidade > 5) {
    printf("Severidade invalida. Deve ser entre 1 e 5.\n");
    return;
  }
  NoAVL *evento = buscarEvento(raiz, id);
  if (evento != NULL) {
    if (evento->status == ATIVO) {
      evento->severidade = nova_severidade;
      printf("Severidade do evento %d atualizada com sucesso.\n", id);
    } else {
      printf("Evento %d nao esta ativo. Nao e possivel alterar a severidade.\n",
             id);
    }
  } else {
    printf("Evento %d nao encontrado.\n", id);
  }
}

// Retorna uma string bonitinha dependendo do enum do tipo de evento
const char *obterNomeTipoEvento(TipoEvento tipo) {
  switch (tipo) {
  case ACIDENTE_TRANSITO:
    return "Acidente de Transito";
  case FALHA_SEMAFORO:
    return "Falha em Semaforo";
  case INTERRUPCAO_ENERGIA:
    return "Interrupcao de Energia";
  case ALAGAMENTO:
    return "Alagamento";
  case INCENDIO:
    return "Incendio";
  default:
    return "Desconhecido";
  }
}

// Transforma o enum de status numa string legível
const char *obterNomeStatus(StatusEvento status) {
  return (status == ATIVO) ? "Ativo" : "Resolvido";
}

// Printa todos os dados do evento em uma única linha formatada
void imprimirEvento(NoAVL *evento) {
  if (evento != NULL) {
    printf("[%04d] %s | Severidade: %d | Data: %02d/%02d/%04d %02d:%02d | "
           "Regiao: %s | Status: %s\n",
           evento->id, obterNomeTipoEvento(evento->tipo), evento->severidade,
           evento->data_hora.dia, evento->data_hora.mes, evento->data_hora.ano,
           evento->data_hora.hora, evento->data_hora.minuto, evento->regiao,
           obterNomeStatus(evento->status));
  }
}

// Lista eventos usando percurso em-ordem para imprimir por ordem crescente de
// ID
void listarAtivosPorSeveridade(NoAVL *raiz, int min_sev, int max_sev) {
  if (raiz != NULL) {
    listarAtivosPorSeveridade(raiz->esq, min_sev, max_sev); // Visita esquerda

    // Checa se bate com os filtros exigidos
    if (raiz->status == ATIVO && raiz->severidade >= min_sev &&
        raiz->severidade <= max_sev) {
      imprimirEvento(raiz);
    }

    listarAtivosPorSeveridade(raiz->dir, min_sev, max_sev); // Visita direita
  }
}

// Imprime todos os ativos de uma mesma região (também usando percurso em-ordem)
void relatorioPorRegiao(NoAVL *raiz, const char *regiao) {
  if (raiz != NULL) {
    relatorioPorRegiao(raiz->esq, regiao);

    // A função strcmp compara strings; se retornar 0, elas são iguais
    if (raiz->status == ATIVO && strcmp(raiz->regiao, regiao) == 0) {
      imprimirEvento(raiz);
    }

    relatorioPorRegiao(raiz->dir, regiao);
  }
}

// Busca eventos em um intervalo [min_id, max_id].
// Ao invés de varrer a árvore inteira, a gente usa a inteligência da BST pra
// não visitar lados desnecessários.
void buscarPorIntervaloID(NoAVL *raiz, int min_id, int max_id) {
  if (raiz == NULL)
    return;

  // Se o ID da raiz for maior que o mínimo, significa que pode ter resultados
  // válidos na esquerda
  if (raiz->id > min_id) {
    buscarPorIntervaloID(raiz->esq, min_id, max_id);
  }

  // Se a raiz está dentro do intervalo, a gente printa
  if (raiz->id >= min_id && raiz->id <= max_id) {
    imprimirEvento(raiz);
  }

  // Se o ID da raiz for menor que o máximo, significa que pode ter resultados
  // válidos na direita
  if (raiz->id < max_id) {
    buscarPorIntervaloID(raiz->dir, min_id, max_id);
  }
}

// Conta quantos nós tem na árvore de forma recursiva
int totalNos(NoAVL *raiz) {
  if (raiz == NULL)
    return 0;
  return 1 + totalNos(raiz->esq) +
         totalNos(raiz->dir); // 1 (nó atual) + filhos esquerdo e direito
}

// Conta só os eventos que estão ATIVOS
int totalAtivos(NoAVL *raiz) {
  if (raiz == NULL)
    return 0;
  int cont = (raiz->status == ATIVO) ? 1 : 0;
  return cont + totalAtivos(raiz->esq) + totalAtivos(raiz->dir);
}

// Função auxiliar que soma o valor absoluto dos fatores de balanceamento de
// todos os nós
int somaFB(NoAVL *no) {
  if (no == NULL)
    return 0;
  int fb = getFatorBalanceamento(no);
  if (fb < 0)
    fb = -fb; // Transformamos em positivo (módulo)
  return fb + somaFB(no->esq) + somaFB(no->dir);
}

// Tira a média aritmética do fator de balanceamento (Soma total / Quantidade de
// Nós)
float fatorBalanceamentoMedio(NoAVL *raiz) {
  int _totalNos = totalNos(raiz);
  if (_totalNos == 0)
    return 0.0f; // Evita divisão por zero

  return (float)somaFB(raiz) / _totalNos;
}

// Função importantíssima para liberar toda a memória (usando percurso
// pós-ordem) antes de fechar o sistema
void liberarArvore(NoAVL *raiz) {
  if (raiz != NULL) {
    liberarArvore(raiz->esq); // Desce tudo pra esquerda
    liberarArvore(raiz->dir); // Desce tudo pra direita
    free(raiz);               // Apaga os nós subindo
  }
}
