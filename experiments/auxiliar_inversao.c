/*
 * Código auxiliar de experimento — inversão de vetor
 *
 * Objetivo:
 * Medir experimentalmente o comportamento da função inverte(),
 * registrando:
 *
 * 1. tempo médio de execução;
 * 2. número de comparações;
 * 3. número de atribuições;
 *
 * para vetores de tamanho:
 *
 * 1.000
 * 50.000
 * 100.000
 * 500.000
 * 1.000.000
 *
 * A função original inverte() é utilizada para medir tempo.
 * Uma versão instrumentada, inverte_conta(), é utilizada apenas
 * para contar comparações e atribuições.
 *
 * Isso evita que os próprios contadores interfiram no tempo medido.
 *
 * Arquivo gerado:
 *
 * data/resultados_inversao.csv
 *
 * Compilação, a partir da raiz do projeto:
 *
 * gcc -Wall -Wextra -O0 experiments/auxiliar_inversao.c src/algoritmos.c -o auxiliar_inversao
 *
 * Execução:
 *
 * ./auxiliar_inversao
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../src/algoritmos.h"

/*
 * Quantidade de tamanhos de vetor utilizados no experimento.
 */
#define QTD_TAMANHOS 5

/*
 * Quantidade de execuções utilizadas apenas para aquecimento.
 *
 * Essas execuções NÃO fazem parte da medição.
 */
#define AQUECIMENTO 10


/*
 * ------------------------------------------------------------
 * FUNÇÃO INSTRUMENTADA
 * ------------------------------------------------------------
 *
 * Esta função possui exatamente a mesma lógica da função original:
 *
 * void inverte(int *v, int n)
 *
 * porém adiciona contadores de comparações e atribuições.
 *
 * Ela NÃO deve ser utilizada para medir tempo.
 *
 *
 * Convenção adotada:
 *
 * COMPARAÇÃO
 * -----------
 * Cada avaliação da condição:
 *
 *     i < n / 2
 *
 * é contabilizada como uma comparação.
 *
 * Isso inclui:
 *
 * - as avaliações verdadeiras;
 * - a última avaliação falsa, que encerra o laço.
 *
 *
 * ATRIBUIÇÃO
 * ----------
 * São contabilizadas como atribuições:
 *
 * - i = 0
 * - j = n - 1
 * - temp = v[i]
 * - v[i] = v[j]
 * - v[j] = temp
 * - i++
 * - j--
 *
 */
void inverte_conta(
    int *v,
    int n,
    long long *comparacoes,
    long long *atribuicoes
)
{
    int i, j, temp;

    /*
     * Inicializações do for:
     *
     * i = 0
     * j = n - 1
     *
     * Duas atribuições.
     */
    i = 0;
    j = n - 1;

    *atribuicoes += 2;

    /*
     * Reescrevemos o for de maneira um pouco mais explícita
     * para facilitar a contagem da condição.
     */
    while (1)
    {
        /*
         * Avaliação da condição:
         *
         * i < n / 2
         */
        (*comparacoes)++;

        /*
         * Caso a condição seja falsa,
         * o algoritmo termina.
         */
        if (!(i < n / 2))
        {
            break;
        }

        /*
         * Troca dos elementos.
         *
         * temp = v[i]
         * v[i] = v[j]
         * v[j] = temp
         *
         * Total: 3 atribuições.
         */
        temp = v[i];
        v[i] = v[j];
        v[j] = temp;

        *atribuicoes += 3;

        /*
         * Atualizações equivalentes ao:
         *
         * i++, j--
         *
         * Consideramos cada atualização como uma atribuição.
         */
        i++;
        j--;

        *atribuicoes += 2;
    }
}


/*
 * ------------------------------------------------------------
 * FUNÇÃO PARA PREENCHER O VETOR
 * ------------------------------------------------------------
 *
 * Preenche o vetor com:
 *
 * 0, 1, 2, 3, ..., n - 1
 *
 * Essa preparação é realizada FORA da região cronometrada.
 *
 * Embora a inversão não exija vetor ordenado,
 * essa escolha padroniza as entradas dos experimentos.
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
 * Para vetores pequenos, 100 execuções podem durar muito pouco
 * e gerar medições imprecisas.
 *
 * Portanto:
 *
 * - entradas pequenas recebem mais repetições;
 * - entradas grandes recebem menos;
 * - nunca usamos menos de 100 execuções.
 *
 * Exemplo:
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
     * Tamanhos exigidos para o experimento.
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
        "data/resultados_inversao.csv",
        "w"
    );

    if (arquivo == NULL)
    {
        printf(
            "Erro ao criar "
            "data/resultados_inversao.csv\n"
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
     * Testa todos os tamanhos.
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
         * 3. DEFINIÇÃO DO NÚMERO DE REPETIÇÕES
         * ----------------------------------------------------
         */
        int repeticoes =
            calcular_repeticoes(n);


        /*
         * ----------------------------------------------------
         * 4. AQUECIMENTO
         * ----------------------------------------------------
         *
         * Executamos algumas inversões antes da medição.
         *
         * O objetivo é evitar que a primeira execução tenha
         * comportamento muito diferente devido a efeitos
         * iniciais de memória/cache.
         *
         * Como a inversão sempre realiza a mesma quantidade
         * de operações, não há problema em inverter o mesmo
         * vetor diversas vezes.
         */
        for (
            int r = 0;
            r < AQUECIMENTO;
            r++
        )
        {
            inverte(v, n);
        }


        /*
         * ----------------------------------------------------
         * 5. MEDIÇÃO DO TEMPO
         * ----------------------------------------------------
         *
         * Dentro da região medida há apenas chamadas
         * à função original inverte().
         *
         * malloc, preenchimento, printf e gravação em arquivo
         * ficam fora.
         */
        clock_t inicio = clock();

        for (
            int r = 0;
            r < repeticoes;
            r++
        )
        {
            inverte(v, n);
        }

        clock_t fim = clock();


        /*
         * Tempo total de todas as execuções.
         */
        double tempo_total =
            (double)(fim - inicio)
            / CLOCKS_PER_SEC;


        /*
         * Tempo médio de uma única execução.
         */
        double tempo_medio =
            tempo_total
            / repeticoes;


        /*
         * ----------------------------------------------------
         * 6. CONTAGEM DAS OPERAÇÕES
         * ----------------------------------------------------
         *
         * Os contadores começam em zero.
         */
        long long comparacoes = 0;
        long long atribuicoes = 0;


        /*
         * Basta uma execução da versão instrumentada,
         * pois para um mesmo n a quantidade de operações
         * é determinística.
         */
        inverte_conta(
            v,
            n,
            &comparacoes,
            &atribuicoes
        );


        /*
         * ----------------------------------------------------
         * 7. VALIDAÇÃO COM A TEORIA
         * ----------------------------------------------------
         *
         * Para os valores de n utilizados no experimento,
         * todos são pares.
         *
         * O laço executa:
         *
         * n / 2
         *
         * vezes.
         *
         * Portanto:
         *
         * Comparações:
         *
         * C(n) = n/2 + 1
         *
         * Atribuições:
         *
         * duas inicializações
         * +
         * cinco atribuições por iteração
         *
         * A(n) = 2 + 5(n/2)
         */
        long long comparacoes_teoricas =
            (long long)n / 2 + 1;

        long long atribuicoes_teoricas =
            2 + 5LL * (n / 2);


        /*
         * Verifica se a instrumentação está coerente
         * com a análise matemática.
         */
        if (
            comparacoes
            != comparacoes_teoricas
        )
        {
            printf(
                "ATENCAO: comparacoes diferentes "
                "do valor teorico!\n"
            );
        }

        if (
            atribuicoes
            != atribuicoes_teoricas
        )
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
            "Repeticoes   : %d\n",
            repeticoes
        );

        printf(
            "Tempo total  : %.9f s\n",
            tempo_total
        );

        printf(
            "Tempo medio  : %.12f s\n",
            tempo_medio
        );

        printf(
            "Comparacoes  : %lld\n",
            comparacoes
        );

        printf(
            "Atribuicoes  : %lld\n",
            atribuicoes
        );

        printf(
            "C(n) teorico : %lld\n",
            comparacoes_teoricas
        );

        printf(
            "A(n) teorico : %lld\n",
            atribuicoes_teoricas
        );


        /*
         * ----------------------------------------------------
         * 9. GRAVAÇÃO NO CSV
         * ----------------------------------------------------
         */
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


        /*
         * ----------------------------------------------------
         * 10. LIBERAÇÃO DA MEMÓRIA
         * ----------------------------------------------------
         */
        free(v);
    }


    /*
     * Fecha o CSV.
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
        "data/resultados_inversao.csv\n"
    );


    return 0;
}