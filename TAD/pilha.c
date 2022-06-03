#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "pilha.h"

struct elemento {
	struct carta card;
	struct elemento *prox;
};


typedef struct elemento Elemento;

int imprimirPilha(Pilha *pila){
	if(pila == NULL) return 0;
	if(*pila == NULL) return -1;
	Elemento *aux;
	aux = *pila;
	while(aux != NULL) {
		printf("[ %d ,  %c ] ->", aux->card.valor, aux->card.naipe);
		aux = aux->prox;
	}
	printf("||");
	return 1;
}

// Tick Pilha
Pilha *criarPilha() {
	Pilha *a;
	a = (Pilha*)malloc(sizeof(Pilha));
	if(a != NULL) {
		*a = NULL;
	}
	return a;
}

int inserirPilha(Pilha *pila, struct carta novacarta){
	if(pila == NULL) return 0;
	Elemento *a;
	a = (Elemento*)malloc(sizeof(Elemento));
	a->card = novacarta;
	a->prox = *pila;
	*pila = a;

	return 1;
}

int quantidadePilha(Pilha *pila) {
	if(pila == NULL) return -1;
	if(*pila == NULL) return 0;
	int count = 1;
	Elemento *aux;
	aux = *pila;
	while(aux != NULL){
		aux = aux->prox;
		count++;
	}

	return count-1;
}

int removerPilha(Pilha * pila, struct carta *cartaRemovida){
	if(pila == NULL) return 0;
	if(*pila == NULL) return -1;
	Elemento *aux;
	aux = *pila;
	*pila = aux->prox;
	*cartaRemovida = aux->card;
	free(aux);
	return 1;
}





