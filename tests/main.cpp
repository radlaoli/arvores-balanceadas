#include <cstdio>
#include "../include/avl.hpp"

int main() {
    AVL* avl = avl_cria();

    printf("altura: %d\n", avl_altura(avl)); 
    printf("consulta 10: %d\n", avl_consulta(avl, 10));
    avl_emOrdem(avl); 
    printf("fim\n");

    return 0;
}