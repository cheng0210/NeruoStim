#include "command_queue.h"

void InitQueue(PQueue queue){
	queue->Front = queue->Rear = (PNODE)malloc(sizeof(NODE));    //    动态创建头节点，使队头，队尾指向该节点
	//头节点相当于哨兵节点的作用，不存储数据（区别于首节点）
	if (queue->Front == NULL) {        //    判断内存是否分配成功
		printf("创建队列，无法分配所需内存...");
		//should reboot here
	}
	queue->Front->Next = NULL;    //    初始队列头节点指针指向为空
}

uint8_t IsEmptyQueue(PQueue queue) {
    if (queue->Front == queue->Rear) {
        return 1;
    }
    else {
        return 0;
    }
}

void EnQueue(PQueue queue,char *commands) {
    PNODE P = (PNODE)malloc(sizeof(NODE));    //    创建一个新节点用于存放插入的元素
    if (P != NULL) {
    	memcpy(P->command,commands,64);
		P->Next = NULL;
		queue->Rear->Next = P;
		queue->Rear = P;
    }
}

void DeQueue(PQueue queue,char *commands) {
    if (IsEmptyQueue(queue)!=1) {
    	PNODE  P= queue->Front->Next;
    	memcpy(commands,P->command,64);
		queue->Front->Next = P->Next;
		if (queue->Rear==P)
			queue->Rear = queue->Front;
		free(P);
		P = NULL;
    }
}

void DestroyQueue(PQueue queue) {
    while (queue->Front != NULL) {
        queue->Rear = queue->Front->Next;
        free(queue->Front);
        queue->Front = queue->Rear;
    }
}
