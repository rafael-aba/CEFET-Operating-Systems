#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "queue.h"
 #include <time.h>

#define NUM_THREADS 2
#define N 10
#define TRUE 1
#define FALSE 0
#define PREC 10

time_t start;
int qtd =0;

typedef struct queue_t_int{
   struct queue_t_int *prev ; 
   struct queue_t_int *next ; 
   int    item ;
} queue_t_int ;
 
queue_t_int elemento[N];
queue_t_int *queue_integer;
 
void print_elem (void *ptr){
   queue_t_int *elem = ptr ;
 
   if (!elem)
	return ;
 
   printf ("%d ", elem->item) ;
}

void *threadBody (void *id){ // LIVRE
     queue_t_int *novo,*velho;
     while (1){
        velho=queue_integer; 
        queue_remove ((queue_t **)&queue_integer, (queue_t *)velho); // Remove primeiro elemento

        novo=(queue_t_int*) malloc (sizeof (queue_t_int)); // Aloca espaço para nova variável
        novo->item = random() % 100;
        novo->prev= NULL;
        novo->next=NULL;

        queue_append ((queue_t **)&queue_integer, (queue_t *)novo);

        printf("thread %ld: tira %d , poe %d",(long)id,velho->item,novo->item);
        queue_print (" Fila:  ", (queue_t*) queue_integer, print_elem) ;

	qtd++;
   	if(qtd == PREC){
		printf("Realizou %f operações por segundo\n",( PREC/((float)(clock() - start)/CLOCKS_PER_SEC) ) );
		exit(0);
	} 
     }
}


int main (int argc, char *argv[])
{
  pthread_t thread [NUM_THREADS] ;
  long i, status ;
  for (i=0; i<N; i++){
      elemento[i].item = random() % 100;
      elemento[i].prev = NULL ;
      elemento[i].next = NULL ;
  }
  
  queue_integer= NULL;

  for (i=0;i<10;i++){
    queue_append((queue_t **)&queue_integer,(queue_t *)&elemento[i]);
  }
      
  for (i=0; i<NUM_THREADS; i++){
	printf ("Main: criando thread %02ld\n", i);
      
        status = pthread_create(&thread[i], NULL, threadBody, (void *) i);
      
        if (status){
             perror ("pthread_create") ;
             exit (1) ;
        }
  }
  pthread_exit (NULL) ;
}
