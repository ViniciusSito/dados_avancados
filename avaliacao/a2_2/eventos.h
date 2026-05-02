#ifndef EVENTOS_H
#define EVENTOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Enums
typedef enum {
  ACIDENTE_TRANSITO,
  FALHA_SEMAFORO,
  INTERRUPCAO_ENERGIA,
  ALAGAMENTO,
  INCENDIO
} TipoEvento;

typedef enum { ATIVO, RESOLVIDO } StatusEvento;

// Structs
typedef struct {
  int dia;
  int mes;
  int ano;
  int hora;
  int minuto;
} DataHora;

typedef struct NoAVL {
  int id;
  TipoEvento tipo;
  int severidade;
  DataHora data_hora;
  char regiao[50];
  StatusEvento status;

  int altura;
  struct NoAVL *esq;
  struct NoAVL *dir;
} NoAVL;

extern int total_rotacoes;

NoAVL *criarNo(int id, TipoEvento tipo, int severidade, DataHora data_hora,
               const char *regiao);
void liberarArvore(NoAVL *raiz);
int max(int a, int b);
int getAltura(NoAVL *no);
int getFatorBalanceamento(NoAVL *no);

NoAVL *inserirEvento(NoAVL *no, int id, TipoEvento tipo, int severidade,
                     DataHora data_hora, const char *regiao);
NoAVL *removerEvento(NoAVL *raiz, int id, int *removido);
NoAVL *buscarEvento(NoAVL *raiz, int id);

void alterarStatus(NoAVL *raiz, int id, StatusEvento novo_status);
void atualizarSeveridade(NoAVL *raiz, int id, int nova_severidade);
void listarAtivosPorSeveridade(NoAVL *raiz, int min_sev, int max_sev);
void relatorioPorRegiao(NoAVL *raiz, const char *regiao);
void buscarPorIntervaloID(NoAVL *raiz, int min_id, int max_id);

int totalNos(NoAVL *raiz);
int totalAtivos(NoAVL *raiz);
float fatorBalanceamentoMedio(NoAVL *raiz);
const char *obterNomeTipoEvento(TipoEvento tipo);
const char *obterNomeStatus(StatusEvento status);
void imprimirEvento(NoAVL *evento);

#endif
