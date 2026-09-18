/*
 * VERSAO EM ARQUIVO UNICO, para submissao no Run.Codes caso a plataforma
 * aceite apenas um arquivo .c. Conteudo identico ao de src/main.c,
 * src/algoritmos.c e src/algoritmos.h juntos.
 *
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

void inverte(int *v, int n);

int busca_sequencial(int *v, int n, int valor);

int busca_binaria_iterativa(int *v, int n, int valor);

int busca_binaria_recursiva(
    int *v,
    int inicio,
    int fim,
    int valor);


/*
 * Inverte a ordem dos elementos do vetor, no proprio vetor.
 *
 * Dois indices caminham das pontas para o centro e trocam os elementos
 * de posicao. Basta percorrer metade do vetor, porque cada troca ja
 * posiciona dois elementos. Com n impar, o elemento central permanece
 * onde esta.
 */
void inverte(int *v, int n)
{
    int i, j, temp;

    for (i = 0, j = n - 1; i < n / 2; i++, j--)
    {
        temp = v[i];
        v[i] = v[j];
        v[j] = temp;
    }
}

/*
 * Busca sequencial. Retorna 1 se o valor esta no vetor e 0 caso contrario.
 *
 * Unica das tres buscas que nao exige vetor ordenado. Em troca, precisa
 * examinar uma posicao por vez, e o numero de comparacoes depende de onde
 * o valor esta.
 */
int busca_sequencial(int *v, int n, int valor)
{
    int i;

    for (i = 0; i < n; i++)
    {
        if (v[i] == valor)
        {
            return 1;
        }
    }

    return 0;
}

/*
 * Busca binaria iterativa. Exige vetor ordenado em ordem crescente.
 * Retorna 1 se o valor esta no vetor e 0 caso contrario.
 *
 * O intervalo de busca e delimitado por inicio e fim, ambos inclusivos,
 * e cai pela metade a cada repeticao: comparando o valor procurado com o
 * elemento central, uma das duas metades pode ser descartada inteira.
 * O laco termina quando o valor e encontrado ou quando o intervalo fica
 * vazio, o que acontece quando inicio passa de fim.
 *
 * O calculo inicio + (fim - inicio) / 2 equivale a (inicio + fim) / 2,
 * mas evita o estouro do int que a soma direta poderia causar com
 * indices grandes.
 */
int busca_binaria_iterativa(int *v, int n, int valor)
{
    int inicio = 0;
    int fim = n - 1;
    int meio;

    while (inicio <= fim)
    {
        meio = inicio + (fim - inicio) / 2;

        if (v[meio] == valor)
        {
            return 1;
        }

        if (v[meio] < valor)
        {
            inicio = meio + 1;
        }
        else
        {
            fim = meio - 1;
        }
    }

    return 0;
}

/*
 * Busca binaria recursiva. Mesma estrategia da versao iterativa, mas a
 * reducao do intervalo e feita por uma nova chamada em vez de um laco.
 *
 * Recebe os limites inclusivos do intervalo, entao a busca no vetor
 * inteiro e busca_binaria_recursiva(v, 0, n - 1, p). O caso-base
 * inicio > fim representa o intervalo vazio, ou seja, valor ausente.
 */
int busca_binaria_recursiva(
    int *v,
    int inicio,
    int fim,
    int p)
{
    if (inicio > fim)
    {
        return 0;
    }

    int meio = inicio + (fim - inicio) / 2;

    if (v[meio] == p)
    {
        return 1;
    }

    if (v[meio] < p)
        return busca_binaria_recursiva(v, meio + 1, fim, p);

    else
        return busca_binaria_recursiva(v, inicio, meio - 1, p);
}


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
