# bizarro-sort-C

Sistema interativo em C para **carregar, ordenar, buscar e medir** vetores de inteiros lidos de arquivo.

Compara cinco algoritmos clássicos de ordenação (Bubble, Insertion, Selection, Merge e Quick) e duas buscas (linear e binária) com o **Sudorm-rf Sort**, uma ordenação híbrida e paralela (OpenMP) feita para este projeto. Ao final, gera um relatório de desempenho em `log.txt`.

---

## Sumário

- [Funcionalidades](#funcionalidades)
- [Requisitos](#requisitos)
- [Compilação](#compilação)
- [Uso](#uso)
- [Formato do arquivo de entrada](#formato-do-arquivo-de-entrada)
- [Relatório de desempenho](#relatório-de-desempenho)
- [Algoritmos](#algoritmos)
- [Sudorm-rf Sort](#sudorm-rf-sort)
- [Estrutura do projeto](#estrutura-do-projeto)
- [Referência da API](#referência-da-api)
- [Limitações conhecidas](#limitações-conhecidas)

---

## Funcionalidades

| Opção do menu | O que faz |
|---|---|
| **1 - Carregar arquivo** | Lê os inteiros de um arquivo de texto para a memória. O vetor cresce sob demanda, então não há limite fixo de tamanho. |
| **2 - Buscar elemento** | Busca binária ou linear, com o tempo medido em ms. Se o vetor estiver desordenado, a busca binária pergunta se você quer ordená-lo antes (com Quick Sort). |
| **3 - Ordenar vetor** | Ordena com o algoritmo escolhido, mostra o tempo gasto e oferece salvar o resultado em um arquivo novo. |
| **4 - Gerar relatório (Log)** | Roda todos os algoritmos sobre **cópias** do vetor carregado e grava os tempos em `log.txt`. |
| **5 - Sair** | Libera a memória e encerra. |

---

## Requisitos

- GCC com suporte a C99 ou mais novo
- OpenMP (`libgomp`)
- Linux ou outro sistema POSIX (o código usa `unistd.h`, `usleep` e `clock_gettime`)

Instalação das dependências:

```bash
# Ubuntu / Debian
sudo apt update && sudo apt install build-essential libomp-dev

# Fedora
sudo dnf install gcc make libgomp

# Arch Linux (o OpenMP já vem com o gcc)
sudo pacman -S gcc make

# openSUSE
sudo zypper install gcc make libgomp1
```

---

## Compilação

Na raiz do repositório:

```bash
gcc -O3 -march=native -flto -DNDEBUG -fopenmp \
    main.c \
    ordenar/ordena.c \
    buscar/buscas.c \
    arquivos/arquivos.c \
    relatorios/relatorio.c \
    EXTRA/ordenacao.c \
    -o programa
```

| Flag | Por que está aqui |
|---|---|
| `-O3` | Otimização agressiva. Sem ela as medições de tempo não representam bem os algoritmos. |
| `-march=native` | Gera instruções para a CPU da máquina que compila. O binário pode não rodar em outra máquina. |
| `-flto` | Otimização em tempo de link, que permite *inlining* entre arquivos `.c`. |
| `-DNDEBUG` | Desativa `assert`s. |
| `-fopenmp` | **Obrigatória.** Ativa o paralelismo do Sudorm-rf Sort. |

> Para gerar um binário que rode em qualquer máquina x86-64, remova `-march=native`.

---

## Uso

```bash
./programa
```

Exemplo de sessão:

```text
=============================================
           SISTEMA DE ORDENACAO
=============================================
 1 - Carregar arquivo
 2 - Buscar elemento
 3 - Ordenar vetor
 4 - Gerar relatório (Log)
 5 - Sair
=============================================
Escolha >1

Digite o caminho do arquivo:
> dados.txt

[OK] 20000 elementos carregados.

Escolha >3
 ...
 6 - Sudorm-rf Sort
 0 - Voltar
Escolha >6

Vetor ordenado com Sudorm-rf Sort.
Tempo de execucao da ordenacao: 2.358000000 ms

Deseja salvar o vetor ordenado em um novo arquivo?
1 - Sim
0 - Nao
Escolha > 1
Digite o caminho do novo arquivo:
> ordenado.txt

[OK] Vetor salvo em ordenado.txt.
```

Para gerar um arquivo de teste com números aleatórios:

```bash
shuf -i 1-1000000 -n 100000 > dados.txt
```

O menu também funciona sem interação, recebendo as opções pela entrada padrão:

```bash
printf '1\ndados.txt\n4\n5\n' | ./programa   # carrega, gera o log.txt e sai
```

---

## Formato do arquivo de entrada

Um arquivo de texto com inteiros (`int`) separados por espaço, tabulação ou quebra de linha:

```text
42
-7
1000
3 8 15
```

- A leitura para no primeiro conteúdo que não for um inteiro.
- Um arquivo sem nenhum inteiro válido é rejeitado, e o vetor carregado antes continua na memória.
- O caminho aceita até 255 caracteres e **não pode ter espaços**, porque é lido com `scanf("%255s")`.

Ao salvar (opção 3), o arquivo é aberto no modo `"wx"`: **um arquivo que já existe nunca é sobrescrito**, e o programa mostra um erro nesse caso.

---

## Relatório de desempenho

A opção 4 grava `log.txt` no diretório atual, substituindo o anterior. Cada algoritmo roda sobre uma **cópia** do vetor original, então todos recebem a mesma entrada e o vetor carregado não muda.

Exemplo real (20 000 inteiros aleatórios):

```text
RELATORIO DE DESEMPENHO
=======================
Quantidade de elementos: 20000
Repeticoes por algoritmo: 1
Valor utilizado nas buscas: 121255

ALGORITMOS DE ORDENACAO
-----------------------
Selection Sort:          185.213000000 ms
Quick Sort:              2.227000000 ms
Merge Sort:              3.597000000 ms
Insertion Sort:          22.441000000 ms
Bubble Sort:             996.103000000 ms
SudoRm-rf Sort:          2.358000000 ms

ALGORITMOS DE BUSCA
--------------------
Busca Linear:            0.001000000 ms
Busca Binaria + Quick Sort: 0.002000000 ms
```

- **Valor buscado:** o último elemento do vetor original.
- **Repetições:** 1 para vetores com até 1 000 elementos, 10 para 1 001 a 5 000 e 1 acima disso. O tempo registrado é a média das repetições.

> ⚠️ O relatório roda Bubble, Selection e Insertion Sort, que são O(n²). Com centenas de milhares de elementos ou mais, ele pode levar **minutos ou horas**.

---

## Algoritmos

### Ordenação

| Algoritmo | Melhor caso | Caso médio | Pior caso | Memória extra | Estável | Arquivo |
|---|---|---|---|---|---|---|
| Bubble Sort (com parada antecipada) | O(n) | O(n²) | O(n²) | O(1) | Sim | `ordenar/ordena.c` |
| Insertion Sort | O(n) | O(n²) | O(n²) | O(1) | Sim | `ordenar/ordena.c` |
| Selection Sort | O(n²) | O(n²) | O(n²) | O(1) | Não | `ordenar/ordena.c` |
| Merge Sort (top-down) | O(n log n) | O(n log n) | O(n log n) | O(n) | Sim | `ordenar/ordena.c` |
| Quick Sort (Lomuto + mediana de três) | O(n log n) | O(n log n) | O(n²) | O(log n) a O(n) de pilha | Não | `ordenar/ordena.c` |
| **Sudorm-rf Sort** | **O(n)** | O(n log n) | O(n log n) | O(n) | Sim | `EXTRA/ordenacao.c` |
| Bogo Sort (só didático, fora do menu) | O(n) | O(n·n!) | ilimitado | O(1) | Não | `ordenar/ordena.c` |

### Busca

| Algoritmo | Complexidade | Pré-requisito | Arquivo |
|---|---|---|---|
| Busca Linear | O(n) | nenhum | `buscar/buscas.c` |
| Busca Binária | O(log n) | vetor em ordem crescente | `buscar/buscas.c` |

As duas devolvem o **índice (base 0)** do elemento ou `-1` se ele não existir.

---

## Sudorm-rf Sort

O algoritmo fica em `EXTRA/ordenacao.c` e é chamado pela função `checar()`. Ele junta várias técnicas, e cada uma entra em ação conforme o tamanho e o estado do vetor.

```text
checar(vet, tam)
 │
 ├─ tam < 2 ......................... nada a fazer
 ├─ já está crescente ............... retorna sem mexer        → O(n)
 ├─ está decrescente ................ inverte no lugar          → O(n)
 └─ caso geral → ordenar()
      ├─ tam < 131 072 .............. Merge Sort sequencial
      │                                (Insertion Sort em trechos ≤ 16)
      └─ tam ≥ 131 072 .............. Merge Sort paralelo (OpenMP)
           1. divide em blocos de 16 384 e ordena cada um em paralelo
           2. faz o merge subindo a árvore, nível por nível,
              dividindo cada merge em fatias de 8 192 saídas (Merge Path)
           3. alterna os buffers origem/destino a cada nível (ping-pong)
           4. copia de volta para o vetor original, se preciso
```

### As técnicas

1. **Pré-checagem em O(n).** Uma única passada identifica se o vetor já está crescente ou decrescente. Entradas já ordenadas, ou em ordem inversa, são resolvidas em tempo linear.

2. **Insertion Sort nas folhas.** Abaixo de 16 elementos, a recursão do Merge Sort custa mais do que o Insertion Sort, que é rápido em trechos pequenos por aproveitar bem o cache.

3. **Paralelismo por blocos.** Acima de 131 072 elementos, o vetor é dividido em blocos de 16 384, que as threads ordenam ao mesmo tempo (`#pragma omp for schedule(static)`).

4. **Merge Path.** Em um merge comum, só uma thread trabalha em cada par de sub-vetores, e nos últimos níveis da árvore sobra uma única thread. O Merge Path divide **a saída** de cada merge em fatias de tamanho fixo (8 192). Para cada fatia, uma busca binária na diagonal (`mergePath()`) encontra exatamente quantos elementos vêm de cada metade. Assim **todas as threads trabalham em todos os níveis**, inclusive no último merge.

5. **Merge sem desvios (branchless).** O laço principal de `mergeDiag()` troca o `if/else` por aritmética:

   ```c
   int bMenor = (valB < valA);
   *out++ = bMenor ? valB : valA;
   i += (1 - bMenor);
   j += bMenor;
   ```

   Isso evita erros de previsão de desvio, que são caros com dados aleatórios.

6. **Buffers ping-pong.** Em vez de copiar o resultado de volta a cada nível, os ponteiros `orig` e `dest` são trocados. A cópia final acontece só uma vez, e só se o resultado tiver terminado no buffer auxiliar.

7. **`restrict`.** Avisa o compilador que os ponteiros não se sobrepõem, o que libera vetorização.

8. **Fallback seguro.** Se o `malloc` do caminho paralelo falhar, o algoritmo cai para o Merge Sort sequencial.

### Parâmetros

Definidos no topo de `EXTRA/ordenacao.c`:

| Constante | Valor | Papel |
|---|---|---|
| `limiteInsercao` | 16 | Tamanho máximo de trecho ordenado com Insertion Sort |
| `blocoSort` | 16 384 | Tamanho de cada bloco ordenado independentemente no modo paralelo |
| `blocoMerge` | 8 192 | Quantos elementos de saída cada tarefa de merge produz |
| `limiteParalelo` | 131 072 | A partir deste tamanho, usa o caminho paralelo |

O número de threads segue `omp_get_max_threads()`, que pode ser controlado pela variável de ambiente `OMP_NUM_THREADS`:

```bash
OMP_NUM_THREADS=4 ./programa
```

---

## Estrutura do projeto

```text
bizarro-sort-C/
├── main.c                  # Menu interativo e medição de tempo das opções 2 e 3
├── ordenar/
│   ├── ordena.h            # Algoritmos clássicos de ordenação + auxiliares
│   └── ordena.c
├── buscar/
│   ├── buscas.h            # Busca linear, busca binária, estaOrdenado()
│   └── buscas.c
├── arquivos/
│   ├── arquivos.h          # Leitura e gravação de vetores em arquivo
│   └── arquivos.c
├── relatorios/
│   ├── relatorio.h         # Relatório comparativo de desempenho (log.txt)
│   └── relatorio.c
├── EXTRA/
│   ├── ordenacao.h         # Sudorm-rf Sort (interface: checar)
│   └── ordenacao.c
└── COMORODAR.txt           # Instruções rápidas de compilação
```

---

## Referência da API

### `EXTRA/ordenacao.h`

```c
int checar(int *vet, int tam);
```
Ordena `vet` em ordem crescente com o Sudorm-rf Sort. É o **único ponto de entrada** do módulo, porque as outras funções são `static`. Retorna `0`.

### `ordenar/ordena.h`

```c
void bubbleSort   (int *vet, int tam);
void insertionSort(int *vet, int tam);
void selectionSort(int *vet, int tam);
void mergeSort    (int *vet, int l, int r);     // r é inclusivo: mergeSort(v, 0, tam - 1)
void quickSort    (int *vet, int ini, int fim); // fim é inclusivo: quickSort(v, 0, tam - 1)
int  bogoSort     (int *vet, int tam);          // só didático

// auxiliares
void merge        (int *vet, int l, int mid, int r);
int  partition    (int *vet, int ini, int fim);
int  medianaDeTres(int *vet, int ini, int fim);
void swap         (int *pos1, int *pos2);
int  troca        (int *vet, int i, int j);
void embaralhar   (int *vet, int tam);          // Fisher–Yates
int  confere      (int *vet, int tam);          // 1 se estiver crescente
```

### `buscar/buscas.h`

```c
int buscaLinear (const int *vet, int tam, int procura); // índice ou -1
int buscaBinaria(const int *vet, int tam, int procura); // índice ou -1, exige vetor crescente
int estaOrdenado(const int *vet, int tam);              // 1 se estiver crescente, 0 caso contrário
```

### `arquivos/arquivos.h`

```c
void lerArquivo (int **vet, const char *nome, int *tam);
int  salvarVetor(const int *vet, int tam, const char *nome); // 1 = ok, 0 = erro ou arquivo já existe
```
`lerArquivo` só substitui `*vet` e `*tam` se a leitura der certo. Nesse caso, libera o vetor anterior.

### `relatorios/relatorio.h`

```c
int gerarRelatorio(const int *vetOriginal, int tam, const char *nomeArquivo); // 1 = ok, 0 = erro
```

---
