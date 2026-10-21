#include <cstdio>
#include <cstdlib>
#include "imagem.hpp"

int gera_imagem(const char* arquivoDot, const char* arquivoPng) {
    char comando[512];
    snprintf(comando, sizeof(comando), "dot -Tpng \"%s\" -o \"%s\"", arquivoDot, arquivoPng);

    if (system(comando) != 0) {
        printf("Graphviz não encontrado. O arquivo %s foi salvo;\n", arquivoDot);
        printf("cole o conteúdo dele em https://dreampuf.github.io/GraphvizOnline\n");
        return 0;
    }
    printf("Imagem gerada: %s\n", arquivoPng);
    return 1;
}
