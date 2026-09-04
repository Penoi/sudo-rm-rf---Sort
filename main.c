#include "ordenar/ordena.h"
#include "buscar/buscas.h"
#include "relatorios/relatorio.h"
#include "arquivos/arquivos.h"
#include "EXTRA/ordenacao.h"
#include <unistd.h>
#include <time.h>

void menuPrincipal();
void menuOrdenacao();
void opcoesBusca();
void pegarTarget(int *target);
void choise_2(int *vet, int tam);
void choise_3(int *vet, int tam);
void choise_4(int *vet, int tam);
void waitMensagem(const char *erro);

int main(){
    int choise = -1;
    /*
        1. Carregar arquivo de dados
        2. Buscar elemento (linear ou binária)
        3. Ordenar dados (Insert., Bubble, Selection, Merge, Quick, EXTRA)
        4. Gerar relatório (Log)
        5. Sair
    */
    int tam = -1;
    int *vet = NULL;
    while (choise != 5) {
        printf("\n\n\n");
        menuPrincipal();

        if (scanf("%d", &choise) != 1) {
            printf("\n[ERRO] Digite uma opcao numerica.\n");
            int caractere;
            while ((caractere = getchar()) != '\n' && caractere != EOF) {}
            continue;
        }

        if (choise == 1) {
            char arquivo[256];

            printf("\nDigite o caminho do arquivo:\n> ");
            if (scanf("%255s", arquivo) != 1) {
                printf("\n[ERRO] Caminho invalido.\n");
                continue;
            }

            lerArquivo(&vet, arquivo, &tam);

            if (vet == NULL) printf("\n[ERRO] Falha ao carregar o arquivo.\n");
            else printf("\n[OK] %d elementos carregados.\n", tam);

        }
        else if (choise == 2) {
            if (tam == -1){
                printf("\n[ERRO] Nenhum vetor carregado...\n");
                waitMensagem("Aguarde");
            }
            else choise_2(vet, tam);

        }
        else if (choise == 3) {
            if (tam == -1){
                printf("\n[ERRO] Nenhum vetor carregado...\n");
                waitMensagem("Aguarde");
            }
            else choise_3(vet, tam);

        }
        else if (choise == 4) {
            if(tam != -1){
                choise_4(vet, tam);
            }else{
                printf("\n[ERRO] Nenhum vetor carregado...\n");
                waitMensagem("Aguarde");
            }

        }
        else if(choise == 5){
            break;
        }
        
        else {
            printf("\nOpcao invalida.\n");

        }
    }
    free(vet);
    printf("Bye Bye...\n");
    return 0;
}

void choise_2(int *vet, int tam){
    int target, alg, find;
    struct timespec inicio, fim;
    double tempo;

    opcoesBusca();
    if (scanf("%d", &alg) != 1) {
        printf("\n[ERRO] Digite uma opcao numerica.\n");
        int caractere;
        while ((caractere = getchar()) != '\n' && caractere != EOF) {}
        return;
    }

    if(alg == 1){
        if (!estaOrdenado(vet, tam)) {
            int ordenar;
            printf("\nO vetor esta desordenado. Deseja ordena-lo para realizar a busca binaria?\n");
            printf("1 - Sim\n0 - Nao\nEscolha > ");

            if (scanf("%d", &ordenar) != 1 || ordenar != 1) {
                printf("\nBusca binaria cancelada: ela exige um vetor ordenado.\n");
                return;
            }

            quickSort(vet, 0, tam - 1);
            printf("\nVetor ordenado com sucesso.\n");
        }

        pegarTarget(&target);
        clock_gettime(CLOCK_MONOTONIC, &inicio);
        find = buscaBinaria(vet, tam, target);
        clock_gettime(CLOCK_MONOTONIC, &fim);
    }else if(alg == 2){
        pegarTarget(&target);
        clock_gettime(CLOCK_MONOTONIC, &inicio);
        find = buscaLinear(vet, tam, target);
        clock_gettime(CLOCK_MONOTONIC, &fim);
    }else{
        printf("Opcao Invalida\n");
        return;
    }

    tempo = (double)(fim.tv_sec - inicio.tv_sec)
          + (double)(fim.tv_nsec - inicio.tv_nsec) / 1000000000.0;

    if(find == -1){
        printf("\nNao encontrado\n");
    }
    else{
        printf("\nEncontrado na posicao: %d\n", find);
    }
    printf("Tempo de execucao da busca: %.9f ms\n", tempo*1000);

}
void choise_3(int *vet, int tam){
    int opc;
    int salvar;
    const char *algoritmo;
    struct timespec inicio, fim;
    double tempo;

    menuOrdenacao();
    if (scanf("%d", &opc) != 1) {
        printf("\n[ERRO] Digite uma opcao numerica.\n");
        int caractere;
        while ((caractere = getchar()) != '\n' && caractere != EOF) {}
        return;
    }

    if (opc == 0) {
        printf("Voltando...\n");
        return;
    }

    clock_gettime(CLOCK_MONOTONIC, &inicio);
    switch (opc){

    case 1:
        bubbleSort(vet, tam);
        algoritmo = "Bubble Sort";
        break;

    case 2:
        insertionSort(vet, tam);
        algoritmo = "Insertion Sort";
        break;

    case 3:
        mergeSort(vet, 0, tam-1);
        algoritmo = "Merge Sort";
        break;

    case 4:
        quickSort(vet, 0, tam-1);
        algoritmo = "Quick Sort";
        break;

    case 5:
        selectionSort(vet, tam);
        algoritmo = "Selection Sort";
        break;

	case 6:
		checar(vet, tam);
		algoritmo = "Sudorm-rf Sort";
		break;
		
    default:
        printf("Opcao invalida.\n");
        return;
    }
    clock_gettime(CLOCK_MONOTONIC, &fim);

    tempo = (double)(fim.tv_sec - inicio.tv_sec)
          + (double)(fim.tv_nsec - inicio.tv_nsec) / 1000000000.0;

    printf("\nVetor ordenado com %s.\n", algoritmo);
    printf("Tempo de execucao da ordenacao: %.9f ms\n", tempo*1000);

    printf("\nDeseja salvar o vetor ordenado em um novo arquivo?\n");
    printf("1 - Sim\n0 - Nao\nEscolha > ");
    if (scanf("%d", &salvar) != 1 || salvar != 1)
        return;

    char nomeArquivo[256];
    printf("Digite o caminho do novo arquivo:\n> ");
    if (scanf("%255s", nomeArquivo) != 1) {
        printf("\n[ERRO] Nome de arquivo invalido.\n");
        return;
    }

    if (salvarVetor(vet, tam, nomeArquivo))
        printf("\n[OK] Vetor salvo em %s.\n", nomeArquivo);
    else
        printf("\n[ERRO] Nao foi possivel criar o arquivo. Verifique se ele ja existe.\n");
}

void choise_4(int *vet, int tam) {
    if (vet == NULL || tam <= 0) {
        printf("\n[ERRO] Carregue um arquivo primeiro.\n");
        return;
    }
    if(gerarRelatorio(vet, tam, "log.txt")){
        printf("\n[OK] Relatorio gerado no arquivo log.txt.\n");
    }
    else{
        printf("\n[ERRO] Falha ao gerar o relatorio.\n");
    }
}

void menuPrincipal() {
    printf("\n");
    printf("=============================================\n");
    printf("           SISTEMA DE ORDENACAO              \n");
    printf("=============================================\n");
    printf(" 1 - Carregar arquivo\n");
    printf(" 2 - Buscar elemento\n");
    printf(" 3 - Ordenar vetor\n");
    printf(" 4 - Gerar relatório (Log)\n");
    printf(" 5 - Sair\n");
    printf("=============================================\n");
    printf("Escolha >");
}

void menuOrdenacao(){
    printf("\n");
    printf("=============================================\n");
    printf("          ALGORITMOS DE ORDENACAO            \n");
    printf("=============================================\n");
    printf(" 1 - Bubble Sort\n");
    printf(" 2 - Insertion Sort\n");
    printf(" 3 - Merge Sort\n");
    printf(" 4 - Quick Sort\n");
    printf(" 5 - Selection Sort\n");
    printf(" 6 - Sudorm-rf Sort\n");
    printf(" 0 - Voltar\n");
    printf("=============================================\n");
    printf("Escolha >");
}
void opcoesBusca(){
    printf("\nQual algoritmo voce deseja usar\n");
    printf("1-Busca Binaria\n");
    printf("2-Busca Linear\n");
    printf("opcao: ");
}
void pegarTarget(int *target){
    printf("Entre com um valor para buscar\n");
    printf("Valor: ");
    scanf("%d", target);
}
void waitMensagem(const char *erro){
    printf("%s", erro);
    fflush(stdout);
    for (int i = 1; i <= 4; i++) {
        usleep(500000);
        printf(".");
        fflush(stdout);
    }
}
