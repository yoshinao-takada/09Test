#include "BMFSM.h"
#include "BMTest.h"

BMStatus_t BMFSM_UpDownCtr();

BMStatus_t BMFSMUT()
{
    BMDLNode_InitPool();
    BMStatus_t status = BMStatus_SUCCESS;
    do {
        if (BMStatus_SUCCESS != (status = BMFSM_UpDownCtr()))
        {
            BMTest_ERRLOGBREAKEX("Fail in BMFSM_UpDownCtr()");
        }
    } while (0);
    BMTest_ENDFUNC(status);
    BMDLNode_DeinitPool();
    return status;
}

typedef struct {
    uint16_t max;
    uint16_t ctr;
} BMUDCCtx_t, *BMUDCCtx_pt;

static BMStateResult_t UDCtr_Ini(BMFSM_pt fsm, BMEv_pt ev);
static BMStateResult_t UDCtr_Up(BMFSM_pt fsm, BMEv_pt ev);
static BMStateResult_t UDCtr_Down(BMFSM_pt fsm, BMEv_pt ev);

BMStatus_t BMFSM_UpDownCtr()
{
    BMStatus_t status = BMStatus_SUCCESS;
    // Declare the FSM.
    BMUDCCtx_t ctx = { 3, 0 };
    BMFSM_DECL(udctr, UDCtr_Ini, &ctx);
    // init input queue and downstream input queue list.
    BMFSM_INIT(&udctr);
    // init the event instance.
    BMEv_t ev = BMEv_INIOBJ(BMEvId_TICK, NULL);
    BMEv_INIT(&ev);
    do {
        status = BMFSM_PUTEV(&udctr, &ev);
        if (status || (ev.listeners != 1) || (BMDLNode_Count(&udctr.iq) != 1))
        {
            status = BMStatus_FAILURE;
            BMTest_ERRLOGBREAKEX("Fail in BMFSM_PUTUT()");
        }
        BMFSM_CRUNCH(&udctr);
        if ((udctr.state != UDCtr_Up) || (ev.listeners != 0) ||
            (BMDLNode_Count(&udctr.iq) != 0))
        {
            status = BMStatus_FAILURE;
            BMTest_ERRLOGBREAKEX("Fail in BMFSM_CRUNCH()");
        }
        for (int i = 0; i < 3; i++)
        {
            if (BMStatus_SUCCESS != (status = BMFSM_PUTEV(&udctr, &ev)))
            {
                BMTest_ERRLOGBREAKEX("Fail in BMFSM_PUTUT()");
            }
        }
        if (status) break;
        if ((ev.listeners != 3) || (BMDLNode_Count(&udctr.iq) != 3))
        {
            status = BMStatus_FAILURE;
            BMTest_ERRLOGBREAKEX("Fail in BMFSM_PUTUT()");
        }
        for (int i = 0; i < 3; i++)
        {
            BMFSM_CRUNCH(&udctr);
        }
        if ((udctr.lastResult != BMStateResult_TRANSIT) ||
            (ctx.ctr != 3) || (udctr.state != UDCtr_Down))
        {
            status = BMStatus_FAILURE;
            BMTest_ERRLOGBREAKEX("Fail in BMFSM_CRUNCH()");
        }
    } while (0);
    BMEv_DEINIT(&ev);
    BMFSM_DEINIT(&udctr);
    BMTest_ENDFUNC(status);
    return status;
}

static BMStateResult_t UDCtr_Ini(BMFSM_pt fsm, BMEv_pt ev)
{
    fsm->lastResult = BMStateResult_TRANSIT;
    fsm->state = UDCtr_Up;
    return fsm->lastResult;
}

static BMStateResult_t UDCtr_Up(BMFSM_pt fsm, BMEv_pt ev)
{
    fsm->lastResult = BMStateResult_ACCEPT;
    BMUDCCtx_pt ctx = (BMUDCCtx_pt)fsm->ctx;
    if (++(ctx->ctr) == (ctx->max))
    {
        fsm->lastResult = BMStateResult_TRANSIT;
        fsm->state = UDCtr_Down;
    }
    return fsm->lastResult;
}

static BMStateResult_t UDCtr_Down(BMFSM_pt fsm, BMEv_pt ev)
{
    fsm->lastResult = BMStateResult_ACCEPT;
    BMUDCCtx_pt ctx = (BMUDCCtx_pt)fsm->ctx;
    if (--(ctx->ctr) == 0)
    {
        fsm->lastResult = BMStateResult_TRANSIT;
        fsm->state = UDCtr_Up;
    }
    return fsm->lastResult;
}
