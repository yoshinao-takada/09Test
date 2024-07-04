#include "BMFSM.h"

BMStatus_t BMFSM_Add(BMFSM_pt fsm, BMDLNode_pt anchor)
{
    BMStatus_t status = BMStatus_SUCCESS;
    do {
        BMDLNode_pt newnode = BMDLNode_SGet();
        if (!newnode)
        {
            status = BMStatus_NORESOURCE;
            break;
        }
        newnode->data = fsm;
        BMDLNode_AddNext(anchor, newnode);
    } while (0);
    return status;
}

BMStatus_t BMFSM_Remove(BMFSM_pt fsm, BMDLNode_pt anchor)
{
    BMStatus_t status = BMStatus_SUCCESS;
    do {
        BMDLNode_pt found = BMDLNode_FIND(anchor, fsm);
        if (!found)
        {
            status = BMStatus_NOTFOUND;
            break;
        }
        BMDLNode_Remove(anchor, found);
        BMDLNode_SReturn(found);
    } while (0);
    return status;
}