#if !defined(BMFSM_H)
#define BMFSM_H
#include "BMEv.h"
#define MAX_FSM_COUNT   8
#define MAX_DOWNSTREAM_QUEUES   4

typedef enum {
    BMStateResult_IGNORE,
    BMStateResult_ACCEPT,
    BMStateResult_TRANSIT,
    BMStateResult_ERROR
} BMStateResult_t;

typedef struct BMFSM {
    // state
    BMStateResult_t (*state)(struct BMFSM* fsm, BMEv_pt ev);

    // Latest result of invoking state
    BMStateResult_t lastResult;

    // input event queue of this instance
    BMDLNode_t iq;

    // input event queues of downstream objects
    BMDLNode_t oq;

    // context can hold a variable of any type.
    void* ctx;
} BMFSM_t, *BMFSM_pt;

/*!
\brief declare an instance of BMFSM_t.
*/
#define BMFSM_DECL(_varname, _state, _ctx) \
    BMFSM_t _varname = { _state, BMStateResult_IGNORE, \
        { &_varname.iq, &_varname.iq, NULL, 0 }, \
        { &_varname.oq, &_varname.oq, NULL, 0 }, \
        _ctx }

#define BMFSM_INIT(_varptr) \
    BMDLNode_INITANCHOR(&(_varptr)->iq); \
    BMDLNode_INITANCHOR(&(_varptr)->oq)

#define BMFSM_DEINIT(_varptr) \
    BMDLNode_DEINITANCHOR(&(_varptr)->iq); \
    BMDLNode_DEINITANCHOR(&(_varptr)->oq)

#define BMFSM_CRUNCH(_fsmptr) \
{ \
    BMEv_pt _evptr = BMEv_DeQ(&(_fsmptr)->iq); \
    if (evptr) \
    { \
        (_fsmptr)->lastResult = (_fsmptr)->state(_fsmptr, _evptr); \
        _evptr->listners--; \
    } \
}

#define BMFSM_PUTEV(_fsmptr, _evptr) BMEv_EnQ(_evptr, &(_fsmptr)->iq)
#endif /* BMFSM_H */