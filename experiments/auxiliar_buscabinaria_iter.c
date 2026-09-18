/*
 * Código auxiliar de experimento — busca binária iterativa
 *
 * Objetivo:
 * Medir experimentalmente o comportamento da função
 * busca_binaria_iterativa(), registrando:
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
 * A função original busca_binaria_iterativa() é utilizada
 * exclusivamente para a medição de tempo.
 *
 * Uma versão instrumentada, busca_binaria_iterativa_conta(),
 * é utilizada separadamente para contar comparações e atribuições.
 *
 * Isso evita que os próprios contadores interfiram no tempo medido.
 *
 * Arquivo gerado:
 *
 * data/resultados_binaria_iterativa.csv
 *
 * Compilação, a partir da raiz do projeto:
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


/*
 * Número de tamanhos utilizados no experimento.
 */
#define QTD_TAMANHOS 5


/*
 * A busca binária é extremamente rápida.
 *
 * Por isso utilizamos um número alto de repetições para que
 * o tempo total seja suficientemente grande para clock()
 * conseguir medi-lo com maior precisão.
 *
 * O enunciado exige pelo menos 100 execuções.
 *
 * Usamos o mesmo valor do experimento da busca binária
 * recursiva, para que as duas versões sejam comparáveis
 * sob o mesmo protocolo.
 */
#define REPETICOES 1000000


/*
 * Execuções realizadas antes da medição.
 *
 * Elas NÃO entram no cálculo do tempo médio.
 */
#define AQUECIMENTO 100


/*
 * ------------------------------------------------------------
 * VERSÃO INSTRUMENTADA DA BUSCA BINÁRIA ITERATIVA
 * ------------------------------------------------------------
 *
 * Mantém a mesma lógica da função original:
 *
 * int busca_binaria_iterativa(int *v, int n, int valor)
 *
 * porém adiciona contadores de comparações e atribuições.
 *
 *
 * CONVENÇÃO DE CONTAGEM
 * ------------------------------------------------------------
 *
 * Comparações:
 *
 * 1. inicio <= fim
 *
 *    Inclui a última avaliação, falsa, que encerra o laço
 *    quando o valor não é encontrado.
 *
 * 2. v[meio] == valor
 *
 * 3. v[meio] < valor
 *
 *
 * Atribuições:
 *
 * 1. inicio = 0
 *
 * 2. fim = n - 1
 *
 * 3. meio = inicio + (fim - inicio) / 2
 *
 * 4. inicio = meio + 1  ou  fim = meio - 1
 *
 *    Apenas uma das duas é executada em cada repetição.
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
 * do trabalho, definida em analysis/README.md. Ela permite
 * comparar diretamente esta versão com a recursiva.
 *
 *
 * O laço original é um while (inicio <= fim). Aqui ele foi
 * reescrito com while (1) e break apenas para que cada
 * avaliação da condição fique explícita. O while (1) e o
 * break não são contabilizados.
 */
int busca_binaria_iterativa_conta(
    int *v,
    int n,
    int valor,
    long long *comparacoes,
    long long *atribuicoes
)
{
    /*
     * Inicialização dos limites do intervalo:
     *
     * inicio = 0
     * fim = n - 1
     *
     * Duas atribuições.
     */
    int inicio = 0;
    int fim = n - 1;
    int meio;

    *atribuicoes += 2;

    while (1)
    {
        /*
         * Avaliação da condição do laço:
         *
         * inicio <= fim
         */
        (*comparacoes)++;

        if (!(inicio <= fim))
        {
            break;
        }

        /*
         * Cálculo do índice central.
         *
         * Uma atribuição.
         */
        meio = inicio + (fim - inicio) / 2;

        (*atribuicoes)++;

        /*
         * Comparação:
         *
         * v[meio] == valor
         */
        (*comparacoes)++;

        if (v[meio] == valor)
        {
            return 1;
        }

        /*
         * Comparação:
         *
         * v[meio] < valor
         */
        (*comparacoes)++;

        if (v[meio] < valor)
        {
            inicio = meio + 1;
        }
        else
        {
            fim = meio - 1;
        }

        /*
         * Exatamente uma das duas atribuições acima é
         * executada em cada repetição.
         */
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
 * A busca binária exige vetor ordenado.
 *
 * Portanto essa entrada já satisfaz a condição necessária
 * para a execução do algoritmo.
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
 * NÚMERO DE REPETIÇÕES DO LAÇO NO PIOR CASO
 * ------------------------------------------------------------
 *
 * Calcula:
 *
 * k = floor(log2(n)) + 1
 *
 * usando apenas divisões inteiras, para evitar erros de
 * arredondamento de ponto flutuante.
 *
 * Esse é o número de repetições com intervalo não vazio
 * quando o valor procurado é maior que todos os elementos.
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
        "data/resultados_binaria_iterativa.csv",
        "w"
    );

    if (arquivo == NULL)
    {
        printf(
            "Erro ao criar "
            "data/resultados_binaria_iterativa.csv\n"
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
         * Como p é maior que todos os elementos, o algoritmo
         * sempre segue para a metade direita, que é a maior
         * das duas com o cálculo de meio utilizado, até que:
         *
         * inicio > fim
         *
         * Esse é o caminho mais longo possível: o intervalo
         * é reduzido o número máximo de vezes.
         */
        int p = n;


        /*
         * ----------------------------------------------------
         * 4. AQUECIMENTO
         * ----------------------------------------------------
         *
         * Algumas buscas são realizadas antes do cronômetro.
         *
         * Essas execuções não entram na medição.
         */
        volatile int resultado = 0;

        for (int r = 0; r < AQUECIMENTO; r++)
        {
            resultado =
                busca_binaria_iterativa(v, n, p);
        }


        /*
         * ----------------------------------------------------
         * 5. MEDIÇÃO DO TEMPO
         * ----------------------------------------------------
         *
         * Dentro da região cronometrada ficam apenas
         * chamadas à função original.
         *
         * O resultado é armazenado em uma variável volatile
         * para reduzir a possibilidade de o compilador
         * eliminar a chamada por considerar seu resultado
         * desnecessário.
         */
        clock_t inicio = clock();

        for (int r = 0; r < REPETICOES; r++)
        {
            resultado =
                busca_binaria_iterativa(v, n, p);
        }

        clock_t fim = clock();


        /*
         * Tempo de todas as execuções.
         */
        double tempo_total =
            (double)(fim - inicio)
            / CLOCKS_PER_SEC;


        /*
         * Tempo médio de UMA busca.
         */
        double tempo_medio =
            tempo_total
            / REPETICOES;


        /*
         * ----------------------------------------------------
         * 6. CONTAGEM DE OPERAÇÕES
         * ----------------------------------------------------
         *
         * A versão instrumentada é executada separadamente
         * para não interferir no tempo.
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
         * ----------------------------------------------------
         * 7. VALIDAÇÃO COM A TEORIA
         * ----------------------------------------------------
         *
         * Seja k o número de repetições com intervalo não
         * vazio:
         *
         * k = floor(log2(n)) + 1
         *
         * Cada uma dessas repetições faz três comparações
         * (inicio <= fim, v[meio] == valor e v[meio] < valor)
         * e duas atribuições (meio e a atualização de um dos
         * limites).
         *
         * Além delas, ocorre uma avaliação final de
         * inicio <= fim, que é falsa e encerra o laço.
         *
         * Somando as duas inicializações:
         *
         * C(n) = 3k + 1
         * A(n) = 2k + 2
         */
        int k_teorico = calcular_k(n);

        long long comparacoes_teoricas =
            3LL * k_teorico + 1;

        long long atribuicoes_teoricas =
            2LL * k_teorico + 2;


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
         * 8. EXIBIÇÃO DOS RESULTADOS
         * ----------------------------------------------------
         */
        printf(
            "Valor procurado : %d (inexistente)\n",
            p
        );

        printf(
            "Repeticoes      : %d\n",
            REPETICOES
        );

        printf(
            "k teorico       : %d\n",
            k_teorico
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
         * 9. GRAVAÇÃO NO CSV
         * ----------------------------------------------------
         */
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


        /*
         * ----------------------------------------------------
         * 10. LIBERAÇÃO DA MEMÓRIA
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
        "data/resultados_binaria_iterativa.csv\n"
    );


    return 0;
}
