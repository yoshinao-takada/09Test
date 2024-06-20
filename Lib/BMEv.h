#if !defined(BMEV_H)
#define BMEV_H
#include "BMLock.h"
#include "BMDLNode.h"

typedef struct {
    BMEvId_t id;
    uint16_t listeners;
    void* param;
    BMLock_t lock;
} BMEv_t, *BMEv_pt;
typedef const BMEv_t *BMEv_cpt;

#define BMEv_INIT(_evptr) ( \
    (_evptr)->listeners = 0, BMLock_INIT(&((_evptr)->lock)))

#define BMEv_DEINIT(_evptr) BMLock_DEINIT(&((_evptr)->lock))

#define BMEv_LOCK(_evptr) BMLock_LOCK(&((_evptr)->lock))

#define BMEv_UNLOCK(_evptr) BMLock_UNLOCK(&((_evptr)->lock))

#define BMEv_INIOBJ(_id, _param) { _id, 0, _param, BMLock_INIOBJ }

/*!
\brief Interlocked decrement of reference counting.
*/
#define BMEv_UNREF(_evptr) ( \
    BMLock_LOCK(&((_evptr)->lock)), (_evptr)->listeners--, \
    BMLock_UNLOCK(&((_evptr)->lock)))

/*!
\brief enqueue an event.
    ev->listners is automatically incremented in the method.
\param ev [in] event ptr
\param q [in] queue anchor
\return SUCCESS or NORESOURCE
*/
BMStatus_t BMEv_EnQ(BMEv_pt ev, BMDLNode_pt q);

/*!
\brief dequeue an event.
    ev->listners is not decremented. Therefore, a caller of the method
    must decrement ev->listners at the end of processing the event.
\param q [in] queue anchor
\return a dequeued event
*/
BMEv_pt BMEv_DeQ(BMDLNode_pt q);

#endif /* BMEV_H */