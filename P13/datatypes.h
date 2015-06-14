// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DAINF UTFPR
// Versão 1.0 -- Março de 2015
//
// Estruturas de dados internas do sistema operacional

#ifndef __DATATYPES__
#define __DATATYPES__

#include <ucontext.h>
#define STACKSIZE 65536

// Estrutura que define uma tarefa
typedef struct task_t
{
		struct task_t *next;
		struct task_t *prev;
		int tid;
	// preencher quando necessário
		ucontext_t context; // Contexto da tarefa
		int prioridade_estatica;
		int prioridade_dinamica;
		short int sTask; // 0: Tarefa do Usuario, 1: Tarefa do sistema.
		int runningTime;
		int activations;
} task_t ;

// estrutura que define um semáforo
typedef struct
{
// preencher quando necessário
} semaphore_t ;

// estrutura que define um mutex
typedef struct
{
// preencher quando necessário
} mutex_t ;

// estrutura que define uma barreira
typedef struct
{
// preencher quando necessário
} barrier_t ;

// estrutura que define uma fila de mensagens
typedef struct
{
// preencher quando necessário
} mqueue_t ;

#endif
