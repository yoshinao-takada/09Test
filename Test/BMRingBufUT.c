#include "BMRingBuf.h"
#include "BMTest.h"
#include "BMDLNode.h"
#include <stdlib.h>

BMStatus_t BMRingBufPoolUT()
{
    BMStatus_t status = BMStatus_SUCCESS;
    BMRingBuf_pt rbs[2 * BMRingBuf_STATIC_POOL_SIZE];
    uint16_t count = 0;
    do {
        while (rbs[count] = BMRingBuf_SGet())
        {
            count++;
        }
        if (count != BMRingBuf_STATIC_POOL_SIZE)
        {
            status = BMStatus_FAILURE;
            BMTest_ERRLOGBREAKEX(
                "(count != BMRingBuf_STATIC_POOL_SIZE)");
        }
        for (int i = 0; i < count; i++)
        {
            if ((rbs[i]->base.count != BMRingBuf_RB_SIZE) ||
                (rbs[i]->base.rdidx != 0) ||
                (rbs[i]->base.wridx != 0))
            {
                status = BMStatus_FAILURE;
                BMTest_ERRLOGBREAKEX(
                    "ring buffer initialize error");
            }
            BMRingBuf_SReturn(rbs[i]);
        }
        if (status) break;
        count = 0;
        while (rbs[count] = BMRingBuf_SGet())
        {
            count++;
        }
        if (count != BMRingBuf_STATIC_POOL_SIZE)
        {
            status = BMStatus_FAILURE;
            BMTest_ERRLOGBREAKEX(
                "(count != BMRingBuf_STATIC_POOL_SIZE)");
        }
        for (int i = 0; i < count; i++)
        {
            if ((rbs[i]->base.count != BMRingBuf_RB_SIZE) ||
                (rbs[i]->base.rdidx != 0) ||
                (rbs[i]->base.wridx != 0))
            {
                status = BMStatus_FAILURE;
                BMTest_ERRLOGBREAKEX(
                    "ring buffer initialize error");
            }
            BMRingBuf_SReturn(rbs[i]);
        }
    } while (0);
    BMTest_ENDFUNC(status);
    return status;
}

static uint8_t TEST_DATA[1024];

static void FillRand(uint8_t* bytes, uint16_t count)
{
    for (uint16_t i = 0; i < count; i++)
    {
        bytes[i] = (uint8_t)rand();
    }
}

#define NEXT_INDEX(_idx) (((_idx) + 1) & (sizeof(TEST_DATA) - 1))
static int g_index = 0;

BMStatus_t BMRingBufPutGetUTCore(BMRingBuf_pt rb, const uint8_t* bytes)
{
    BMStatus_t status = BMStatus_SUCCESS;
    uint8_t getbyte;
    do {
        int wr_index = g_index;
        while (BMStatus_SUCCESS == BMRingBuf_Put(rb, TEST_DATA + wr_index)) 
        {
            wr_index = NEXT_INDEX(wr_index);
        }
        for (; g_index != wr_index; g_index = NEXT_INDEX(g_index))
        {
            if (BMStatus_SUCCESS != BMRingBuf_Get(rb, &getbyte))
            {
                BMTest_ERRLOGBREAKEX(
                    "(BMStatus_SUCCESS != BMRingBuf_Get(rb, &getbyte))");
            }
            if (getbyte != TEST_DATA[g_index])
            {
                status = BMStatus_FAILURE;
                BMTest_ERRLOGBREAKEX(
                    "(getbyte != TEST_DATA[index])");
            }
        }
        if (BMStatus_SUCCESS == BMRingBuf_Get(rb, &getbyte))
        {
            status = BMStatus_FAILURE;
            BMTest_ERRLOGBREAKEX("BMRingBUf_Get() fail to detect empty.");
        }
    } while (0);
    BMTest_ENDFUNC(status);
    return status;
}


BMStatus_t BMRingBufPutGetUT()
{
    BMStatus_t status = BMStatus_SUCCESS;
    FillRand(TEST_DATA, sizeof(TEST_DATA));
    do {
        BMRingBuf_pt rb = BMRingBuf_SGet();
        if (!rb) 
        {
            BMTest_ERRLOGBREAKEX("Fail in BMRingBuf_Sget()");
        }
        for (int i = 0; i < 10; i++)
        {
            if (BMStatus_SUCCESS != 
                (status = BMRingBufPutGetUTCore(rb, TEST_DATA)))
            {
                BMTest_ERRLOGBREAKEX(
                    "Fail in BMRingBufPutGetUTCore() i = %d", i);
            }
        }
        BMRingBuf_SReturn(rb);
    } while (0);
    BMTest_ENDFUNC(status);
    return status;
}

BMStatus_t BMRingBufUT()
{
    BMStatus_t status = BMStatus_SUCCESS;
    BMDLNode_InitPool();
    FillRand(TEST_DATA, sizeof(TEST_DATA));
    do {
        if (BMStatus_SUCCESS != (status = BMRingBuf_SInit()))
        {
            BMTest_ERRLOGBREAKEX("Fail in BMRingBuf_SInit()");
        }
        if (BMStatus_SUCCESS != (status = BMRingBufPoolUT()))
        {
            BMTest_ERRLOGBREAKEX("Fail in BMRingBufPoolUT()");
        }
        if (BMStatus_SUCCESS != (status = BMRingBufPutGetUT()))
        {
            BMTest_ERRLOGBREAKEX("Fail in BMRingBufPutGetUT()");
        }
        if (BMStatus_SUCCESS != (status = BMRingBuf_SDeinit()))
        {
            BMTest_ERRLOGBREAKEX("Fail in BMRingBuf_SDeinit()");
        }
    } while (0);
    BMDLNode_DeinitPool();
    BMTest_ENDFUNC(status);
    return status;
}