#ifndef RN_HPP
#define RN_HPP

typedef struct NO_RN* RN;

// operações principais
RN*  rn_cria(void);
void rn_libera(RN* raiz);
int  rn_insere(RN* raiz, int valor);    // 1 = inseriu, 0 = valor repetido
int  rn_remove(RN* raiz, int valor);    // 1 = removeu, 0 = valor não existe
int  rn_consulta(RN* raiz, int valor);  // 1 = encontrou, 0 = não encontrou
void rn_emOrdem(RN* raiz);
void rn_imprime(RN* raiz);              // mostra a cor de cada nó (R) ou (B)
int  rn_altura(RN* raiz);               // folha = 0, árvore vazia = -1
int  rn_alturaNegra(RN* raiz);          // nós pretos da raiz até uma folha

// visualização com Graphviz (gera o arquivo .dot com os nós coloridos)
int  rn_exportaDot(RN* raiz, const char* arquivo);

int  rn_verifica(RN* raiz);             // 1 = todas as propriedades da RB valem
int  rn_totalNos(RN* raiz);
int  rn_numFilhos(RN* raiz, int valor); // -1 se o valor não existe
void rn_zeraEstatisticas(void);
long rn_rotacoes(void);                 // rotações feitas desde o último zera
long rn_trocasCor(void);                // trocas de cor 

#endif
