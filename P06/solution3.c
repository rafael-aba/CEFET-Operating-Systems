/*PROGRAMA RETIRADO DO SITE:
https://macboypro.wordpress.com/2009/05/25/producer-consumer-problem-using-cpthreadsbounded-buffer/

ALGUMAS PARTES DO CÓDIGO FORAM ALTERADAS.
*/

#include <stdlib.h>
#include <stdio.h>


#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include "queue.h"



#define Sleep_Time 4
#define Buffer_Lenght 5
#define Number 1000
#define true 1

typedef struct queue_t_int{
   struct queue_t_int *prev ; 
   struct queue_t_int *next ; 
   int    item ;
} queue_t_int ;

// Buffer
queue_t_int *queue_integer;

// Mutex
pthread_mutex_t s_buffer, s_vaga, s_item;

pthread_t tid;       //Thread ID
pthread_attr_t attr; //Set of thread attributes

void *producer(void *id); // the producer thread
void *consumer(void *id); // the consumer thread

int counter;


void initializeData() {
	queue_integer= NULL;
	counter = 0;

	// Cria Mutex s_buffer
	pthread_mutex_init(&s_buffer, NULL);

	// Cria Mutex s_buffer
	pthread_mutex_init(&s_vaga, NULL);

	// Cria Mutex s_buffer
	pthread_mutex_init(&s_item, NULL);

	// Atributos default
	pthread_attr_init(&attr);

}

// Producer Thread 
void *producer(void *id) {
   while (true){
//      pause = random (0..3)
	int pause = rand() % Sleep_Time;
	sleep (pause);

//      down (s_vaga)
	pthread_mutex_lock(&s_vaga);;
//      down (s_buffer)
	pthread_mutex_lock(&s_buffer);

//      item = random (0..999)
	int item = rand() % Number;

//      insere item no buffer
	if (insert_item(item) != 0)
		printf("p%ld produziu BUFFER CHEIO\n",((long)id)+1);

//      up (s_buffer)
	pthread_mutex_unlock(&s_buffer);
//      up (s_item)
	pthread_mutex_unlock(&s_item);


//      print item
	printf("p%ld produziu %d\n",((long)id)+1,item);
   }
}

// Consumer Thread
void *consumer(void *id) {
   while (true){

//      down (s_item)
	pthread_mutex_lock(&s_item);
//      down (s_buffer)
	pthread_mutex_lock(&s_buffer);

//	retira item do buffer
	int item = remove_item();

//      up (s_buffer)
	pthread_mutex_unlock(&s_buffer);

//      up (s_vaga)
	pthread_mutex_unlock(&s_vaga);

	if (item < 0)
		printf("\t\t\tc%ld consumiu BUFFER VAZIO\n",((long)id)+1);
	else{
//		print item
		printf("\t\t\tc%ld consumiu %d\n",((long)id)+1,item);
	}

//      pause = random (0..3)
	int pause = rand() % Sleep_Time;
	sleep (pause);
   }
}

/* Add an item to the buffer */
int insert_item(int item) {
   // When the buffer is not full add the item and increment the counter
   if(counter < Buffer_Lenght) {
	queue_t_int *novo;
        novo = (queue_t_int*) malloc (sizeof (queue_t_int));
        novo->item = item;
        novo->prev = NULL;
        novo->next = NULL;
	queue_append((queue_t **) &queue_integer, (queue_t*) novo);

	counter++;
	return 0;
   }
   else { // Buffer cheio
	return -1;
   }
}

/* Remove an item from the buffer */
int remove_item() {
   // When the buffer is not empty remove the item and decrement the counter
	if(counter > 0) {
		queue_t_int *velho;
	        velho = queue_integer;
		queue_remove ((queue_t**) &queue_integer, (queue_t*) velho);

		counter--;
		return velho->item;
	}else{ // Error buffer empty
		return -1;
	}
}

int main() {

	// Inicializando
	initializeData();
	long i;
	pthread_t thread [5];

	// Criando Threads produtoras 
	for(i = 0; i < 3; i++) {
	// Create the thread
	thread[i]=pthread_create(&tid,&attr,producer,(void *) i);
	}

	// Criando Threads consumidoras
	for(i = 0; i < 2; i++) {
		// Create the thread
		thread[3+i]=pthread_create(&tid,&attr,consumer, (void *) i);
	}

	sleep(10000);

	return 0;

}

