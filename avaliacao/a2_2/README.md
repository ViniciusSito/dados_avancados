# Sistema de Gerenciamento de Eventos Críticos de uma Cidade Inteligente

Este sistema gerencia eventos críticos urbanos (acidentes de trânsito, falhas em semáforos, interrupções de energia, alagamentos, incêndios) utilizando uma **Árvore AVL** em linguagem C.

## Estrutura do Projeto

O código está dividido nos seguintes arquivos:
- `eventos.h`: Declarações das estruturas (enums, NoAVL) e assinaturas das funções.
- `eventos.c`: Implementação da lógica de negócio e das operações de gerenciamento da Árvore AVL (inserção, remoção, rotações, buscas, atualizações e métricas).
- `main.c`: Interface em linha de comando interativa para o usuário realizar operações no sistema.
- `Relatorio.md`: Arquivo para registrar as capturas de testes realizados.

## Instruções de Compilação e Execução

### Pré-requisitos
- Compilador GCC instalado.
- O utilitário `make` instalado no ambiente (ou `mingw32-make` no Windows).

### Compilar e Rodar
Via prompt de comando (Terminal/PowerShell), acesse a pasta `a2_2` e execute o comando:
```bash
mingw32-make
```
Ou se tiver o make padrão configurado:
```bash
make
```

Para executar o programa compilado:
```bash
a2_2.exe
```

## Funcionalidades
1. **Gerenciamento da AVL**: Inserção de eventos (mantendo a árvore balanceada através de fator de balanceamento e rotações), remoção condicional (somente status `RESOLVIDO` pode ser removido, com rebalanceamento subsequente) e busca por ID.
2. **Consultas Avançadas**: Listagem de eventos por intervalo de severidade, listagem de eventos ativos de uma região específica e listagem de eventos dentro de um intervalo de IDs.
3. **Atualizações**: Alterar status de ativo para resolvido e modificar o grau de severidade de um evento.
4. **Métricas**: Visualizar altura total da árvore, total de nós, quantidade de eventos ativos, fator de balanceamento médio e total de rotações realizadas desde a execução.

A chave primária utilizada para manter a árvore ordenada é o `ID` do evento.
