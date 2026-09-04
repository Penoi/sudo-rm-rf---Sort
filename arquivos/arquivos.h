#include <stdio.h>
#include <stdlib.h>

/*
* Lê um arquivo e coloca os numeros dentro de um vetor
*/
void lerArquivo(int **vet, const char *nome, int *tam);

/*
 * Salva os elementos do vetor em um novo arquivo, um por linha.
 * Nao sobrescreve um arquivo que ja exista.
 * Retorna 1 em caso de sucesso e 0 em caso de erro.
 */
int salvarVetor(const int *vet, int tam, const char *nome);
