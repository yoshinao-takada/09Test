#if !defined(BMLOCK_H)
#define BMLOCK_H
#include "BMDefs.h"
#include <pthread.h>
typedef pthread_spinlock_t BMLock_t;
typedef pthread_spinlock_t *BMLock_pt;

#define BMLock_LOCK(lockptr) pthread_spin_lock(lockptr)

#define BMLock_TRYLOCK(lockptr) pthread_spin_trylock(lockptr)

#define BMLock_UNLOCK(lockptr) pthread_spin_unlock(lockptr)

#define BMLock_INIT(lockptr) \
    (pthread_spin_init(lockptr, PTHREAD_PROCESS_PRIVATE) ? \
    BMStatus_NORESOURCE : BMStatus_SUCCESS)

#define BMLock_DEINIT(lockptr) pthread_spin_destroy(lockptr)

#define BMLock_INIOBJ   (0)
#endif /* BMLOCK_H */