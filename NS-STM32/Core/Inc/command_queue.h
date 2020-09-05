#ifndef command_queue_h
#define command_queue_h
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

typedef struct Node {
    char command[64];
    struct Node * Next;
}NODE, *PNODE;


typedef struct QNode {
    PNODE Front, Rear;
} Queue, *PQueue;

void InitQueue(PQueue queue);
uint8_t IsEmptyQueue(PQueue queue);
void EnQueue(PQueue queue, char *commands);
void DeQueue(PQueue queue,char *commands);
void DestroyQueue(PQueue queue);


#endif
