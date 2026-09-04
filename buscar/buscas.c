#include "buscas.h"
#include <stddef.h>

int buscaLinear(const int *vet, int tam, int procura){
    for(int i = 0; i < tam; i++) 
        if(vet[i] == procura)
            return i;
    return -1;
}

int estaOrdenado(const int *vet, int tam){
    if (vet == NULL || tam < 0)
        return 0;

    for (int i = 1; i < tam; i++)
        if (vet[i - 1] > vet[i])
            return 0;

    return 1;
}

int buscaBinaria(const int *vet, int tam, int procura){
    int l = 0;
    int r = tam - 1;

    while (l <= r){
        int mid = l + (r - l) / 2;

        if (vet[mid] < procura)
            l = mid + 1;
        else if (vet[mid] > procura)
            r = mid - 1;
        else
            return mid;
    }

    return -1;
}
