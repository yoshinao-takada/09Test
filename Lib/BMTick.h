#if !defined(BMTICK_H)
#define BMTICK_H
#include "BMISR.h"
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
    (_varptr)->handler = _handler; \
    (_varptr)->result = (_varptr)->param = _param

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
\brief Tick the subtimers linked to anchor.
\param Anchor [in,out] An anchor of a linked list of BMDLNode_t containing
    BMSubtimer_t instances.
*/
BMStatus_t BMSubtimers_Tick(BMDLNode_pt anchor);
#pragma endregion subtimer

#pragma region AOTick
/*!
\brief declare an active object of systick.
*/
#define BMAOTick_DECL(_varname, _inistate) \
    BMDLNode_DECLANCHOR(_varname ## _subtimers); \
    BMFSM_t _varname = { _inistate, \
        { &_varname.iq, &_varname.iq, NULL, 0 }, \
        { &_varname.oq, &_varname.oq, NULL, 0 }, \
        &_varname ## _subtimers }
/*!
\brief Initialize the active object of systick.
*/
#define BMAOTick_INIT(_varptr) \
    BMFSM_INIT(_varptr); \
    BMDLNode_INITANCHOR((BMDLNode_pt)(_varptr)->ctx)

#define BMAOTick_DEINIT(_varptr) \
    BMFSM_DEINIT(_varptr); \
    BMDLNode_DEINITANCHOR((BMDLNode_pt)(_varptr)->ctx)
#pragma endregion AOTick

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
\brief Declare an ISR for systick timer.
*/
#define BMTick_DECL(_varname, _interval) \
    BMTickCtx_t _varname ## _ctx = { _interval, \
        ITimerVal_DEFAULT, ITimerVal_DEFAULT, \
        SigAction_DEFAULT, SigAction_DEFAULT }; \
    BMISR_DECL(_varname, &(_varname ## _ctx))

#define BMTick_SDECL(_varname, _interval) \
    static BMTickCtx_t _varname ## _ctx = { _interval, \
        ITimerVal_DEFAULT, ITimerVal_DEFAULT, \
        SigAction_DEFAULT, SigAction_DEFAULT }; \
    BMISR_SDECL(_varname, &(_varname ## _ctx))

/*!
\brief register handlers for SIGALRM and SIGUSR1.
*/
BMStatus_t BMTick_Init(BMISR_pt tickptr);

/*!
\brief reset registered signal handlers.
*/
BMStatus_t BMTick_Deinit(BMISR_pt tickptr);

/*!
\brief start interval timer.
*/
BMStatus_t BMTick_Start(BMISR_pt tickptr);

/*!
\brief stop interval timer
*/
BMStatus_t BMTick_Stop(BMISR_pt tickptr);
#pragma region Timer_Signal_handler
#endif /* BMTICK_H */