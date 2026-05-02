#include "eventos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void exibirMenu() {
  printf("\n======================================================\n");
  printf("   SISTEMA DE EVENTOS CRITICOS DA CIDADE INTELIGENTE  \n");
  printf("======================================================\n");
  printf("1.  Inserir novo evento\n");
  printf("2.  Buscar evento por ID\n");
  printf("3.  Alterar status do evento (Ativo -> Resolvido)\n");
  printf("4.  Atualizar severidade de evento ativo\n");
  printf("5.  Remover evento (apenas Resolvidos)\n");
  printf("6.  Listar eventos ativos por intervalo de severidade\n");
  printf("7.  Relatorio de eventos ativos por regiao\n");
  printf("8.  Buscar eventos por intervalo de ID\n");
  printf("9.  Metricas da Arvore AVL\n");
  printf("0.  Sair\n");
  printf("======================================================\n");
  printf("Escolha uma opcao: ");
}

void limparBuffer() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

int main() {
  NoAVL *raiz = NULL;
  int opcao;

  do {
    exibirMenu();
    if (scanf("%d", &opcao) != 1) {
      printf("Entrada invalida.\n");
      limparBuffer();
      continue;
    }

    switch (opcao) {
    case 1: {
      int id, tipo_int, sev, d, m, a, h, min;
      char regiao[50];

      printf("\n--- Inserir Novo Evento ---\n");
      printf("ID do evento: ");
      scanf("%d", &id);

      if (buscarEvento(raiz, id) != NULL) {
        printf("Erro: Ja existe um evento com o ID %d.\n", id);
        break;
      }

      printf("Tipo de evento (0-Acidente, 1-Falha Semaforo, 2-Energia, "
             "3-Alagamento, 4-Incendio): ");
      scanf("%d", &tipo_int);
      if (tipo_int < 0 || tipo_int > 4) {
        printf("Tipo invalido!\n");
        break;
      }

      printf("Severidade (1 a 5): ");
      scanf("%d", &sev);
      if (sev < 1 || sev > 5) {
        printf("Severidade invalida!\n");
        break;
      }

      printf("Data (DD MM AAAA): ");
      scanf("%d %d %d", &d, &m, &a);

      printf("Hora (HH MM): ");
      scanf("%d %d", &h, &min);

      limparBuffer();
      printf("Regiao: ");
      fgets(regiao, sizeof(regiao), stdin);
      regiao[strcspn(regiao, "\n")] = 0;

      DataHora dh = {d, m, a, h, min};
      raiz = inserirEvento(raiz, id, (TipoEvento)tipo_int, sev, dh, regiao);
      printf("Evento inserido com sucesso!\n");
      break;
    }
    case 2: {
      int id;
      printf("Digite o ID do evento: ");
      scanf("%d", &id);
      NoAVL *encontrado = buscarEvento(raiz, id);
      if (encontrado) {
        printf("\nEvento Encontrado:\n");
        imprimirEvento(encontrado);
      } else {
        printf("Evento ID %d nao encontrado.\n", id);
      }
      break;
    }
    case 3: {
      int id;
      printf("Digite o ID do evento para resolver: ");
      scanf("%d", &id);
      alterarStatus(raiz, id, RESOLVIDO);
      break;
    }
    case 4: {
      int id, nova_sev;
      printf("Digite o ID do evento: ");
      scanf("%d", &id);
      printf("Digite a nova severidade (1 a 5): ");
      scanf("%d", &nova_sev);
      atualizarSeveridade(raiz, id, nova_sev);
      break;
    }
    case 5: {
      int id;
      int removido = 0;
      printf("Digite o ID do evento a remover: ");
      scanf("%d", &id);

      NoAVL *evento = buscarEvento(raiz, id);
      if (evento == NULL) {
        printf("Evento ID %d nao encontrado.\n", id);
      } else {
        raiz = removerEvento(raiz, id, &removido);
        if (removido) {
          printf("Evento ID %d removido com sucesso.\n", id);
        }
      }
      break;
    }
    case 6: {
      int min, max;
      printf("Severidade minima (1 a 5): ");
      scanf("%d", &min);
      printf("Severidade maxima (1 a 5): ");
      scanf("%d", &max);
      printf("\nEventos ativos com severidade entre %d e %d:\n", min, max);
      listarAtivosPorSeveridade(raiz, min, max);
      break;
    }
    case 7: {
      char regiao[50];
      limparBuffer();
      printf("Digite a regiao: ");
      fgets(regiao, sizeof(regiao), stdin);
      regiao[strcspn(regiao, "\n")] = 0;

      printf("\nEventos ativos na regiao '%s' (Ordenados por ID):\n", regiao);
      relatorioPorRegiao(raiz, regiao);
      break;
    }
    case 8: {
      int min_id, max_id;
      printf("ID minimo: ");
      scanf("%d", &min_id);
      printf("ID maximo: ");
      scanf("%d", &max_id);

      printf("\nEventos com ID entre %d e %d:\n", min_id, max_id);
      buscarPorIntervaloID(raiz, min_id, max_id);
      break;
    }
    case 9: {
      printf("\n--- Metricas da Arvore AVL ---\n");
      printf("Altura total da arvore: %d\n", getAltura(raiz));
      printf("Numero total de nos: %d\n", totalNos(raiz));
      printf("Numero de eventos ativos: %d\n", totalAtivos(raiz));
      printf("Fator de balanceamento medio: %.2f\n",
             fatorBalanceamentoMedio(raiz));
      printf("Quantidade total de rotacoes realizadas: %d\n", total_rotacoes);
      break;
    }
    case 0:
      printf("Saindo do sistema...\n");
      liberarArvore(raiz);
      break;
    default:
      printf("Opcao invalida!\n");
    }
  } while (opcao != 0);

  return 0;
}
