#ifndef ALGORITMOS_H
#define ALGORITMOS_H
#include <stdint.h>

/* Problema 1 (Caso 1): maior valor de uma matriz quadrada.
 * Recorrencia T(n) = 4T(n/2) + Theta(1) = Theta(n^2), n = lado da matriz.
 * matriz : vetor com a matriz original, linha a linha (largura x largura).
 * largura: lado da matriz original; linha, coluna: canto do bloco atual.
 * n      : lado do bloco atual. PRE-CONDICAO: n potencia de 2. */
int64_t maximo_matriz(const int64_t *matriz, int largura, int linha, int coluna, int n);

/* Problema 2 (Caso 2): soma de um vetor com copias explicitas dos tercos.
 * Recorrencia T(n) = 3T(n/3) + Theta(n) = Theta(n log n).
 * PRE-CONDICAO: n potencia de 3. As copias fazem parte do algoritmo. */
int64_t somar_vetor(const int64_t *vetor, int n);

/* Problema 3 (Caso 3): soma de (vetor[i]-vetor[j])^2 para todos os pares i<j.
 * Recorrencia T(n) = 2T(n/2) + Theta(n^2) = Theta(n^2).
 * Os testes e as medicoes usam n potencia de 2 (a analise supoe divisoes exatas). */
int64_t somar_distancias(const int64_t *vetor, int n);

#endif
