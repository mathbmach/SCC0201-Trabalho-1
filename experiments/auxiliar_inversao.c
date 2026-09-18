/*
 * Código auxiliar de experimento — inversão do vetor
 *
 * Mede, para vetores de 1.000, 50.000, 100.000, 500.000 e 1.000.000 de
 * elementos, o tempo médio de execução de inverte() e o número de
 * comparações e atribuições de uma execução.
 *
 * Duas decisões de método sustentam o experimento:
 *
 * 1. Tempo e contagem vêm de funções diferentes. O tempo vem da função
 *    original de src/algoritmos.c; a contagem vem da versão
 *    instrumentada definida aqui. Se os contadores estivessem dentro da
 *    função cronometrada, o tempo mediria também o custo de
 *    incrementá-los.
 *
 * 2. O cronômetro mede um bloco de repetições, não uma execução isolada,
 *    porque uma inversão sozinha é curta demais para a resolução prática
 *    de clock().
 *
 * A inversão não tem pior caso distinto: para um mesmo n, ela executa a
 * mesma quantidade de operações qualquer que seja o conteúdo do vetor.
 *
 * Arquivo gerado:
 *
 * data/resultados_inversao.csv
 *
 * Compilação, a partir da raiz do projeto (a pasta data/ precisa existir):
 *
 * gcc -Wall -Wextra -O0 experiments/auxiliar_inversao.c src/algoritmos.c -o auxiliar_inversao
 *
 * Execução no PowerShell:
 *
 * .\auxiliar_inversao.exe
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../src/algoritmos.h"

#define QTD_TAMANHOS 5

/* Execuções feitas antes do cronômetro, fora da medição */
#define AQUECIMENTO 10


/*
 * ------------------------------------------------------------
 * FUNÇÃO INSTRUMENTADA
 * ------------------------------------------------------------
 *
 * Mesma lógica de inverte(), com contadores. Não deve ser usada para
 * medir tempo.
 *
 * Convenção de contagem, a mesma dos quatro algoritmos do trabalho
 * (definida em analysis/README.md):
 *
 *   comparações: i < n / 2, incluindo a última avaliação, falsa, que
 *                encerra o laço
 *   atribuições: i = 0, j = n - 1, as três da troca (temp, v[i], v[j])
 *                e as duas atualizações (i++, j--)
 *
 * Não são contados: declarações sem inicialização, aritmética, acessos
 * ao vetor, passagem de parâmetros, return e os próprios contadores.
 *
 * O laço original é um for. Aqui virou while (1) com break apenas para
 * que cada avaliação da condição fique visível e possa ser contada; o
 * while (1) e o break não são contabilizados.
 */
void inverte_conta(
    int *v,
    int n,
    long long *comparacoes,
    long long *atribuicoes
)
{
    int i = 0;
    int j = n - 1;
    int temp;

    *atribuicoes += 2;

    while (1)
    {
        (*comparacoes)++;

        if (!(i < n / 2))
        {
            break;
        }

        temp = v[i];
        v[i] = v[j];
        v[j] = temp;

        *atribuicoes += 3;

        i++;
        j--;

        *atribuicoes += 2;
    }
}


/*
 * ------------------------------------------------------------
 * PREENCHIMENTO DO VETOR
 * ------------------------------------------------------------
 *
 * Preenche com 0, 1, 2, ..., n - 1.
 *
 * A inversão não exige vetor ordenado, mas usamos a mesma entrada dos
 * outros três experimentos para que a comparação entre os quatro
 * algoritmos seja feita sobre os mesmos dados.
 */
void preencher_vetor(int *v, int n)
{
    for (int i = 0; i < n; i++)
    {
        v[i] = i;
    }
}


/*
 * ------------------------------------------------------------
 * NÚMERO DE REPETIÇÕES CRONOMETRADAS
 * ------------------------------------------------------------
 *
 * O custo da inversão cresce com n, então entradas maiores precisam de
 * menos repetições para formar um bloco mensurável. O mínimo de 100
 * atende à exigência do enunciado.
 *
 * Resulta em 100.000 / 2.000 / 1.000 / 200 / 100 repetições.
 */
int calcular_repeticoes(int n)
{
    int repeticoes = 100000000 / n;

    if (repeticoes < 100)
    {
        repeticoes = 100;
    }

    return repeticoes;
}


/*
 * ------------------------------------------------------------
 * MAIN
 * ------------------------------------------------------------
 */
int main(void)
{
    int tamanhos[QTD_TAMANHOS] = {
        1000,
        50000,
        100000,
        500000,
        1000000
    };

    FILE *arquivo = fopen("data/resultados_inversao.csv", "w");

    if (arquivo == NULL)
    {
        printf("Erro ao criar data/resultados_inversao.csv\n");
        return 1;
    }

    fprintf(
        arquivo,
        "algoritmo,"
        "n,"
        "repeticoes,"
        "tempo_total_s,"
        "tempo_medio_s,"
        "comparacoes,"
        "atribuicoes\n"
    );

    for (int k = 0; k < QTD_TAMANHOS; k++)
    {
        int n = tamanhos[k];

        printf("\n----------------------------------------\n");
        printf("Testando n = %d\n", n);

        /* 1. Alocação do vetor */
        int *v = malloc((size_t)n * sizeof(int));

        if (v == NULL)
        {
            printf("Erro de alocacao para n = %d\n", n);
            fclose(arquivo);
            return 1;
        }

        /* 2. Preparação da entrada, fora da região cronometrada */
        preencher_vetor(v, n);

        /* 3. Número de repetições */
        int repeticoes = calcular_repeticoes(n);

        /*
         * 4. Aquecimento
         *
         * Reduz o peso do primeiro acesso ao código e aos dados, que é
         * mais lento que os seguintes. Como a inversão sempre faz a
         * mesma quantidade de operações, inverter o mesmo vetor várias
         * vezes não altera o custo.
         */
        for (int r = 0; r < AQUECIMENTO; r++)
        {
            inverte(v, n);
        }

        /* 5. Medição: só chamadas à função original dentro do cronômetro */
        clock_t inicio = clock();

        for (int r = 0; r < repeticoes; r++)
        {
            inverte(v, n);
        }

        clock_t fim = clock();

        double tempo_total =
            (double)(fim - inicio) / CLOCKS_PER_SEC;

        double tempo_medio = tempo_total / repeticoes;

        /*
         * 6. Contagem das operações, fora do cronômetro
         *
         * Uma execução basta: para um mesmo n, a quantidade de operações
         * é sempre a mesma.
         */
        long long comparacoes = 0;
        long long atribuicoes = 0;

        inverte_conta(v, n, &comparacoes, &atribuicoes);

        /*
         * 7. Conferência com a teoria
         *
         * Todos os n do experimento são pares, então o laço executa
         * n / 2 vezes:
         *
         *   C(n) = n/2 + 1       (uma por repetição, mais a falsa final)
         *   A(n) = 2 + 5(n/2)    (duas inicializações, cinco por repetição)
         */
        long long comparacoes_teoricas = (long long)n / 2 + 1;
        long long atribuicoes_teoricas = 2 + 5LL * (n / 2);

        if (comparacoes != comparacoes_teoricas)
        {
            printf("ATENCAO: comparacoes diferentes do valor teorico!\n");
        }

        if (atribuicoes != atribuicoes_teoricas)
        {
            printf("ATENCAO: atribuicoes diferentes do valor teorico!\n");
        }

        /* 8. Exibição dos resultados */
        printf("Repeticoes      : %d\n", repeticoes);
        printf("Tempo total     : %.9f s\n", tempo_total);
        printf("Tempo medio     : %.12f s\n", tempo_medio);
        printf("Comparacoes     : %lld\n", comparacoes);
        printf("Atribuicoes     : %lld\n", atribuicoes);
        printf("C(n) teorico    : %lld\n", comparacoes_teoricas);
        printf("A(n) teorico    : %lld\n", atribuicoes_teoricas);

        /* 9. Gravação no CSV */
        fprintf(
            arquivo,
            "inversao,"
            "%d,"
            "%d,"
            "%.12e,"
            "%.12e,"
            "%lld,"
            "%lld\n",
            n,
            repeticoes,
            tempo_total,
            tempo_medio,
            comparacoes,
            atribuicoes
        );

        /* 10. Liberação da memória */
        free(v);
    }

    fclose(arquivo);

    printf("\n========================================\n");
    printf("Experimento concluido.\n");
    printf("Resultados salvos em:\n");
    printf("data/resultados_inversao.csv\n");

    return 0;
}
