/**
 * File : queue.c
 * 
 * Queue
**/
#include "queue.h"

void InitQueue(SQueue_T *xStQueue)
{
    MEMSET( (void __FAR *)&xStQueue->mUdata, 0, sizeof(xStQueue->mUdata) );

    xStQueue->mUrear = 0;
    xStQueue->mUfront = 0;
}

U8 IsQueueEmpty(SQueue_T *xStQueue)
{
    if( xStQueue->mUrear == xStQueue->mUfront )
    {
        return TRUE;
    }

    return FALSE;
}

U8 IsQueueFull(SQueue_T *xStQueue)
{
    if( xStQueue->mUfront == ( xStQueue->mUrear + 1 ) % QUEUE_SIZE )
    {
        return TRUE;
    }

    return FALSE;
}

void EnQueue(SQueue_T *xStQueue, U32 xUdata)
{
    if( IsQueueFull(xStQueue) == TRUE )
    {
        return;
    }

    xStQueue->mUrear = (xStQueue->mUrear + 1) % QUEUE_SIZE;

    xStQueue->mUdata[xStQueue->mUrear] = xUdata;
}

U8 DeQueue(SQueue_T *xStQueue, U32 *xUptrData)
{
    if( IsQueueEmpty(xStQueue) == TRUE )
    {
        return FALSE;
    }

    xStQueue->mUfront = (xStQueue->mUfront + 1) % QUEUE_SIZE;

    *xUptrData = xStQueue->mUdata[xStQueue->mUfront];

    return TRUE;
}

U8 PeekQueue(SQueue_T *xStQueue, U32 *xUptrData)
{
    if( IsQueueEmpty(xStQueue) == TRUE )
    {
        return FALSE;
    }

    *xUptrData = xStQueue->mUdata[(xStQueue->mUfront + 1) % QUEUE_SIZE];

    return TRUE;
}
