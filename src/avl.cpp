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

static void atualizaAltura(struct NO_AVL* no) {
    no->alt = 1 + maior(alt_no(no->esq), alt_no(no->dir));
}

// o filho esquerdo B sobe
static void rotacaoLL(AVL* raiz) {
    struct NO_AVL* A = *raiz;
    struct NO_AVL* B = A->esq;
    A->esq = B->dir;      // a subárvore direita de B passa para A
    B->dir = A;           // A desce para a direita de B
    //primeiro atualiza o que desceu, depois o que subiu
    atualizaAltura(A);
    atualizaAltura(B);    
    *raiz = B;
}

static void rotacaoRR(AVL* raiz) {
    struct NO_AVL* A = *raiz;
    struct NO_AVL* B = A->dir;
    A->dir = B->esq;
    B->esq = A;
    atualizaAltura(A);
    atualizaAltura(B);
    *raiz = B;
}

// Jas rotações duplas é só reaproveitar as simples
static void rotacaoLR(AVL* raiz) {
    rotacaoRR(&(*raiz)->esq);
    rotacaoLL(raiz);
}

static void rotacaoRL(AVL* raiz) {
    rotacaoLL(&(*raiz)->dir);
    rotacaoRR(raiz); 
}

// Escolhe a rotação
// sinais iguais -> simples, sinais diferentes -> dupla
static void balanceia(AVL* raiz) {
    struct NO_AVL* no = *raiz;
    int fb = fb_no(no);

    if (fb == 2) {                         // pesada à esquerda
        if (fb_no(no->esq) >= 0) rotacaoLL(raiz);
        else rotacaoLR(raiz);
    } else if (fb == -2) {                 // pesada à direita
        if (fb_no(no->dir) <= 0) rotacaoRR(raiz);
        else rotacaoRL(raiz);
    }
}

static struct NO_AVL* procuraMenor(struct NO_AVL* no) {
    while (no->esq != NULL)
        no = no->esq;     
    return no;
}

// a impressão da arvore deitada
static void imprime_no(struct NO_AVL* no, int nivel) {
    if (no == NULL) return;
    imprime_no(no->dir, nivel + 1);
    for (int i = 0; i < nivel; i++) printf("        ");
    printf("%d [h=%d fb=%d]\n", no->info, no->alt, fb_no(no));
    imprime_no(no->esq, nivel + 1);
}

// funções públicas

AVL* avl_cria(void) {
    AVL *nova_avl = (AVL *) malloc(sizeof(AVL));
    if (nova_avl != NULL) // inicialmente é nula até atribuir uma raiz a ela
        *nova_avl = NULL;
    return nova_avl;
}

// TODO: int avl_insere(AVL* raiz, int valor) {}

// TODO: int avl_remove(AVL* raiz, int valor) {}

int avl_consulta(AVL* raiz, int valor) {
    if (raiz == NULL) return 0;

    struct NO_AVL* atual = *raiz;

    while (atual != NULL && atual->info != valor)
    {
        if (valor < atual->info) atual = atual->esq;
        else atual = atual->dir; 
    }

    return atual == NULL ? 0 : 1;
}

// começa a imprimir da folha mais a esquerda e termina na folha mais a direita
void avl_emOrdem(AVL* raiz) {
    if (raiz == NULL || *raiz == NULL) return;

    struct NO_AVL * no = *raiz;
    avl_emOrdem(&no->esq);
    printf("%d ", no->info);
    avl_emOrdem(&no->dir);
}

// TODO: void avl_imprime(AVL* raiz) {}

int avl_altura(AVL* raiz) {
    return raiz == NULL ? -1 : alt_no(*raiz);
}