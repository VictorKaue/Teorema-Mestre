/* Driver: gera entradas, valida resultados e mede os tres algoritmos.
 * Compilar com src/algoritmos.c. Comandos no Apêndice C do relatório.
 * ./experimento [saida.csv] [n_max] [amostras]
 * n_max: 16 a 4096. amostras: 1 a 30. O CSV e sobrescrito.
 * Relogio: QueryPerformanceCounter no Windows; clock_gettime(CLOCK_MONOTONIC)
 * em macOS/Linux (tempo decorrido, como no Windows).
 */
#ifndef _WIN32
#define _POSIX_C_SOURCE 199309L
#endif
#include "algoritmos.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#endif
static volatile int64_t resultado_observavel;
static double agora(void) {
#ifdef _WIN32
    LARGE_INTEGER t, f;
    QueryPerformanceCounter(&t); QueryPerformanceFrequency(&f);
    return (double)t.QuadPart / (double)f.QuadPart;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
#endif
}
static uint32_t estado = 20261028u;
static int64_t aleatorio(void) {
    estado = 1664525u * estado + 1013904223u;
    return (int64_t)(estado % 21u) - 10;
}
static int parametro(const char *texto, int minimo, int maximo) {
    char *fim;
    long valor = strtol(texto, &fim, 10);
    if (!*texto || *fim || valor < minimo || valor > maximo) {
        fprintf(stderr, "Argumento invalido: %s\n", texto);
        exit(EXIT_FAILURE);
    }
    return (int)valor;
}
static int64_t executar(int caso, const int64_t *dados, int n) {
    if (caso == 1) return maximo_matriz(dados, n, 0, 0, n);
    if (caso == 2) return somar_vetor(dados, n);
    return somar_distancias(dados, n);
}
/* Referencias iterativas, fora da regiao cronometrada. */
static int64_t referencia(int caso, const int64_t *dados, int n) {
    int64_t resposta = 0;
    if (caso == 1) {
        resposta = dados[0];
        for (size_t i = 1; i < (size_t)n * n; i++)
            if (dados[i] > resposta) resposta = dados[i];
    } else if (caso == 2) {
        for (int i = 0; i < n; i++) resposta += dados[i];
    } else {
        for (int i = 0; i < n; i++) for (int j = i + 1; j < n; j++) {
            int64_t d = dados[i] - dados[j]; resposta += d * d;
        }
    }
    return resposta;
}
int main(int argc, char **argv) {
    int limite = argc > 2 ? parametro(argv[2], 16, 4096) : 1024;
    int amostras = argc > 3 ? parametro(argv[3], 1, 30) : 5;
    FILE *csv = fopen(argc > 1 ? argv[1] : "results/experimentos.csv", "w");
    if (!csv) { perror("CSV"); return 1; }
    fprintf(csv, "caso,n,amostra,repeticoes,tempo_ms,metrica_exata,referencia_assintotica,validado\n");
    for (int caso = 1; caso <= 3; caso++) {
        int fator = caso == 2 ? 3 : 2;
        for (int n = caso == 2 ? 9 : 16; n <= limite; n *= fator) {
            size_t quantidade = caso == 1 ? (size_t)n * n : (size_t)n;
            int64_t *dados = malloc(quantidade * sizeof(*dados));
            if (!dados) { perror("malloc"); fclose(csv); return 1; }
            for (size_t i = 0; i < quantidade; i++) dados[i] = aleatorio();
            if (executar(caso, dados, n) != referencia(caso, dados, n)) {
                fprintf(stderr, "Falha: caso %d n=%d\n", caso, n);
                free(dados); fclose(csv); return 2;
            }
            /* Contagens ANALITICAS: comparacoes, copias ou pares. */
            double teoria = caso == 2 ? n * log(n) / log(3) : (double)n * n;
            double metrica = caso == 1 ? (double)n * n - 1 :
                caso == 2 ? n * round(log(n) / log(3)) : (double)n * (n - 1) / 2;
            for (int amostra = 1; amostra <= amostras; amostra++) {
                unsigned repeticoes = 1;
                double duracao;
                while (1) {
                    double inicio = agora();
                    for (unsigned r = 0; r < repeticoes; r++)
                        resultado_observavel = executar(caso, dados, n);
                    duracao = agora() - inicio;
                    if (duracao >= 0.025 || repeticoes >= 1048576u) break;
                    repeticoes *= 2;
                }
                fprintf(csv, "%d,%d,%d,%u,%.9f,%.0f,%.9f,1\n", caso, n,
                    amostra, repeticoes, 1000 * duracao / repeticoes, metrica, teoria);
                fflush(csv);
            }
            printf("Caso %d n=%d: validado\n", caso, n);
            free(dados);
        }
    }
    if (fclose(csv)) { perror("CSV"); return 1; }
    return 0;
}
