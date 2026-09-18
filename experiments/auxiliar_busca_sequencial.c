/*
 * Código auxiliar de experimento — busca sequencial
 *
 * Objetivo:
 * Medir experimentalmente o comportamento da função
 * busca_sequencial(), registrando:
 *
 * 1. tempo médio de execução;
 * 2. número de comparações no pior caso;
 * 3. número de atribuições no pior caso.
 *
 * Tamanhos testados:
 *
 * 1.000
 * 50.000
 * 100.000
 * 500.000
 * 1.000.000
 *
 * A função original busca_sequencial() é utilizada exclusivamente
 * para a medição de tempo.
 *
 * Uma versão instrumentada, busca_sequencial_conta(), é utilizada
 * separadamente para contar comparações e atribuições.
 *
 * Isso evita que os próprios contadores interfiram no tempo medido.
 *
 * Arquivo gerado:
 *
 * data/resultados_busca_sequencial.csv
 *
 * Compilação, a partir da raiz do projeto:
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


/*
 * Número de tamanhos utilizados no experimento.
 */
#define QTD_TAMANHOS 5


/*
 * Execuções realizadas antes da medição.
 *
 * Elas NÃO entram no cálculo do tempo médio.
 */
#define AQUECIMENTO 10


/*
 * ------------------------------------------------------------
 * VERSÃO INSTRUMENTADA DA BUSCA SEQUENCIAL
 * ------------------------------------------------------------
 *
 * Mantém a mesma lógica da função original:
 *
 * int busca_sequencial(int *v, int n, int valor)
 *
 * porém adiciona contadores de comparações e atribuições.
 *
 * Ela NÃO deve ser utilizada para medir tempo.
 *
 *
 * CONVENÇÃO DE CONTAGEM
 * ------------------------------------------------------------
 *
 * Comparações:
 *
 * 1. i < n
 *
 *    Inclui a última avaliação, falsa, que encerra o laço
 *    quando o valor não é encontrado.
 *
 * 2. v[i] == valor
 *
 *
 * Atribuições:
 *
 * 1. i = 0
 *
 * 2. i++
 *
 *
 * Não contamos como atribuições ou comparações:
 *
 * - declarações sem inicialização;
 * - operações aritméticas e acessos ao vetor;
 * - passagem de parâmetros e instruções return;
 * - atualizações dos próprios contadores.
 *
 * Essa é a mesma convenção adotada nos demais algoritmos
 * do trabalho, definida em analysis/README.md.
 *
 *
 * O laço original é um for. Aqui ele foi reescrito com
 * while (1) e break apenas para que cada avaliação da
 * condição fique explícita. O while (1) e o break não são
 * contabilizados: cada incremento representa uma operação
 * existente no algoritmo original.
 */
int busca_sequencial_conta(
    int *v,
    int n,
    int valor,
    long long *comparacoes,
    long long *atribuicoes
)
{
    /*
     * Inicialização do for:
     *
     * i = 0
     *
     * Uma atribuição.
     */
    int i = 0;

    (*atribuicoes)++;

    while (1)
    {
        /*
         * Avaliação da condição:
         *
         * i < n
         */
        (*comparacoes)++;

        if (!(i < n))
        {
            break;
        }

        /*
         * Comparação do elemento atual com o valor procurado:
         *
         * v[i] == valor
         */
        (*comparacoes)++;

        if (v[i] == valor)
        {
            return 1;
        }

        /*
         * Atualização do índice:
         *
         * i++
         *
         * Uma atribuição.
         */
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
 * Cria:
 *
 * 0, 1, 2, 3, ..., n - 1
 *
 * A busca sequencial não exige vetor ordenado, mas usamos a
 * mesma entrada dos demais experimentos para padronizar a
 * comparação entre os quatro algoritmos.
 *
 * Essa preparação ocorre FORA da região cronometrada.
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
 * DEFINIÇÃO DO NÚMERO DE REPETIÇÕES
 * ------------------------------------------------------------
 *
 * O enunciado exige no mínimo 100 execuções.
 *
 * Como o custo da busca sequencial cresce linearmente com n,
 * usamos a mesma regra do experimento de inversão:
 *
 * - entradas pequenas recebem mais repetições;
 * - entradas grandes recebem menos;
 * - nunca usamos menos de 100 execuções.
 *
 * Isso mantém o tempo total de cada bloco em uma faixa
 * mensurável pelo relógio.
 *
 * n = 1.000       -> 100.000 repetições
 * n = 50.000      -> 2.000
 * n = 100.000     -> 1.000
 * n = 500.000     -> 200
 * n = 1.000.000   -> 100
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
    /*
     * Tamanhos exigidos pelo experimento.
     */
    int tamanhos[QTD_TAMANHOS] = {
        1000,
        50000,
        100000,
        500000,
        1000000
    };


    /*
     * Abre o arquivo CSV.
     *
     * A pasta data/ precisa existir.
     */
    FILE *arquivo = fopen(
        "data/resultados_busca_sequencial.csv",
        "w"
    );

    if (arquivo == NULL)
    {
        printf(
            "Erro ao criar "
            "data/resultados_busca_sequencial.csv\n"
        );

        return 1;
    }


    /*
     * Cabeçalho do CSV.
     */
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


    /*
     * Executa o experimento para cada tamanho.
     */
    for (int k = 0; k < QTD_TAMANHOS; k++)
    {
        int n = tamanhos[k];


        printf(
            "\n----------------------------------------\n"
        );

        printf(
            "Testando n = %d\n",
            n
        );


        /*
         * ----------------------------------------------------
         * 1. ALOCAÇÃO DO VETOR
         * ----------------------------------------------------
         */
        int *v = malloc(
            (size_t)n * sizeof(int)
        );

        if (v == NULL)
        {
            printf(
                "Erro de alocacao para n = %d\n",
                n
            );

            fclose(arquivo);

            return 1;
        }


        /*
         * ----------------------------------------------------
         * 2. PREPARAÇÃO DA ENTRADA
         * ----------------------------------------------------
         *
         * Essa etapa não é cronometrada.
         */
        preencher_vetor(v, n);


        /*
         * ----------------------------------------------------
         * 3. DEFINIÇÃO DO PIOR CASO
         * ----------------------------------------------------
         *
         * O vetor contém:
         *
         * 0, 1, 2, ..., n - 1
         *
         * Portanto o valor:
         *
         * p = n
         *
         * NÃO existe no vetor.
         *
         * A busca sequencial precisa examinar todas as n
         * posições antes de concluir que o valor está ausente.
         *
         * Esse é o maior número de comparações possível para
         * um vetor desse tamanho.
         */
        int p = n;


        /*
         * ----------------------------------------------------
         * 4. DEFINIÇÃO DO NÚMERO DE REPETIÇÕES
         * ----------------------------------------------------
         */
        int repeticoes =
            calcular_repeticoes(n);


        /*
         * ----------------------------------------------------
         * 5. AQUECIMENTO
         * ----------------------------------------------------
         *
         * Algumas buscas são realizadas antes do cronômetro,
         * para reduzir o peso do primeiro acesso ao código e
         * aos dados. Essas execuções não entram na medição.
         */
        volatile int resultado = 0;

        for (int r = 0; r < AQUECIMENTO; r++)
        {
            resultado =
                busca_sequencial(v, n, p);
        }


        /*
         * ----------------------------------------------------
         * 6. MEDIÇÃO DO TEMPO
         * ----------------------------------------------------
         *
         * Dentro da região cronometrada ficam apenas chamadas
         * à função original.
         *
         * O resultado é armazenado em uma variável volatile
         * para reduzir a possibilidade de o compilador
         * eliminar a chamada por considerar seu resultado
         * desnecessário.
         */
        clock_t inicio = clock();

        for (int r = 0; r < repeticoes; r++)
        {
            resultado =
                busca_sequencial(v, n, p);
        }

        clock_t fim = clock();


        /*
         * Tempo total de todas as execuções.
         */
        double tempo_total =
            (double)(fim - inicio)
            / CLOCKS_PER_SEC;


        /*
         * Tempo médio de UMA busca.
         */
        double tempo_medio =
            tempo_total
            / repeticoes;


        /*
         * ----------------------------------------------------
         * 7. CONTAGEM DAS OPERAÇÕES
         * ----------------------------------------------------
         *
         * A versão instrumentada é executada separadamente
         * para não interferir no tempo.
         *
         * Basta uma execução, pois para um mesmo n e um mesmo
         * valor procurado a quantidade de operações é
         * determinística.
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
         * ----------------------------------------------------
         * 8. VALIDAÇÃO COM A TEORIA
         * ----------------------------------------------------
         *
         * No pior caso, com o valor ausente:
         *
         * A condição i < n é avaliada n + 1 vezes: uma para
         * cada posição examinada e mais uma, falsa, que
         * encerra o laço.
         *
         * A comparação v[i] == valor ocorre n vezes.
         *
         * Portanto:
         *
         * C(n) = (n + 1) + n = 2n + 1
         *
         * As atribuições são a inicialização i = 0 e os n
         * incrementos i++:
         *
         * A(n) = n + 1
         */
        long long comparacoes_teoricas =
            2LL * n + 1;

        long long atribuicoes_teoricas =
            (long long)n + 1;


        /*
         * Como p = n não pertence ao vetor, o retorno correto
         * das duas versões é zero.
         */
        if (resultado != 0)
        {
            printf(
                "ATENCAO: resultado incorreto "
                "na funcao original!\n"
            );
        }

        if (resultado_contagem != 0)
        {
            printf(
                "ATENCAO: resultado incorreto "
                "na funcao instrumentada!\n"
            );
        }

        if (comparacoes != comparacoes_teoricas)
        {
            printf(
                "ATENCAO: comparacoes diferentes "
                "do valor teorico!\n"
            );
        }

        if (atribuicoes != atribuicoes_teoricas)
        {
            printf(
                "ATENCAO: atribuicoes diferentes "
                "do valor teorico!\n"
            );
        }


        /*
         * ----------------------------------------------------
         * 9. EXIBIÇÃO DOS RESULTADOS
         * ----------------------------------------------------
         */
        printf(
            "Valor procurado : %d (inexistente)\n",
            p
        );

        printf(
            "Repeticoes      : %d\n",
            repeticoes
        );

        printf(
            "Tempo total     : %.9f s\n",
            tempo_total
        );

        printf(
            "Tempo medio     : %.12f s\n",
            tempo_medio
        );

        printf(
            "Comparacoes     : %lld\n",
            comparacoes
        );

        printf(
            "Atribuicoes     : %lld\n",
            atribuicoes
        );

        printf(
            "C(n) teorico    : %lld\n",
            comparacoes_teoricas
        );

        printf(
            "A(n) teorico    : %lld\n",
            atribuicoes_teoricas
        );


        /*
         * ----------------------------------------------------
         * 10. GRAVAÇÃO NO CSV
         * ----------------------------------------------------
         */
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


        /*
         * ----------------------------------------------------
         * 11. LIBERAÇÃO DA MEMÓRIA
         * ----------------------------------------------------
         */
        free(v);
    }


    /*
     * Fecha o arquivo CSV.
     */
    fclose(arquivo);


    printf(
        "\n========================================\n"
    );

    printf(
        "Experimento concluido.\n"
    );

    printf(
        "Resultados salvos em:\n"
        "data/resultados_busca_sequencial.csv\n"
    );


    return 0;
}
