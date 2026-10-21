#ifndef DADOS_HPP
#define DADOS_HPP

// Lê um arquivo de texto com um número inteiro por linha.
// Devolve quantos números foram lidos ou -1 se o arquivo não abriu
// e coloca em *valores um vetor alocado com malloc (quem chama dá free).
int carrega_arquivo(const char* caminho, int** valores);

#endif
