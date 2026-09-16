/*
 * Código auxiliar: inversão do vetor
 *
 * Mede o tempo médio de execução da função inverte() e conta as
 * comparações e atribuições realizadas no pior caso, para vetores de
 * tamanho 1 000, 50 000, 100 000, 500 000 e 1 000 000. Os resultados são
 * exibidos na tela e gravados em data/resultados_inversao.csv.
 *
 * Compilação e execução (a partir da raiz do repositório):
 *   gcc -Wall experiments/auxiliar_inversao.c src/algoritmos.c -o auxiliar
 *   ./auxiliar
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../src/algoritmos.h"

/* Contadores de operações, incrementados por inverte_conta(). */
long long comparacoes = 0;
long long atribuicoes = 0;

/*
 * Versão instrumentada de inverte() (src/algoritmos.c), com a mesma lógica.
 * Os contadores ficam em uma função separada para que os incrementos não
 * interfiram na medição de tempo, feita sobre a função original.
 *
 * Convenção de contagem adotada:
 *   - comparação: cada avaliação da condição do laço (i < n/2);
 *   - atribuição: inicializações, as três atribuições da troca e as
 *     atualizações i++ e j--.
 */
void inverte_conta(int *v, int n)
{
    int i, j, temp;

    j = n - 1;
    atribuicoes++;
    i = 0;
    atribuicoes++;

    for (; i < n / 2; i++, j--)
    {
        comparacoes++;          /* avaliação verdadeira da condição */

        temp = v[i];
        v[i] = v[j];
        v[j] = temp;
        atribuicoes += 3;       /* troca de v[i] com v[j] */

        atribuicoes += 2;       /* i++ e j-- */
    }
    comparacoes++;              /* avaliação final (falsa) que encerra o laço */
}

int main(void)
{
    int tamanhos[] = {1000, 50000, 100000, 500000, 1000000};
    int qtd = 5;

    FILE *arq = fopen("data/resultados_inversao.csv", "w");
    if (arq == NULL)
    {
        printf("Erro ao criar data/resultados_inversao.csv\n");
        return 1;
    }
    fprintf(arq, "algoritmo,n,repeticoes,tempo_medio_s,comparacoes,atribuicoes\n");

    for (int k = 0; k < qtd; k++)
    {
        int n = tamanhos[k];

        /*
         * Vetor de entrada ordenado (0, 1, ..., n-1), conforme a restrição
         * do enunciado. Como a inversão não possui condição de parada
         * antecipada, o número de operações depende apenas de n: qualquer
         * entrada de tamanho n corresponde ao pior caso (que coincide com o
         * melhor e o médio).
         */
        int *v = malloc(n * sizeof(int));
        if (v == NULL)
        {
            printf("Erro de alocacao para n = %d\n", n);
            fclose(arq);
            return 1;
        }
        for (int i = 0; i < n; i++)
            v[i] = i;

        /*
         * Número de repetições: no mínimo 100, conforme o enunciado.
         * Para vetores pequenos, 100 execuções duram menos que a resolução
         * de clock() (cerca de 1 ms em algumas plataformas), o que tornaria
         * a medição imprecisa. Por isso o número de repetições é escolhido
         * de forma inversamente proporcional a n, mantendo o tempo total
         * medido aproximadamente constante entre os tamanhos. O valor
         * registrado é a média por execução.
         */
        int repeticoes = 100000000 / n;
        if (repeticoes < 100)
            repeticoes = 100;

        /* Medição de tempo sobre a função original (sem contadores). */
        clock_t inicio = clock();
        for (int r = 0; r < repeticoes; r++)
            inverte(v, n);
        clock_t fim = clock();

        double tempo_medio = (double)(fim - inicio) / CLOCKS_PER_SEC / repeticoes;

        /*
         * Contagem de operações: uma única execução é suficiente, pois o
         * número de operações é determinístico para cada n.
         */
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
    printf("\nResultados gravados em data/resultados_inversao.csv\n");
    return 0;
}
