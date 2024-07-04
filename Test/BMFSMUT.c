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
} BMUDCtrCtx_t, *BMUDCtrCtx_pt;

typedef struct {
    BMFSM_t base;
    BMUDCtrCtx_t ctx;
} BMUDCtr_t, *BMUDCtr_pt;

#define BMUDCtr_DECL(_varname, _state, _max, _ctr) \
    BMUDCtr_t _varname = { BMFSM_INIOBJ(_varname.base, _state), \
    { _max, _ctr } }

static BMStateResult_t UDCtr_Ini(BMFSM_pt fsm, BMEv_pt ev);
static BMStateResult_t UDCtr_Up(BMFSM_pt fsm, BMEv_pt ev);
static BMStateResult_t UDCtr_Down(BMFSM_pt fsm, BMEv_pt ev);

BMStatus_t BMFSM_UpDownCtr()
{
    BMStatus_t status = BMStatus_SUCCESS;
    // Declare the FSM.
    BMUDCtr_DECL(udctr, UDCtr_Ini, 3, 0);
    // init input queue and downstream input queue list.
    BMFSM_INIT(&udctr.base);
    // init the event instance.
    BMEv_t ev = BMEv_INIOBJ(BMEvId_TICK, NULL);
    BMEv_INIT(&ev);
    do {
        status = BMFSM_PUTEV(&udctr.base, &ev);
        if (status || (ev.listeners != 1) || (BMDLNode_Count(&udctr.base.iq) != 1))
        {
            status = BMStatus_FAILURE;
            BMTest_ERRLOGBREAKEX("Fail in BMFSM_PUTUT()");
        }
        BMFSM_CRUNCH(&udctr.base);
        if ((udctr.base.state != UDCtr_Up) || (ev.listeners != 0) ||
            (BMDLNode_Count(&udctr.base.iq) != 0))
        {
            status = BMStatus_FAILURE;
            BMTest_ERRLOGBREAKEX("Fail in BMFSM_CRUNCH()");
        }
        for (int i = 0; i < 3; i++)
        {
            if (BMStatus_SUCCESS != (status = BMFSM_PUTEV(&udctr.base, &ev)))
            {
                BMTest_ERRLOGBREAKEX("Fail in BMFSM_PUTUT()");
            }
        }
        if (status) break;
        if ((ev.listeners != 3) || (BMDLNode_Count(&udctr.base.iq) != 3))
        {
            status = BMStatus_FAILURE;
            BMTest_ERRLOGBREAKEX("Fail in BMFSM_PUTUT()");
        }
        for (int i = 0; i < 3; i++)
        {
            BMFSM_CRUNCH(&udctr.base);
        }
        if ((udctr.base.lastResult != BMStateResult_TRANSIT) ||
            (udctr.ctx.ctr != 3) || (udctr.base.state != UDCtr_Down))
        {
            status = BMStatus_FAILURE;
            BMTest_ERRLOGBREAKEX("Fail in BMFSM_CRUNCH()");
        }
    } while (0);
    BMEv_DEINIT(&ev);
    BMFSM_DEINIT(&udctr.base);
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
    BMUDCtr_pt udctr = (BMUDCtr_pt)fsm;
    BMUDCtrCtx_pt ctx = (BMUDCtrCtx_pt)&udctr->ctx;
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
    BMUDCtr_pt udctr = (BMUDCtr_pt)fsm;
    BMUDCtrCtx_pt ctx = (BMUDCtrCtx_pt)&udctr->ctx;
    if (--(ctx->ctr) == 0)
    {
        fsm->lastResult = BMStateResult_TRANSIT;
        fsm->state = UDCtr_Up;
    }
    return fsm->lastResult;
}
