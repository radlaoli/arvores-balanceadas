#ifndef IMAGEM_HPP
#define IMAGEM_HPP

// Chama o Graphviz para converter o .dot em .png.
// Retorna 1 se gerou a imagem, 0 se o Graphviz não estiver instalado.
int gera_imagem(const char* arquivoDot, const char* arquivoPng);

#endif
