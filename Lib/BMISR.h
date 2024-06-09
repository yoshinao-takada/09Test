#if !defined(BMISR_H)
#define BMISR_H
#include "BMEv.h"

/*!
\file BMISR.h
\brief Interrupt service routine basic struct and related macros
*/
typedef struct {
    BMLock_t lock;
    BMDLNode_pt dsiq; // input queue of downstream object
    void *ddctx; // device dependent context
} BMISR_t, *BMISR_pt;

#define BMISR_DECL(_varname, _ddctx) \
    BMISR_t _varname = { \
        BMLock_INIOBJ, BMEv_INIOBJ(BMEvId_TICK, &_varname), \
        BMDLNode_INIOBJ(&_varname.dsiq), _ddctx }

#define BMISR_SDECL(_varname, _ddctx) \
    static BMISR_t _varname = { \
        BMLock_INIOBJ, BMEv_INIOBJ(BMEvId_TICK, &_varname), \
        BMDLNode_INIOBJ(&_varname.dsiq), _ddctx }

#define BMISR_INIT(_varptr) BMLock_INIT(&(_varptr)->lock)

#define BMISR_DEINIT(_varptr) BMLock_DEINIT(&(_varptr)->lock)

/*!
\brief Pseudo interrupt service routine implemented with pthread.
*/
typedef struct {
    BMISR_t base;
    pthread_t thread;
} BMISR2_t, BMISR2_pt;
#endif /* BMISR_H */