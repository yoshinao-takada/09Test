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
    BMDLNode_t dsiq;
} BMFSM_t, *BMFSM_pt;

/*!
\brief declare an instance of BMFSM_t.
*/
#define BMFSM_INIOBJ(_varname, _state) { \
    _state, BMStateResult_IGNORE, \
    { &_varname.iq, &_varname.iq, NULL, 0 }, \
    { &_varname.dsiq, &_varname.dsiq, NULL, 0 }}

#define BMFSM_DECL(_varname, _state) \
    BMFSM_t _varname = BMFSM_INIOBJ(_varname, _state)

/*!
\brief Init input event queue and output queue list
*/
#define BMFSM_INIT(_varptr) \
    BMDLNode_INITANCHOR(&(_varptr)->iq); \
    BMDLNode_INITANCHOR(&(_varptr)->dsiq)


#define BMFSM_DEINIT(_varptr) \
    BMDLNode_DEINITANCHOR(&(_varptr)->iq); \
    BMDLNode_DEINITANCHOR(&(_varptr)->dsiq)

/*!
\brief crunch the oldest event in the input queue of the FSM.
*/
#define BMFSM_CRUNCH(_fsmptr) \
{ \
    BMEv_pt _evptr = BMEv_DeQ(&(_fsmptr)->iq); \
    if (_evptr) \
    { \
        (_fsmptr)->lastResult = (_fsmptr)->state(_fsmptr, _evptr); \
        _evptr->listeners--; \
    } \
}

/*!
\brief Put an event into the input queue of the FSM.
*/
#define BMFSM_PUTEV(_fsmptr, _evptr) BMEv_EnQ(_evptr, &(_fsmptr)->iq)

/*!
\brief Add an instance of BMFSM_t to the linked list.
\param fsm [in] The FSM instance.
\param anchor [in,out] The linked list.
*/
BMStatus_t BMFSM_Add(BMFSM_pt fsm, BMDLNode_pt anchor);

/*!
\brief Remove an instance of BMFSM_t from the linked list.
\param fsm [in] The FSM instance.
\param anchor [in,out] The linked list.
*/
BMStatus_t BMFSM_Remove(BMFSM_pt fsm, BMDLNode_pt anchor);
#endif /* BMFSM_H */