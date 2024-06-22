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
#pragma endregion param_and_handler_for_TickUT

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

#pragma region params_and_handlers_for_BMSubtimers_TickUT
static const uint16_t intervals[] = { 4, 3, 2 };
typedef struct {
    uint16_t count;
    uint16_t delta;
} CountAndDelta_t, *CountAndDelta_pt;

void* handler1(void* param)
{
    CountAndDelta_pt param_ = (CountAndDelta_pt)param;
    param_->count += param_->delta;
    return param;
}

static BMThread_f handlers[] = 
{
    handler1, handler1, handler1
};

static CountAndDelta_t params1[] = 
{
    { 0, 2 }, { 0, 3 }, { 0, 4 }
};

static const uint16_t PARAMS1_END[] = { 4, 9, 20 };

static BMStatus_t Params1_MatchEND()
{
    int match =
        (params1[0].count == PARAMS1_END[0]) &&
        (params1[1].count == PARAMS1_END[1]) &&
        (params1[2].count == PARAMS1_END[2]);
    return match ? BMStatus_SUCCESS : BMStatus_FAILURE;
}

#pragma endregion params_and_handlers_for_BMSubtimers_TickUT
BMStatus_t BMSubtimers_TickUT()
{
    BMStatus_t status = BMStatus_SUCCESS;
    BMDLNode_DECLANCHOR(subtimers);
    BMDLNode_INITANCHOR(&subtimers);
    BMSubtimer_pt subtimer_mon[BMArray_SIZE(params1)];
    do {
        for (int i = 0; i < BMArray_SIZE(params1); i++)
        {
            BMSubtimer_pt subtimer = BMSubtimer_SGet();
            if (!subtimer) 
            {
                status = BMStatus_FAILURE;
                BMTest_ERRLOGBREAKEX("Fail in BMSubtimer_SGet()");
            }
            BMSubtimer_SET_COUNTER(subtimer, intervals[i], intervals[i]);
            BMSubtimer_SET_HANDLER(subtimer, handlers[i], (void*)&params1[i]);
            subtimer_mon[i] = subtimer;
            status = BMSubtimers_Add(&subtimers, subtimer);
        }
        if (status) break;

        for (int i = 0; i < 10; i ++)
        {
            status = BMSubtimers_Tick(&subtimers);
            if (status) 
            {
                BMTest_ERRLOGBREAKEX("Fail in BMSubtimers_Tick()");
            }
            // printf("%i: (%u, %u) (%u, %u) (%u, %u)\n", i,
            //     subtimer_mon[0]->downcount, params1[0].count,
            //     subtimer_mon[1]->downcount, params1[1].count,
            //     subtimer_mon[2]->downcount, params1[2].count);
        }
        // printf("END: (%u, %u) (%u, %u) (%u, %u)\n", 
        //     subtimer_mon[0]->downcount, params1[0].count,
        //     subtimer_mon[1]->downcount, params1[1].count,
        //     subtimer_mon[2]->downcount, params1[2].count);
        if (BMStatus_SUCCESS != (status = Params1_MatchEND()))
        {
            BMTest_ERRLOGBREAKEX("Fail in Params1_MatchEND()");
        }
        for (int i = 0; i < BMArray_SIZE(subtimer_mon); i++)
        {
            status = BMSubtimers_Remove(&subtimers, subtimer_mon[i]);
            if (status)
            {
                BMTest_ERRLOGBREAKEX("%d: Fail in BMSubtimers_Remove()", i);
            }
        }
    } while (0);
    BMDLNode_DEINITANCHOR(&subtimers);
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
        if (BMStatus_SUCCESS != (status = BMSubtimers_TickUT()))
        {
            BMTest_ERRLOGBREAKEX("Fail in BMSubtimers_TickUT()");
        }
        if (BMStatus_SUCCESS != (status = BMSubtimer_SDeinit()))
        {
            BMTest_ERRLOGBREAKEX("Fail in BMSubtimer_SDeinit()");
        }
    } while (0);
    BMTest_ENDFUNC(status);
    return status;
}