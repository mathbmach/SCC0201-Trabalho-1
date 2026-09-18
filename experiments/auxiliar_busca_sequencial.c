/*
 * Código auxiliar de experimento — busca sequencial
 *
 * Mede, para vetores de 1.000, 50.000, 100.000, 500.000 e 1.000.000 de
 * elementos, o tempo médio de execução de busca_sequencial() e o número
 * de comparações e atribuições no pior caso.
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
 *    porque uma busca sozinha é curta demais para a resolução prática de
 *    clock().
 *
 * Arquivo gerado:
 *
 * data/resultados_busca_sequencial.csv
 *
 * Compilação, a partir da raiz do projeto (a pasta data/ precisa existir):
 *
 * gcc -Wall -Wextra -O0 experiments/auxiliar_busca_sequencial.c src/algoritmos.c -o auxiliar_busca_sequencial
 *
 * Execução no PowerShell:
 *
 * .\auxiliar_busca_sequencial.exe
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
 * Mesma lógica de busca_sequencial(), com contadores. Não deve ser usada
 * para medir tempo.
 *
 * Convenção de contagem, a mesma dos quatro algoritmos do trabalho
 * (definida em analysis/README.md):
 *
 *   comparações: i < n, incluindo a última avaliação, falsa, que encerra
 *                o laço; e v[i] == valor
 *   atribuições: i = 0 e cada i++
 *
 * Não são contados: declarações sem inicialização, aritmética, acessos
 * ao vetor, passagem de parâmetros, return e os próprios contadores.
 *
 * O laço original é um for. Aqui virou while (1) com break apenas para
 * que cada avaliação da condição fique visível e possa ser contada; o
 * while (1) e o break não são contabilizados.
 */
int busca_sequencial_conta(
    int *v,
    int n,
    int valor,
    long long *comparacoes,
    long long *atribuicoes
)
{
    int i = 0;

    (*atribuicoes)++;

    while (1)
    {
        (*comparacoes)++;

        if (!(i < n))
        {
            break;
        }

        (*comparacoes)++;

        if (v[i] == valor)
        {
            return 1;
        }

        i++;

        (*atribuicoes)++;
    }

    return 0;
}


/*
 * ------------------------------------------------------------
 * PREENCHIMENTO DO VETOR
 * ------------------------------------------------------------
 *
 * Preenche com 0, 1, 2, ..., n - 1.
 *
 * A busca sequencial não exige vetor ordenado, mas usamos a mesma
 * entrada dos outros três experimentos para que a comparação entre os
 * quatro algoritmos seja feita sobre os mesmos dados.
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
 * O custo da busca sequencial cresce com n, então entradas maiores
 * precisam de menos repetições para formar um bloco mensurável. O mínimo
 * de 100 atende à exigência do enunciado. Mesma regra do auxiliar da
 * inversão, que também é linear.
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

    FILE *arquivo = fopen("data/resultados_busca_sequencial.csv", "w");

    if (arquivo == NULL)
    {
        printf("Erro ao criar data/resultados_busca_sequencial.csv\n");
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

        /*
         * 3. Definição do pior caso
         *
         * O vetor contém 0, 1, ..., n - 1, então p = n não existe nele, e
         * a busca é obrigada a examinar todas as n posições antes de
         * concluir que o valor está ausente. Esse é o maior número de
         * comparações possível para esse tamanho.
         */
        int p = n;

        /* 4. Número de repetições */
        int repeticoes = calcular_repeticoes(n);

        /*
         * 5. Aquecimento
         *
         * Reduz o peso do primeiro acesso ao código e aos dados, que é
         * mais lento que os seguintes.
         *
         * O retorno vai para uma variável volatile para reduzir a chance
         * de o compilador descartar a chamada por considerar o resultado
         * inútil, o que zeraria o tempo medido.
         */
        volatile int resultado = 0;

        for (int r = 0; r < AQUECIMENTO; r++)
        {
            resultado = busca_sequencial(v, n, p);
        }

        /* 6. Medição: só chamadas à função original dentro do cronômetro */
        clock_t inicio = clock();

        for (int r = 0; r < repeticoes; r++)
        {
            resultado = busca_sequencial(v, n, p);
        }

        clock_t fim = clock();

        double tempo_total =
            (double)(fim - inicio) / CLOCKS_PER_SEC;

        double tempo_medio = tempo_total / repeticoes;

        /*
         * 7. Contagem das operações, fora do cronômetro
         *
         * Uma execução basta: para um mesmo n e um mesmo valor procurado,
         * a quantidade de operações é sempre a mesma.
         */
        long long comparacoes = 0;
        long long atribuicoes = 0;

        int resultado_contagem =
            busca_sequencial_conta(
                v,
                n,
                p,
                &comparacoes,
                &atribuicoes
            );

        /*
         * 8. Conferência com a teoria
         *
         * No pior caso, com o valor ausente, i < n é avaliada n + 1 vezes
         * (uma por posição, mais a falsa que encerra o laço) e
         * v[i] == valor, n vezes:
         *
         *   C(n) = (n + 1) + n = 2n + 1
         *   A(n) = 1 + n                 (i = 0 e os n incrementos)
         */
        long long comparacoes_teoricas = 2LL * n + 1;
        long long atribuicoes_teoricas = (long long)n + 1;

        /* p = n não pertence ao vetor, então o retorno correto é zero */
        if (resultado != 0)
        {
            printf("ATENCAO: resultado incorreto na funcao original!\n");
        }

        if (resultado_contagem != 0)
        {
            printf("ATENCAO: resultado incorreto na funcao instrumentada!\n");
        }

        if (comparacoes != comparacoes_teoricas)
        {
            printf("ATENCAO: comparacoes diferentes do valor teorico!\n");
        }

        if (atribuicoes != atribuicoes_teoricas)
        {
            printf("ATENCAO: atribuicoes diferentes do valor teorico!\n");
        }

        /* 9. Exibição dos resultados */
        printf("Valor procurado : %d (inexistente)\n", p);
        printf("Repeticoes      : %d\n", repeticoes);
        printf("Tempo total     : %.9f s\n", tempo_total);
        printf("Tempo medio     : %.12f s\n", tempo_medio);
        printf("Comparacoes     : %lld\n", comparacoes);
        printf("Atribuicoes     : %lld\n", atribuicoes);
        printf("C(n) teorico    : %lld\n", comparacoes_teoricas);
        printf("A(n) teorico    : %lld\n", atribuicoes_teoricas);

        /* 10. Gravação no CSV */
        fprintf(
            arquivo,
            "busca_sequencial,"
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

        /* 11. Liberação da memória */
        free(v);
    }

    fclose(arquivo);

    printf("\n========================================\n");
    printf("Experimento concluido.\n");
    printf("Resultados salvos em:\n");
    printf("data/resultados_busca_sequencial.csv\n");

    return 0;
}
