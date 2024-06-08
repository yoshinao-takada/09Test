#if !defined(BMLINBUF_H)
#define BMLINBUF_H
#include "BMDefs.h"
#include "BMDLNode.h"
typedef struct {
    uint8_t* buf;
    uint16_t filled, crunched, count;
} BMLinBuf_t, *BMLinBuf_pt;
typedef const BMLinBuf_t *BMLinBuf_cpt;

#define BMLinBuf_DEFAULT(_contents, _count) { _contents, 0, 0, _count }

/*!
\brief initialize the static pool of linear buffers.
*/
BMStatus_t BMLinBuf_SInit();

/*!
\brief deinitialize the static pool.
*/
BMStatus_t BMLinBuf_SDeinit();

/*!
\brief Get an instance of linear buffer from the static pool.
*/
BMLinBuf_pt BMLinBuf_SGet();

/*!
\brief Return an instance of a linear buffer into the static pool.
*/
BMStatus_t BMLinBuf_SReturn(BMLinBuf_pt bufptr);
#endif /* BMLINBUF_H */