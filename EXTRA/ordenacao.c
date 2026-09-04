#include "ordenacao.h"
#include <stdlib.h>
#include <omp.h>

// limites e tamanhos de bloco
static const int limiteInsercao = 16;
static const int blocoSort = 16384;
static const int blocoMerge = 8192;
static const int limiteParalelo = 131072;

// macro pra n precisa de usa função
#define swap(a, b) do { int t = (a); (a) = (b); (b) = t; } while(0)

static inline void insertionSort(int *vet, int l, int r) {
    for (int i = l + 1; i < r; i++) {
        int val = vet[i];
        int j = i;
        while (j > l && vet[j - 1] > val) {
            vet[j] = vet[j - 1];
            j--;
        }
        vet[j] = val;
    }
}

static inline void mergeSeq(int *vet, int l, int meio, int r, int *tmp) {
    int i = l, j = meio, k = l;
    
    while (i < meio && j < r) {
        if (vet[i] <= vet[j]) {
            tmp[k++] = vet[i++];
        } else {
            tmp[k++] = vet[j++];
        }
    }
    
    while (i < meio) {
        tmp[k++] = vet[i++];
    }
    while (j < r) {
        tmp[k++] = vet[j++];
    }
    
    for (i = l; i < r; i++) {
        vet[i] = tmp[i];
    }
}

static void mergeSortSeq(int *vet, int l, int r, int *tmp) {
    if (r - l <= limiteInsercao) {
        insertionSort(vet, l, r);
        return;
    }
    
    int meio = l + (r - l) / 2;
    mergeSortSeq(vet, l, meio, tmp);
    mergeSortSeq(vet, meio, r, tmp);
    mergeSeq(vet, l, meio, r, tmp);
}

static inline int mergePath(int diag, const int *a, int tamA, const int *b, int tamB) {
    int l = (diag > tamB) ? diag - tamB : 0;
    int r = (diag < tamA) ? diag : tamA;

    while (l < r) {
        int i = l + (r - l) / 2;
        int j = diag - i;
        
        if (j > 0 && i < tamA && b[j - 1] > a[i]) {
            l = i + 1;
        } else {
            r = i;
        }
    }
    return l;
}

static void mergeDiag(const int *restrict orig, int *restrict dest, int base, int meio, int fim, int diag, int diagFim) {
    const int *restrict a = orig + base;
    const int *restrict b = orig + meio;
    
    int tamA = meio - base;
    int tamB = fim - meio;
    
    int i = mergePath(diag, a, tamA, b, tamB);
    int j = diag - i;
    
    int rA = mergePath(diagFim, a, tamA, b, tamB);
    int rB = diagFim - rA;
    
    int *restrict out = dest + base + diag;
    
    while (i < rA && j < rB) {
        int valA = a[i];
        int valB = b[j];
        
        int bMenor = (valB < valA);
        
        *out++ = bMenor ? valB : valA;
        
        i += (1 - bMenor);
        j += bMenor;
    }
    
    // copia oq sobra
    while (i < rA){
        *out++ = a[i++];
    }
    while (j < rB){ 
        *out++ = b[j++];
    }
}

static int parallelSort(int *vet, int tam, int threads) {
    int *tmp = malloc(tam * sizeof(int));
    if (!tmp) return 0;

    int *orig = vet;
    int *dest = tmp;
    int qtdBlocos = (tam + blocoSort - 1) / blocoSort;

    #pragma omp parallel num_threads(threads) shared(orig, dest)
    {
        // ordena os pedacos separados
        #pragma omp for schedule(static)
        for (int b = 0; b < qtdBlocos; b++) {
            int ini = b * blocoSort;
            int fim = ini + blocoSort;
            if (fim > tam) fim = tam;

            mergeSortSeq(vet, ini, fim, dest);
        }

        #pragma omp barrier

        // faz o merge subindo a arvore
        for (int largura = blocoSort; largura < tam; largura *= 2) {
            int qtdMerge = (tam + blocoMerge - 1) / blocoMerge;

            #pragma omp for schedule(static)
            for (int b = 0; b < qtdMerge; b++) {
                int k = b * blocoMerge;
                int larguraTotal = largura * 2;
                
                if (larguraTotal > tam) larguraTotal = tam;

                int base = (k / larguraTotal) * larguraTotal;
                int meio = base + largura;
                if (meio > tam) meio = tam;

                int fim = base + larguraTotal;
                if (fim > tam) fim = tam;

                int diagIni = k - base;
                int diagFim = diagIni + blocoMerge;
                if (diagFim > fim - base) diagFim = fim - base;

                mergeDiag(orig, dest, base, meio, fim, diagIni, diagFim);
            }

            #pragma omp barrier

            // inverte os ponteiros
            #pragma omp single
            {
                int *aux = orig;
                orig = dest;
                dest = aux;
            }

            #pragma omp barrier

            if (largura > tam / 2) break;
        }

        // garante que o resultado volta pro array principal
        if (orig != vet) {
            #pragma omp for schedule(static)
            for (int i = 0; i < tam; i++) {
                vet[i] = orig[i];
            }
        }
    }

    free(tmp);
    return 1;
}

static void ordenar(int *restrict vet, int tam) {
    if (tam < 2) return;

    if (tam >= limiteParalelo) {
        if (parallelSort(vet, tam, omp_get_max_threads())) {
            return;
        }
    }
    
    int *tmp = malloc(tam * sizeof(int));
    mergeSortSeq(vet, 0, tam, tmp);
    free(tmp);
}

int checar(int *vet, int tam) {
    if (tam < 2) {
        return 0;
    }

    int crescente = 1;
    int decrescente = 1;

    for (int i = 1; i < tam; i++) {
        if (vet[i - 1] > vet[i]) crescente = 0;
        if (vet[i - 1] < vet[i]) decrescente = 0;

        // se quebrou pros dois lados, nao ta ordenado
        if (!crescente && !decrescente) break;
    }

    if (crescente) return 0;

    if (decrescente) {
        // se ta ao contrario, e mais rapido so inverter o array
        for (int esq = 0, dir = tam - 1; esq < dir; esq++, dir--) {
            swap(vet[esq], vet[dir]);
        }
        return 0;
    }

    ordenar(vet, tam);
    return 0;
}