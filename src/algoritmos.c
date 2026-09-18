/*
 * Implementacao dos quatro algoritmos do trabalho.
 *
 * Estas funcoes nao contem contadores: elas sao usadas pelo programa
 * principal e tambem na medicao de tempo dos codigos auxiliares. As
 * versoes instrumentadas, com contadores de comparacoes e atribuicoes,
 * ficam dentro de cada auxiliar em experiments/.
 */

#include <stdio.h>
#include <stdlib.h>
#include "algoritmos.h"

/*
 * Inverte a ordem dos elementos do vetor, no proprio vetor.
 *
 * Dois indices caminham das pontas para o centro e trocam os elementos
 * de posicao. Basta percorrer metade do vetor, porque cada troca ja
 * posiciona dois elementos. Com n impar, o elemento central permanece
 * onde esta.
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
 * Busca sequencial. Retorna 1 se o valor esta no vetor e 0 caso contrario.
 *
 * Unica das tres buscas que nao exige vetor ordenado. Em troca, precisa
 * examinar uma posicao por vez, e o numero de comparacoes depende de onde
 * o valor esta.
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
 * Busca binaria iterativa. Exige vetor ordenado em ordem crescente.
 * Retorna 1 se o valor esta no vetor e 0 caso contrario.
 *
 * O intervalo de busca e delimitado por inicio e fim, ambos inclusivos,
 * e cai pela metade a cada repeticao: comparando o valor procurado com o
 * elemento central, uma das duas metades pode ser descartada inteira.
 * O laco termina quando o valor e encontrado ou quando o intervalo fica
 * vazio, o que acontece quando inicio passa de fim.
 *
 * O calculo inicio + (fim - inicio) / 2 equivale a (inicio + fim) / 2,
 * mas evita o estouro do int que a soma direta poderia causar com
 * indices grandes.
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
 * Busca binaria recursiva. Mesma estrategia da versao iterativa, mas a
 * reducao do intervalo e feita por uma nova chamada em vez de um laco.
 *
 * Recebe os limites inclusivos do intervalo, entao a busca no vetor
 * inteiro e busca_binaria_recursiva(v, 0, n - 1, p). O caso-base
 * inicio > fim representa o intervalo vazio, ou seja, valor ausente.
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
