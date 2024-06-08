#include "BMLinBuf.h"
static uint8_t lb_contents[BMLinBuf_POOLSIZE * BMLinBuf_BUFSIZE];

static BMLinBuf_t lb[BMLinBuf_POOLSIZE] = {
    BMLinBuf_DEFAULT(lb_contents, BMLinBuf_BUFSIZE),
};

BMDLNode_SDECLANCHOR(lb_pool);


BMStatus_t BMLinBuf_SInit()
{
    BMStatus_t  status = BMDLNode_INITANCHOR(&lb_pool) ?
            BMStatus_NORESOURCE : BMStatus_SUCCESS;
    for (int i = 1; i < BMLinBuf_POOLSIZE; i++)
    {
        lb[i].buf = lb[i-1].buf + BMLinBuf_BUFSIZE;
        lb[i].filled = lb[i].crunched = 0;
        lb[i].count = BMLinBuf_BUFSIZE;
        BMDLNode_pt newnode = BMDLNode_SGet();
        if (!newnode)
        {
            status = BMStatus_NORESOURCE;
            break;
        }
        newnode->data = lb + i;
        BMDLNode_AddNext(&lb_pool, newnode);
    }
    return status;
}

/*!
\brief deinitialize the static pool.
*/
BMStatus_t BMLinBuf_SDeinit()
{
    return  BMDLNode_DEINITANCHOR(&lb_pool) ?
        BMStatus_FAILURE : BMStatus_SUCCESS;
}

/*!
\brief Get an instance of linear buffer from the static pool.
*/
BMLinBuf_pt BMLinBuf_SGet()
{
    BMLinBuf_pt linbuf = NULL;
    do {
        BMDLNode_pt node = BMDLNode_GetPrev(&lb_pool);
        if (!node)
        {
            break;
        }
        linbuf = node->data;
        BMDLNode_SReturn(node);
    } while (0);
    return linbuf;
}

/*!
\brief Return an instance of a linear buffer into the static pool.
*/
BMStatus_t BMLinBuf_SReturn(BMLinBuf_pt bufptr)
{
    BMStatus_t status = BMStatus_SUCCESS;
    do {
        BMDLNode_pt newnode = BMDLNode_SGet();
        if (!newnode)
        {
            status = BMStatus_NORESOURCE;
            break;
        }
        bufptr->crunched = bufptr->filled = 0;
        newnode->data = bufptr;
        BMDLNode_AddNext(&lb_pool, newnode);
    } while (0);
    return status;
}
