#include "BMEv.h"
#include "BMTest.h"

BMStatus_t BMEv_EnQ_DeQ()
{
    BMStatus_t status = BMStatus_SUCCESS;
    do {

    } while (0);
    BMTest_ENDFUNC(status);
    return status;
}

BMStatus_t BMEvUT()
{
    BMStatus_t status = BMStatus_SUCCESS;
    do {
        if (BMStatus_SUCCESS != (status = BMEv_EnQ_DeQ()))
        {
            BMTest_ERRLOGBREAKEX("Fail in BMEv_EnQ_DeQ()");
        }
    } while (0);
    BMTest_ENDFUNC(status);
    return status;
}