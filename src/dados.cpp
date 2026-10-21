#include <cstdio>
#include <cstdlib>
#include "dados.hpp"

int carrega_arquivo(const char* caminho, int** valores) {
    *valores = NULL;
    FILE* f = fopen(caminho, "r");
    if (f == NULL) return -1;

    int capacidade = 1024, total = 0, x;
    int* v = (int*) malloc(capacidade * sizeof(int));
    if (v == NULL) { fclose(f); return -1; }

    while (fscanf(f, "%d", &x) == 1) {
        if (total == capacidade) {             // vetor cheio -> dobra o tamanho
            capacidade *= 2;
            int* novo = (int*) realloc(v, capacidade * sizeof(int));
            if (novo == NULL) { free(v); fclose(f); return -1; }
            v = novo;
        }
        v[total++] = x;
    }
    fclose(f);
    *valores = v;
    return total;
}
