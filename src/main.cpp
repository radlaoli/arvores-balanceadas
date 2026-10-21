#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include "avl.hpp"
#include "rn.hpp"
#include "imagem.hpp"
#include "dados.hpp"

enum TipoArvore { TIPO_AVL = 1, TIPO_RN = 2 };

static int lerInteiro(const char* mensagem, int* valor) {
    char linha[128];
    while (1) {
        printf("%s", mensagem);
        if (fgets(linha, sizeof(linha), stdin) == NULL) return 0;
        char* fim;
        long x = strtol(linha, &fim, 10);
        while (*fim == ' ' || *fim == '\t' || *fim == '\r' || *fim == '\n') fim++;
        if (fim != linha && *fim == '\0' && x >= -2147483647L && x <= 2147483647L) {
            *valor = (int) x;
            return 1;
        }
        printf("Entrada inválida, digite um número inteiro.\n");
    }
}

static int lerTexto(const char* mensagem, char* texto, int tamanho) {
    printf("%s", mensagem);
    if (fgets(texto, tamanho, stdin) == NULL) return 0;
    texto[strcspn(texto, "\r\n")] = '\0';
    return 1;
}

static int insere(int tipo, AVL* avl, RN* rn, int v) {
    return tipo == TIPO_AVL ? avl_insere(avl, v) : rn_insere(rn, v);
}
static int removeValor(int tipo, AVL* avl, RN* rn, int v) {
    return tipo == TIPO_AVL ? avl_remove(avl, v) : rn_remove(rn, v);
}
static int consulta(int tipo, AVL* avl, RN* rn, int v) {
    return tipo == TIPO_AVL ? avl_consulta(avl, v) : rn_consulta(rn, v);
}
static int totalNos(int tipo, AVL* avl, RN* rn) {
    return tipo == TIPO_AVL ? avl_totalNos(avl) : rn_totalNos(rn);
}

static void mostraEstatisticas(int tipo, AVL* avl, RN* rn) {
    if (tipo == TIPO_AVL) {
        printf("Nós: %d | Altura: %d | Rotações: %ld | Propriedades da AVL: %s\n",
               avl_totalNos(avl), avl_altura(avl), avl_rotacoes(),
               avl_verifica(avl) ? "OK" : "VIOLADAS");
    } else {
        printf("Nós: %d | Altura: %d | Altura negra: %d | Rotações: %ld | Recolorações: %ld"
               " | Propriedades da RN: %s\n",
               rn_totalNos(rn), rn_altura(rn), rn_alturaNegra(rn), rn_rotacoes(),
               rn_recoloracoes(), rn_verifica(rn) ? "OK" : "VIOLADAS");
    }
}

static void escolheArvore(int* tipo) {
    int op = 0;
    while (op != TIPO_AVL && op != TIPO_RN) {
        printf("\nQual árvore deseja usar?\n");
        printf("  1 - Árvore AVL\n");
        printf("  2 - Árvore Rubro-Negra\n");
        if (!lerInteiro("Opção: ", &op)) exit(0);
        if (op != TIPO_AVL && op != TIPO_RN) printf("Opção inválida.\n");
    }
    *tipo = op;
}

static void menu(int tipo) {
    printf("\n========== %s ==========\n", tipo == TIPO_AVL ? "ÁRVORE AVL" : "ÁRVORE RUBRO-NEGRA");
    printf("  1 - Inserir elemento\n");
    printf("  2 - Remover elemento\n");
    printf("  3 - Buscar elemento\n");
    printf("  4 - Percurso em ordem\n");
    printf("  5 - Imprimir estrutura da árvore\n");
    printf("  6 - Exportar imagem (Graphviz)\n");
    printf("  7 - Estatísticas (altura, rotações, propriedades)\n");
    printf("  8 - Carregar elementos de um arquivo\n");
    printf("  9 - Trocar de árvore (a atual é apagada)\n");
    printf("  0 - Sair\n");
}

int main() {
    std::error_code erro;
    if (!std::filesystem::exists("tests/dados", erro) && std::filesystem::exists("../tests/dados", erro))
        std::filesystem::current_path("..", erro);   // aberto de dentro de bin/ (duplo clique)

    std::filesystem::create_directories("saida", erro);

    printf("Amazon Engineering Challenge - Árvores Balanceadas\n");

    int tipo;
    escolheArvore(&tipo);
    AVL* avl = avl_cria();
    RN* rn = rn_cria();
    avl_zeraEstatisticas();
    rn_zeraEstatisticas();

    int op = -1;
    while (op != 0) {
        menu(tipo);
        if (!lerInteiro("Opção: ", &op)) break;
        int v;

        switch (op) {
        case 1:
            if (!lerInteiro("Valor a inserir: ", &v)) break;
            if (insere(tipo, avl, rn, v)) printf("Valor %d inserido.\n", v);
            else printf("O valor %d já existe na árvore: não foi inserido de novo.\n", v);
            break;

        case 2:
            if (!lerInteiro("Valor a remover: ", &v)) break;
            if (totalNos(tipo, avl, rn) == 0) printf("A árvore está vazia.\n");
            else if (removeValor(tipo, avl, rn, v)) printf("Valor %d removido.\n", v);
            else printf("O valor %d não existe na árvore.\n", v);
            break;

        case 3:
            if (!lerInteiro("Valor a buscar: ", &v)) break;
            if (consulta(tipo, avl, rn, v)) printf("Valor %d ENCONTRADO.\n", v);
            else printf("Valor %d NÃO encontrado.\n", v);
            break;

        case 4:
            if (totalNos(tipo, avl, rn) == 0) { printf("A árvore está vazia.\n"); break; }
            printf("Em ordem: ");
            if (tipo == TIPO_AVL) avl_emOrdem(avl); else rn_emOrdem(rn);
            printf("\n");
            break;

        case 5:
            if (tipo == TIPO_AVL) {
                printf("(raiz à esquerda, filhos à direita; h = altura, fb = fator de balanceamento)\n\n");
                avl_imprime(avl);
            } else {
                printf("(raiz à esquerda, filhos à direita; (R) = vermelho, (B) = preto)\n\n");
                rn_imprime(rn);
            }
            break;

        case 6: {
            const char* dot = tipo == TIPO_AVL ? "saida/arvore_avl.dot" : "saida/arvore_rn.dot";
            const char* png = tipo == TIPO_AVL ? "saida/arvore_avl.png" : "saida/arvore_rn.png";
            int ok = tipo == TIPO_AVL ? avl_exportaDot(avl, dot) : rn_exportaDot(rn, dot);
            if (!ok) { printf("Não foi possível criar %s.\n", dot); break; }
            printf("Arquivo %s salvo.\n", dot);
            gera_imagem(dot, png);
            break;
        }

        case 7:
            mostraEstatisticas(tipo, avl, rn);
            break;

        case 8: {
            char caminho[256];
            if (!lerTexto("Arquivo (ex.: tests/dados/aleatorio.txt): ", caminho, sizeof(caminho))) break;
            int* valores;
            int n = carrega_arquivo(caminho, &valores);
            if (n < 0) { printf("Não foi possível abrir %s.\n", caminho); break; }
            int inseridos = 0;
            for (int i = 0; i < n; i++) inseridos += insere(tipo, avl, rn, valores[i]);
            free(valores);
            printf("%d valores lidos, %d inseridos (%d repetidos ignorados).\n",
                   n, inseridos, n - inseridos);
            mostraEstatisticas(tipo, avl, rn);
            break;
        }

        case 9:
            avl_libera(avl);
            rn_libera(rn);
            avl = avl_cria();
            rn = rn_cria();
            avl_zeraEstatisticas();
            rn_zeraEstatisticas();
            escolheArvore(&tipo);
            break;

        case 0:
            printf("Encerrando.\n");
            break;

        default:
            printf("Opção inválida.\n");
        }
    }

    avl_libera(avl);
    rn_libera(rn);
    return 0;
}