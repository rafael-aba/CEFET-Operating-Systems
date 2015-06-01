#include <stdlib.h>
#include <stdio.h>


#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include "queue.h"


#define num_prod 3
#define num_cons 2
#define Sleep_Time 4
#define Buffer_Lenght 5
#define Number 1000
#define true 1
#define CONT 10

int mainsleep;
int  counter;

typedef struct queue_t_int{
   struct queue_t_int *prev ; 
   struct queue_t_int *next ; 
   int    item ;
} queue_t_int ;

// Buffer
queue_t_int *queue_integer;

// Semáfotos
sem_t s_vaga, s_item, s_buffer;

pthread_t tid;       //Thread ID
pthread_attr_t attr; //Set of thread attributes

void *producer(void *id); // the producer thread
void *consumer(void *id); // the consumer thread

void initializeData() {
	mainsleep = 1;
	counter = 0;
	queue_integer = NULL;

	// Cria semáfoto s_buffer
	sem_init(&s_buffer, 0, 1);

	// Cria semáfoto s_vaga
	sem_init(&s_vaga, 0, num_prod);

	// Cria semaforo s_item
	sem_init(&s_item, 0, 0);

	// Atributos default
	pthread_attr_init (&attr) ;
	pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_JOINABLE) ;

}

// Producer Thread 
void *producer(void *id) {
   while (mainsleep){

//      pause = random (0..3)
	int pause = rand() % Sleep_Time;
	sleep (pause);

//      down (s_vaga)
	sem_wait(&s_vaga);

//      down (s_buffer)
	sem_wait(&s_buffer);

//      item = random (0..999)
	int item = rand() % Number;

//      insere item no buffer
	queue_t_int *novo;
        novo = (queue_t_int*) malloc (sizeof (queue_t_int));
        novo->item = item;
        novo->prev = NULL;
        novo->next = NULL;
	queue_append((queue_t **) &queue_integer, (queue_t*) novo);

	counter++;

//      up (s_buffer)
	sem_post(&s_buffer);
//      up (s_item)
	sem_post(&s_item);


//      print item
	printf("p%ld produziu %d\n",((long)id)+1,item);
	}

	pthread_exit (NULL) ;
}

// Consumer Thread
void *consumer(void *id) {
   while (mainsleep || counter>0){

//      down (s_item)
	sem_wait(&s_item);


//      down (s_buffer)
	sem_wait(&s_buffer);

//	retira item do buffer

	queue_t_int *velho;
        velho = queue_integer;
	queue_remove ((queue_t**) &queue_integer, (queue_t*) velho);

	int item = velho->item;

	counter--;

//      up (s_buffer)
	sem_post(&s_buffer);

//      up (s_vaga)
	sem_post(&s_vaga);


//	print item
	printf("\t\t\tc%ld consumiu %d\n",((long)id)+1,item);

//      pause = random (0..3)
	int pause = rand() % Sleep_Time;
	sleep (pause);
   }

   pthread_exit (NULL) ;
}

int main() {

	// Inicializando
	initializeData();
	long i, status;
	pthread_t thread [num_prod+num_cons];

	// Criando Threads produtoras 
	for(i = 0; i < num_prod; i++) {
	// Create the thread
		status = pthread_create(&thread[i],&attr,producer,(void *) i);
	}

	// Criando Threads consumidoras
	for(i = 0; i < num_cons; i++) {
		// Create the thread
		status = pthread_create(&thread[num_prod+i],&attr,consumer, (void *) i);
	}

	sleep(10);
	mainsleep = 0;

	for (i=0; i<num_prod; i++){
		printf ("Main: aguardando thread p%ld\n", i);
		status = pthread_join (thread[i], NULL) ;
		if (status){
			perror ("pthread_join") ;
			exit (1) ;
		}
	}

	for (i=0; i<num_cons; i++){
		printf ("Main: aguardando thread c%ld\n", i);
		status = pthread_join (thread[num_prod+i], NULL);
		if (status){
			perror ("pthread_join") ;
			exit (1) ;
		}
	}

	return 0;

}

