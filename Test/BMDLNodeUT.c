#include "BMDLNode.h"
#include "BMTest.h"
#include "BMDefs.h"

BMStatus_t BMDLNodePoolUT()
{
    BMStatus_t status = BMStatus_SUCCESS;
    BMDLNode_DECLANCHOR(anchor);
    BMDLNode_INITANCHOR(&anchor);
    do {
        int nodeCount = 0;
        for (BMDLNode_pt node = NULL; node = BMDLNode_SGet(); nodeCount++)
        {
            BMDLNode_AddNext(&anchor, node);
        }
        if (nodeCount != BMDLNode_POOLSIZE)
        {
            BMTest_ERRLOGBREAKEX("Pool of BMDLNode has sth wrong.");
        }
        if (nodeCount != BMDLNode_Count(&anchor))
        {
            status = BMStatus_FAILURE;
            BMTest_ERRLOGBREAKEX("Fail in BMDLNode_Count(&anchor)");
        }
        for (int i = 0; i < nodeCount; i++)
        {
            BMDLNode_pt node = BMDLNode_GetPrev(&anchor);
            BMDLNode_SReturn(node);
        }
        nodeCount = 0;
        for (BMDLNode_pt node = NULL; node = BMDLNode_SGet(); nodeCount++)
        {
            BMDLNode_AddNext(&anchor, node);
        }
        if (nodeCount != BMDLNode_POOLSIZE)
        {
            BMTest_ERRLOGBREAKEX("Pool of BMDLNode has sth wrong.");
        }
        for (int i = 0; i < nodeCount; i++)
        {
            BMDLNode_pt node = BMDLNode_GetPrev(&anchor);
            BMDLNode_SReturn(node);
        }
    } while (0);
    BMDLNode_DEINITANCHOR(&anchor);
    BMTest_ENDFUNC(status);
    return status;
}

void* CONTENTS[] = {
    (void*)0, (void*)1, (void*)2, (void*)3
};

BMStatus_t BMDLNodeAddPrevGetPrev()
{
    BMStatus_t status = BMStatus_SUCCESS;
    BMDLNode_DECLANCHOR(anchor);
    BMDLNode_INITANCHOR(&anchor);
    void* toTest[BMArray_SIZE(CONTENTS)];
    do {
        for (int i = 0; i < BMArray_SIZE(CONTENTS); i++)
        {
            BMDLNode_pt node = BMDLNode_SGet();
            node->data = CONTENTS[i];
            BMDLNode_AddPrev(&anchor, node);
        }
        for (int i = 0; i < BMArray_SIZE(CONTENTS); i++)
        {
            BMDLNode_pt node = BMDLNode_GetPrev(&anchor);
            if (!node)
            {
                status = BMStatus_FAILURE;
                BMTest_ERRLOGBREAKEX("Fail in BMDLNode_GetPrev(&anchor)");
            }
            toTest[i] = node->data;
        }
        if (status) break;
        for (int i = 0; i < BMArray_SIZE(CONTENTS); i++)
        {
            if (CONTENTS[i] != toTest[BMArray_SIZE(CONTENTS) - i - 1])
            {
                status = BMStatus_FAILURE;
                BMTest_ERRLOGBREAKEX(
                    "(CONTENTS[i] != toTest[BMArray_SIZE(CONTENTS) - i - 1])"
                );
            }
        }
    } while (0);
    BMDLNode_DEINITANCHOR(&anchor);
    BMTest_ENDFUNC(status);
    return status;
}

BMStatus_t BMDLNodeAddPrevGetNext()
{
    BMStatus_t status = BMStatus_SUCCESS;
    BMDLNode_DECLANCHOR(anchor);
    BMDLNode_INITANCHOR(&anchor);
    void* toTest[BMArray_SIZE(CONTENTS)];
    do {
        for (int i = 0; i < BMArray_SIZE(CONTENTS); i++)
        {
            BMDLNode_pt node = BMDLNode_SGet();
            node->data = CONTENTS[i];
            BMDLNode_AddPrev(&anchor, node);
        }
        for (int i = 0; i < BMArray_SIZE(CONTENTS); i++)
        {
            BMDLNode_pt node = BMDLNode_GetNext(&anchor);
            if (!node)
            {
                status = BMStatus_FAILURE;
                BMTest_ERRLOGBREAKEX("Fail in BMDLNode_GetNext(&anchor)");
            }
            toTest[i] = node->data;
        }
        if (status) break;
        for (int i = 0; i < BMArray_SIZE(CONTENTS); i++)
        {
            if (CONTENTS[i] != toTest[i])
            {
                status = BMStatus_FAILURE;
                BMTest_ERRLOGBREAKEX(
                    "(CONTENTS[i] != toTest[i])"
                );
            }
        }
    } while (0);
    BMDLNode_DEINITANCHOR(&anchor);
    BMTest_ENDFUNC(status);
    return status;
}

BMStatus_t BMDLNodeAddNextGetPrev()
{
    BMStatus_t status = BMStatus_SUCCESS;
    BMDLNode_DECLANCHOR(anchor);
    BMDLNode_INITANCHOR(&anchor);
    void* toTest[BMArray_SIZE(CONTENTS)];
    do {
        for (int i = 0; i < BMArray_SIZE(CONTENTS); i++)
        {
            BMDLNode_pt node = BMDLNode_SGet();
            node->data = CONTENTS[i];
            BMDLNode_AddNext(&anchor, node);
        }
        for (int i = 0; i < BMArray_SIZE(CONTENTS); i++)
        {
            BMDLNode_pt node = BMDLNode_GetPrev(&anchor);
            if (!node)
            {
                status = BMStatus_FAILURE;
                BMTest_ERRLOGBREAKEX("Fail in BMDLNode_GetPrev(&anchor)");
            }
            toTest[i] = node->data;
        }
        if (status) break;
        for (int i = 0; i < BMArray_SIZE(CONTENTS); i++)
        {
            if (CONTENTS[i] != toTest[i])
            {
                status = BMStatus_FAILURE;
                BMTest_ERRLOGBREAKEX(
                    "(CONTENTS[i] != toTest[i)"
                );
            }
        }
    } while (0);
    BMDLNode_DEINITANCHOR(&anchor);
    BMTest_ENDFUNC(status);
    return status;
}

BMStatus_t BMDLNodeAddNextGetNext()
{
    BMStatus_t status = BMStatus_SUCCESS;
    BMDLNode_DECLANCHOR(anchor);
    BMDLNode_INITANCHOR(&anchor);
    void* toTest[BMArray_SIZE(CONTENTS)];
    do {
        for (int i = 0; i < BMArray_SIZE(CONTENTS); i++)
        {
            BMDLNode_pt node = BMDLNode_SGet();
            node->data = CONTENTS[i];
            BMDLNode_AddNext(&anchor, node);
        }
        for (int i = 0; i < BMArray_SIZE(CONTENTS); i++)
        {
            BMDLNode_pt node = BMDLNode_GetNext(&anchor);
            if (!node)
            {
                status = BMStatus_FAILURE;
                BMTest_ERRLOGBREAKEX("Fail in BMDLNode_GetNext(&anchor)");
            }
            toTest[i] = node->data;
        }
        if (status) break;
        for (int i = 0; i < BMArray_SIZE(CONTENTS); i++)
        {
            if (CONTENTS[i] != toTest[BMArray_SIZE(CONTENTS) - i - 1])
            {
                status = BMStatus_FAILURE;
                BMTest_ERRLOGBREAKEX(
                    "(CONTENTS[i] != toTest[BMArray_SIZE(CONTENTS) - i - 1])"
                );
            }
        }
    } while (0);
    BMDLNode_DEINITANCHOR(&anchor);
    BMTest_ENDFUNC(status);
    return status;
}

int zeroifmatch(const void* pv0, const void* pv1)
{
    return (int)(uint64_t)pv1 - (int)(uint64_t)pv0;
}

BMStatus_t BMDLNodeFindUT()
{
    BMStatus_t status = BMStatus_SUCCESS;
    BMDLNode_DECLANCHOR(anchor);
    BMDLNode_INITANCHOR(&anchor);
    BMDLNode_pt nodes[BMArray_SIZE(CONTENTS)];
    do {
        for (int i = 0; i < BMArray_SIZE(CONTENTS); i++)
        {
            nodes[i] = BMDLNode_SGet();
            nodes[i]->data = CONTENTS[i];
            BMDLNode_AddNext(&anchor, nodes[i]);
        }
        for (int i = 0; i < BMArray_SIZE(CONTENTS); i++)
        {
            BMDLNode_pt found = 
                BMDLNode_Find(&anchor, CONTENTS[i], zeroifmatch);
            if ((nodes[i] != found) || (found->data != CONTENTS[i]))
            {
                status = BMStatus_FAILURE;
                BMTest_ERRLOGBREAKEX(
                    "(nodes[i] != found) || (found->data != CONTENTS[i])");
            }
        }
        if (status) break;
        BMDLNode_pt found = BMDLNode_Find(&anchor, (void*)4, zeroifmatch);
        if (found)
        {
            status = BMStatus_FAILURE;
            BMTest_ERRLOGBREAKEX(
                "BMDLNode_Find() returned invalid result."
            );
        }
    } while(0);
    BMDLNode_DEINITANCHOR(&anchor);
    BMTest_ENDFUNC(status);
    return status;
}

BMStatus_t BMDLNodeUT()
{
    BMStatus_t status = BMStatus_SUCCESS;
    BMDLNode_InitPool();
   do {
        if (BMStatus_SUCCESS != (status = BMDLNodePoolUT()))
        {
            BMTest_ERRLOGBREAKEX("Fail in BMDLNodePoolUT()");
        }
        if (BMStatus_SUCCESS != (status = BMDLNodeAddPrevGetPrev()))
        {
            BMTest_ERRLOGBREAKEX("Fail in BMDLNodeAddPrevGetPrev()");
        }
        if (BMStatus_SUCCESS != (status = BMDLNodeAddPrevGetNext()))
        {
            BMTest_ERRLOGBREAKEX("Fail in BMDLNodeAddPrevGetNext()");
        }
        if (BMStatus_SUCCESS != (status = BMDLNodeAddNextGetPrev()))
        {
            BMTest_ERRLOGBREAKEX("Fail in BMDLNodeAddNextGetPrev()");
        }
        if (BMStatus_SUCCESS != (status = BMDLNodeAddNextGetNext()))
        {
            BMTest_ERRLOGBREAKEX("Fail in BMDLNodeAddNextGetNext()");
        }
        if (BMStatus_SUCCESS != (status = BMDLNodeFindUT()))
        {
            BMTest_ERRLOGBREAKEX("Fail in BMDLNodeFindUT()");
        }
    } while (0);
    BMDLNode_DeinitPool();
    BMTest_ENDFUNC(status);
    return status;
}