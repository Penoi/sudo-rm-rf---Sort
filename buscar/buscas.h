// Otimização que aprendi para conferir se ja foi incluido o .h
#ifndef BUSCAS_H
#define BUSCAS_H


/* ============================================================
 *                 ALGORITMOS DE BUSCA
 * ============================================================ */

/*
 * Busca Linear
 *
 * Percorre o vetor sequencialmente até encontrar o elemento
 * procurado.
 *
 * Retorno:
 *  Índice (base 0) do elemento, caso seja encontrado.
 *  -1, caso o elemento não exista no vetor.
 * 
 *  Exemplo:
 *  int pos = buscaLinear(vet, tam, 10);
 */

int buscaLinear(const int *vet, int tam, int procura);

/*
 * Verifica se o vetor esta ordenado em ordem crescente.
 *
 * Retorno:
 *  1 - vetor ordenado (ou com menos de dois elementos).
 *  0 - vetor desordenado ou parametros invalidos.
 */
int estaOrdenado(const int *vet, int tam);

/*
 * Busca Binária
 *
 * Localiza um elemento em um vetor ordenado utilizando
 * o algoritmo de Busca Binária.
 *
 * O vetor deve estar previamente ordenado em ordem crescente.
 *
 * Retorno:
 *  Índice (base 0) do elemento, caso seja encontrado.
 *  -1, caso o elemento não exista no vetor.
 *
 * Exemplo:
 *  int pos = buscaBinaria(vet, 10);
 */
int buscaBinaria(const int *vet, int tam, int procura);

#endif
