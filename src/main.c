/*
 * SCC0502 - Algoritmos e Estruturas de Dados I - ICMC/USP
 * Trabalho 1: Contagem de Operacoes e de Tempo de Execucao
 *
 * Integrantes:
 *   Enzo Ribeiro Ferrari - No USP 15574520
 *   Matheus de Souza Santana - No USP 17839560
 *   Pietro Augusto Venancio da Costa - No USP 17839104
 *   Matheus Bastos Machado - No USP 18126601
 *
 * Le um vetor ordenado de N inteiros e executa comandos ate receber o
 * comando de encerramento:
 *
 *   1 - Inversao da ordem do vetor (imprime o vetor invertido)
 *   2 - Busca sequencial        (le P e imprime SIM ou NAO)
 *   3 - Busca binaria iterativa (le P e imprime SIM ou NAO)
 *   4 - Busca binaria recursiva (le P e imprime SIM ou NAO)
 *   5 - Encerra o programa
 *
 * Os algoritmos estao em algoritmos.c.
 */

#include <stdio.h>
#include <stdlib.h>
#include "algoritmos.h"

int main()
{
    int n, q, i, p;

    if (scanf("%d", &n) != 1)
    {
        return 1;
    }

    /*
     * Alocacao dinamica: n so e conhecido em tempo de execucao e pode
     * chegar a 1 000 000, o que excederia o limite da pilha em algumas
     * plataformas se o vetor fosse declarado localmente.
     */
    int *v = malloc(n * sizeof(int));

    if (v == NULL)
    {
        return 1;
    }

    for (i = 0; i < n; i++)
    {
        if (scanf("%d", &v[i]) != 1)
        {
            free(v);
            return 1;
        }
    }

    do
    {
        if (scanf("%d", &q) != 1)
        {
            break;
        }

        switch (q)
        {
        case 1:
        {
            /*
             * A inversao trabalha sobre uma copia. Assim o vetor original
             * permanece ordenado, condicao necessaria para que as buscas
             * binarias dos comandos seguintes continuem corretas.
             */
            int *vcopia = malloc(n * sizeof(int));

            if (vcopia == NULL)
            {
                free(v);
                return 1;
            }

            for (i = 0; i < n; i++)
            {
                vcopia[i] = v[i];
            }

            inverte(vcopia, n);

            /*
             * O primeiro valor sai isolado e os demais precedidos de
             * espaco, para nao deixar espaco sobrando no fim da linha.
             */
            printf("%d", vcopia[0]);
            for (i = 1; i < n; i++)
            {
                printf(" %d", vcopia[i]);
            }
            printf("\n");

            free(vcopia);
            break;
        }

        /*
         * As tres buscas retornam 1 ou 0; a impressao de SIM e NAO fica
         * aqui, fora das funcoes, para que a medicao de tempo feita nos
         * codigos auxiliares nao inclua custo de escrita na tela.
         */
        case 2:
            if (scanf("%d", &p) != 1)
            {
                free(v);
                return 1;
            }
            if (busca_sequencial(v, n, p) == 0)
            {
                printf("NAO\n");
            }
            else
            {
                printf("SIM\n");
            }
            break;

        case 3:
            if (scanf("%d", &p) != 1)
            {
                free(v);
                return 1;
            }
            if (busca_binaria_iterativa(v, n, p) == 0)
            {
                printf("NAO\n");
            }
            else
            {
                printf("SIM\n");
            }
            break;

        case 4:
            if (scanf("%d", &p) != 1)
            {
                free(v);
                return 1;
            }
            /* A versao recursiva recebe os limites do intervalo */
            if (busca_binaria_recursiva(v, 0, n - 1, p) == 0)
            {
                printf("NAO\n");
            }
            else
            {
                printf("SIM\n");
            }
            break;

        case 5:
            free(v);
            return 0;
        }
    } while (q != 5);

    free(v);

    return 0;
}
