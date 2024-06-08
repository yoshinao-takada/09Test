#include "BMEv.h"

BMStatus_t BMEv_EnQ(BMEv_pt ev, BMDLNode_pt q)
{
    BMStatus_t status = BMStatus_SUCCESS;
    BMLock_LOCK(&(ev->lock));
    do {
        BMDLNode_pt newnode = BMDLNode_SGet();
        if (!newnode)
        {
            status = BMStatus_NORESOURCE;
            break;
        }
        newnode->data = ev;
        BMDLNode_AddNext(q, newnode);
        ev->listeners++;
    } while (0);
    BMLock_UNLOCK(&(ev->lock));
    return status;
}

BMEv_pt BMEv_DeQ(BMDLNode_pt q)
{
    if (BMDLNode_EMPTY(q)) return NULL;
    BMDLNode_pt node = BMDLNode_GetPrev(q);
    BMEv_pt evptr = (BMEv_pt)(node->data);
    node->data = NULL;
    BMDLNode_SReturn(node);
    return evptr;
}