/**
 * File : queue.h
 * 
 * Queue
**/
#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "prj_type.h"

#define QUEUE_SIZE      (20)

typedef struct _queue_
{
    U32 mUdata[QUEUE_SIZE];
    U8  mUrear;
    U8  mUfront;
} SQueue_T;

void InitQueue(SQueue_T *xStQueue);

U8 IsQueueEmpty(SQueue_T *xStQueue);
U8 IsQueueFull(SQueue_T *xStQueue);

void EnQueue(SQueue_T *xStQueue, U32 xUdata);
U8 DeQueue(SQueue_T *xStQueue, U32 *xUptrData);
U8 PeekQueue(SQueue_T *xStQueue, U32 *xUptrData);

#endif /* __QUEUE_H__ */
