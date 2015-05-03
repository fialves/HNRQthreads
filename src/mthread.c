//
//  mthread.c
//


#include <stdio.h>
#include <ucontext.h>
#include "../include/mthread.h"
#include "../include/mdata.h"

int lastTID;
queueDescriptor_t highPriorityQueue;
queueDescriptor_t mediumPriorityQueue;
queueDescriptor_t lowPriorityQueue;
ThreadController_t controller;
TCB_t *controlThread[QUEUE_COUNT];

TCB_t mainThread;
TCB_t *executingThread;

ucontext_t nextContext;

int minit(void){
    lastTID = MAIN_THREAD_TID;

    highPriorityQueue = queuecreate();
    mediumPriorityQueue = queuecreate();
    lowPriorityQueue = queuecreate();

    mcreatemain(&mainThread);

    getcontext(&nextContext); // O contexto sera controlado pelo escalonador

    controlThread[0] = highPriorityQueue.beginning;
    controlThread[1] = mediumPriorityQueue.beginning;
    controlThread[2] = lowPriorityQueue.beginning;

    controller.queues = controlThread;

    return lastTID;
}

int mcreatemain(TCB_t *tcb){
    tcb->tid = lastTID;
    tcb->state = THREAD_STATE_CREATED;
    tcb->prio = THREAD_PRIORITY_HIGH;
    tcb->next = NULL;
    tcb->prev = NULL;

    queue(&highPriorityQueue,&mainThread);

    getcontext(&(mainThread.context));

    return tcb->tid;
}

int mcreate (int prio, void (*start)(void*), void *arg){
    char stack[SIGSTKSZ];
    TCB_t newThread;

    if (prio < THREAD_PRIORITY_HIGH || prio > THREAD_PRIORITY_LOW) {
        return -1;
    } else if(lastTID == MAIN_THREAD_TID){
        minit();
        /* minit DEBUG */
//        printf("\n========= MainThread is created =========\n");
//        printf(">> TID test:\n");
//        printf("controlThread: %d\n",(*controlThread[0]).tid);
//        printf("highPriorityQueue.Beginning: %d\n", (*highPriorityQueue.beginning).tid);
//        printf("mainThread: %d\n", mainThread.tid);
//        printf("newThread: %d(before TID receiving)\n\n", newThread.tid);
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

    getcontext(&nextContext); // TODO: nextContext  apontar para o escalonador para que ele diga quem sera o proximo contexto em execucao

    // TODO: adicionar thread na fila de aptos;

    if(executed == 0){
        executed = 1;
        setcontext(&(newThread.context));
    }

    // TODO: escalonador

    /* newThread DEBUG */
//    printf("========= Thread is created =========\n");
//    printf("TID=%d \n",newThread.tid);

    return newThread.tid;
}

int myield(void){
    // TODO: escalonador passa a bola
    return 0;
}

int mwait(int tid){
    // TODO: enquanto thread com esse tid nao terminar, espera
    return 0;
}

int scheduler(int operation){
    // TODO: nextContext apontar pro proximo contexto
    // TODO: implementar politica
    switch(operation){
        case OPERATION_CREATE:
            executingThread->state = THREAD_STATE_CREATED;
            // TODO: colocar na fila conforme a prioridade
            break;
        case OPERATION_BLOCK:
            executingThread->state = THREAD_STATE_BLOCKED;
            // TODO: colocar na fila conforme a prioridade
            break;
        case OPERATION_DISPATCH:
            executingThread->state = THREAD_STATE_EXECUTING;
            break;
        case OPERATION_YIELD:
            executingThread->state = THREAD_STATE_READY;
            break;
        case OPERATION_FINISH:
            executingThread->state = THREAD_STATE_TERMINATED;
            break;
        //default:
            // TODO: ERRO!
    }

    return 0;
}
