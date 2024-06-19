#include "BMRingBuf.h"
#include "BMDLNode.h"

// static pool's raw buffer
static uint8_t rb_buf[BMRingBuf_STATIC_POOL_SIZE * BMRingBuf_RB_SIZE];

// static pool's elements
static BMRingBuf_t rb[BMRingBuf_STATIC_POOL_SIZE] = {
    { BMRBBase_DEFAULT(BMRingBuf_RB_SIZE), rb_buf },
};

// static pool's anchor
BMDLNode_SDECLANCHOR(rb_anchor);

void BMRingBuf_Clear(BMRingBuf_pt rb)
{
    BMRBBase_LOCK(&rb->base);
    rb->base.wridx = rb->base.rdidx = 0;
    BMRBBase_UNLOCK(&rb->base);
}

BMStatus_t BMRingBuf_Put(BMRingBuf_pt rb, const uint8_t *byteptr)
{
    BMStatus_t status = BMStatus_SUCCESS;
    BMRBBase_LOCK(&rb->base);
    do {
        uint16_t next_wridx = BMRBBase_NextWridx(&rb->base);
        if (next_wridx == rb->base.rdidx)
        {
            status = BMStatus_NORESOURCE;
            break;
        }
        rb->buf[rb->base.wridx] = *byteptr;
        rb->base.wridx = next_wridx;
    } while (0);
    BMRBBase_UNLOCK(&rb->base);
    return status;
}

BMStatus_t BMRingBuf_Get(BMRingBuf_pt rb, uint8_t *byteptr)
{
    BMStatus_t status = BMStatus_SUCCESS;
    BMRBBase_LOCK(&rb->base);
    do {
        if (rb->base.rdidx == rb->base.wridx)
        {
            status = BMStatus_NORESOURCE;
            break;
        }
        *byteptr = rb->buf[rb->base.rdidx];
        rb->base.rdidx = BMRBBase_NextRdidx(&rb->base);
    } while (0);
    BMRBBase_UNLOCK(&rb->base);
    return status;
}

BMStatus_t BMRingBuf_SInit()
{
    BMStatus_t status = BMStatus_SUCCESS;
    BMDLNode_INITANCHOR(&rb_anchor);
    for (int i = 0; i < BMRingBuf_STATIC_POOL_SIZE; i++)
    {
        if (i > 0)
        {
            memcpy(rb + i, rb, sizeof(BMRingBuf_t));
            rb[i].buf += i * BMRingBuf_RB_SIZE;
        }
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
    BMDLNode_pt node = NULL;
    while (node = BMDLNode_GetNext(&rb_anchor))
    {
        node->data = NULL;
        BMDLNode_SReturn(node);
    }
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
    BMRingBuf_pt retval = (BMRingBuf_pt)node->data;
    BMDLNode_SReturn(node);
    if (BMLock_INIT(&retval->base.lock))
    {
        return NULL;
    }
    retval->base.rdidx = retval->base.wridx = 0;
    return retval;
}

/*!
\brief Return a ring buffer instance into the static pool.
*/
BMStatus_t BMRingBuf_SReturn(BMRingBuf_pt rbptr)
{
    BMLock_DEINIT(&rbptr->base.lock);
    BMDLNode_pt newnode = BMDLNode_SGet();
    if (!newnode) return BMStatus_NORESOURCE;
    newnode->data = rbptr;
    BMDLNode_AddNext(&rb_anchor, newnode);
    return BMStatus_SUCCESS;
}
