#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <filesystem>
#include "avl.hpp"
#include "rn.hpp"
#include "dados.hpp"

struct Arvore {
    const char* nome;
    void* (*cria)();
    void  (*libera)(void*);
    int   (*insere)(void*, int);
    int   (*remove)(void*, int);
    int   (*consulta)(void*, int);
    int   (*altura)(void*);
    int   (*total)(void*);
    int   (*verifica)(void*);
    int   (*numFilhos)(void*, int);
    void  (*zera)();
    long  (*rotacoes)();
    long  (*recoloracoes)();
};

static void* avlCria() { return avl_cria(); }
static void  avlLibera(void* a) { avl_libera((AVL*) a); }
static int   avlInsere(void* a, int v) { return avl_insere((AVL*) a, v); }
static int   avlRemove(void* a, int v) { return avl_remove((AVL*) a, v); }
static int   avlConsulta(void* a, int v) { return avl_consulta((AVL*) a, v); }
static int   avlAltura(void* a) { return avl_altura((AVL*) a); }
static int   avlTotal(void* a) { return avl_totalNos((AVL*) a); }
static int   avlVerifica(void* a) { return avl_verifica((AVL*) a); }
static int   avlNumFilhos(void* a, int v) { return avl_numFilhos((AVL*) a, v); }
static long  avlRecoloracoes() { return 0; }

static void* rnCria() { return rn_cria(); }
static void  rnLibera(void* a) { rn_libera((RN*) a); }
static int   rnInsere(void* a, int v) { return rn_insere((RN*) a, v); }
static int   rnRemove(void* a, int v) { return rn_remove((RN*) a, v); }
static int   rnConsulta(void* a, int v) { return rn_consulta((RN*) a, v); }
static int   rnAltura(void* a) { return rn_altura((RN*) a); }
static int   rnTotal(void* a) { return rn_totalNos((RN*) a); }
static int   rnVerifica(void* a) { return rn_verifica((RN*) a); }
static int   rnNumFilhos(void* a, int v) { return rn_numFilhos((RN*) a, v); }

static const Arvore ARVORES[2] = {
    {"AVL", avlCria, avlLibera, avlInsere, avlRemove, avlConsulta, avlAltura, avlTotal,
     avlVerifica, avlNumFilhos, avl_zeraEstatisticas, avl_rotacoes, avlRecoloracoes},
    {"Rubro-Negra", rnCria, rnLibera, rnInsere, rnRemove, rnConsulta, rnAltura, rnTotal,
     rnVerifica, rnNumFilhos, rn_zeraEstatisticas, rn_rotacoes, rn_recoloracoes},
};

static int passou = 0, falhou = 0;

static void confere(int condicao, const char* arvore, const char* descricao) {
    if (condicao) passou++;
    else falhou++;
    printf("  [%s] %-12s %s\n", condicao ? "PASSOU" : "FALHOU", arvore, descricao);
}

struct Conjunto {
    const char* nome;
    int* valores;
    int n;
};

static Conjunto carrega(const char* nome, const char* caminho) {
    Conjunto c;
    c.nome = nome;
    c.n = carrega_arquivo(caminho, &c.valores);
    if (c.n < 0) {
        printf("ERRO: não foi possível abrir %s (rode a partir da raiz do projeto)\n", caminho);
        exit(1);
    }
    return c;
}

static int insereVerificando(const Arvore& T, void* arv, const Conjunto& c, int n) {
    int ok = 1;
    for (int i = 0; i < n; i++) {
        T.insere(arv, c.valores[i]);
        if (!T.verifica(arv)) ok = 0;
    }
    return ok;
}

static void testeArvoreVazia(const Arvore& T) {
    void* a = T.cria();
    confere(T.altura(a) == -1, T.nome, "arvore vazia: altura -1");
    confere(T.total(a) == 0, T.nome, "arvore vazia: 0 nos");
    confere(T.consulta(a, 10) == 0, T.nome, "arvore vazia: busca nao encontra");
    confere(T.remove(a, 10) == 0, T.nome, "arvore vazia: remocao retorna 0 sem travar");
    confere(T.verifica(a), T.nome, "arvore vazia: propriedades validas");
    T.libera(a);
}

static void testeUmElemento(const Arvore& T) {
    void* a = T.cria();
    confere(T.insere(a, 42) == 1, T.nome, "um elemento: insercao do 42");
    confere(T.altura(a) == 0 && T.total(a) == 1, T.nome, "um elemento: altura 0 e 1 no");
    confere(T.consulta(a, 42) == 1, T.nome, "um elemento: busca encontra o 42");
    confere(T.consulta(a, 7) == 0, T.nome, "um elemento: busca nao encontra o 7");
    confere(T.verifica(a), T.nome, "um elemento: propriedades validas");
    confere(T.remove(a, 42) == 1 && T.total(a) == 0 && T.altura(a) == -1, T.nome,
            "um elemento: remocao deixa a arvore vazia");
    T.libera(a);
}

static void testeRepetido(const Arvore& T) {
    void* a = T.cria();
    T.insere(a, 5);
    T.insere(a, 3);
    T.insere(a, 8);
    confere(T.insere(a, 5) == 0 && T.total(a) == 3, T.nome,
            "valor repetido: nao e inserido de novo");
    confere(T.verifica(a), T.nome, "valor repetido: propriedades validas");
    T.libera(a);
}

static void testeInsercaoEBusca(const Arvore& T, const Conjunto& c) {
    void* a = T.cria();
    char desc[128];

    snprintf(desc, sizeof(desc), "insercao %s (%d): propriedades apos cada insercao", c.nome, c.n);
    confere(insereVerificando(T, a, c, c.n), T.nome, desc);

    int todosEncontrados = 1;
    for (int i = 0; i < c.n; i++)
        if (!T.consulta(a, c.valores[i])) todosEncontrados = 0;
    snprintf(desc, sizeof(desc), "busca %s: todos os %d elementos existentes encontrados", c.nome, c.n);
    confere(todosEncontrados, T.nome, desc);

    int inexistentes[] = {0, -1, -500, c.n + 1, c.n + 1000};
    int nenhum = 1;
    for (int x : inexistentes)
        if (T.consulta(a, x)) nenhum = 0;
    snprintf(desc, sizeof(desc), "busca %s: elementos inexistentes nao encontrados", c.nome);
    confere(nenhum, T.nome, desc);

    T.libera(a);
}

static int achaNoComFilhos(const Arvore& T, void* a, const Conjunto& c, int n, int filhos) {
    for (int i = 0; i < n; i++)
        if (T.numFilhos(a, c.valores[i]) == filhos) return c.valores[i];
    return -1;
}

static void testeRemocoes(const Arvore& T, const Conjunto& aleatorio, const Conjunto& remocoes) {
    const int N = 1000;
    void* a = T.cria();
    insereVerificando(T, a, aleatorio, N);
    char desc[128];

    const char* casos[3] = {"folha (0 filhos)", "no com 1 filho", "no com 2 filhos"};
    for (int f = 0; f <= 2; f++) {
        int v = achaNoComFilhos(T, a, aleatorio, N, f);
        int antes = T.total(a);
        int ok = v != -1 && T.remove(a, v) == 1 && !T.consulta(a, v)
                 && T.total(a) == antes - 1 && T.verifica(a);
        snprintf(desc, sizeof(desc), "remocao de %s (valor %d)", casos[f], v);
        confere(ok, T.nome, desc);
    }

    int antes = T.total(a);
    confere(T.remove(a, -10) == 0 && T.remove(a, 999999) == 0 && T.total(a) == antes, T.nome,
            "remocao de elemento inexistente: retorna 0 e nao altera a arvore");

    int ok = 1;
    for (int i = 0; i < N; i++) {
        T.remove(a, aleatorio.valores[i]);
        if (!T.verifica(a)) ok = 0;
    }
    confere(ok && T.total(a) == 0, T.nome,
            "remover todos os 1000 elementos: propriedades apos cada remocao");
    T.libera(a);

    a = T.cria();
    insereVerificando(T, a, aleatorio, aleatorio.n);
    ok = 1;
    for (int i = 0; i < remocoes.n; i++) {
        T.remove(a, remocoes.valores[i]);
        if (!T.verifica(a)) ok = 0;
    }
    snprintf(desc, sizeof(desc), "%d remocoes em arvore de %d: propriedades apos cada remocao",
             remocoes.n, aleatorio.n);
    confere(ok && T.total(a) == aleatorio.n - remocoes.n, T.nome, desc);
    T.libera(a);
}

struct Medida {
    int altura;
    long rotacoes;
    long recoloracoes;
    double ms;
};

static Medida mede(const Arvore& T, const Conjunto& c, int n) {
    void* a = T.cria();
    T.zera();
    clock_t inicio = clock();
    for (int i = 0; i < n; i++) T.insere(a, c.valores[i]);
    clock_t fim = clock();
    Medida m = {T.altura(a), T.rotacoes(), T.recoloracoes(), 1000.0 * (fim - inicio) / CLOCKS_PER_SEC};
    T.libera(a);
    return m;
}

static void analiseComparativa(Conjunto* conjuntos, int qtd, const Conjunto& remocoes, FILE* csv) {
    int tamanhos[] = {100, 1000, 10000};
    fprintf(csv, "fase,conjunto,n,altura_avl,altura_rn,rotacoes_avl,rotacoes_rn,recoloracoes_rn,"
                 "tempo_ms_avl,tempo_ms_rn,altura_minima\n");

    printf("\n========================= ANALISE COMPARATIVA: INSERCAO =========================\n");
    printf("%-12s %6s | %6s %6s | %8s %8s %8s | %6s\n",
           "conjunto", "n", "h AVL", "h RN", "rot AVL", "rot RN", "recol RN", "h min");
    printf("---------------------------------------------------------------------------------\n");

    for (int i = 0; i < qtd; i++) {
        for (int n : tamanhos) {
            if (n > conjuntos[i].n) continue;
            Medida avl = mede(ARVORES[0], conjuntos[i], n);
            Medida rn = mede(ARVORES[1], conjuntos[i], n);
            int hmin = (int) floor(log2((double) n));
            printf("%-12s %6d | %6d %6d | %8ld %8ld %8ld | %6d\n", conjuntos[i].nome, n,
                   avl.altura, rn.altura, avl.rotacoes, rn.rotacoes, rn.recoloracoes, hmin);
            fprintf(csv, "insercao,%s,%d,%d,%d,%ld,%ld,%ld,%.3f,%.3f,%d\n", conjuntos[i].nome, n,
                    avl.altura, rn.altura, avl.rotacoes, rn.rotacoes, rn.recoloracoes,
                    avl.ms, rn.ms, hmin);
        }
    }
    printf("(h = altura final; rot = rotacoes simples; recol = nos recoloridos; h min = piso(log2 n))\n");

    const Conjunto& base = conjuntos[2];
    printf("\n========================= ANALISE COMPARATIVA: REMOCAO ==========================\n");
    printf("Arvore com %d elementos aleatorios; depois %d remocoes (tests/dados/remocoes.txt)\n",
           base.n, remocoes.n);
    printf("%-12s | %10s %10s | %8s %8s\n", "arvore", "h antes", "h depois", "rotacoes", "recolor.");
    printf("---------------------------------------------------------------------------------\n");
    int hDepois[2];
    long rot[2], cor[2];
    for (int t = 0; t < 2; t++) {
        const Arvore& T = ARVORES[t];
        void* a = T.cria();
        for (int i = 0; i < base.n; i++) T.insere(a, base.valores[i]);
        int hAntes = T.altura(a);
        T.zera();
        for (int i = 0; i < remocoes.n; i++) T.remove(a, remocoes.valores[i]);
        hDepois[t] = T.altura(a);
        rot[t] = T.rotacoes();
        cor[t] = T.recoloracoes();
        printf("%-12s | %10d %10d | %8ld %8ld\n", T.nome, hAntes, hDepois[t], rot[t], cor[t]);
        T.libera(a);
    }
    fprintf(csv, "remocao,aleatorio,%d,%d,%d,%ld,%ld,%ld,,,\n", remocoes.n,
            hDepois[0], hDepois[1], rot[0], rot[1], cor[1]);
}

int main() {
    std::error_code erro0;
    if (!std::filesystem::exists("tests/dados", erro0) && std::filesystem::exists("../tests/dados", erro0))
        std::filesystem::current_path("..", erro0);   // aberto de dentro de bin/

    Conjunto crescente = carrega("crescente", "tests/dados/crescente.txt");
    Conjunto decrescente = carrega("decrescente", "tests/dados/decrescente.txt");
    Conjunto aleatorio = carrega("aleatorio", "tests/dados/aleatorio.txt");
    Conjunto remocoes = carrega("remocoes", "tests/dados/remocoes.txt");
    Conjunto conjuntos[3] = {crescente, decrescente, aleatorio};

    printf("========================= TESTES FUNCIONAIS =========================\n");
    for (int t = 0; t < 2; t++) {
        const Arvore& T = ARVORES[t];
        printf("\n--- %s ---\n", T.nome);
        testeArvoreVazia(T);
        testeUmElemento(T);
        testeRepetido(T);
        for (int i = 0; i < 3; i++) testeInsercaoEBusca(T, conjuntos[i]);
        testeRemocoes(T, aleatorio, remocoes);
    }
    printf("\nResultado: %d testes passaram, %d falharam.\n", passou, falhou);

    std::error_code erro;
    std::filesystem::create_directories("saida", erro);
    FILE* csv = fopen("saida/comparacao.csv", "w");
    if (csv == NULL) csv = fopen("comparacao.csv", "w");
    analiseComparativa(conjuntos, 3, remocoes, csv);
    fclose(csv);
    printf("\nTabela salva em saida/comparacao.csv\n");

    free(crescente.valores);
    free(decrescente.valores);
    free(aleatorio.valores);
    free(remocoes.valores);
    return falhou == 0 ? 0 : 1;
}