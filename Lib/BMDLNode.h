#if !defined(BMDLNODE_H)
#define BMDLNODE_H
#include "BMLock.h"

typedef struct BMDLNode {
    struct BMDLNode *prev, *next;
    void* data;
    BMLock_t lock;
} BMDLNode_t, *BMDLNode_pt;

#define BMDLNode_INIOBJ(_varname) { &_varname, &_varname, NULL, 0 }


#define BMDLNode_HAS_ANY(_anchorptr) ((_anchorptr)->prev != (_anchorptr))
#define BMDLNode_EMPTY(_anchorptr) ((_anchorptr)->prev == (_anchorptr))
/*!
\brief count nodes linked to the anchor.
*/
uint16_t BMDLNode_Count(BMDLNode_pt anchor);

/*!
\brief add a new node at anchor->next. It is same as to insert a new node
    at the beginning of the linked list.
*/
void BMDLNode_AddNext(BMDLNode_pt anchor, BMDLNode_pt newnode);

/*!
\brief add a new node at anchor->prev. It is same as to append a new node
    at the end point of the linked list.
*/
void BMDLNode_AddPrev(BMDLNode_pt anchor, BMDLNode_pt newnode);

/*!
\brief get the node at anchor->next and remove the node from the list.
*/
BMDLNode_pt BMDLNode_GetNext(BMDLNode_pt anchor);

/*!
\brief get the node at anchor->prev and remove the node from the list.
*/
BMDLNode_pt BMDLNode_GetPrev(BMDLNode_pt anchor);

/*!
\brief remove the node matching to toRemove.
*/
BMStatus_t BMDLNode_Remove(BMDLNode_pt anchor, BMDLNode_pt toRemove);

#define BMDLNode_DECLANCHOR(_varname) \
    BMDLNode_t _varname = BMDLNode_INIOBJ(_varname)

#define BMDLNode_SDECLANCHOR(_varname) \
    static BMDLNode_t _varname = BMDLNode_INIOBJ(_varname)

#define BMDLNode_INITANCHOR(_varptr) BMLock_INIT(&(_varptr)->lock)

#define BMDLNode_DEINITANCHOR(_varptr) BMLock_DEINIT(&(_varptr)->lock)

#define BMDLNode_LOCK(_nodeptr) BMLock_LOCK(&((_nodeptr)->lock))

#define BMDLNode_UNLOCK(_nodeptr) BMLock_UNLOCK(&((_nodeptr)->lock))

/*!
\brief find the node with the matched condition specified by zeroifmatch.
    Searching begins from anchor->next and the first matching node is reported.
    Other matching nodes are ignored even it they exist.
\param tofind [in] payload pointer to compare
\param zeroifmatch [in] comparator function which returns zero if the two
    payload pointers satisfy match conditions.
\return Found node if a matching found, otherwise NULL.
*/
BMDLNode_pt BMDLNode_Find(BMDLNode_pt anchor, const void* tofind,
    int (*zeroifmatch)(const void*, const void*));

int BMDLNode_DefaultMatch(const void* pv0, const void* pv1);

#define BMDLNode_FIND(_anchor, _tofind) \
BMDLNode_Find(_anchor, _tofind, BMDLNode_DefaultMatch)

/*!
\brief Initialize the static pool.
*/
void BMDLNode_InitPool();

/*!
\brief Deinitialize the static pool.
*/
void BMDLNode_DeinitPool();

/*!
\brief Get a node from the static pool.
*/
BMDLNode_pt BMDLNode_SGet();

/*!
\brief Return a node into the static pool.
*/
void BMDLNode_SReturn(BMDLNode_pt node);

#endif /* BMDLNODE_H */