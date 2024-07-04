#include "BMEv.h"
#include "BMTick.h"
#include "BMFSM.h"
#include "BMDefs.h"
#include "BMTest.h"

// declare a timer dispatcher instance
static BMTimerDispatcher_DECL(tmrdsp);

static BMStateResult_t state(BMFSM_pt fsm, BMEv_pt ev)
{
    BMStateResult_t result = BMStateResult_ACCEPT;
    printf("@ %s, %d\n", __FUNCTION__, __LINE__);
    return result;
}

// declare downstream FSM instances.
static BMFSM_DECL(fsm0, state, "FSM0");
static BMFSM_DECL(fsm1, state, "FSM1");

static BMSubtimerParam_t stparam0 = BMSubtimerParam_DEFAULT(&fsm0.iq);
static BMSubtimerParam_t stparam1 = BMSubtimerParam_DEFAULT(&fsm1.iq);
static const uint16_t SUBTMR_INTERVALS[] = { 4, 7 };
static BMSubtimer_t subtimer0 = 
{ BMSubtimer_DefaultHandler, &stparam0, NULL, 
    SUBTMR_INTERVALS[0], SUBTMR_INTERVALS[0] };
static BMSubtimer_t subtimer1 = 
{ BMSubtimer_DefaultHandler, &stparam1, NULL,
    SUBTMR_INTERVALS[1], SUBTMR_INTERVALS[1] };

/*!
\brief Initialize an instance of BMtimerDispatcher_t and several instances of
    BMFSM_t. The instances of BMFSM_t are receive events from the instance
    of BMTimerDispatcher_t.
*/
BMStatus_t BMEvLoop_Init()
{
    BMStatus_t status = BMStatus_SUCCESS;
    do {
        BMTimerDispatcher_INIT(&tmrdsp);
        BMFSM_INIT(&fsm0);
        BMFSM_INIT(&fsm1);
        BMSubtimerParam_INIT(&stparam0);
        BMSubtimerParam_INIT(&stparam1);
        BMSubtimers_Add(&tmrdsp.subtimers, &subtimer0);
        BMSubtimers_Add(&tmrdsp.subtimers, &subtimer1);
    } while (0);
    BMTest_ENDFUNC(status);
    return status;
}

BMStatus_t BMEvLoop_Deinit()
{
    BMStatus_t status = BMStatus_SUCCESS;
    do {
        BMSubtimers_Remove(&tmrdsp.subtimers, &subtimer0);
        BMSubtimers_Remove(&tmrdsp.subtimers, &subtimer1);
        BMTimerDispatcher_DEINIT(&tmrdsp);
        BMFSM_DEINIT(&fsm0);
        BMFSM_DEINIT(&fsm1);
        BMSubtimerParam_DEINIT(&stparam0);
        BMSubtimerParam_DEINIT(&stparam1);
    } while (0);
    BMTest_ENDFUNC(status);
    return status;
}

void MainLoop(BMActCtx_pt ctx)
{
    while (ctx->stat == BMActStatus_ACTIVE)
    { // infinite loop until stop status or error.
        
    }
}

BMStatus_t BMEvLoop_Main()
{
    BMActCtx_t actctx = { NULL, NULL, BMActStatus_ACTIVE };
    MainLoop(&actctx);
    BMStatus_t status = actctx.stat == BMActStatus_ERRSTOP ?
        BMStatus_FAILURE : BMStatus_SUCCESS;
    BMTest_ENDFUNC(status);
    return status;
}

BMStatus_t BMEvLoop(int argc, const char* const *argv)
{
    BMStatus_t status = BMStatus_SUCCESS;
    BMDLNode_InitPool();
    do {
        if (BMStatus_SUCCESS != (status = BMEvLoop_Init()))
        {
            BMTest_ERRLOGBREAKEX("Fail in BMEvLoop_Init()");
        }
        if (BMStatus_SUCCESS != (status = BMEvLoop_Main()))
        {
            BMTest_ERRLOGBREAKEX("Fail in BMEvLoop_Main()");
        }
    } while (0);
    BMTest_ENDFUNC(status);
    BMDLNode_DeinitPool();
    return status;
}