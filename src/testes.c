#include "algoritmos.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
int main(void) {
    int total = 0;
    /* Maximo em cada quadrante, e matriz com todos os valores iguais. */
    for (int n = 1; n <= 64; n *= 2) {
        int64_t *matriz = malloc((size_t)n * n * sizeof(*matriz)); assert(matriz);
        for (int tipo = 0; tipo < 6; tipo++) {
            for (int i = 0; i < n * n; i++) matriz[i] = -20;
            int posicao = tipo == 0 ? 0 : tipo == 1 ? n - 1 :
                tipo == 2 ? n * (n - 1) : tipo == 3 ? n * n - 1 : n * n / 2;
            if (tipo < 5) matriz[posicao] = -3;
            assert(maximo_matriz(matriz, n, 0, 0, n) == (tipo < 5 ? -3 : -20)); total++;
        }
        free(matriz);
    }
    for (int n = 1; n <= 729; n *= 3) for (int tipo = 0; tipo < 4; tipo++) {
        int64_t *v = malloc((size_t)n * sizeof(*v)); assert(v);
        int64_t soma = 0;
        for (int i = 0; i < n; i++) {
            v[i] = tipo == 0 ? 0 : tipo == 1 ? -7 : tipo == 2 ? i : i % 21 - 10;
            soma += v[i];
        }
        assert(somar_vetor(v, n) == soma); total++; free(v);
    }
    for (int n = 1; n <= 128; n *= 2) for (int tipo = 0; tipo < 4; tipo++) {
        int64_t *v = malloc((size_t)n * sizeof(*v)); assert(v);
        int64_t esperado = 0;
        for (int i = 0; i < n; i++) v[i] = tipo == 0 ? 0 : tipo == 1 ? -5 : tipo == 2 ? i : i % 21 - 10;
        for (int i = 0; i < n; i++) for (int j = i + 1; j < n; j++) {
            int64_t d = v[i] - v[j]; esperado += d * d;
        }
        assert(somar_distancias(v, n) == esperado); total++; free(v);
    }
    printf("%d verificacoes aprovadas.\n", total);
    return 0;
}
