#include <stdio.h>
#include "../include/mdata.h"

queueDescriptor_t queuecreate (void){
    queueDescriptor_t queue;
    queue.beginning = NULL;
    queue.end = NULL;

    return queue;
}

int queue(queueDescriptor_t *queue, TCB_t *tcb){

    tcb->next = queue->end;
    tcb->prev = NULL;
    queue->end = tcb;

    if(queue->beginning == NULL)
        queue->beginning = tcb;

    /* DEBUG */
//    printf("QUEUE_BEGINNING>TCB_TID: %d\n", (*queue->beginning).tid);
//    printf("QUEUE_END>TCB_TID: %d\n", (*queue->end).tid);

    return 0;
}

int dequeue(queueDescriptor_t *queue, TCB_t *tcb){
    (tcb->next)->prev = tcb->prev;
    (tcb->prev)->next = tcb->next;

    if(queue->end == tcb)
        queue->end = tcb->next;
    if(queue->beginning == tcb)
        queue->beginning = tcb->prev;

    tcb->next = NULL;
    tcb->prev = NULL;

    /* DEBUG */
//    printf("QUEUE_BEGINNING>TCB_TID: %d\n", (*queue->beginning).tid);
//    printf("QUEUE_END>TCB_TID: %d\n", (*queue->end).tid);

    return 0;
}

int isempty(queueDescriptor_t *queue){
    if(queue->beginning == NULL && queue->end == NULL)
        return 1;
    else if(queue->beginning == NULL || queue->end == NULL){
        printf("ERROR: Queue is broken.");
        return -1;
    } else return 0;
}

