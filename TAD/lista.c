#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "lista.h"

struct elemento {
	struct carta card;
	struct elemento *prox;
};


typedef struct elemento Elemento;

// Render
int imprimir(Lista *ldse){
	if(ldse == NULL) return 0;
	if(*ldse == NULL) return -1;
	Elemento *aux;
	aux = *ldse;
	while(aux != NULL) {
		printf("[ %d ,  %c ] ->", aux->card.valor, aux->card.naipe);
		aux = aux->prox;
	}
	printf("||");
	return 1;
}

// Tick Lista
// CRUD
Lista *criar(){
	Lista *a;
	a = (Lista*)malloc(sizeof(Lista));
	if(a != NULL) {
		*a = NULL;
	}
	return a;
}

int inserirInicio(Lista *ldse, struct carta novacarta){
	if(ldse == NULL) return 0;
	Elemento *a;
	a = (Elemento*)malloc(sizeof(Elemento));
	a->card = novacarta;
	a->prox = *ldse;
	*ldse = a;

	return 1;
}

int removerIndice(Lista *ldse, int num) {
	if(ldse == NULL) return 0;
	if(*ldse == NULL) return -1;
	int count = 1;
	Elemento *aux;
	Elemento *ant;
	aux = *ldse;
	ant = NULL;
	if(num == 1) {
		*ldse = aux->prox;
	} else {
		while(aux != NULL && count != num){
			ant = aux;
			aux = aux->prox;
			count++;
		}

	ant->prox = aux->prox;
	}

	free(aux);
	return 1;
}

int removerCarta(Lista *ldse, struct carta cartaRemovida) {
	if(ldse == NULL) return 0;
	if(*ldse == NULL) return -1;
	Elemento *aux;
	Elemento *ant;
	aux = *ldse;
	ant = NULL;
	if(cartaRemovida.valor == (*ldse)->card.valor && cartaRemovida.naipe == (*ldse)->card.naipe) {
		*ldse = aux->prox;
	}
		else {
			while(aux != NULL && (cartaRemovida.valor != aux->card.valor || cartaRemovida.naipe != aux->card.naipe) ){
				ant = aux;
				aux = aux->prox;
			}
			if(aux == NULL) {
				return 2;
			}
			ant->prox = aux->prox;
		}
	free(aux);
	return 1;
}

int acessarIndice(Lista *ldse, struct carta *cartaindice, int num){
	if(ldse == NULL) return 0;
	if(*ldse == NULL) return -1;
	int count = 1;
	Elemento *aux;
	aux = *ldse;
	while(aux != NULL && count != num){
		aux = aux->prox;
		count++;
	}

	*cartaindice = aux->card;

	return 1;
}

int quantidade(Lista *ldse) {
	if(ldse == NULL) return -1;
	if(*ldse == NULL) return 0;
	int count = 1;
	Elemento *aux;
	aux = *ldse;
	while(aux != NULL){
		aux = aux->prox;
		count++;
	}

	return count-1;
}

int somaValores(Lista *collection){
	if(collection == NULL) return -1;
	if(*collection == NULL) return 0;
	Elemento *aux;
	aux = *collection;
	int count;
	while(aux != NULL) {
		count += aux->card.valor;
		aux = aux->prox;
	}
	return count;
}

