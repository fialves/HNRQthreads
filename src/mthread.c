//
//  mthread.c
//


#include <stdio.h>
#include <ucontext.h>
#include "../include/mthread.h"
#include "../include/mdata.h"

int lastTID = 0; // TODO: iniciar o id na funcao
queueDescriptor_t highPriorityQueue;
queueDescriptor_t mediumPriorityQueue;
queueDescriptor_t lowPriorityQueue;
TCB_t mainThread[QUEUE_COUNT];
ThreadController_t controllerQueue;

ucontext_t nextContext;

//int minit(void){
//    lastTID = 0;
//
//    highPriorityQueue.beginning = NULL;
//    mediumPriorityQueue.beginning = NULL;
//    mediumPriorityQueue.beginning = NULL;
//
//    mainThread[0] = highPriorityQueue.beginning;
//    mainThread[1] = mediumPriorityQueue.beginning;
//    mainThread[2] = lowPriorityQueue.beginning;
//
//    ThreadController_t controllerQueue;
//    controllerQueue.queues = &mainThread;
//
//}
//
//int mcreatemain(TCB_t *tcb){
//    tcb->tid = lastTID;
//    tcb->state = THREAD_STATE_CREATED;
//    tcb->prio = 0;
//    // FIXME: contexto ?
//    tcb->next = NULL;
//    tcb->prev = NULL;
//
//    return 0; // FIXME: What's the return!?
//
//}

int mcreate (int prio, void (*start)(void*), void *arg){
    char stack[SIGSTKSZ];
    TCB_t newThread;

    if (prio < 0 || prio > 2) {
        return -1;
    }
    int executed = 0;
    newThread.tid = ++lastTID;
    newThread.prio = prio;

    newThread.next = NULL;
    newThread.prev = NULL;

    getcontext(&(newThread.context));
    newThread.context.uc_stack.ss_sp = stack;
    newThread.context.uc_stack.ss_size = sizeof(stack);
    newThread.context.uc_link = &nextContext;
    makecontext(&(newThread.context),(void (*)(void))start, 1, arg);

    getcontext(&nextContext);

    // TODO: trocar o codigo abaixo pelo escalonador
    if(executed==0){
        executed = 1;
        setcontext(&(newThread.context));
    }

    // TODO: adicionar thread na fila de aptos

    return newThread.tid;
}
