/*
 * SCC0502 - Algoritmos e Estruturas de Dados I - ICMC/USP
 * Trabalho 1: Contagem de Operações e de Tempo de Execução
 *
 * Integrantes:
 *   Nome Completo - Nº USP
 *   Nome Completo - Nº USP
 *   Nome Completo - Nº USP
 *   Nome Completo - Nº USP
 *
 * Programa principal: lê um vetor ordenado de N inteiros e executa
 * comandos até receber o comando de encerramento.
 *
 * Comandos:
 *   1 - Inversão da ordem do vetor (imprime o vetor invertido)
 *   2 - Busca sequencial        (lê P e imprime SIM ou NAO)
 *   3 - Busca binária iterativa (lê P e imprime SIM ou NAO)
 *   4 - Busca binária recursiva (lê P e imprime SIM ou NAO)
 *   5 - Encerra o programa
 *
 * As funções dos algoritmos estão implementadas em algoritmos.c.
 */

#include <stdio.h>
#include <stdlib.h>
#include "algoritmos.h"

int main()
{
    int n, q, i, p;

    /* Leitura do tamanho do vetor */
    scanf("%d", &n);

    /*
     * O vetor é alocado dinamicamente, pois seu tamanho só é conhecido em
     * tempo de execução e pode chegar a 1 000 000 de elementos, o que
     * excederia o limite da pilha em algumas plataformas.
     */
    int *v = malloc(n * sizeof(int));

    /* Leitura dos N elementos (o enunciado garante que estão ordenados) */
    for (i = 0; i < n; i++)
    {
        scanf("%d", &v[i]);
    }

    /* Laço de comandos: executa até que o comando 5 seja lido */
    do
    {
        scanf("%d", &q);

        switch (q)
        {
        case 1:
        {
            /*
             * A inversão é feita sobre uma cópia do vetor. Assim, o vetor
             * original permanece ordenado, condição necessária para que
             * as buscas binárias dos comandos seguintes funcionem.
             */
            int *vcopia = malloc(n * sizeof(int));

            for (i = 0; i < n; i++)
            {
                vcopia[i] = v[i];
            }

            inverte(vcopia, n);

            /*
             * Impressão sem espaço após o último elemento: o primeiro
             * valor é impresso isoladamente e os demais são precedidos
             * por um espaço. Como N >= 1, vcopia[0] sempre existe.
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
         * Comandos de busca: leem o valor P a ser buscado. As funções
         * retornam 1 se P está no vetor e 0 caso contrário; a impressão
         * fica no main para não interferir na medição de tempo dos
         * algoritmos, feita nos códigos auxiliares.
         */
        case 2:
            scanf("%d", &p);
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
            scanf("%d", &p);
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
            scanf("%d", &p);
            /* A busca recursiva recebe os limites do intervalo: [0, n-1] */
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
            /* Encerramento: libera a memória do vetor original */
            free(v);
            return 0;
        }
    } while (q != 5);

    free(v);

    return 0;
}
