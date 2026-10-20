#ifndef AVL_HPP
#define AVL_HPP

typedef struct NO_AVL* AVL;

AVL* avl_cria(void);
void avl_libera(AVL* raiz);
int  avl_insere(AVL* raiz, int valor);
int  avl_remove(AVL* raiz, int valor);  
int  avl_consulta(AVL* raiz, int valor); 
void avl_emOrdem(AVL* raiz);
void avl_imprime(AVL* raiz);
int  avl_altura(AVL* raiz);              

// visualização com Graphviz (gera o arquivo .dot)
int  avl_exportaDot(AVL* raiz, const char* arquivo);

int  avl_verifica(AVL* raiz);  
int  avl_totalNos(AVL* raiz);
int  avl_numFilhos(AVL* raiz, int valor); 
void avl_zeraEstatisticas(void);
long avl_rotacoes(void); 

#endif
