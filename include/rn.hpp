#ifndef RN_HPP
#define RN_HPP

typedef struct NO_RN* RN;

RN*  rn_cria(void);
void rn_libera(RN* raiz);
int  rn_insere(RN* raiz, int valor);
int  rn_remove(RN* raiz, int valor);
int  rn_consulta(RN* raiz, int valor);
void rn_emOrdem(RN* raiz);
void rn_imprime(RN* raiz);
int  rn_altura(RN* raiz);
int  rn_alturaNegra(RN* raiz);

int  rn_exportaDot(RN* raiz, const char* arquivo);

int  rn_verifica(RN* raiz);
int  rn_totalNos(RN* raiz);
int  rn_numFilhos(RN* raiz, int valor);
void rn_zeraEstatisticas(void);
long rn_rotacoes(void);
long rn_recoloracoes(void);

#endif