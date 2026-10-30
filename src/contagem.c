/* Contagem instrumentada: confere as formulas analiticas do CSV.
 * Copia dos tres algoritmos com contadores (comparacoes, copias, pares, chamadas).
 * Nao participa das medicoes de tempo. Compilar: gcc -std=c11 -O1 src/contagem.c -o contagem */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
static long comparacoes, copias, pares, chamadas;

static int64_t maximo(const int64_t *m, int w, int l, int c, int n) {
    chamadas++;
    if (n == 1) return m[(size_t)l * w + c];
    int h = n / 2;
    int64_t a = maximo(m, w, l, c, h), v = maximo(m, w, l, c + h, h);
    comparacoes++; if (v > a) a = v;
    v = maximo(m, w, l + h, c, h); comparacoes++; if (v > a) a = v;
    v = maximo(m, w, l + h, c + h, h); comparacoes++; if (v > a) a = v;
    return a;
}
static int64_t somar(const int64_t *v, int n) {
    chamadas++;
    if (n == 1) return v[0];
    int t = n / 3;
    int64_t *p1 = malloc(t * sizeof *p1), *p2 = malloc(t * sizeof *p2), *p3 = malloc(t * sizeof *p3);
    for (int i = 0; i < t; i++) { p1[i] = v[i]; p2[i] = v[i + t]; p3[i] = v[i + 2 * t]; copias += 3; }
    int64_t s = somar(p1, t); s += somar(p2, t); s += somar(p3, t);
    free(p1); free(p2); free(p3);
    return s;
}
static int64_t distancias(const int64_t *v, int n) {
    chamadas++;
    if (n <= 1) return 0;
    int h = n / 2;
    int64_t s = distancias(v, h); s += distancias(v + h, n - h);
    for (int i = 0; i < h; i++) for (int j = h; j < n; j++) {
        int64_t d = v[i] - v[j]; s += d * d; pares++;
    }
    return s;
}
int main(void) {
    int64_t *dados = calloc(1 << 20, sizeof *dados);
    int erros = 0;
    printf("Caso 1: n, comparacoes, n^2-1, chamadas, (4n^2-1)/3\n");
    for (int n = 2; n <= 1024; n *= 2) {
        comparacoes = chamadas = 0; maximo(dados, n, 0, 0, n);
        long esperado = (long)n * n - 1, calls = ((long)n * n * 4 - 1) / 3;
        printf("%5d %9ld %9ld %9ld %9ld\n", n, comparacoes, esperado, chamadas, calls);
        if (comparacoes != esperado || chamadas != calls) erros++;
    }
    printf("Caso 2: n, copias, n*log3(n), chamadas, (3n-1)/2\n");
    for (int n = 3, L = 1; n <= 729; n *= 3, L++) {
        copias = chamadas = 0; somar(dados, n);
        printf("%5d %9ld %9d %9ld %9d\n", n, copias, n * L, chamadas, (3 * n - 1) / 2);
        if (copias != (long)n * L || chamadas != (3 * n - 1) / 2) erros++;
    }
    printf("Caso 3: n, pares, n(n-1)/2\n");
    for (int n = 2; n <= 1024; n *= 2) {
        pares = 0; distancias(dados, n);
        printf("%5d %9ld %9ld\n", n, pares, (long)n * (n - 1) / 2);
        if (pares != (long)n * (n - 1) / 2) erros++;
    }
    printf(erros ? "DIVERGENCIAS: %d\n" : "Todas as contagens conferem.\n", erros);
    free(dados);
    return erros != 0;
}
