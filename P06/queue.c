#include <stdio.h>
#include <stdlib.h>
#include "queue.h"


//------------------------------------------------------------------------------
// Insere um elemento no final da fila.
// Condicoes a verificar, gerando msgs de erro:
// - a fila deve existir
// - o elemento deve existir
// - o elemento nao deve estar em outra fila
void queue_append (queue_t **queue, queue_t *elem){
	queue_t *aux;
	if ( elem == NULL); //Elemento Não existe
	else if ( elem->prev !=NULL || elem->next != NULL); //Elemento já faz parte de outra lista
	else if (*queue == NULL) { // Fila Vazia
		aux = elem;
		aux->prev = elem;
		aux->next = elem;
		*queue = aux;
	}
	else{ // Fila Nao fazia
		aux = (*queue)->prev;
		elem->prev = aux;	
		elem->next = (*queue);
		aux->next = elem;
		(*queue)->prev = elem;
	}
}

//------------------------------------------------------------------------------
// Remove o elemento indicado da fila, sem o destruir.
// Condicoes a verificar, gerando msgs de erro:
// - a fila deve existir
// - a fila nao deve estar vazia
// - o elemento deve existir
// - o elemento deve pertencer a fila indicada
// Retorno: apontador para o elemento removido, ou NULL se erro
queue_t *queue_remove (queue_t **queue, queue_t *elem){
	queue_t *aux;
	if ( elem == NULL) return NULL; // Elemento não existe
	else if (*queue == NULL) return NULL; // Fila Vazia
	else{ // Fila Nao fazia
		if(elem == *queue){ // elemento desejado é o primeiro da lista
			if (queue_size(*queue) == 1) *queue = NULL;
			else {
				elem->prev->next = elem->next;
				elem->next->prev = elem->prev;
				*queue = elem->next;
			}
			elem->prev = NULL;
			elem->next = NULL;
			return elem;
		}else{ // elemento desejado não é o primeiro da lista!
			aux = (*queue)->next;
			while(aux != *queue){
				if (elem == aux){
					elem->prev->next = elem->next;
					elem->next->prev = elem->prev;
					elem->prev = NULL;
					elem->next = NULL;
					return elem;
				}
				aux = aux->next;
			}
		} 		
	}
	//Elemento não pertence a lista
	return NULL;
}
 
//------------------------------------------------------------------------------
// Conta o numero de elementos na fila
// Retorno: numero de elementos na fila
 
int queue_size (queue_t *queue){
	if (queue == NULL) return 0;
	int i = 1;
	queue_t *aux = queue;
	while (aux->next != queue) {
		aux = aux->next;
		i++;
	}
	return i;
}
 
//------------------------------------------------------------------------------
// Percorre a fila e imprime na tela seu conteúdo. A impressão de cada
// elemento é feita por uma função externa, definida pelo programa que
// usa a biblioteca. Essa função deve ter o seguinte protótipo:
//
// void print_elem (void *ptr) ; // ptr aponta para o elemento a imprimir
 
void queue_print (char *name, queue_t *queue, void print_elem (void*) ){
	queue_t *queue_aux = queue, *first = queue;
	if (queue != NULL) {
		printf("%s: [", name);
		print_elem((void *)queue_aux);
		while (queue_aux->next != first) {
			queue_aux = queue_aux->next;
			printf(" ");
			print_elem((void *)queue_aux);
		}
		printf("]\n");
	}else printf("%s: []\n", name);
}
