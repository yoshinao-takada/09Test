#if !defined(BMTICK_H)
#define BMTICK_H
#include "BMEv.h"
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>

#pragma region subtimer
typedef struct {
    BMThread_f handler;
    void *param, *result;
    uint16_t interval, downcount;   
} BMSubtimer_t, *BMSubtimer_pt;

#define BMSubtimer_DEFAULT { NULL, NULL, NULL, 0, 0 }

#define BMSubtimer_SET_COUNTER(_varptr, _interval, _downcount) \
    (_varptr)->interval = _interval; (_varptr)->downcount = _downcount

#define BMSubtimer_SET_HANDLER(_varptr, _handler, _param) \
    (_varptr)->handler = (_handler); \
    (_varptr)->result = (_varptr)->param = (_param)

typedef struct {
    BMEv_t ev;  // event to send to the output queue
    BMDLNode_pt oq; // output queue
} BMSubtimerParam_t, *BMSubtimerParam_pt;

#define BMSubtimerParam_DEFAULT(_oq) \
    { BMEv_INIOBJ(BMEvId_TICK, NULL), _oq }

#define BMSubtimerParam_INIT(_varptr) BMEv_INIT(&(_varptr)->ev)

#define BMSubtimerParam_DEINIT(_varptr) BMEv_DEINIT(&(_varptr)->ev)

/*!
\brief The default subtimer handler send the default event to
    the registered queue.
*/
void* BMSubtimer_DefaultHandler(void* param);

/*!
\brief init the static pool of subtimers.
*/
BMStatus_t BMSubtimer_SInit();

/*!
\brief deinit the static pool of subtimers.
*/
BMStatus_t BMSubtimer_SDeinit();

/*!
\brief get a subtimer instance from the static pool.
*/
BMSubtimer_pt BMSubtimer_SGet();

/*!
\brief return a subtimer instance into the static pool.
*/
BMStatus_t BMSubtimer_SReturn(BMSubtimer_pt subtimer);

/*!
\brief Clock the subtimer; clock the downcounter. Invoke a registered
    callback if the cownter reaches at zero.
*/
BMStatus_t BMSubtimer_Tick(BMSubtimer_pt subtimer);

/*!
\brief Add a subtimer to a subtimer list.
*/
BMStatus_t BMSubtimers_Add(BMDLNode_pt anchor, BMSubtimer_pt subtimer);

/*!
\brief Remove a subtimer from a subtimer list.
*/
BMStatus_t BMSubtimers_Remove(BMDLNode_pt anchor, BMSubtimer_pt subtimer);

/*!
\brief Trigger the subtimers linked to anchor.
\param Anchor [in,out] An anchor of a linked list of BMDLNode_t containing
    BMSubtimer_t instances.
*/
BMStatus_t BMSubtimers_Tick(BMDLNode_pt anchor);
#pragma endregion subtimer

#pragma region Timer_dispatcher_object
typedef struct {
    BMDLNode_t iq; // The input queue receives timer overflow events.
    BMDLNode_t subtimers; // subtimer list
} BMTimerDispatcher_t, *BMTimerDispatcher_pt;

#define BMTimerDispatcher_DECL(_varname) \
    BMTimerDispatcher_t _varname = { \
        { &_varname.iq, &_varname.iq, NULL, 0 }, \
        { &_varname.subtimers, &_varname.subtimers, NULL, 0 }}

#define BMTimerDispatcher_INIT(_varptr) \
    BMDLNode_INITANCHOR(&((_varptr)->iq)); \
    BMDLNode_INITANCHOR(&((_varptr)->subtimers))

#define BMTimerDispatcher_DEINIT(_varptr) \
    BMDLNode_DEINITANCHOR(&((_varptr)->iq)); \
    BMDLNode_DEINITANCHOR(&((_varptr)->subtimers))

/*!
\brief Enqueue an event.
*/
#define BMTimerDispatcher_ENQ(_varptr, _evptr) \
    BMEv_EnQ(_evptr, &((_varptr)->iq))

/*!
\brief Crunch the oldest queued event.
*/
BMStatus_t BMTimerDispatcher_Crunch(BMTimerDispatcher_pt dispather);
#pragma endregion Timer_dispatcher_object
#pragma region Timer_Signal_handler
/*!
\struct BMTickCtx
\brief systick device dependent context for an ISR.
*/
typedef struct {
    uint16_t interval;
    struct itimerval it_new, it_old;
    struct sigaction sa_new, sa_old;
} BMTickCtx_t, *BMTickCtx_pt;

/*!
\brief default initializer of BMTickCtx_t.
*/
#define BMTickCtx_DEFAULT(_interval) { _interval, \
    ITimerVal_DEFAULT, ITimerVal_DEFAULT, \
    SigAction_DEFAULT, SigAction_DEFAULT }

/*!
\brief Initialize static resources;
    e.g. sigaction for SIGALRM, timer dispatcher, etc.
*/
BMStatus_t BMTick_Init(uint16_t interval);

/*!
\brief Release static resources.
*/
BMStatus_t BMTick_Deinit();

/*!
\brief start interval timer.
*/
BMStatus_t BMTick_Start();

/*!
\brief stop interval timer
*/
BMStatus_t BMTick_Stop();

/*!
\brief add a subtimer to the static dispatcher.
*/
BMStatus_t BMTick_AddSubtimer(BMSubtimer_pt subtimer);

/*!
\brief remove a subtimer to the static dispatcher.
*/
BMStatus_t BMTick_RemoveSubtimer(BMSubtimer_pt subtimer);
#pragma endregion Timer_Signal_handler
#endif /* BMTICK_H */