#include "BMTick.h"
#include "BMDefs.h"
#include "BMEv.h"
#include <assert.h>
#include <memory.h>

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
#pragma region Systick_ISR_impl
// event queue of interval timer dispatcher
static BMDLNode_pt evq_tick = NULL;
static BMEv_t ev_tick = BMEv_INIOBJ(BMEvId_TICK, NULL);

static void SIGALRMHandler(int sig)
{
    if (BMEv_EnQ(&ev_tick, evq_tick))
    { // no resource in BMDLNode static pool.
        assert(0);
    }
}

BMStatus_t BMTick_Init(BMISR_pt tickptr)
{
    BMStatus_t status = BMStatus_SUCCESS;
    BMTickCtx_pt ctx = (BMTickCtx_pt)tickptr->ddctx;
    assert(ctx);
    do {
        ITimerVal_FromMillisec(&ctx->it_new, ctx->interval);
        // init sigaction
        ctx->sa_new.sa_flags = 0;
        ctx->sa_new.sa_handler = SIGALRMHandler;
        if (sigaction(SIGALRM, &ctx->sa_new, &ctx->sa_old))
        {
            status = BMStatus_FAILURE;
            break;
        }
    } while (0);
    return status;
}

BMStatus_t BMTick_Deinit(BMISR_pt tickptr)
{
    BMStatus_t status = BMStatus_SUCCESS;
    BMTickCtx_pt ctx = (BMTickCtx_pt)tickptr->ddctx;
    assert(ctx);
    do {
        if (sigaction(SIGALRM, &ctx->sa_old, &ctx->sa_new))
        {
            status = BMStatus_FAILURE;
            break;
        }
    } while (0);
    return status;
}

BMStatus_t BMTick_Start(BMISR_pt tickptr)
{
    BMStatus_t status = BMStatus_SUCCESS;
    BMTickCtx_pt ctx = (BMTickCtx_pt)tickptr->ddctx;
    assert(ctx);
    do {
        ITimerVal_FromMillisec(&ctx->it_new, ctx->interval);
        if (setitimer(ITIMER_REAL, &ctx->it_new, &ctx->it_old))
        {
            status = BMStatus_FAILURE;
            break;
        }
    } while (0);
    return status;
}

BMStatus_t BMTick_Stop(BMISR_pt tickptr)
{
    BMStatus_t status = BMStatus_SUCCESS;
    BMTickCtx_pt ctx = (BMTickCtx_pt)tickptr->ddctx;
    assert(ctx);
    do {
        if (setitimer(ITIMER_REAL, &ctx->it_old, &ctx->it_new))
        {
            status = BMStatus_FAILURE;
            break;
        }
    } while (0);
    return status;
}

#pragma endregion Systick_ISR_impl

