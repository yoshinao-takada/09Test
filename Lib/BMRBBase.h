#if !defined(BMRBBASE_H)
#define BMRBBASE_H
#include "BMLock.h"
#include "BMDefs.h"

/*!
\struct BMRBBase
\brief Base of ring buffer structs.
*/
typedef struct {
    BMLock_t lock;
    uint16_t wridx, rdidx, count;
} BMRBBase_t, *BMRBBase_pt;
typedef const BMRBBase_t *BMRBBase_cpt;

/*!
\brief Default initializer
*/
#define BMRBBase_DEFAULT(_count)    { 0, 0, 0, (_count) }

/*!
\brief Initialize a lock object.
*/
#define BMRBBase_INIT(_objptr) BMLock_INIT(&(_objptr)->lock)

/*!
\brief release resouces of the lock object.
*/
#define BMRBBase_DEINIT(_objptr) BMLock_DEINIT(&(_objptr)->lock)

/*!
\brief lock the object.
*/
#define BMRBBase_LOCK(_objptr) BMLock_LOCK(&(_objptr)->lock)

/*!
\brief unlock the object
*/
#define BMRBBase_UNLOCK(_objptr) BMLock_UNLOCK(&(_objptr)->lock)

/*!
\brief get the next value of wridx.
*/
uint16_t BMRBBase_NextWridx(BMRBBase_pt RBBaseptr);

/*!
\brief get the next value of rdidx.
*/
uint16_t BMRBBase_NextRdidx(BMRBBase_pt RBBaseptr);

/*!
\brief get a byte count in the queue.
*/
uint16_t BMRBBase_Available(BMRBBase_pt RBBaseptr);

/*!
\brief get a byte count vacant in the queue.
*/
uint16_t BMRBBase_Vacant(BMRBBase_pt RBBaseptr);

#endif /* BMRBBASE_H */