#include <stdio.h>
#include <stdlib.h>
#include "algoritmos.h"

/*
 * Inverte a ordem dos elementos do vetor v, no proprio vetor.
 *
 * Dois indices caminham das pontas para o centro: i comeca em 0 e j
 * comeca em n - 1. A cada repeticao os elementos dessas posicoes sao
 * trocados. Basta percorrer metade do vetor, pois cada troca ja coloca
 * dois elementos nas posicoes corretas. Quando n e impar, o elemento
 * central permanece onde esta.
 */
void inverte(int *v, int n)
{
    int i, j, temp;

    for (i = 0, j = n - 1; i < n / 2; i++, j--)
    {
        temp = v[i];
        v[i] = v[j];
        v[j] = temp;
    }
}

/*
 * Busca sequencial: percorre o vetor da primeira ate a ultima posicao,
 * comparando cada elemento com o valor procurado.
 *
 * Retorna 1 se o valor estiver no vetor e 0 caso contrario. O laco para
 * assim que o valor e encontrado, de modo que o numero de comparacoes
 * depende da posicao do elemento.
 *
 * Nao exige vetor ordenado.
 */
int busca_sequencial(int *v, int n, int valor)
{
    int i;

    for (i = 0; i < n; i++)
    {
        if (v[i] == valor)
        {
            return 1;
        }
    }

    return 0;
}

/*
 * Busca binaria iterativa: exige vetor ordenado em ordem crescente.
 *
 * O intervalo de busca e delimitado por inicio e fim, ambos inclusivos.
 * A cada repeticao o elemento central do intervalo e comparado com o
 * valor procurado: se for igual, a busca termina; se for menor, o valor
 * so pode estar na metade direita; se for maior, apenas na metade
 * esquerda. Assim o intervalo cai aproximadamente pela metade a cada
 * passo, ate que o valor seja encontrado ou o intervalo fique vazio.
 *
 * Retorna 1 se o valor estiver no vetor e 0 caso contrario.
 *
 * O calculo inicio + (fim - inicio) / 2 e equivalente a (inicio + fim) / 2
 * para os valores usados aqui, mas evita o estouro do inteiro que a soma
 * direta poderia causar com indices grandes.
 */
int busca_binaria_iterativa(int *v, int n, int valor)
{
    int inicio = 0;
    int fim = n - 1;
    int meio;

    while (inicio <= fim)
    {
        meio = inicio + (fim - inicio) / 2;

        if (v[meio] == valor)
        {
            return 1;
        }

        if (v[meio] < valor)
        {
            inicio = meio + 1;
        }
        else
        {
            fim = meio - 1;
        }
    }

    return 0;
}

/*
 * Busca binaria recursiva: mesma estrategia da versao iterativa, porem a
 * reducao do intervalo e feita por uma nova chamada da propria funcao.
 *
 * Recebe os limites inclusivos do intervalo. Para buscar no vetor inteiro,
 * a chamada e busca_binaria_recursiva(v, 0, n - 1, p).
 *
 * O caso-base inicio > fim representa o intervalo vazio, ou seja, o valor
 * nao esta no vetor. Retorna 1 se o valor for encontrado e 0 caso
 * contrario. Exige vetor ordenado em ordem crescente.
 */
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
