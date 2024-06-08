#include "BMRBBase.h"

uint16_t BMRBBase_NextWridx(BMRBBase_pt RBBaseptr)
{
    uint16_t nextval = RBBaseptr->wridx + 1;
    if (nextval >= RBBaseptr->count)
    {
        nextval -= RBBaseptr->count;
    }
    return nextval;
}

uint16_t BMRBBase_NextRdidx(BMRBBase_pt RBBaseptr)
{
    uint16_t nextval = RBBaseptr->rdidx + 1;
    if (nextval >= RBBaseptr->count)
    {
        nextval -= RBBaseptr->count;
    }
    return nextval;
}

uint16_t BMRBBase_Available(BMRBBase_pt RBBaseptr)
{
    uint16_t available = (RBBaseptr->wridx >= RBBaseptr->rdidx) ?
        (RBBaseptr->wridx - RBBaseptr->rdidx) :
        (RBBaseptr->wridx + RBBaseptr->count - RBBaseptr->rdidx);
    return available;
}

uint16_t BMRBBase_Vacant(BMRBBase_pt RBBaseptr)
{
    uint16_t vacant = (RBBaseptr->rdidx > RBBaseptr->wridx) ?
        (RBBaseptr->rdidx - 1 - RBBaseptr->wridx) :
        (RBBaseptr->rdidx + RBBaseptr->count - 1 - RBBaseptr->wridx);
    return vacant;
}
