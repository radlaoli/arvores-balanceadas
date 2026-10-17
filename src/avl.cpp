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

// TODO: static int alt_no(struct NO_AVL* no) {}

// TODO: static int fb_no(struct NO_AVL* no) {}

// TODO: static int maior(int x, int y) {}

// TODO: static void rotacaoLL(AVL* raiz) {}

// TODO: static void rotacaoRR(AVL* raiz) {}

// TODO: static void rotacaoLR(AVL* raiz) {}

// TODO: static void rotacaoRL(AVL* raiz) {}

// TODO: static void balanceia(AVL* raiz) {}

// TODO: static struct NO_AVL* procuraMenor(struct NO_AVL* no) {}

// TODO: static void imprime_no(struct NO_AVL* no, int nivel) {}

// funções públicas

// TODO: AVL* avl_cria(void) {}

// TODO: int avl_insere(AVL* raiz, int valor) {}

// TODO: int avl_remove(AVL* raiz, int valor) {}

// TODO: int avl_consulta(AVL* raiz, int valor) {}

// TODO: void avl_emOrdem(AVL* raiz) {}

// TODO: void avl_imprime(AVL* raiz) {}

// TODO: int avl_altura(AVL* raiz) {}