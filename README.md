# Teorema Mestre — análise experimental

Implementações em C11 de algoritmos de divisão e conquista, usados para comparar a análise pelo Teorema Mestre com tempos medidos.

## Estrutura

- `Relatorio_Teorema_Mestre.pdf` — relatório final
- `src/` — código em C (`algoritmos.c/.h`, `experimento.c`, `testes.c`, `contagem.c`)
- `dados/` — resultados: `experimentos.csv` (medições), `resumo.csv` (resumo por caso e tamanho), `mac_info.txt` (máquina usada)

## Como compilar e rodar

```bash
clang -std=c11 -O2 -Wall -Wextra -pedantic src/testes.c src/algoritmos.c -o testes && ./testes
clang -std=c11 -O2 -Wall -Wextra -pedantic src/experimento.c src/algoritmos.c -o experimento
./experimento experimentos.csv 1024 5
```

Os tempos valem apenas para a máquina descrita em `dados/mac_info.txt` (MacBook, Apple M4, macOS).
