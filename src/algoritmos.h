#ifndef ALGORITMOS_H
#define ALGORITMOS_H

void inverte(int *v, int n);

int busca_sequencial(int *v, int n, int valor);

int busca_binaria_iterativa(int *v, int n, int valor);

int busca_binaria_recursiva(
    int *v,
    int inicio,
    int fim,
    int valor);

#endif