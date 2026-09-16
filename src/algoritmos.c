#include "algoritmos.h"

void inverte_vetor(int *v, int n) {

}

int busca_sequencial(int *v, int n, int valor) {
    return 0;
}

int busca_binaria_iterativa(int *v, int n, int valor) {
    return 0;
}

void busca_binaria_recursiva(
    int *v,
    int inicio,
    int fim,
    int p
) {
    if (inicio > fim) {
        printf("NAO\n");
        return;
    }

    int meio = inicio + (fim-inicio) / 2;

    if (v[meio] == p) {
        printf("SIM\n");
        return;
    }
    
    if (v[meio] < p) 
        busca_binaria_recursiva(v, meio + 1, fim, p);
    
    else
        busca_binaria_recursiva(v, inicio, meio - 1, p);
}