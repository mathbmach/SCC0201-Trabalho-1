/*
 * Código auxiliar de experimento — busca binária recursiva
 *
 * Objetivo:
 * Medir experimentalmente o comportamento da função
 * busca_binaria_recursiva(), registrando:
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
 * A função original busca_binaria_recursiva() é utilizada
 * exclusivamente para a medição de tempo.
 *
 * Uma versão instrumentada, busca_binaria_recursiva_conta(),
 * é utilizada separadamente para contar comparações e atribuições.
 *
 * Isso evita que os próprios contadores interfiram no tempo medido.
 *
 * Arquivo gerado:
 *
 * data/resultados_binaria_recursiva.csv
 *
 * Compilação, a partir da raiz do projeto:
 *
 * gcc -Wall -Wextra -O0 experiments/auxiliar_buscabinaria_rec.c src/algoritmos.c -o auxiliar_buscabinaria_rec
 *
 * Execução no PowerShell:
 *
 * .\auxiliar_buscabinaria_rec.exe
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
 * VERSÃO INSTRUMENTADA DA BUSCA BINÁRIA RECURSIVA
 * ------------------------------------------------------------
 *
 * Mantém a mesma lógica da função original:
 *
 * int busca_binaria_recursiva(
 *     int *v,
 *     int inicio,
 *     int fim,
 *     int p
 * )
 *
 * Porém adiciona contadores de:
 *
 * - comparações;
 * - atribuições.
 *
 *
 * CONVENÇÃO DE CONTAGEM
 * ------------------------------------------------------------
 *
 * Comparações:
 *
 * 1. inicio > fim
 *
 * 2. v[meio] == p
 *
 * 3. v[meio] < p
 *
 *
 * Atribuições:
 *
 * 1. meio = inicio + (fim - inicio) / 2
 *
 *
 * Não contamos como atribuições:
 *
 * - passagem dos parâmetros da chamada recursiva;
 * - return;
 * - operações internas de infraestrutura;
 * - atualizações dos próprios contadores.
 *
 * Essa convenção deve ser mantida nos demais algoritmos
 * para permitir comparação coerente.
 */
int busca_binaria_recursiva_conta(
    int *v,
    int inicio,
    int fim,
    int p,
    long long *comparacoes,
    long long *atribuicoes
)
{
    /*
     * Comparação:
     *
     * inicio > fim
     *
     * Essa comparação ocorre em TODA chamada da função,
     * inclusive na chamada final que determina que o
     * elemento não existe.
     */
    (*comparacoes)++;

    if (inicio > fim)
    {
        return 0;
    }


    /*
     * Cálculo do índice central.
     *
     * Consideramos a atribuição ao meio como uma atribuição.
     */
    int meio = inicio + (fim - inicio) / 2;

    (*atribuicoes)++;


    /*
     * Comparação:
     *
     * v[meio] == p
     */
    (*comparacoes)++;

    if (v[meio] == p)
    {
        return 1;
    }


    /*
     * Comparação:
     *
     * v[meio] < p
     */
    (*comparacoes)++;

    if (v[meio] < p)
    {
        return busca_binaria_recursiva_conta(
            v,
            meio + 1,
            fim,
            p,
            comparacoes,
            atribuicoes
        );
    }

    else
    {
        return busca_binaria_recursiva_conta(
            v,
            inicio,
            meio - 1,
            p,
            comparacoes,
            atribuicoes
        );
    }
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
        "data/resultados_binaria_recursiva.csv",
        "w"
    );

    if (arquivo == NULL)
    {
        printf(
            "Erro ao criar "
            "data/resultados_binaria_recursiva.csv\n"
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
         * A busca binária continuará dividindo o intervalo
         * até que:
         *
         * inicio > fim
         *
         * representando um caso de busca malsucedida que
         * percorre a árvore de decisões até o final.
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
                busca_binaria_recursiva(
                    v,
                    0,
                    n - 1,
                    p
                );
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
                busca_binaria_recursiva(
                    v,
                    0,
                    n - 1,
                    p
                );
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
            busca_binaria_recursiva_conta(
                v,
                0,
                n - 1,
                p,
                &comparacoes,
                &atribuicoes
            );


        /*
         * ----------------------------------------------------
         * 7. VERIFICAÇÃO DO RESULTADO
         * ----------------------------------------------------
         *
         * Como p = n não pertence ao vetor, o retorno correto
         * é zero.
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


        /*
         * ----------------------------------------------------
         * 9. GRAVAÇÃO NO CSV
         * ----------------------------------------------------
         */
        fprintf(
            arquivo,
            "binaria_recursiva,"
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
        "data/resultados_binaria_recursiva.csv\n"
    );


    return 0;
}