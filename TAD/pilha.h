#include "jogo.h"

typedef struct elemento *Pilha;

// Funções pilha
Pilha *criar(); // OK
int inserirPilha(Pilha *, struct carta); // OK
int removerPilha(Pilha *, struct carta *); // OK
int quantidadePilha(Pilha *); // OK
int imprimirPilha(Pilha *); // OK
