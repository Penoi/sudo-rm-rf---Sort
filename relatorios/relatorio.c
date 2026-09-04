#include "relatorio.h"
#include "../ordenar/ordena.h"
#include "../buscar/buscas.h"
#include "../EXTRA/ordenacao.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define REPETICOES_MAXIMAS 1

static int calcularRepeticoes(int tam){
    if (tam <= 1000) return REPETICOES_MAXIMAS;
    if (tam <= 5000) return 10;
    return 1;
}

static double tempoAtualMs(void){
    return ((double)clock() / CLOCKS_PER_SEC) * 1000.0;
}

static int *copiarVetor(const int *vetOriginal,int tam){
    int *copia = malloc(tam * sizeof(int));
    if (copia == NULL) return NULL;
    memcpy(copia, vetOriginal, tam * sizeof(int));
    return copia;
}

static void executarSelection(int *vet,int tam){
    selectionSort(vet, tam);
} 
static void executarQuick(int *vet,int tam){
    quickSort(vet, 0, tam - 1);
}
static void executarMerge(int *vet, int tam){
    mergeSort(vet, 0, tam - 1);
}
static void executarInsertion(int *vet, int tam){
    insertionSort(vet, tam);
}
static void executarBubble(int *vet, int tam){
    bubbleSort(vet, tam);
}
static void executarSudormrf(int *vet, int tam){
    checar(vet, tam);
}


static double medirOrdenacao(void (*algoritmo)(int *, int), const int *vetOriginal, int tam, int repeticoes){
    double tempoTotal = 0.0;
    for(int i = 0; i < repeticoes; i++){
        int *copia = copiarVetor(vetOriginal, tam);
        if(copia == NULL){
            return -1.0;
        }
        double inicio = tempoAtualMs();
        algoritmo(copia, tam);
        double fim = tempoAtualMs();
        tempoTotal += fim - inicio;
        free(copia);
    }
    return tempoTotal / repeticoes;
}

double medirBuscaLinear(const int *vet, int tam, int valorProcurado, int repeticoes){
    int resultado = -1;
    double inicio = tempoAtualMs();
    for (int i = 0; i < repeticoes; i++){
        resultado = buscaLinear((int *)vet, tam, valorProcurado);
    }
    double fim = tempoAtualMs();
    resultado;
    return (fim - inicio) / repeticoes;
}

static double medirBuscaBinaria(const int *vetOriginal, int tam, int valorProcurado, int repeticoes){
    double tempoTotal = 0.0;
    volatile int resultado = -1;
    for(int i = 0; i < repeticoes; i++){
        int *copia = copiarVetor(vetOriginal, tam);
        if(copia == NULL){
            return -1.0;
        }

        double inicio = tempoAtualMs();
        resultado = buscaBinaria(copia, tam, valorProcurado);
        double fim = tempoAtualMs();
        
        tempoTotal += fim - inicio;
        free(copia);
    }
    return tempoTotal / repeticoes;
}

void registrarTempo(FILE *arquivo, const char *nomeAlgoritmo, double media){
    if(media < 0.0){
        fprintf(arquivo, "%-24s ERRO AO EXECUTAR\n", nomeAlgoritmo);
        return;
    }
    fprintf(arquivo, "%-24s %.9f ms\n", nomeAlgoritmo, media);
}

int gerarRelatorio(const int *vetOriginal, int tam, const char *nomeArquivo){
    if(vetOriginal == NULL || tam <= 0 || nomeArquivo == NULL){
        return 0;
    }
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL){
        return 0;
    }
    int valorProcurado = vetOriginal[tam - 1];
    int repeticoes = calcularRepeticoes(tam);
    fprintf(arquivo,
        "RELATORIO DE DESEMPENHO\n"
        "=======================\n"
        "Quantidade de elementos: %d\n"
        "Repeticoes por algoritmo: %d\n"
        "Valor utilizado nas buscas: %d\n\n",
        tam,
        repeticoes,
        valorProcurado);

    fprintf(arquivo, "ALGORITMOS DE ORDENACAO\n" "-----------------------\n");
    registrarTempo(arquivo, "Selection Sort:", medirOrdenacao(executarSelection, vetOriginal, tam, repeticoes));
    registrarTempo(arquivo, "Quick Sort:", medirOrdenacao(executarQuick, vetOriginal, tam, repeticoes));
    registrarTempo(arquivo, "Merge Sort:", medirOrdenacao(executarMerge, vetOriginal, tam, repeticoes));
    registrarTempo(arquivo, "Insertion Sort:", medirOrdenacao(executarInsertion, vetOriginal, tam, repeticoes));
    registrarTempo(arquivo, "Bubble Sort:", medirOrdenacao(executarBubble, vetOriginal, tam, repeticoes));
    registrarTempo(arquivo, "SudoRm-rf Sort:", medirOrdenacao(executarSudormrf, vetOriginal, tam, repeticoes));
    fprintf(arquivo,
        "\nALGORITMOS DE BUSCA\n"
        "--------------------\n");
    registrarTempo(arquivo,
        "Busca Linear:", medirBuscaLinear(vetOriginal, tam, valorProcurado, repeticoes));
    registrarTempo(arquivo, "Busca Binaria + Quick Sort:", medirBuscaBinaria(vetOriginal, tam, valorProcurado, repeticoes));
    fclose(arquivo);
    return 1;
}
