#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include "queue.h"

#define NUM_THREADS 2
#define N 10
#define TRUE 1
#define FALSE 0

typedef struct filaint_t
{
   struct filaint_t *prev ;  
   struct filaint_t *next ;  
   int    item ;
} filaint_t ;

filaint_t elemento[N];
filaint_t *fila_teste;
filaint_t *novo,*velho;
long turn;
long flag[2];


void print_elem (void *ptr)
{
   filaint_t *elem = ptr ;

   if (!elem)
      return ;

   printf ("<%d>", elem->item) ;


}
/*
 void *threadBody (void *id){//sem controle{
	 while (1)
		{
        novo=(filaint_t*) malloc (sizeof (filaint_t));    
        velho=fila_teste;  
			  queue_remove ((queue_t **)&fila_teste, (queue_t *)velho);
			  novo->item = random() % 100;
			  novo->prev= NULL;
			  novo->next=NULL;
			  queue_append ((queue_t **)&fila_teste, (queue_t *)novo);
        assert (queue_size ((queue_t*) fila_teste) == N) ;
			  printf("thread %ld: tira %d , poe %d \n",(long)id,velho->item,novo->item);
			  queue_print ("Fila:  ", (queue_t*) fila_teste, print_elem) ;
		}
}//*/

void *threadBody (void *id){ //com controle de concorrencia
    while(1){
        while(turn != (long)id){
        //espera
        }
        //seção crítica
        novo = (filaint_t*) malloc (sizeof (filaint_t));
        velho = fila_teste;
        queue_remove ((queue_t**) &fila_teste, (queue_t*) velho);
        novo->item = random() % 100;
        novo->prev = NULL;
        novo->next = NULL;
        queue_append((queue_t **) &fila_teste, (queue_t*) novo);
        assert (queue_size ((queue_t*) fila_teste) == N) ;
        printf("\nThread %ld: tira %d, põe %d, ", (long)id, velho->item, novo->item);
        queue_print ("Fila:  ", (queue_t*) fila_teste, print_elem);
        //fim seção crítica
        if(turn == 1) turn = 0;
        else turn = 1;
         
    }
    pthread_exit (NULL) ;
}//*/
/*    
void *threadBody (void *id){ //com algoritmo de Peterson
    while(1){
        long process = (long) id;
        long other;
        if(process == 0){
            other = 1;
        }
        else
            other = 0;
        flag[process] = TRUE;
        turn = process;
        while(flag[other]  && turn == process){
        //espera
        }
        //seção crítica
        novo = (filaint_t*) malloc (sizeof (filaint_t));
        velho = fila_teste;
        queue_remove ((queue_t**) &fila_teste, (queue_t*) velho);
        novo->item = random() % 100;
        novo->prev = NULL;
        novo->next = NULL;
        queue_append((queue_t **) &fila_teste, (queue_t*) novo);
        assert (queue_size ((queue_t*) fila_teste) == N) ;
        printf("\nThread %ld: tira %d, põe %d, ",(long) id, velho->item, novo->item);
        queue_print ("Fila:  ", (queue_t*) fila_teste, print_elem);
        //fim seção crítica
        flag[process] = FALSE;
    }
    pthread_exit (NULL) ;
}//*/


int main (int argc, char *argv[])
{
  pthread_t thread [NUM_THREADS] ;
  long i, status ;
  for (i=0; i<N; i++){
      elemento[i].item = random() % 100;
      elemento[i].prev = NULL ;
      elemento[i].next = NULL ;
  }
	fila_teste= NULL;
  for (i=0;i<10;i++){
    assert(queue_size((queue_t *)fila_teste)==i);
    queue_append((queue_t **)&fila_teste,(queue_t *)&elemento[i]);
  }
     
       for (i=0; i<NUM_THREADS; i++)
       {
          printf ("Main: criando thread %02ld\n", i) ;
     
          status = pthread_create(&thread[i], NULL, threadBody, (void *) i);
     
          if (status)
          {
             perror ("pthread_create") ;
             exit (1) ;
          }
       }
       pthread_exit (NULL) ;
    }
