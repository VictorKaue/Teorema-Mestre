/* As tres solucoes recursivas do trabalho. O driver esta em experimento.c.
 * Em cada funcao: a = numero de chamadas, b = fator de reducao do tamanho,
 * f(n) = trabalho fora das chamadas recursivas (veja a secao 4 do relatorio). */
#include "algoritmos.h"
#include <stdio.h>
#include <stdlib.h>

/* CASO 1: a = 4, b = 2, f(n) = Theta(1).
 * T(n) = 4T(n/2) + Theta(1) = Theta(n^2); n e o lado, nao o total de elementos.
 * Base: n = 1 devolve o unico elemento. Passo: o maximo da matriz e o maior dos
 * maximos dos quatro quadrantes (cobrem a matriz sem sobreposicao); a combinacao
 * custa tres comparacoes. Os quadrantes sao acessados por indices, sem copias. */
int64_t maximo_matriz(const int64_t *matriz, int largura, int linha, int coluna, int n) {
    if (n == 1) return matriz[(size_t)linha * largura + coluna];
    int metade = n / 2;
    int64_t maior = maximo_matriz(matriz, largura, linha, coluna, metade);
    int64_t valor = maximo_matriz(matriz, largura, linha, coluna + metade, metade);
    if (valor > maior) maior = valor;
    valor = maximo_matriz(matriz, largura, linha + metade, coluna, metade);
    if (valor > maior) maior = valor;
    valor = maximo_matriz(matriz, largura, linha + metade, coluna + metade, metade);
    if (valor > maior) maior = valor;
    return maior;
}

/* CASO 2: a = 3, b = 3, f(n) = Theta(n) (copia dos n elementos).
 * T(n) = 3T(n/3) + Theta(n) = Theta(n log n).
 * Base: n = 1 devolve o unico elemento. Passo: copia cada terco para um vetor
 * proprio, soma os resultados das tres chamadas e libera os vetores.
 * Uma soma comum seria linear (3T(n/3) + Theta(1)); esta versao didatica inclui
 * copias reais para ilustrar o Caso 2. Espaco auxiliar: O(n) de vetores + O(log n) de pilha. */
int64_t somar_vetor(const int64_t *vetor, int n) {
    if (n == 1) return vetor[0];
    int tamanho = n / 3;
    int64_t *parte1 = malloc((size_t)tamanho * sizeof(*parte1));
    int64_t *parte2 = malloc((size_t)tamanho * sizeof(*parte2));
    int64_t *parte3 = malloc((size_t)tamanho * sizeof(*parte3));
    if (!parte1 || !parte2 || !parte3) {
        fprintf(stderr, "Memoria insuficiente.\n");
        free(parte1); free(parte2); free(parte3);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < tamanho; i++) {
        parte1[i] = vetor[i];
        parte2[i] = vetor[i + tamanho];
        parte3[i] = vetor[i + 2 * tamanho];
    }
    int64_t soma = somar_vetor(parte1, tamanho);
    soma += somar_vetor(parte2, tamanho);
    soma += somar_vetor(parte3, tamanho);
    free(parte1); free(parte2); free(parte3);
    return soma;
}

/* CASO 3: a = 2, b = 2, f(n) = Theta(n^2) (pares cruzados entre as metades).
 * T(n) = 2T(n/2) + Theta(n^2) = Theta(n^2).
 * Base: n <= 1 nao tem pares. Passo: pares internos de cada metade (duas
 * chamadas) mais todos os pares (i, j) com i na primeira e j na segunda metade;
 * assim cada par e contado exatamente uma vez. Pilha: O(log n). */
int64_t somar_distancias(const int64_t *vetor, int n) {
    if (n <= 1) return 0;
    int metade = n / 2;
    int64_t soma = somar_distancias(vetor, metade);
    soma += somar_distancias(vetor + metade, n - metade);
    for (int i = 0; i < metade; i++) {
        for (int j = metade; j < n; j++) {
            int64_t diferenca = vetor[i] - vetor[j];
            soma += diferenca * diferenca;
        }
    }
    return soma;
}
