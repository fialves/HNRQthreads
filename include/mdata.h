/*
 * mdata.h: arquivo de inclusão de uso apenas na geração da libmthread
 *
 * Esse arquivo pode ser modificado. ENTRETANTO, deve ser utilizada a TCB fornecida.
 *
 */
#ifndef __mdata__
#define __mdata__

#include <stdio.h>
#include <ucontext.h>

/* Thread states */
#define THREAD_STATE_CREATED 0
#define THREAD_STATE_READY 1
#define THREAD_STATE_EXECUTING 2
#define THREAD_STATE_BLOCKED 3
#define THREAD_STATE_TERMINATED 4

/* Thread priorities */
#define THREAD_PRIORITY_HIGH 0
#define THREAD_PRIORITY_MEDIUM 1
#define THREAD_PRIORITY_LOW 2

/* Number of queues */
#define QUEUE_COUNT 3

/* Main Thread TID */
#define MAIN_THREAD_TID 0

/* NÃO ALTERAR ESSA struct */
typedef struct TCB {
	int  tid;		// identificador da thread
	int  state;	// estado em que a thread se encontra
					// 0: Criação; 1: Apto; 2: Execução; 3: Bloqueado e 4: Término
	int  prio;		// prioridade da thread (0:alta; 1: média, 2:baixa)
	ucontext_t   context;	// contexto de execução da thread (SP, PC, GPRs e recursos)
	struct TCB   *prev;		// ponteiro para o TCB anterior da lista
	struct TCB   *next;		// ponteiro para o próximo TCB da lista
} TCB_t;

/*
	queueDescriptor: estrutura que serve como descritor para cada uma das filas de prioridade
	possui um ponteiro para o início (extração) e outro para o fim (inserção)
*/
typedef struct queueDescriptor{
	TCB_t *beginning;
	TCB_t *end;
}queueDescriptor_t;

/*
	threadController: estrutura que controla as threads
*/
typedef struct ThreadController
{
	TCB_t *queues; //vetor com três posições; uma para cada fila de prioridade
}ThreadController_t;

#endif
