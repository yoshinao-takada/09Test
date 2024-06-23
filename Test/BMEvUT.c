#include "BMEv.h"
#include "BMTest.h"

BMStatus_t BMEv_EnQ_DeQ()
{
    BMStatus_t status = BMStatus_SUCCESS;
    BMEv_t ev[] = 
    {
        BMEv_INIOBJ(0, NULL),
        BMEv_INIOBJ(1, NULL),
        BMEv_INIOBJ(2, NULL),
    };
    BMDLNode_DECLANCHOR(anchor);
    do {
        if (BMStatus_SUCCESS != (status = BMDLNode_INITANCHOR(&anchor)))
        {
            BMTest_ERRLOGBREAKEX("Fail in BMDLNode_INITANCHOR()");
        }
        for (int i = 0; i < BMArray_SIZE(ev); i++)
        {
            BMEv_INIT(ev + i);
            for (int j = 0; j < 4; j++)
            {
                status = BMEv_EnQ(ev + i, &anchor);
                if (status)
                {
                    BMTest_ERRLOGBREAKEX("Fail in BMDLNode_EnQ()");
                }
            }
            if (status) break;
        }
        if (status) break;
        for (int i = 0; i < BMArray_SIZE(ev); i++)
        {
            if (ev[i].listeners != 4)
            {
                status = BMStatus_FAILURE;
                BMTest_ERRLOGBREAKEX("(ev[i].listners != 4)");
            }
        }
        if (status) break;
        for (int i = 0; i < BMArray_SIZE(ev); i++)
        {
            for (int j = 0; j < 4; j++)
            {
                BMEv_pt evptr = BMEv_DeQ(&anchor);
                if (evptr->id != i)
                {
                    status = BMStatus_FAILURE;
                    BMTest_ERRLOGBREAKEX("BMEv_DeQ()");
                }
                status = BMEv_UNREF(evptr);
                if (status)
                {
                    BMTest_ERRLOGBREAKEX("Fail in BMEv_UNREF(evptr)");
                }
            }
            BMEv_DEINIT(ev + i);
            if (status) break;
            if (ev[i].listeners)
            {
                status = BMStatus_FAILURE;
                BMTest_ERRLOGBREAKEX("ev[%d].listeners was not cleared.", i);
            }
        }
        if (status) break;
        if (BMStatus_SUCCESS != (status = BMDLNode_DEINITANCHOR(&anchor)))
        {
            BMTest_ERRLOGBREAKEX("Fail in BMDLNode_DEINITANCHOR()");
        }
    } while (0);
    BMTest_ENDFUNC(status);
    return status;
}

BMStatus_t BMEvUT()
{
    BMStatus_t status = BMStatus_SUCCESS;
    BMDLNode_InitPool();
    do {
        if (BMStatus_SUCCESS != (status = BMEv_EnQ_DeQ()))
        {
            BMTest_ERRLOGBREAKEX("Fail in BMEv_EnQ_DeQ()");
        }
    } while (0);
    BMDLNode_DeinitPool();
    BMTest_ENDFUNC(status);
    return status;
}

