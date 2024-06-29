#include "BMTick.h"
#include "BMDefs.h"
#include <assert.h>
#include <memory.h>

#pragma region subtimer_impl
static BMSubtimer_t subtimers[BMSubtimer_POOLSIZE] =
{
    BMSubtimer_DEFAULT,
};

BMDLNode_SDECLANCHOR(subtimer_pool);

void *BMSubtimer_DefaultHandler(void *param)
{
    BMSubtimerParam_pt evq = (BMSubtimerParam_pt)param;
    BMStatus_t status = BMEv_EnQ(&evq->ev, evq->oq);
    return status ? NULL : param;
}

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
    return BMDLNode_DEINITANCHOR(&subtimer_pool);
}

static const BMSubtimer_t SUBTIMER_TEMPLATE = BMSubtimer_DEFAULT;

BMSubtimer_pt BMSubtimer_SGet()
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
        memcpy(subtimer, &SUBTIMER_TEMPLATE, sizeof(BMSubtimer_t));
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
    subtimer->result = subtimer->param;
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

BMStatus_t BMSubtimers_Add(BMDLNode_pt anchor, BMSubtimer_pt subtimer)
{
    BMStatus_t status = BMStatus_SUCCESS;
    BMDLNode_pt newnode = NULL;
    do {
        if (!(newnode = BMDLNode_SGet()))
        {
            status = BMStatus_NORESOURCE;
            break;
        }
        newnode->data = (void*)subtimer;
        BMDLNode_AddPrev(anchor, newnode);
    } while (0);
    return status;
}

BMStatus_t BMSubtimers_Remove(BMDLNode_pt anchor, BMSubtimer_pt subtimer)
{
    BMStatus_t status = BMStatus_SUCCESS;
    BMDLNode_pt newnode = NULL;
    do {
        BMDLNode_pt found = BMDLNode_FIND(anchor, subtimer);
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

BMStatus_t BMTimerDispatcher_Crunch(BMTimerDispatcher_pt dispather)
{
    BMStatus_t status = BMStatus_SUCCESS;
    BMEv_pt evptr = BMEv_DeQ(&dispather->iq);
    if (evptr)
    {
        status = BMSubtimers_Tick(&dispather->subtimers);
        evptr->listeners--;
    }
    return status;
}
#pragma endregion subtimer_impl
#pragma region TIME_CONVERSION_METHODS
static void TimeVal_FromMillisec(struct timeval* t, uint16_t millisec)
{
    t->tv_sec = millisec / 1000;
    t->tv_usec = 1000 * (millisec - t->tv_sec * 1000);
}

static void ITimerVal_FromMillisec(struct itimerval* t, uint16_t millisec)
{
    TimeVal_FromMillisec(&t->it_interval, millisec);
    TimeVal_FromMillisec(&t->it_value, millisec);
}
#pragma endregion TIME_CONVERSION_METHODS

static BMTickCtx_t tickctx;
static BMEv_t tickevent = BMEv_INIOBJ(BMEvId_TICK, NULL);
static BMTimerDispatcher_t dispatcher = {
    BMDLNode_INIOBJ(&dispatcher.iq),
    BMDLNode_INIOBJ(&dispatcher.subtimers)
};

#pragma region Systick_timer_access_methods_for_test
BMTickCtx_pt TickCtx() { return &tickctx; }
BMEv_pt  TickEvent() { return &tickevent; }
BMTimerDispatcher_pt TimerDispatcher() { return &dispatcher; }
#pragma endregion Systick_timer_access_methods_for_test

static void SIGALRM_Handler(int sig)
{
    BMTimerDispatcher_ENQ(&dispatcher, &tickevent);
}

BMStatus_t BMTick_Init(uint16_t interval)
{
    BMStatus_t status = BMStatus_SUCCESS;
    do {
        BMTickCtx_t iniobj = BMTickCtx_DEFAULT(interval);
        memcpy(&tickctx, &iniobj, sizeof(BMTickCtx_t));
        BMEv_INIT(&tickevent);
        BMTimerDispatcher_INIT(&dispatcher);
        ITimerVal_FromMillisec(&tickctx.it_new, interval);
        // init sigaction
        tickctx.sa_new.sa_flags = 0;
        tickctx.sa_new.sa_handler = SIGALRM_Handler;
        if (sigaction(SIGALRM, &tickctx.sa_new, &tickctx.sa_old))
        {
            status = BMStatus_FAILURE;
            break;
        }
    } while (0);
    return status;
}

BMStatus_t BMTick_Deinit()
{
    BMStatus_t status = BMStatus_SUCCESS;
    do {
        BMEv_DEINIT(&tickevent);
        BMTimerDispatcher_DEINIT(&dispatcher);
        if (sigaction(SIGALRM, &tickctx.sa_old, &tickctx.sa_new))
        {
            status = BMStatus_FAILURE;
            break;
        }
    } while (0);
    return status;
}

BMStatus_t BMTick_Start()
{
    BMStatus_t status = BMStatus_SUCCESS;
    do {
        if (setitimer(ITIMER_REAL, &tickctx.it_new, &tickctx.it_old))
        {
            status = BMStatus_FAILURE;
            break;
        }
    } while (0);
    return status;
}

BMStatus_t BMTick_Stop()
{
    BMStatus_t status = BMStatus_SUCCESS;
    do {
        if (setitimer(ITIMER_REAL, &tickctx.it_old, &tickctx.it_new))
        {
            status = BMStatus_FAILURE;
            break;
        }
    } while (0);
    return status;
}

BMStatus_t BMTick_AddSubtimer(BMSubtimer_pt subtimer)
{
    return BMSubtimers_Add(&dispatcher.subtimers, subtimer);
}

BMStatus_t BMTick_RemoveSubtimer(BMSubtimer_pt subtimer)
{
    return BMSubtimers_Remove(&dispatcher.subtimers, subtimer);
}
