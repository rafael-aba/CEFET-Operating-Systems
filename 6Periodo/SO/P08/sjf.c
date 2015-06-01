#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "queue.h"

#define tmax 100

typedef struct queue_process_t {
    struct queue_process_t *prev;
    struct queue_process_t *next;
    int id;
    int inicio;
    int duracao;
    int prioridade_estatica;
    int prioridade_dinamica;
    int estado_atual; // 0 - nova, 1 - pronta, 2 - rodando, 3 - terminada
    int tempo_de_vida;
    int tempo_executado_total;
    int tempo_executado_quantum_atual;
    int tempo_espera;
} queue_process_t;

pthread_mutex_t processador;

queue_process_t *processes = NULL, *process;
queue_process_t *processo_corrente ;
queue_process_t	*ready;
queue_process_t	*finesh;
queue_process_t	*iterador;
queue_process_t	*aux;

int qtd_Eprocess, qtd_Tprocess, tempo, numero_troca_contexto;
float tempo_medio_vida, tempo_medio_espera;

void read_input(){
	int a, b, c, id = 0;
	while (scanf("%d %d %d",&a,&b,&c) != -1) {
		process = malloc(sizeof(queue_process_t));
		process->prev = NULL;
		process->next = NULL;
		process->id = id;
		process->tempo_de_vida = 0;
		process->estado_atual = 0; // tarefa nova
		process->inicio = a;
		process->duracao = b;
		process->prioridade_estatica = c;
		process->tempo_executado_total = 0;
	    queue_append((queue_t **) &processes, (queue_t *) process);
	    id++;
	}
}

void init(){
	pthread_mutex_init(&processador, NULL);

	processo_corrente = NULL;
	ready = NULL;
	finesh = NULL;
	iterador = NULL;
	aux = NULL;

	tempo = 0;
	qtd_Eprocess = 0;
	qtd_Tprocess = queue_size((queue_t *) processes);

	tempo_medio_vida = 0.0;
	tempo_medio_espera = 0.0;
	numero_troca_contexto = -1;
}

void cabecalho(){
	printf("Tempo");
	int i;
	for( i = 1; i <= qtd_Tprocess; i++) printf("\tP%d",i);
	printf("\n");
}



void imprime(){
	int area,i;
	aux = NULL;

	printf("%d-%d",tempo,(tempo+1));
	for(area = 0; area < qtd_Tprocess; area++){

		if (processes!= NULL){
			iterador = processes;
			for(i = 0; i < queue_size((queue_t *) processes); i++){
				if (iterador->id == area) aux = iterador;
				else iterador = iterador->next;
			}
		}


		if (ready!= NULL){
			iterador = ready;
			for(i = 0; i < queue_size((queue_t *) ready); i++){
				if (iterador->id == area) aux = iterador;
				else iterador = iterador->next;
			}
		}

		if (processo_corrente != NULL){
			if (processo_corrente->id == area){
				aux = processo_corrente;
				processo_corrente->tempo_executado_total++;
			}
		}

		if (finesh!= NULL){
			iterador = finesh;
			for(i = 0; i < queue_size((queue_t *) finesh); i++){
				if (iterador->id == area) aux = iterador;
				else iterador = iterador->next;
			}
		}

		if (aux != NULL){
			switch (aux->estado_atual){ // 0 - nova, 1 - pronta, 2 - rodando, 3 - terminada
				case 0:
					printf("\t");
					break;
				case 1:
					printf("\t--");
					tempo_medio_espera++;
					break;
				case 2:
					printf("\t##");
					break;
				case 3:
					printf("\t");
					break;
			}
		}
	}
	printf("\n");
}

void sjf(){
//    se há uma tarefa rodando
	if ( processo_corrente != NULL ){
//      se a tarefa rodando chegou ao fim da execução
		if (processo_corrente->duracao == processo_corrente->tempo_executado_total) {    
            //migra a tarefa para o estado terminado 
			processo_corrente->estado_atual = 3; // terminado
			queue_append((queue_t **) &finesh, (queue_t *) processo_corrente);
            //libera o processador
			processo_corrente = NULL;
			qtd_Eprocess++;
			pthread_mutex_unlock(&processador);
//      senão
		}else{
//        se a tarefa rodando chegou ao fim de seu quantum
//        migra a tarefa para a fila de prontos
//        libera o processador
//      fim se
		}
//    fim se
	}
	if ((queue_size((queue_t *) finesh)) < (qtd_Tprocess)){
//    para cada tarefa i
		int i;
		iterador = processes;
		for(i = 0; i < queue_size((queue_t *) processes); i++){
//      se a tarefa i inicia agora (em t)
			if( iterador->inicio == tempo){
//        coloca a tarefa na fila de prontos
				aux = (queue_process_t *) queue_remove((queue_t **) &processes, (queue_t *) iterador);
				queue_append((queue_t **) &ready, (queue_t *) iterador);
				iterador->estado_atual = 1;
				iterador = processes;
//      fim se	
			}else{
				iterador = iterador->next;
			}
//    fim para
		}
//    se o processador estiver livre
		if (!pthread_mutex_trylock(&processador)) {
//      se houver tarefa na fila de prontas
			if (queue_size((queue_t *) ready) > 0) {
//        escolhe uma tarefa da fila de prontas
				aux = ready;
				iterador = ready;
				for(i = 0; i < queue_size((queue_t *) ready); i++){
					if (aux->duracao > iterador->duracao){
						aux = iterador;
						iterador = iterador->next;
					}else iterador = iterador->next;
				}
//        migra essa tarefa para o estado "rodando"
				processo_corrente = (queue_process_t *) queue_remove((queue_t **) &ready, (queue_t *) aux);
				processo_corrente->estado_atual = 2;
				numero_troca_contexto++;
//      fim se
			}
//    fim se
		}
//    imprime linha do diagrama com o estado de cada tarefa
		imprime();
	}else{
		tempo_medio_vida = (float)(tempo) / (float)qtd_Tprocess;
		tempo = tmax;
	}
}

//início
int main(){
	
//  lê dados das tarefas da entrada padrão
	read_input();
	init();
//  imprime cabeçalho do diagrama
	cabecalho();
//  t = 0
	tempo = 0;
//  enquanto t < tmax
	while ( tempo < tmax ){
		sjf();
//    incrementa o tempo (t++)
		tempo++;

//    incrementa contadores da tarefa corrente (tempo de vida e de quantum)
//  fim enquanto
	}
		printf("\nTempo Medio de vida: %.2f",tempo_medio_vida);
		printf("\nTempo Medio de espera: %.2f",tempo_medio_espera/qtd_Tprocess);
		printf("\nNumero de trocas de contexto: %d\n",numero_troca_contexto);
//  calcula e imprime tempos médios
//fim
}
