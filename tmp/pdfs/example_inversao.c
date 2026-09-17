#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "../../src/algoritmos.h"

typedef struct { long long c, a; } Contagem;

static void preencher(int *v, int n) {
    for (int i = 0; i < n; ++i) v[i] = i;
}

static Contagem contar(int *v, int n) {
    Contagem op = {0, 2};
    int i = 0, j = n - 1, temp;
    for (;;) {
        op.c++;
        if (!(i < n / 2)) break;
        temp = v[i]; v[i] = v[j]; v[j] = temp;
        i++; j--;
        op.a += 5;
    }
    return op;
}

static int validar(int *v, int n, int invertido) {
    for (int i = 0; i < n; ++i)
        if (v[i] != (invertido ? n - 1 - i : i))
            return 0;
    return 1;
}

static double lote(int *v, int n, int r,
                   LARGE_INTEGER freq) {
    LARGE_INTEGER inicio, fim;
    if (!QueryPerformanceCounter(&inicio)) exit(2);
    for (int i = 0; i < r; ++i) inverte(v, n);
    if (!QueryPerformanceCounter(&fim)) exit(2);
    return (double)(fim.QuadPart - inicio.QuadPart)
           / (double)freq.QuadPart;
}

static void preparar(int *v, int n) {
    preencher(v, n);
    for (int i = 0; i < 20; ++i) inverte(v, n);
}

int main(void) {
    const int ns[] = {1000, 50000, 100000, 500000,
                      1000000};
    const int M = 10;
    LARGE_INTEGER freq;
    if (!QueryPerformanceFrequency(&freq) ||
        freq.QuadPart <= 0) return 2;
    FILE *bruto = fopen("data/exemplo_rodadas.csv", "w");
    FILE *res = fopen("data/exemplo_resumo.csv", "w");
    if (!bruto || !res) {
        if (bruto) fclose(bruto);
        if (res) fclose(res);
        return 3;
    }
    fprintf(bruto, "algoritmo,n,rodada,repeticoes,"
                   "tempo_total_s\n");
    fprintf(res, "algoritmo,n,rodadas,repeticoes,media_s,"
                 "dp_s,comparacoes,atribuicoes\n");

    for (int t = 0; t < 5; ++t) {
        int n = ns[t], R = 100;
        int *v = malloc((size_t)n * sizeof *v);
        if (!v) return 4;
        preencher(v, n);
        inverte(v, n);
        if (!validar(v, n, 1)) return 5;
        preencher(v, n);
        Contagem op = contar(v, n);
        long long m = n / 2;
        if (!validar(v, n, 1) || op.c != m + 1 ||
            op.a != 5 * m + 2) return 6;
        for (;;) {
            preparar(v, n);
            double b = lote(v, n, R, freq);
            if (!validar(v, n, R % 2)) return 7;
            if (b >= 0.05) break;
            if (R > INT_MAX / 2) return 8;
            R *= 2;
        }
        double tempos[10], media = 0.0;
        for (int r = 0; r < M; ++r) {
            preparar(v, n);
            double b = lote(v, n, R, freq);
            if (!validar(v, n, R % 2)) return 7;
            tempos[r] = b / R;
            media += tempos[r];
            fprintf(bruto, "inversao,%d,%d,%d,%.12e\n",
                    n, r + 1, R, b);
        }
        media /= M;

        double soma = 0.0;
        for (int r = 0; r < M; ++r) {
            double d = tempos[r] - media;
            soma += d * d;
        }
        double dp = sqrt(soma / (M - 1));
        fprintf(res, "inversao,%d,%d,%d,%.12e,%.12e,"
                     "%lld,%lld\n",
                n, M, R, media, dp, op.c, op.a);
        free(v);
    }
    int erro = ferror(bruto) || ferror(res);
    if (fclose(bruto) != 0) erro = 1;
    if (fclose(res) != 0) erro = 1;
    return erro ? 10 : 0;
}