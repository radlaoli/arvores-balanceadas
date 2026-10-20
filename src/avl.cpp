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

// contador de rotações simples
static long totalRotacoes = 0;

// funções auxiliares

static int alt_no(struct NO_AVL* no) {
    return no == NULL ? -1 : no->alt;
}

// positivo = pesada à esquerda, negativo = pesada à direita
static int fb_no(struct NO_AVL* no) {
    return alt_no(no->esq) - alt_no(no->dir);
}

// se for = devolve x, porque tanto faz ser x ou y
static int maior(int x, int y) {
    return x >= y ? x : y;
}

// recalcula a altura do nó a partir das alturas dos filhos
static void atualizaAltura(struct NO_AVL* no) {
    no->alt = 1 + maior(alt_no(no->esq), alt_no(no->dir));
}

static void rotacaoLL(AVL* raiz) {
    struct NO_AVL* A = *raiz;
    struct NO_AVL* B = A->esq;
    A->esq = B->dir;      // a subárvore direita de B passa para A
    B->dir = A;           // A desce para a direita de B
    // primeiro atualiza o que desceu, depois o que subiu
    atualizaAltura(A);
    atualizaAltura(B);
    *raiz = B;
    totalRotacoes++;
}

static void rotacaoRR(AVL* raiz) {
    struct NO_AVL* A = *raiz;
    struct NO_AVL* B = A->dir;
    A->dir = B->esq;
    B->esq = A;
    atualizaAltura(A);
    atualizaAltura(B);
    *raiz = B;
    totalRotacoes++;
}

// Nas rotações duplas é só reaproveitar as simples
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

// menor nó de uma subárvore -> desce sempre para a esquerda
static struct NO_AVL* procuraMenor(struct NO_AVL* no) {
    while (no->esq != NULL)
        no = no->esq;
    return no;
}

// a impressão da árvore deitada
static void imprime_no(struct NO_AVL* no, int nivel) {
    if (no == NULL) return;
    imprime_no(no->dir, nivel + 1);
    for (int i = 0; i < nivel; i++) printf("        ");
    printf("%d [h=%d fb=%d]\n", no->info, no->alt, fb_no(no));
    imprime_no(no->esq, nivel + 1);
}

// libera os filhos antes do pai
static void libera_no(struct NO_AVL* no) {
    if (no == NULL) return;
    libera_no(no->esq);
    libera_no(no->dir);
    free(no);
}

static int conta_no(struct NO_AVL* no) {
    if (no == NULL) return 0;
    return 1 + conta_no(no->esq) + conta_no(no->dir);
}

// confere recursivamente, altura guardada correta e |fb| <= 1
// min e max são os limites que os valores desta subárvore precisam respeitar
static int verifica_no(struct NO_AVL* no, long long min, long long max) {
    if (no == NULL) return 1;
    if (no->info <= min || no->info >= max) return 0;  
    if (no->alt != 1 + maior(alt_no(no->esq), alt_no(no->dir))) return 0;  // altura
    int fb = fb_no(no);
    if (fb < -1 || fb > 1) return 0;                                       // balanceamento
    return verifica_no(no->esq, min, no->info) && verifica_no(no->dir, no->info, max);
}

// escreve cada nó e as setas para os filhos no formato do Graphviz
// quando o nó tem só um filho, um nó invisível ocupa o outro lado,
// para a imagem mostrar se o filho está à esquerda ou à direita
static void dot_no(FILE* f, struct NO_AVL* no) {
    if (no == NULL) return;
    fprintf(f, "  n%d [label=\"%d\\nfb=%d\"];\n", no->info, no->info, fb_no(no));
    if (no->esq == NULL && no->dir == NULL) return;

    if (no->esq != NULL) fprintf(f, "  n%d -> n%d;\n", no->info, no->esq->info);
    else fprintf(f, "  e%d [style=invis]; n%d -> e%d [style=invis];\n", no->info, no->info, no->info);

    if (no->dir != NULL) fprintf(f, "  n%d -> n%d;\n", no->info, no->dir->info);
    else fprintf(f, "  d%d [style=invis]; n%d -> d%d [style=invis];\n", no->info, no->info, no->info);

    dot_no(f, no->esq);
    dot_no(f, no->dir);
}

// funções públicas

AVL* avl_cria(void) {
    AVL *nova_avl = (AVL *) malloc(sizeof(AVL));
    if (nova_avl != NULL) // inicialmente é nula até atribuir uma raiz a ela
        *nova_avl = NULL;
    return nova_avl;
}

void avl_libera(AVL* raiz) {
    if (raiz == NULL) return;
    libera_no(*raiz);
    free(raiz);
}

// insere e rebalanceia na volta da recursão
int avl_insere(AVL* raiz, int valor) {
    if (raiz == NULL) return 0;

    if (*raiz == NULL) { // cria o nó no lugar correto
        struct NO_AVL* novo = (struct NO_AVL*) malloc(sizeof(struct NO_AVL));
        if (novo == NULL) return 0;
        novo->info = valor;
        novo->alt = 0; // nó novo é folha
        novo->esq = NULL;
        novo->dir = NULL;
        *raiz = novo;
        return 1;
    }

    struct NO_AVL* no = *raiz;
    int res;
    if (valor == no->info) return 0; // se for repetido não insere
    else if (valor < no->info) res = avl_insere(&no->esq, valor);
    else res = avl_insere(&no->dir, valor);

    if (res) { // corrige altura e balanceia
        atualizaAltura(*raiz);
        balanceia(raiz);
    }
    return res;
}

// remove o valor e rebalanceia na volta da recursão
int avl_remove(AVL* raiz, int valor) {
    if (raiz == NULL || *raiz == NULL) return 0; // não existe

    struct NO_AVL* no = *raiz;
    int res;

    if (valor < no->info) res = avl_remove(&no->esq, valor);
    else if (valor > no->info) res = avl_remove(&no->dir, valor);
    else {
        if (no->esq == NULL || no->dir == NULL) {  // 0 ou 1 filho
            // o filho que existir (ou NULL) ocupa o lugar do nó
            *raiz = (no->esq != NULL) ? no->esq : no->dir;
            free(no);
            return 1;
        }
        // quando tem 2 filhos copia o sucessor e remove ele da direita
        struct NO_AVL* sucessor = procuraMenor(no->dir);
        no->info = sucessor->info;
        res = avl_remove(&no->dir, sucessor->info);
    }

    if (res && *raiz != NULL) {
        atualizaAltura(*raiz);
        balanceia(raiz);
    }
    return res;
}

int avl_consulta(AVL* raiz, int valor) {
    if (raiz == NULL) return 0;

    struct NO_AVL* atual = *raiz;

    while (atual != NULL && atual->info != valor) {
        if (valor < atual->info) atual = atual->esq;
        else atual = atual->dir;
    }

    return atual == NULL ? 0 : 1;
}

// começa a imprimir da folha mais à esquerda e termina na folha mais à direita
void avl_emOrdem(AVL* raiz) {
    if (raiz == NULL || *raiz == NULL) return;

    struct NO_AVL* no = *raiz;
    avl_emOrdem(&no->esq);
    printf("%d ", no->info);
    avl_emOrdem(&no->dir);
}

void avl_imprime(AVL* raiz) {
    if (raiz == NULL || *raiz == NULL) {
        printf("arvore vazia\n");
        return;
    }
    imprime_no(*raiz, 0);
}

int avl_altura(AVL* raiz) {
    return raiz == NULL ? -1 : alt_no(*raiz);
}

// gera um arquivo .dot para o Graphviz desenhar a árvore
int avl_exportaDot(AVL* raiz, const char* arquivo) {
    FILE* f = fopen(arquivo, "w");
    if (f == NULL) return 0;
    fprintf(f, "digraph AVL {\n  node [shape=circle, fontname=\"Helvetica\"];\n");
    if (raiz != NULL) dot_no(f, *raiz);
    fprintf(f, "}\n");
    fclose(f);
    return 1;
}

int avl_verifica(AVL* raiz) {
    if (raiz == NULL) return 1;
    return verifica_no(*raiz, -2147483649LL, 2147483648LL);
}

int avl_totalNos(AVL* raiz) {
    return raiz == NULL ? 0 : conta_no(*raiz);
}

// quantos filhos o nó com esse valor tem (usado para testar os 3 casos de remoção)
int avl_numFilhos(AVL* raiz, int valor) {
    if (raiz == NULL) return -1;
    struct NO_AVL* atual = *raiz;
    while (atual != NULL && atual->info != valor)
        atual = (valor < atual->info) ? atual->esq : atual->dir;
    if (atual == NULL) return -1;
    return (atual->esq != NULL) + (atual->dir != NULL);
}

void avl_zeraEstatisticas(void) {
    totalRotacoes = 0;
}

long avl_rotacoes(void) {
    return totalRotacoes;
}
