#include "arquivos.h"

void lerArquivo(int **vet, const char *nome, int *tam) {
    FILE *arq = fopen(nome, "r");

    if(arq == NULL) {
        printf("Problema ao abrir o arquivo.\n");
        return;
    }

    int capacidade = 16;
    int quantidade = 0;
    int *novoVetor = malloc((size_t)capacidade * sizeof(int));

    if (novoVetor == NULL) {
        printf("Problema ao reservar memoria.\n");
        fclose(arq);
        return;
    }

    int valor;
    while (fscanf(arq, "%d", &valor) == 1) {
        if (quantidade == capacidade) {
            capacidade *= 2;
            int *vetorRedimensionado = realloc(
                novoVetor, (size_t)capacidade * sizeof(int));

            if (vetorRedimensionado == NULL) {
                printf("Problema ao reservar memoria.\n");
                free(novoVetor);
                fclose(arq);
                return;
            }
            novoVetor = vetorRedimensionado;
        }
        novoVetor[quantidade++] = valor;
    }

    fclose(arq);

    if (quantidade == 0) {
        printf("O arquivo nao contem numeros validos.\n");
        free(novoVetor);
        return;
    }

    free(*vet);
    *vet = novoVetor;
    *tam = quantidade;
}

int salvarVetor(const int *vet, int tam, const char *nome) {
    if (vet == NULL || tam <= 0 || nome == NULL || nome[0] == '\0')
        return 0;

    /* O modo "x" garante que um arquivo existente nao seja sobrescrito. */
    FILE *arq = fopen(nome, "wx");
    if (arq == NULL)
        return 0;

    for (int i = 0; i < tam; i++) {
        if (fprintf(arq, "%d\n", vet[i]) < 0) {
            fclose(arq);
            return 0;
        }
    }

    if (fclose(arq) != 0)
        return 0;

    return 1;
}
