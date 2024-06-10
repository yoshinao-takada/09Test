#include "BMRingBuf.h"
#include "BMDLNode.h"

// static pool's raw buffer
static uint8_t rb_buf[BMRingBuf_POOLSIZE * BMRingBuf_BUFSIZE];

// static pool's elements
static BMRingBuf_t rb[BMRingBuf_POOLSIZE] = {
    { BMRBBase_DEFAULT(BMRingBuf_BUFSIZE), rb_buf },
};

// static pool's anchor
BMDLNode_SDECLANCHOR(rb_anchor);

BMStatus_t BMRingBuf_SInit()
{
    BMStatus_t status = BMStatus_SUCCESS;
    BMDLNode_INITANCHOR(&rb_anchor);
    for (int i = 1; i < BMRingBuf_POOLSIZE; i++)
    {
        memcpy(rb + i, rb, sizeof(BMRingBuf_t));
        rb[i].buf += i * BMRingBuf_BUFSIZE;
        BMDLNode_pt newnode = BMDLNode_SGet();
        if (!newnode)
        {
            status = BMStatus_NORESOURCE;
            break;
        }
        newnode->data = rb + i;
        BMDLNode_AddNext(&rb_anchor, newnode);
    }
    return status;
}

/*!
\brief Deinitialize the static pool.
*/
BMStatus_t BMRingBuf_SDeinit()
{
    return BMDLNode_DEINITANCHOR(&rb_anchor) ?
        BMStatus_FAILURE : BMStatus_SUCCESS;
}

/*!
\brief Get a ring buffer instance from the static pool.
*/
BMRingBuf_pt BMRingBuf_SGet()
{
    BMDLNode_pt node = BMDLNode_GetNext(&rb_anchor);
    if (!node)
    {
        return NULL;
    }
    BMDLNode_SReturn(node);
    return (BMRingBuf_pt)node->data;
}

/*!
\brief Return a ring buffer instance into the static pool.
*/
BMStatus_t BMRingBuf_SReturn(BMRingBuf_pt rbptr)
{
    BMDLNode_pt newnode = BMDLNode_SGet();
    if (!newnode) return BMStatus_NORESOURCE;
    newnode->data = rbptr;
    BMDLNode_AddNext(&rb_anchor, newnode);
    return BMStatus_SUCCESS;
}
