#include <cstdio>
#include "avl.hpp"

int main() {
    int casos[4][3] = {{30,20,10}, {10,20,30}, {30,10,20}, {10,30,20}};
    const char* nomes[4] = {"LL", "RR", "LR", "RL"};

    for (int c = 0; c < 4; c++) {
        AVL* a = avl_cria();
        for (int i = 0; i < 3; i++) avl_insere(a, casos[c][i]);
        printf("Caso %s:\n", nomes[c]);
        avl_imprime(a);
        printf("\n");
    }

    AVL* a = avl_cria();
    for (int i = 1; i <= 1000; i++) avl_insere(a, i);
    printf("1000 crescentes -> altura %d\n", avl_altura(a));
    printf("repetido 500 -> %d\n", avl_insere(a, 500));
    printf("remove 500 -> %d\n", avl_remove(a, 500));
    printf("remove 500 de novo -> %d\n", avl_remove(a, 500));
    printf("consulta 500 -> %d\n", avl_consulta(a, 500));
    return 0;
}