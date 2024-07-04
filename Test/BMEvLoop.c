#include "BMEv.h"
#include "BMTick.h"
#include "BMFSM.h"
#include "BMDefs.h"
#include "BMTest.h"

/*!
\brief Initialize an instance of BMtimerDispatcher_t and several instances of
    BMFSM_t. The instances of BMFSM_t are receive events from the instance
    of BMTimerDispatcher_t.
*/
BMStatus_t BMEvLoop_Init()
{
    BMStatus_t status = BMStatus_SUCCESS;
    do {
    } while (0);
    BMTest_ENDFUNC(status);
    return status;
}

BMStatus_t BMEvLoop_Deinit()
{
    BMStatus_t status = BMStatus_SUCCESS;
    do {
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