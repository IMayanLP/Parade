#include "jogo.h"

typedef struct elemento *Lista;

// Funções lista
Lista *criar(); // OK
int inserirInicio(Lista *, struct carta); // OK
int removerCarta(Lista *, struct carta); // OK
int removerIndice(Lista *, int); // OK
int acessarIndice(Lista *, struct carta *, int); // OK
int quantidade(Lista *); // OK
int imprimir(Lista *); // OK

// Funções Jogo
int gerarBaralho(Lista *); // OK
int somaValores(Lista *); // OK
