#ifndef ORDENACAO_H
#define ORDENACAO_H
#include <stdlib.h>
#include <omp.h>
#include <stdio.h>


// VOCE VAI CHAMAR ESSA FUNÇÃO AQUI TINOCO
/*
 * Verifica o estado inicial antes de chamar ordenar.
 * Vetores crescentes não são alterados e vetores decrescentes são invertidos.
 * Nos demais casos, chama ordenar normalmente.
 * Retorna 0 em caso de sucesso e -1 para argumentos inválidos.
 */
int checar(int *vet, int tam);

#endif
