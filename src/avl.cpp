#include <cstdio>
#include <cstdlib>
#include "avl.hpp"

// folha tem altura 0, NULL tem altura -1
struct NO_AVL {
    int info;
    int alt;
    struct NO_AVL *esq;
    struct NO_AVL *dir;
};

// funções auxiliares

static int alt_no(struct NO_AVL* no) {
    return no == NULL ? -1 : no->alt;
}

static int fb_no(struct NO_AVL* no) {
    return alt_no(no->esq) - alt_no(no->dir);
}

// se for = devolve x, porque tanto faz ser x ou y
static int maior(int x, int y) {
    return x >= y ? x : y;
}

// TODO: static void rotacaoLL(AVL* raiz) {}

// TODO: static void rotacaoRR(AVL* raiz) {}

// TODO: static void rotacaoLR(AVL* raiz) {}

// TODO: static void rotacaoRL(AVL* raiz) {}

// TODO: static void balanceia(AVL* raiz) {}

// TODO: static struct NO_AVL* procuraMenor(struct NO_AVL* no) {}

// TODO: static void imprime_no(struct NO_AVL* no, int nivel) {}

// funções públicas

AVL* avl_cria(void) {
    AVL *nova_avl = (AVL *) malloc(sizeof(AVL));
    if (nova_avl != NULL) // inicialmente é nula até atribuir uma raiz a ela
        *nova_avl = NULL;
    return nova_avl;
}

// TODO: int avl_insere(AVL* raiz, int valor) {}

// TODO: int avl_remove(AVL* raiz, int valor) {}

// TODO: int avl_consulta(AVL* raiz, int valor) {}

// TODO: void avl_emOrdem(AVL* raiz) {}

// TODO: void avl_imprime(AVL* raiz) {}

int avl_altura(AVL* raiz) {
    return raiz == NULL ? -1 : alt_no(*raiz);
}