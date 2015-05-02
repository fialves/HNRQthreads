//
//  mthread.c
//


#include <stdio.h>
#include <ucontext.h>
#include "mthread.h"

FHtid lastTID;

int minit(void){
    lastTID = 0;


}

int mcreate (int prio, void (*start)(void*), void *arg){
    FHthread_t newThread;
    newThread.tid = getNewTID();
    newThread.parent = NULL;

    getcontext(&(newThread.context));
    // TODO: config thread context

    // TODO: adicionar thread na fila de aptos

    return newThread.tid;
}

int getNewTID(void){
    return ++lastTID;
}
