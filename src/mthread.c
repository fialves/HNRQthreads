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

    highPriorityQueue.beginning = &mainThread;
    highPriorityQueue.end = &mainThread;
    mediumPriorityQueue.beginning = NULL;
    mediumPriorityQueue.end = NULL;
    lowPriorityQueue.beginning = NULL;
    lowPriorityQueue.end = NULL;

    controlThread[0] = highPriorityQueue.beginning;
    controlThread[1] = mediumPriorityQueue.beginning;
    controlThread[2] = lowPriorityQueue.beginning;

    controller.queues = controlThread;

    return mcreatemain(&mainThread);
}

int mcreatemain(TCB_t *tcb){
    tcb->tid = lastTID;
    tcb->state = THREAD_STATE_CREATED;
    tcb->prio = THREAD_PRIORITY_HIGH;
    tcb->next = NULL;
    tcb->prev = NULL;
    getcontext(&(mainThread.context));

    return 0; // FIXME: What's the return!?
}

int mcreate (int prio, void (*start)(void*), void *arg){
    char stack[SIGSTKSZ];
    TCB_t newThread;

    if (prio < THREAD_PRIORITY_HIGH || prio > THREAD_PRIORITY_LOW) {
        return -1;
    } else if(lastTID == MAIN_THREAD_TID){
        minit();
        /* minit DEBUG */
        printf("\n========= MainThread is created =========\n");
        printf("controlThread: %d\nhighPriorityQueue.Beginning: %d\nmainThread: %d\nnewThread: %d(before TID receiving)\n\n",(*controlThread[0]).tid, (*highPriorityQueue.beginning).tid, mainThread.tid, newThread.tid);
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
    if(executed == 0){
        executed = 1;
        setcontext(&(newThread.context));
    }

    // TODO: adicionar thread na fila de aptos

    /* newThread DEBUG */
    printf("========= Thread is created =========\n");
    printf("TID=%d \n",newThread.tid);

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
    // TODO: implementar politica
    switch(operation){
        case 0: // thread foi criada
            executingThread->state = THREAD_STATE_READY;
            // TODO: colocar na fila conforme a prioridade
            break;
        case 1:
             executingThread->state = THREAD_STATE_BLOCKED;
             break;
        case 2:
            //executingThread->state =
            break;
    }

    return 0;
}
