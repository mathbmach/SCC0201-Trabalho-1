/*
 * VERSAO EM ARQUIVO UNICO (para submissao no Run.Codes, caso a
 * plataforma aceite apenas um arquivo .c). O conteudo e identico ao
 * de src/main.c + src/algoritmos.c + src/algoritmos.h.
 *
 * SCC0502 - Algoritmos e Estruturas de Dados I - ICMC/USP
 * Trabalho 1: Contagem de Operacoes e de Tempo de Execucao
 *
 * Integrantes:
 *   Enzo Ribeiro Ferrari - 15574520
 *   Matheus Bastos Machado - 18126601
 *   Pietro Augusto Venancio da Costa - 17839104
 *   Matheus de Souza Santana - 17839560
 *
 * Programa principal: le um vetor ordenado de N inteiros e executa
 * comandos ate receber o comando de encerramento.
 *
 * Comandos:
 *   1 - Inversao da ordem do vetor (imprime o vetor invertido)
 *   2 - Busca sequencial        (le P e imprime SIM ou NAO)
 *   3 - Busca binaria iterativa (le P e imprime SIM ou NAO)
 *   4 - Busca binaria recursiva (le P e imprime SIM ou NAO)
 *   5 - Encerra o programa
 *
 * As funcoes dos algoritmos estao implementadas em algoritmos.c.
 */

#include <stdio.h>
#include <stdlib.h>

/* ------------------------------------------------------------------
 * Prototipos das funcoes dos algoritmos
 * ------------------------------------------------------------------ */

void inverte(int *v, int n);

int busca_sequencial(int *v, int n, int valor);

int busca_binaria_iterativa(int *v, int n, int valor);

int busca_binaria_recursiva(
    int *v,
    int inicio,
    int fim,
    int valor);


/* ------------------------------------------------------------------
 * Implementacao dos algoritmos
 * ------------------------------------------------------------------ */

/*
 * Inverte a ordem dos elementos do vetor v, no proprio vetor.
 *
 * Dois indices caminham das pontas para o centro: i comeca em 0 e j
 * comeca em n - 1. A cada repeticao os elementos dessas posicoes sao
 * trocados. Basta percorrer metade do vetor, pois cada troca ja coloca
 * dois elementos nas posicoes corretas. Quando n e impar, o elemento
 * central permanece onde esta.
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
 * Busca sequencial: percorre o vetor da primeira ate a ultima posicao,
 * comparando cada elemento com o valor procurado.
 *
 * Retorna 1 se o valor estiver no vetor e 0 caso contrario. O laco para
 * assim que o valor e encontrado, de modo que o numero de comparacoes
 * depende da posicao do elemento.
 *
 * Nao exige vetor ordenado.
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
 * Busca binaria iterativa: exige vetor ordenado em ordem crescente.
 *
 * O intervalo de busca e delimitado por inicio e fim, ambos inclusivos.
 * A cada repeticao o elemento central do intervalo e comparado com o
 * valor procurado: se for igual, a busca termina; se for menor, o valor
 * so pode estar na metade direita; se for maior, apenas na metade
 * esquerda. Assim o intervalo cai aproximadamente pela metade a cada
 * passo, ate que o valor seja encontrado ou o intervalo fique vazio.
 *
 * Retorna 1 se o valor estiver no vetor e 0 caso contrario.
 *
 * O calculo inicio + (fim - inicio) / 2 e equivalente a (inicio + fim) / 2
 * para os valores usados aqui, mas evita o estouro do inteiro que a soma
 * direta poderia causar com indices grandes.
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
 * Busca binaria recursiva: mesma estrategia da versao iterativa, porem a
 * reducao do intervalo e feita por uma nova chamada da propria funcao.
 *
 * Recebe os limites inclusivos do intervalo. Para buscar no vetor inteiro,
 * a chamada e busca_binaria_recursiva(v, 0, n - 1, p).
 *
 * O caso-base inicio > fim representa o intervalo vazio, ou seja, o valor
 * nao esta no vetor. Retorna 1 se o valor for encontrado e 0 caso
 * contrario. Exige vetor ordenado em ordem crescente.
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


/* ------------------------------------------------------------------
 * Programa principal
 * ------------------------------------------------------------------ */


int main()
{
    int n, q, i, p;

    /* Leitura do tamanho do vetor */
    if (scanf("%d", &n) != 1)
    {
        return 1;
    }

    /*
     * O vetor e alocado dinamicamente, pois seu tamanho so e conhecido em
     * tempo de execucao e pode chegar a 1 000 000 de elementos, o que
     * excederia o limite da pilha em algumas plataformas.
     */
    int *v = malloc(n * sizeof(int));

    if (v == NULL)
    {
        return 1;
    }

    /* Leitura dos N elementos (o enunciado garante que estao ordenados) */
    for (i = 0; i < n; i++)
    {
        if (scanf("%d", &v[i]) != 1)
        {
            free(v);
            return 1;
        }
    }

    /* Laco de comandos: executa ate que o comando 5 seja lido */
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
             * A inversao e feita sobre uma copia do vetor. Assim, o vetor
             * original permanece ordenado, condicao necessaria para que
             * as buscas binarias dos comandos seguintes funcionem.
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
             * Impressao sem espaco apos o ultimo elemento: o primeiro
             * valor e impresso isoladamente e os demais sao precedidos
             * por um espaco. Como N >= 1, vcopia[0] sempre existe.
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
         * Comandos de busca: leem o valor P a ser buscado. As funcoes
         * retornam 1 se P esta no vetor e 0 caso contrario; a impressao
         * fica no main para nao interferir na medicao de tempo dos
         * algoritmos, feita nos codigos auxiliares.
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
            /* Encerramento: libera a memoria do vetor original */
            free(v);
            return 0;
        }
    } while (q != 5);

    free(v);

    return 0;
}
