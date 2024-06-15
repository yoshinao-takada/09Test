#if !defined(BMRINGBUF_H)
#define BMRINGBUF_H
#include "BMRBBase.h"
#include <memory.h>

#define BMRingBuf_STATIC_POOL_SIZE  4
#define BMRingBuf_RB_SIZE           128

#pragma region BMRingBuf_DECL
typedef struct {
    BMRBBase_t base;
    uint8_t* buf;
} BMRingBuf_t, *BMRingBuf_pt;

typedef const BMRingBuf_t *BMRingBuf_cpt;

#define BMRingBuf_DECL(_varname, _bufsize) \
    uint8_t _varname ## _buf[_bufsize]; \
    BMRingBuf_t _varname = { BMRBBase_DEFAULT(_bufsize), _varname ## _buf }


#define BMRingBuf_SDECL(_varname, _bufsize) \
    static uint8_t _varname ## _buf[_bufsize]; \
    static BMRingBuf_t _varname = { BMRBBase_DEFAULT(_bufsize), _varname ## _buf }

#define BMRingBuf_INIT(_varptr) BMRBBase_INIT(&(_varptr)->base)

#define BMRingBuf_DEINIT(_varptr) BMRBBase_DEINIT(&(_varptr)->base)
/*!
\brief clear the ring buffer.
*/
void BMRingBuf_Clear(BMRingBuf_pt rb);

/*!
\brief put a byte into a ring buffer.
*/
BMStatus_t BMRingBuf_Put(BMRingBuf_pt rb, const uint8_t* byteptr);

/*!
\brief get a byte from a ring buffer.
\return BMStatus_SUCCESS: a byte was retrieved.
\return BMStatus_NORESOURCE: rb is empty and no byte was retrieved.
*/
BMStatus_t BMRingBuf_Get(BMRingBuf_pt rb, uint8_t* byteptr);
#pragma endregion BMRingBuf_DECL

#pragma region BMRingBuf_static_pool_related_defs
/*!
\brief Initialize the static pool.
*/
BMStatus_t BMRingBuf_SInit();

/*!
\brief Deinitialize the static pool.
*/
BMStatus_t BMRingBuf_SDeinit();

/*!
\brief Get a ring buffer instance from the static pool.
*/
BMRingBuf_pt BMRingBuf_SGet();

/*!
\brief Return a ring buffer instance into the static pool.
*/
BMStatus_t BMRingBuf_SReturn(BMRingBuf_pt rbptr);
#pragma endregion BMRingBuf_static_pool_related_defs
#endif /* BMRINGBUF_H */