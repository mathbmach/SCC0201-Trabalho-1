#include <stdio.h>
#include <stdlib.h>
#include "algoritmos.h"

int main()
{
    int n, q, i, p;

    scanf("%d", &n);

    int *v = malloc(n * sizeof(int));

    for (i = 0; i < n; i++)
    {
        scanf("%d", &v[i]);
    }

    do
    {
        scanf("%d", &q);

        switch (q)
        {
        case 1:
        {
            int *vcopia = malloc(n * sizeof(int));

            for (i = 1; i < n; i++)
            {
                vcopia[i] = v[i];
            }

            inverte(vcopia, n);

            printf("%d", vcopia[0]);
            for (i = 0; i < n; i++)
            {
                printf(" %d", vcopia[i]);
            }
            printf("\n");

            free(vcopia);
            break;
        }

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