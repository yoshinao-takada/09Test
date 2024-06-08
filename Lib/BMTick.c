#include "BMTick.h"
#include "BMDefs.h"
#pragma region subtimer_impl
static BMSubtimer_t subtimers[BMSubtimer_POOLSIZE] =
{
    BMSubtimer_DEFAULT,
};

BMDLNode_SDECLANCHOR(subtimer_pool);

BMStatus_t BMSubtimer_SInit()
{
    BMStatus_t status = BMStatus_SUCCESS;
    do {
        if (BMDLNode_INITANCHOR(&subtimer_pool))
        {
            status = BMStatus_NORESOURCE;
            break;
        }
        for (int i = 0; i < BMSubtimer_POOLSIZE; i++)
        {
            BMDLNode_pt newnode = BMDLNode_SGet();
            if (!newnode)
            {
                status = BMStatus_NORESOURCE;
                break;
            }
            newnode->data = subtimers + i;
            BMDLNode_AddNext(&subtimer_pool, newnode);
        }
    } while (0);
    return status;
}

BMStatus_t BMSubtimer_SDeinit()
{
    return BMDLNode_DEINITANCHOR(&subtimer_pool) ?
        BMStatus_FAILURE : BMStatus_SUCCESS;
}

BMSubtimer_pt BMSubtimer_Sget()
{
    BMSubtimer_pt subtimer = NULL;
    do {
        BMDLNode_pt node = BMDLNode_GetPrev(&subtimer_pool);
        if (!node)
        {
            break;
        }
        subtimer = node->data;
        BMDLNode_SReturn(node);
    } while (0);
    return subtimer;
}

BMStatus_t BMSubtimer_SReturn(BMSubtimer_pt subtimer)
{
    BMStatus_t status = BMStatus_SUCCESS;
    do {
        BMDLNode_pt newnode = BMDLNode_SGet();
        if (!newnode)
        {
            status = BMStatus_NORESOURCE;
            break;
        }
        newnode->data = subtimer;
        BMDLNode_AddNext(&subtimer_pool, newnode);
    } while (0);
    return status;
}

BMStatus_t BMSubtimer_Tick(BMSubtimer_pt subtimer)
{
    if (subtimer->downcount && subtimer->handler)
    {        
        if (0 == (--(subtimer->downcount)))
        {
            subtimer->result = subtimer->handler(subtimer->param);
            subtimer->downcount = subtimer->interval;
        }
    }
    return subtimer->result == subtimer->param ?
        BMStatus_SUCCESS : BMStatus_FAILURE;
}

BMStatus_t BMSubtimers_Tick(BMDLNode_pt anchor)
{
    BMStatus_t status = BMStatus_SUCCESS;
    BMDLNode_LOCK(anchor);
    for (BMDLNode_pt node = anchor->next; node != anchor; node = node->next)
    {
        BMSubtimer_pt subtmr = (BMSubtimer_pt)node->data;
        status |= BMSubtimer_Tick(subtmr);
    }
    BMDLNode_UNLOCK(anchor);
    return status;
}
#pragma endregion subtimer_impl

#pragma region Systick_ISR_impl
#pragma endregion Systick_ISR_impl

