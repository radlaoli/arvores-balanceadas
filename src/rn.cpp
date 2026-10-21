#include <cstdio>
#include <cstdlib>
#include "rn.hpp"

#define RED 1
#define BLACK 0

struct NO_RN {
    int info;
    struct NO_RN *esq;
    struct NO_RN *dir;
    struct NO_RN *pai;
    int cor;
};

static long totalRotacoes = 0;
static long totalRecoloracoes = 0;

// funções auxiliares

// NULL conta como preto
static int cor(struct NO_RN* H) {
    if (H == NULL)
        return BLACK;
    else
        return H->cor;
}

static void pinta(struct NO_RN* H, int novaCor) {
    if (H != NULL && H->cor != novaCor) {
        H->cor = novaCor;
        totalRecoloracoes++;
    }
}

// coloca "novo" no lugar de "antigo" (no pai ou na raiz)
static void trocaNoPai(RN* raiz, struct NO_RN* antigo, struct NO_RN* novo) {
    if (antigo->pai == NULL)
        *raiz = novo;
    else if (antigo == antigo->pai->esq)
        antigo->pai->esq = novo;
    else
        antigo->pai->dir = novo;
    if (novo != NULL)
        novo->pai = antigo->pai;
}

static void rotacaoEsquerda(RN* raiz, struct NO_RN* A) {
    struct NO_RN* B = A->dir;
    A->dir = B->esq;
    if (B->esq != NULL) B->esq->pai = A;
    trocaNoPai(raiz, A, B);
    B->esq = A;
    A->pai = B;
    totalRotacoes++;
}

static void rotacaoDireita(RN* raiz, struct NO_RN* B) {
    struct NO_RN* A = B->esq;
    B->esq = A->dir;
    if (A->dir != NULL) A->dir->pai = B;
    trocaNoPai(raiz, B, A);
    A->dir = B;
    B->pai = A;
    totalRotacoes++;
}

// Só há violação se o pai do nó novo também for vermelho.
// Tio vermelho -> troca de cores e o problema sobe para o avô.
// Tio preto -> rotação no avô (LL/RR) ou rotação dupla (LR/RL).
static void corrigeInsercao(RN* raiz, struct NO_RN* z) {
    while (cor(z->pai) == RED) {
        struct NO_RN* pai = z->pai;
        struct NO_RN* avo = pai->pai;

        if (pai == avo->esq) {
            struct NO_RN* tio = avo->dir;
            if (cor(tio) == RED) {
                pinta(pai, BLACK);
                pinta(tio, BLACK);
                pinta(avo, RED);
                z = avo;
            } else {
                if (z == pai->dir) {       // caso LR
                    rotacaoEsquerda(raiz, pai);
                    z = pai;
                    pai = z->pai;
                }
                pinta(pai, BLACK);         // caso LL
                pinta(avo, RED);
                rotacaoDireita(raiz, avo);
            }
        } else {
            struct NO_RN* tio = avo->esq;
            if (cor(tio) == RED) {
                pinta(pai, BLACK);
                pinta(tio, BLACK);
                pinta(avo, RED);
                z = avo;
            } else {
                if (z == pai->esq) {       // caso RL
                    rotacaoDireita(raiz, pai);
                    z = pai;
                    pai = z->pai;
                }
                pinta(pai, BLACK);         // caso RR
                pinta(avo, RED);
                rotacaoEsquerda(raiz, avo);
            }
        }
    }
    pinta(*raiz, BLACK);
}

// Chamada quando um nó preto é removido -> o caminho por x ficou com um preto a menos.
// x pode ser NULL, por isso o pai é passado separado. Os casos dependem do irmão w.
static void corrigeRemocao(RN* raiz, struct NO_RN* x, struct NO_RN* pai) {
    while (x != *raiz && cor(x) == BLACK) {
        if (x == pai->esq) {
            struct NO_RN* w = pai->dir;
            if (cor(w) == RED) {     // irmão vermelho
                pinta(w, BLACK);
                pinta(pai, RED);
                rotacaoEsquerda(raiz, pai);
                w = pai->dir;
            }
            if (cor(w->esq) == BLACK && cor(w->dir) == BLACK) {   // sobrinhos pretos
                pinta(w, RED);
                x = pai;
                pai = x->pai;
            } else {
                if (cor(w->dir) == BLACK) {   // sobrinho distante preto
                    pinta(w->esq, BLACK);
                    pinta(w, RED);
                    rotacaoDireita(raiz, w);
                    w = pai->dir;
                }
                pinta(w, pai->cor);   // sobrinho distante vermelho
                pinta(pai, BLACK);
                pinta(w->dir, BLACK);
                rotacaoEsquerda(raiz, pai);
                x = *raiz;
            }
        } else {
            struct NO_RN* w = pai->esq;
            if (cor(w) == RED) { // caso 1
                pinta(w, BLACK);
                pinta(pai, RED);
                rotacaoDireita(raiz, pai);
                w = pai->esq;
            }
            if (cor(w->dir) == BLACK && cor(w->esq) == BLACK) {   // caso 2
                pinta(w, RED);
                x = pai;
                pai = x->pai;
            } else {
                if (cor(w->esq) == BLACK) {  // caso 3
                    pinta(w->dir, BLACK);
                    pinta(w, RED);
                    rotacaoEsquerda(raiz, w);
                    w = pai->esq;
                }
                pinta(w, pai->cor);  // caso 4
                pinta(pai, BLACK);
                pinta(w->esq, BLACK);
                rotacaoDireita(raiz, pai);
                x = *raiz;
            }
        }
    }
    pinta(x, BLACK);
}

static struct NO_RN* procuraMenor(struct NO_RN* atual) {
    while (atual->esq != NULL)
        atual = atual->esq;
    return atual;
}

static struct NO_RN* buscaNo(RN* raiz, int valor) {
    if (raiz == NULL) return NULL;
    struct NO_RN* atual = *raiz;
    while (atual != NULL && atual->info != valor)
        atual = (valor < atual->info) ? atual->esq : atual->dir;
    return atual;
}

#define VERMELHO_ANSI "\033[1;31m"
#define RESET_ANSI    "\033[0m"

static void imprime_no(struct NO_RN* no, int nivel) {
    if (no == NULL) return;
    imprime_no(no->dir, nivel + 1);
    for (int i = 0; i < nivel; i++) printf("        ");
    if (no->cor == RED)
        printf(VERMELHO_ANSI "%d (R)" RESET_ANSI "\n", no->info);
    else
        printf("%d (B)\n", no->info);
    imprime_no(no->esq, nivel + 1);
}

static void emOrdem_no(struct NO_RN* no) {
    if (no == NULL) return;
    emOrdem_no(no->esq);
    printf("%d ", no->info);
    emOrdem_no(no->dir);
}

static void libera_no(struct NO_RN* no) {
    if (no == NULL) return;
    libera_no(no->esq);
    libera_no(no->dir);
    free(no);
}

static int altura_no(struct NO_RN* no) {
    if (no == NULL) return -1;
    int e = altura_no(no->esq);
    int d = altura_no(no->dir);
    return 1 + (e >= d ? e : d);
}

static int conta_no(struct NO_RN* no) {
    if (no == NULL) return 0;
    return 1 + conta_no(no->esq) + conta_no(no->dir);
}

// devolve a altura negra da subárvore, ou -1 se alguma propriedade foi violada
static int verifica_no(struct NO_RN* no, long long min, long long max) {
    if (no == NULL) return 0;
    if (no->info <= min || no->info >= max) return -1;
    if (no->esq != NULL && no->esq->pai != no) return -1;
    if (no->dir != NULL && no->dir->pai != no) return -1;
    if (no->cor == RED && (cor(no->esq) == RED || cor(no->dir) == RED)) return -1;

    int e = verifica_no(no->esq, min, no->info);
    int d = verifica_no(no->dir, no->info, max);
    if (e == -1 || d == -1 || e != d) return -1;
    return e + (no->cor == BLACK ? 1 : 0);
}

// nó invisível no lado sem filho, para a imagem mostrar se o filho é esquerdo ou direito
static void dot_no(FILE* f, struct NO_RN* no) {
    if (no == NULL) return;
    fprintf(f, "  n%d [label=\"%d\", fillcolor=%s];\n",
            no->info, no->info, no->cor == RED ? "red" : "black");
    if (no->esq == NULL && no->dir == NULL) return;

    if (no->esq != NULL)
        fprintf(f, "  n%d -> n%d;\n", no->info, no->esq->info);
    else
        fprintf(f, "  e%d [style=invis]; n%d -> e%d [style=invis];\n", no->info, no->info, no->info);

    if (no->dir != NULL)
        fprintf(f, "  n%d -> n%d;\n", no->info, no->dir->info);
    else
        fprintf(f, "  d%d [style=invis]; n%d -> d%d [style=invis];\n", no->info, no->info, no->info);

    dot_no(f, no->esq);
    dot_no(f, no->dir);
}

// funções publicas

RN* rn_cria(void) {
    RN* raiz = (RN*) malloc(sizeof(RN));
    if (raiz != NULL)
        *raiz = NULL;
    return raiz;
}

void rn_libera(RN* raiz) {
    if (raiz == NULL) return;
    libera_no(*raiz);
    free(raiz);
}

int rn_insere(RN* raiz, int valor) {
    if (raiz == NULL) return 0;

    struct NO_RN* pai = NULL;
    struct NO_RN* atual = *raiz;
    while (atual != NULL) {
        if (valor == atual->info) return 0;
        pai = atual;
        atual = (valor < atual->info) ? atual->esq : atual->dir;
    }

    struct NO_RN* novo = (struct NO_RN*) malloc(sizeof(struct NO_RN));
    if (novo == NULL) return 0;
    novo->info = valor;
    novo->esq = NULL;
    novo->dir = NULL;
    novo->pai = pai;
    novo->cor = RED;

    if (pai == NULL) *raiz = novo;
    else if (valor < pai->info) pai->esq = novo;
    else pai->dir = novo;

    corrigeInsercao(raiz, novo);
    return 1;
}

int rn_consulta(RN* raiz, int valor) {
    return buscaNo(raiz, valor) != NULL;
}

int rn_remove(RN* raiz, int valor) {
    struct NO_RN* z = buscaNo(raiz, valor);
    if (z == NULL) return 0;

    // 2 filhos -> copia o sucessor e remove ele (tem no máximo 1 filho)
    if (z->esq != NULL && z->dir != NULL) {
        struct NO_RN* sucessor = procuraMenor(z->dir);
        z->info = sucessor->info;
        z = sucessor;
    }

    struct NO_RN* filho = (z->esq != NULL) ? z->esq : z->dir;
    struct NO_RN* pai = z->pai;
    trocaNoPai(raiz, z, filho);

    // remover um vermelho não altera a altura negra
    if (z->cor == BLACK)
        corrigeRemocao(raiz, filho, pai);

    free(z);
    return 1;
}

void rn_emOrdem(RN* raiz) {
    if (raiz == NULL) return;
    emOrdem_no(*raiz);
}

void rn_imprime(RN* raiz) {
    if (raiz == NULL || *raiz == NULL) {
        printf("arvore vazia\n");
        return;
    }
    imprime_no(*raiz, 0);
}

int rn_altura(RN* raiz) {
    return raiz == NULL ? -1 : altura_no(*raiz);
}

int rn_alturaNegra(RN* raiz) {
    if (raiz == NULL) return 0;
    int pretos = 0;
    for (struct NO_RN* no = *raiz; no != NULL; no = no->esq)
        if (no->cor == BLACK) pretos++;
    return pretos;
}

int rn_exportaDot(RN* raiz, const char* arquivo) {
    FILE* f = fopen(arquivo, "w");
    if (f == NULL) return 0;
    fprintf(f, "digraph RN {\n");
    fprintf(f, "  node [shape=circle, style=filled, fontcolor=white, fontname=\"Helvetica\"];\n");
    if (raiz != NULL) dot_no(f, *raiz);
    fprintf(f, "}\n");
    fclose(f);
    return 1;
}

int rn_verifica(RN* raiz) {
    if (raiz == NULL || *raiz == NULL) return 1;
    if ((*raiz)->cor != BLACK || (*raiz)->pai != NULL) return 0;
    return verifica_no(*raiz, -2147483649LL, 2147483648LL) != -1;
}

int rn_totalNos(RN* raiz) {
    return raiz == NULL ? 0 : conta_no(*raiz);
}

int rn_numFilhos(RN* raiz, int valor) {
    struct NO_RN* no = buscaNo(raiz, valor);
    if (no == NULL) return -1;
    return (no->esq != NULL) + (no->dir != NULL);
}

void rn_zeraEstatisticas(void) {
    totalRotacoes = 0;
    totalRecoloracoes = 0;
}

long rn_rotacoes(void) {
    return totalRotacoes;
}

long rn_recoloracoes(void) {
    return totalRecoloracoes;
}