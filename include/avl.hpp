#ifndef AVL_H
#define AVL_H

typedef struct NO_AVL* AVL;

AVL* avl_cria(void);
int  avl_insere(AVL* raiz, int valor);
int  avl_remove(AVL* raiz, int valor);
int  avl_consulta(AVL* raiz, int valor);
void avl_emOrdem(AVL* raiz);
void avl_imprime(AVL* raiz);
int  avl_altura(AVL* raiz);

#endif