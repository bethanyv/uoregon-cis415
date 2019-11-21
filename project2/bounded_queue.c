#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "bounded_queue.h"

#define MAXENTRIES 100

struct bounded_queue
{
        pthread_mutex_t topic_lock;
        int size;       // capacity
        //void **buffer;  // storage
        long long head; // 
        long long tail; //
        struct topicentry circular_buffer[MAXENTRIES];
        int topic_counter;
};

int RoundIDToBufferIndex(int size, long long index)
{
        long long value = (index % ((long long)size));
        return (int)value;
}

BoundedQueue *BB_MallocBoundedQueue(long size)
{
        struct bounded_queue *returnValue = NULL;
        returnValue.size = size;
        returnValue[0].head = 0;
        returnValue[0].tail = 0;
        returnValue[0].topic_counter = 0;
        return (BoundedQueue *)returnValue;
}

long long BB_TryEnqueue(struct bounded_queue *queue,void *item)
{
        long long returnValue = -1;
}

int BB_TryDequeue(struct bounded_queue *queue,long long id)
{
        int  returnValue = 0;
        return returnValue;
}

long long BB_GetFront(struct bounded_queue *queue)
{
        long long returnValue = -1;
        if(!BB_IsEmpty(queue))
        {
                returnValue = queue->head-1;
        }
        return returnValue;
}

long long BB_GetBack(struct bounded_queue *queue)
{
        long long returnValue = -1;
        return returnValue;
}

int BB_GetCount(struct bounded_queue *queue)
{
        long long returnValue = 0;
        
        return (int)returnValue;
}

int BB_IsIdValid(struct bounded_queue *queue,long long id)
{
        int returnValue = 0;  
        return returnValue;
}

void *BB_GetItem(struct bounded_queue *queue,long long id)
{
        void *returnValue = NULL;
        return returnValue;
}

int BB_IsFull(struct bounded_queue *queue)
{
        int returnValue = 0;
        return returnValue;
}

int BB_IsEmpty(struct bounded_queue *queue)
{
        int returnValue = 0;
        return returnValue;
}

void BB_FreeBoundedQueue(struct bounded_queue *queue)
{
}

