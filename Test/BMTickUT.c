#include "BMTest.h"
#include "BMTick.h"
#include <memory.h>

BMStatus_t BMSubtimer_SGetSReturn()
{
    BMStatus_t status = BMStatus_SUCCESS;
    BMSubtimer_pt subtimers[BMSubtimer_POOLSIZE * 2];
    int count = 0;
    do {
        while (subtimers[count] = BMSubtimer_SGet()) 
        {
            count++;
        }
        if (count != BMSubtimer_POOLSIZE)
        {
            BMTest_ERRLOGBREAKEX("(count != BMSubtimer_POOLSIZE)");
        }
        for (int i = 0; i < count; i++)
        {
            if (BMStatus_SUCCESS != (status = BMSubtimer_SReturn(subtimers[i])))
            {
                BMTest_ERRLOGBREAKEX("Fail in BMSubtimer_SReturn()");
            }
        }
        if (status) break;
        count = 0;
        while (subtimers[count] = BMSubtimer_SGet()) 
        {
            count++;
        }
        if (count != BMSubtimer_POOLSIZE)
        {
            BMTest_ERRLOGBREAKEX("(count != BMSubtimer_POOLSIZE)");
        }
        for (int i = 0; i < count; i++)
        {
            if (BMStatus_SUCCESS != (status = BMSubtimer_SReturn(subtimers[i])))
            {
                BMTest_ERRLOGBREAKEX("Fail in BMSubtimer_SReturn()");
            }
        }
    } while (0);
    BMTest_ENDFUNC(status);
    return status;
}

#pragma region param_and_handler_for_TickUT
static int param0;

static void* handler0(void* param)
{
    int* param_iptr = (int*)param;
    (*param_iptr)++;
    return (*param_iptr == 3) ? NULL : param;
}
#pragma region param_and_handler_for_TickUT

static BMStatus_t BMSubtimer_CheckDefault(BMSubtimer_pt toTest)
{
    BMSubtimer_t ref = BMSubtimer_DEFAULT;
    BMStatus_t status = BMStatus_FAILURE;
    do {
        if (ref.downcount != toTest->downcount) break;
        if (ref.interval != toTest->interval) break;
        if (ref.handler != toTest->handler) break;
        if (ref.param != toTest->param) break;
        if (ref.result != toTest->result) break;
        status = BMStatus_SUCCESS;
    } while (0);
    return status;
}

BMStatus_t BMSubtimer_TickUT()
{
    BMStatus_t status = BMStatus_SUCCESS, status2 = BMStatus_SUCCESS;
    BMSubtimer_pt subtimer = NULL;
    int i;
    do {
        subtimer = BMSubtimer_SGet();
        if (BMStatus_SUCCESS != (status = BMSubtimer_CheckDefault(subtimer)))
        {
            BMTest_ERRLOGBREAKEX("BMSubtimer_SGet() returned instance is not inititalized.");
        }
        BMSubtimer_SET_COUNTER(subtimer, 4, 3);
        BMSubtimer_SET_HANDLER(subtimer, handler0, &param0);
        param0 = 0;
        for (i = 0; (status2 == BMStatus_SUCCESS) && (i < 100); i++)
        {
            status2 = BMSubtimer_Tick(subtimer);
            // printf("i = %d, downcount = %u, status2 = %u\n",
            // i, subtimer->downcount, status2);
        }
        if (i != 11)
        {
            BMTest_ERRLOGBREAKEX("Fail to detect error in BMSubtimer_Tick()");
        }
    } while (0);
    BMTest_ENDFUNC(status);
    return status;
}


BMStatus_t BMTickUT()
{
    BMStatus_t status = BMStatus_SUCCESS;
    do {
        if (BMStatus_SUCCESS != (status = BMSubtimer_SInit()))
        {
            BMTest_ERRLOGBREAKEX("Fail in BMSubtimer_SInit()");
        }
        if (BMStatus_SUCCESS != (status = BMSubtimer_SGetSReturn()))
        {
            BMTest_ERRLOGBREAKEX("Fail in BMSubtimer_SGetSReturn()");
        }
        if (BMStatus_SUCCESS != (status = BMSubtimer_TickUT()))
        {
            BMTest_ERRLOGBREAKEX("Fail in BMSubtimer_TickUT()");
        }
        if (BMStatus_SUCCESS != (status = BMSubtimer_SDeinit()))
        {
            BMTest_ERRLOGBREAKEX("Fail in BMSubtimer_SDeinit()");
        }
    } while (0);
    BMTest_ENDFUNC(status);
    return status;
}