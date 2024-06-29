#if !defined(BMTICKTEST_H)
#define BMTICKTEST_H
#include "BMTick.h"

BMTickCtx_pt TickCtx();
BMEv_pt  TickEvent();
BMTimerDispatcher_pt TimerDispatcher();

#endif /* BMTICKTEST_H */