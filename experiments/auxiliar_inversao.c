/*
 * Programa auxiliar da INVERSAO (nao vai para o Run.Codes).
 * Mede o tempo medio da inversao e conta comparacoes e atribuicoes
 * para n = 1000, 50000, 100000, 500000 e 1000000.
 * Resultado: tela + arquivo resultados_inversao.csv
 *
 * Compilar (dentro da pasta src):
 *   gcc -Wall auxiliar_inversao.c algoritmos.c -o aux
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../src/algoritmos.h"

/* Contadores globais: a funcao abaixo soma neles */
long long comparacoes = 0;
long long atribuicoes = 0;

/*
 * Mesma logica da inverte() do algoritmos.c, com contadores.
 * Fica separada para os contadores nao atrapalharem a medicao de tempo.
 */
void inverte_conta(int *v, int n)
{
    int i, j, temp;

    j = n - 1;
    atribuicoes++;              /* j = n - 1 */
    i = 0;
    atribuicoes++;              /* i = 0 */

    for (; i < n / 2; i++, j--)
    {
        comparacoes++;          /* teste i < n/2 que deu VERDADEIRO */

        temp = v[i];
        v[i] = v[j];
        v[j] = temp;
        atribuicoes += 3;       /* as 3 atribuicoes da troca */

        atribuicoes += 2;       /* i++ e j-- */
    }
    comparacoes++;              /* ultimo teste i < n/2, que deu FALSO */
}

int main(void)
{
    int tamanhos[] = {1000, 50000, 100000, 500000, 1000000};
    int qtd = 5;

    FILE *arq = fopen("data/resultados_inversao.csv", "w");
    if (arq == NULL)
    {
        printf("Erro ao criar o arquivo\n");
        return 1;
    }
    fprintf(arq, "algoritmo,n,repeticoes,tempo_medio_s,comparacoes,atribuicoes\n");

    for (int k = 0; k < qtd; k++)
    {
        int n = tamanhos[k];

        /* Vetor ordenado 0, 1, ..., n-1 (para a inversao, qualquer vetor e pior caso) */
        int *v = malloc(n * sizeof(int));
        for (int i = 0; i < n; i++)
            v[i] = i;

        /*
         * Vetores pequenos invertem rapido demais para o clock() medir.
         * Por isso repetimos mais vezes quando n e pequeno (minimo de 100,
         * como pede o enunciado). Total de trabalho parecido para todo n.
         */
        int repeticoes = 100000000 / n;
        if (repeticoes < 100)
            repeticoes = 100;

        /* ---- Medicao de tempo (usa a inverte() original, sem contadores) ---- */
        clock_t inicio = clock();
        for (int r = 0; r < repeticoes; r++)
            inverte(v, n);
        clock_t fim = clock();

        double tempo_medio = (double)(fim - inicio) / CLOCKS_PER_SEC / repeticoes;

        /* ---- Contagem de operacoes (uma execucao basta: o numero e sempre o mesmo) ---- */
        comparacoes = 0;
        atribuicoes = 0;
        inverte_conta(v, n);

        fprintf(arq, "inversao,%d,%d,%e,%lld,%lld\n",
                n, repeticoes, tempo_medio, comparacoes, atribuicoes);
        printf("n = %7d | tempo medio = %e s | comparacoes = %lld | atribuicoes = %lld\n",
               n, tempo_medio, comparacoes, atribuicoes);

        free(v);
    }

    fclose(arq);
    printf("\nResultados salvos em resultados_inversao.csv\n");
    return 0;
}
