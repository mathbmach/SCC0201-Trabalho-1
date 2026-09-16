#include <stdio.h>
#include <stdlib.h>
#include "algoritmos.h"

void inverte(int *v, int n)
{
    int i, j = n - 1, temp;

    for (i = 0; i < n / 2; i++, j--)
    {
        temp = v[i];
        v[i] = v[j];
        v[j] = temp;
    }
}

int busca_sequencial(int *v, int n, int valor)
{
    return 0;
}

int busca_binaria_iterativa(int *v, int n, int valor)
{
    return 0;
}

int busca_binaria_recursiva(
    int *v,
    int inicio,
    int fim,
    int p)
{
    if (inicio > fim)
    {
        return 0;
    }

    int meio = inicio + (fim - inicio) / 2;

    if (v[meio] == p)
    {
        return 1;
    }

    if (v[meio] < p)
        return busca_binaria_recursiva(v, meio + 1, fim, p);

    else
        return busca_binaria_recursiva(v, inicio, meio - 1, p);
}