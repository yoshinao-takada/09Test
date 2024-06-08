#if !defined(BMTICK_H)
#define BMTICK_H
#include "BMFSM.h"
#include "BMISR.h"
#include <time.h>
#include <signal.h>
#include <sys/time.h>

#pragma region subtimer
typedef struct {
    BMThread_f handler;
    void *param, *result;
    uint16_t interval, downcount;   
} BMSubtimer_t, *BMSubtimer_pt;

#define BMSubtimer_DEFAULT { NULL, NULL, NULL, 0, 0 }

BMStatus_t BMSubtimer_SInit();

BMStatus_t BMSubtimer_SDeinit();

BMSubtimer_pt BMSubtimer_Sget();

BMStatus_t BMSubtimer_SReturn(BMSubtimer_pt subtimer);

BMStatus_t BMSubtimer_Tick(BMSubtimer_pt subtimer);

#define BMSubtimer_SET(_subtmptr, _interval, _downcount) \
    (_subtmptr)->interval = _interval; \
    (_subtmptr)->downcount = _downcount

#define BMSubtimer_INIT(_subtmptr, _handler, _param, _interval, _downcount) \
    (_subtmptr)->handler = _handler; \
    (_subtmptr)->param = _param; \
    BMSubtimer_SET(_subtmptr, _interval, _downcount)

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

BMStatus_t BMTick_Init(BMISR_pt tickptr);

BMStatus_t BMTick_Deinit(BMISR_pt tickptr);

BMStatus_t BMTick_Start(BMISR_pt tickptr);

BMStatus_t BMTick_Stop(BMISR_pt tickptr);
#pragma region Timer_Signal_handler
#endif /* BMTICK_H */