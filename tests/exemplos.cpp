#include <cstdio>
#include <filesystem>
#include "avl.hpp"
#include "rn.hpp"
#include "imagem.hpp"

static void salvaAVL(AVL* a, const char* nome) {
    char dot[128], png[128];
    snprintf(dot, sizeof(dot), "docs/img/%s.dot", nome);
    snprintf(png, sizeof(png), "docs/img/%s.png", nome);
    avl_exportaDot(a, dot);
    gera_imagem(dot, png);
}

static void salvaRN(RN* r, const char* nome) {
    char dot[128], png[128];
    snprintf(dot, sizeof(dot), "docs/img/%s.dot", nome);
    snprintf(png, sizeof(png), "docs/img/%s.png", nome);
    rn_exportaDot(r, dot);
    gera_imagem(dot, png);
}

// árvore antes de inserir o valor que desbalanceia e depois da rotação
static void exemploRotacaoAVL(const char* caso, int a, int b, int c) {
    AVL* t = avl_cria();
    avl_insere(t, a);
    avl_insere(t, b);
    char nome[64];
    snprintf(nome, sizeof(nome), "avl_%s_antes", caso);
    salvaAVL(t, nome);
    avl_insere(t, c);
    snprintf(nome, sizeof(nome), "avl_%s_depois", caso);
    salvaAVL(t, nome);
    avl_libera(t);
}

int main() {
    std::error_code erro;
    std::filesystem::create_directories("docs/img", erro);

    // as 4 rotações da AVL
    exemploRotacaoAVL("LL", 30, 20, 10);
    exemploRotacaoAVL("RR", 10, 20, 30);
    exemploRotacaoAVL("LR", 30, 10, 20);
    exemploRotacaoAVL("RL", 10, 30, 20);

    // a sequência de inserções
    int seq[] = {10, 20, 30, 25, 5, 1, 2};
    RN* r = rn_cria();
    AVL* a = avl_cria();
    for (int i = 0; i < 7; i++) {
        rn_insere(r, seq[i]);
        avl_insere(a, seq[i]);
        char nome[64];
        snprintf(nome, sizeof(nome), "rn_passo%d_insere_%d", i + 1, seq[i]);
        salvaRN(r, nome);
    }
    salvaAVL(a, "avl_sequencia_de_insersoes");

    // remoção na RN
    rn_remove(r, 20);
    salvaRN(r, "rn_apos_remover_20");
    avl_remove(a, 20);
    salvaAVL(a, "avl_apos_remover_20");
    rn_libera(r);
    avl_libera(a);

    // comparação com dados crescentes 1..15
    r = rn_cria();
    a = avl_cria();
    for (int v = 1; v <= 15; v++) {
        rn_insere(r, v);
        avl_insere(a, v);
    }
    salvaAVL(a, "comparacao_avl_crescente_1a15");
    salvaRN(r, "comparacao_rn_crescente_1a15");
    rn_libera(r);
    avl_libera(a);

    // comparação com 20 valores fora de ordem
    int mix[] = {50, 20, 80, 10, 30, 70, 90, 5, 15, 25, 35, 60, 75, 85, 95, 1, 7, 12, 17, 22};
    r = rn_cria();
    a = avl_cria();
    for (int v : mix) {
        rn_insere(r, v);
        avl_insere(a, v);
    }
    salvaAVL(a, "comparacao_avl_20_valores");
    salvaRN(r, "comparacao_rn_20_valores");
    rn_libera(r);
    avl_libera(a);

    printf("Imagens geradas em docs/img\n");
    return 0;
}
