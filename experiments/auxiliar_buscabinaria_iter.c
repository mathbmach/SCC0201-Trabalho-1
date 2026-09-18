/*
 * Código auxiliar de experimento — busca binária iterativa
 *
 * Mede, para vetores de 1.000, 50.000, 100.000, 500.000 e 1.000.000 de
 * elementos, o tempo médio de execução de busca_binaria_iterativa() e o
 * número de comparações e atribuições no pior caso.
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
 *    porque uma busca binária leva dezenas de nanossegundos, muito
 *    abaixo da resolução prática de clock().
 *
 * O número de repetições é o mesmo do auxiliar da busca binária
 * recursiva, de propósito: é isso que permite comparar as duas versões
 * sob o mesmo protocolo.
 *
 * Arquivo gerado:
 *
 * data/resultados_binaria_iterativa.csv
 *
 * Compilação, a partir da raiz do projeto (a pasta data/ precisa existir):
 *
 * gcc -Wall -Wextra -O0 experiments/auxiliar_buscabinaria_iter.c src/algoritmos.c -o auxiliar_buscabinaria_iter
 *
 * Execução no PowerShell:
 *
 * .\auxiliar_buscabinaria_iter.exe
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../src/algoritmos.h"

#define QTD_TAMANHOS 5

/* Repetições cronometradas por tamanho (o enunciado exige no mínimo 100) */
#define REPETICOES 1000000

/* Execuções feitas antes do cronômetro, fora da medição */
#define AQUECIMENTO 100


/*
 * ------------------------------------------------------------
 * FUNÇÃO INSTRUMENTADA
 * ------------------------------------------------------------
 *
 * Mesma lógica de busca_binaria_iterativa(), com contadores. Não deve
 * ser usada para medir tempo.
 *
 * Convenção de contagem, a mesma dos quatro algoritmos do trabalho
 * (definida em analysis/README.md):
 *
 *   comparações: inicio <= fim, incluindo a última avaliação, falsa, que
 *                encerra o laço; v[meio] == valor; e v[meio] < valor
 *   atribuições: inicio = 0, fim = n - 1, cada cálculo de meio e cada
 *                atualização de inicio ou fim (apenas uma das duas por
 *                repetição)
 *
 * Não são contados: declarações sem inicialização, aritmética, acessos
 * ao vetor, passagem de parâmetros, return e os próprios contadores.
 *
 * Essa convenção vale igual na versão recursiva, o que torna as duas
 * diretamente comparáveis.
 *
 * O laço original é um while (inicio <= fim). Aqui virou while (1) com
 * break apenas para que cada avaliação da condição fique visível e possa
 * ser contada; o while (1) e o break não são contabilizados.
 */
int busca_binaria_iterativa_conta(
    int *v,
    int n,
    int valor,
    long long *comparacoes,
    long long *atribuicoes
)
{
    int inicio = 0;
    int fim = n - 1;
    int meio;

    *atribuicoes += 2;

    while (1)
    {
        (*comparacoes)++;

        if (!(inicio <= fim))
        {
            break;
        }

        meio = inicio + (fim - inicio) / 2;

        (*atribuicoes)++;

        (*comparacoes)++;

        if (v[meio] == valor)
        {
            return 1;
        }

        (*comparacoes)++;

        if (v[meio] < valor)
        {
            inicio = meio + 1;
        }
        else
        {
            fim = meio - 1;
        }

        /* Exatamente uma das duas atribuições acima foi executada */
        (*atribuicoes)++;
    }

    return 0;
}


/*
 * ------------------------------------------------------------
 * PREENCHIMENTO DO VETOR
 * ------------------------------------------------------------
 *
 * Preenche com 0, 1, 2, ..., n - 1, que já está na ordem crescente
 * exigida pela busca binária.
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
 * REPETIÇÕES DO LAÇO NO PIOR CASO
 * ------------------------------------------------------------
 *
 * Calcula k = floor(log2(n)) + 1 por divisões inteiras sucessivas, para
 * evitar os erros de arredondamento que log2() em ponto flutuante pode
 * introduzir nos limites das potências de dois.
 */
int calcular_k(int n)
{
    int k = 0;

    while (n > 0)
    {
        n = n / 2;
        k++;
    }

    return k;
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

    FILE *arquivo = fopen("data/resultados_binaria_iterativa.csv", "w");

    if (arquivo == NULL)
    {
        printf("Erro ao criar data/resultados_binaria_iterativa.csv\n");
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
         * O vetor contém 0, 1, ..., n - 1, então p = n não existe nele e
         * é maior que todos os elementos. Por isso o algoritmo sempre
         * segue para a metade direita, que é a maior das duas com o
         * cálculo de meio utilizado, e só para quando o intervalo fica
         * vazio. Esse é o caminho mais longo possível.
         */
        int p = n;

        /*
         * 4. Aquecimento
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
            resultado = busca_binaria_iterativa(v, n, p);
        }

        /* 5. Medição: só chamadas à função original dentro do cronômetro */
        clock_t inicio = clock();

        for (int r = 0; r < REPETICOES; r++)
        {
            resultado = busca_binaria_iterativa(v, n, p);
        }

        clock_t fim = clock();

        double tempo_total =
            (double)(fim - inicio) / CLOCKS_PER_SEC;

        double tempo_medio = tempo_total / REPETICOES;

        /*
         * 6. Contagem das operações, fora do cronômetro
         *
         * Uma execução basta: para um mesmo n e um mesmo valor procurado,
         * a quantidade de operações é sempre a mesma.
         */
        long long comparacoes = 0;
        long long atribuicoes = 0;

        int resultado_contagem =
            busca_binaria_iterativa_conta(
                v,
                n,
                p,
                &comparacoes,
                &atribuicoes
            );

        /*
         * 7. Conferência com a teoria
         *
         * Sendo k o número de repetições com intervalo não vazio, cada
         * uma faz 3 comparações e 2 atribuições. Somando a comparação
         * final, falsa, e as duas inicializações:
         *
         *   C(n) = 3k + 1
         *   A(n) = 2k + 2
         */
        int k_teorico = calcular_k(n);

        long long comparacoes_teoricas = 3LL * k_teorico + 1;
        long long atribuicoes_teoricas = 2LL * k_teorico + 2;

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

        /* 8. Exibição dos resultados */
        printf("Valor procurado : %d (inexistente)\n", p);
        printf("Repeticoes      : %d\n", REPETICOES);
        printf("k teorico       : %d\n", k_teorico);
        printf("Tempo total     : %.9f s\n", tempo_total);
        printf("Tempo medio     : %.12f s\n", tempo_medio);
        printf("Comparacoes     : %lld\n", comparacoes);
        printf("Atribuicoes     : %lld\n", atribuicoes);
        printf("C(n) teorico    : %lld\n", comparacoes_teoricas);
        printf("A(n) teorico    : %lld\n", atribuicoes_teoricas);

        /* 9. Gravação no CSV */
        fprintf(
            arquivo,
            "binaria_iterativa,"
            "%d,"
            "%d,"
            "%.12e,"
            "%.12e,"
            "%lld,"
            "%lld\n",
            n,
            REPETICOES,
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
    printf("data/resultados_binaria_iterativa.csv\n");

    return 0;
}
