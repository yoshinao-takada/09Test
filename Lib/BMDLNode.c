#include "BMDLNode.h"
#include "BMDefs.h"

uint16_t BMDLNode_Count(BMDLNode_pt anchor)
{
    uint16_t ct = 0;
    BMLock_LOCK(&anchor->lock);
    for (BMDLNode_pt iter = anchor->next; iter != anchor;
        iter = iter->next, ct++);
    BMLock_UNLOCK(&anchor->lock);
    return ct;
}

void BMDLNode_AddNext(BMDLNode_pt anchor, BMDLNode_pt newnode)
{
    BMLock_LOCK(&anchor->lock);
    BMDLNode_pt curnext = anchor->next;
    curnext->prev = newnode;
    newnode->next = curnext;
    anchor->next = newnode;
    newnode->prev = anchor;
    BMLock_UNLOCK(&anchor->lock);
}

void BMDLNode_AddPrev(BMDLNode_pt anchor, BMDLNode_pt newnode)
{
    BMLock_LOCK(&anchor->lock);
    BMDLNode_pt curprev = anchor->prev;
    curprev->next = newnode;
    newnode->prev = curprev;
    anchor->prev = newnode;
    newnode->next = anchor;
    BMLock_UNLOCK(&anchor->lock);
}

BMDLNode_pt BMDLNode_GetNext(BMDLNode_pt anchor)
{
    // check empty link.
    if (BMDLNode_EMPTY(anchor)) return NULL;
    BMLock_LOCK(&anchor->lock);
    BMDLNode_pt curnext = anchor->next;
    anchor->next = curnext->next;
    curnext->next->prev = anchor;
    curnext->next = curnext->prev = curnext;
    BMLock_UNLOCK(&anchor->lock);
    return curnext;
}

BMDLNode_pt BMDLNode_GetPrev(BMDLNode_pt anchor)
{
    // check empty link.
    if (BMDLNode_EMPTY(anchor)) return NULL;
    BMLock_LOCK(&anchor->lock);
    BMDLNode_pt curprev = anchor->prev;
    anchor->prev = curprev->prev;
    curprev->prev->next = anchor;
    curprev->next = curprev->prev = curprev;
    BMLock_UNLOCK(&anchor->lock);
    return curprev;
}

BMDLNode_pt BMDLNode_Find(
    BMDLNode_pt anchor, const void *tofind, 
    int (*zeroifmatch)(const void *, const void *))
{
    BMDLNode_pt found = NULL;
    BMLock_LOCK(&anchor->lock);
    BMDLNode_pt iter = anchor->next;
    BMDLNode_pt end = anchor;
    for (; iter != end; iter = iter->next)
    {
        if (0 == zeroifmatch(iter->data, tofind))
        {
            found = iter;
            break;
        }
    }
    BMLock_UNLOCK(&anchor->lock);
    return found;
}

static BMDLNode_t nodes[BMDLNode_POOLSIZE];

BMDLNode_SDECLANCHOR(poolAnchor);

void BMDLNode_InitPool()
{
    for (int i = 0; i < BMArray_SIZE(nodes); i++)
    {
        nodes[i].next = nodes[i].prev = &nodes[i];
        nodes[i].data = NULL;
        nodes[i].lock = 0;
        BMDLNode_AddNext(&poolAnchor, &nodes[i]);
    }
    BMDLNode_INITANCHOR(&poolAnchor);
}

void BMDLNode_DeinitPool()
{
    BMDLNode_DEINITANCHOR(&poolAnchor);
}

BMDLNode_pt BMDLNode_SGet()
{
    BMDLNode_pt node = NULL;
    BMLock_LOCK(&poolAnchor.lock);
    if (BMDLNode_HAS_ANY(&poolAnchor))
    {
        node = BMDLNode_GetNext(&poolAnchor);
    }
    BMLock_UNLOCK(&poolAnchor.lock);
    return node;
}

void BMDLNode_SReturn(BMDLNode_pt node)
{
    BMLock_LOCK(&poolAnchor.lock);
    BMDLNode_AddPrev(&poolAnchor, node);
    BMLock_UNLOCK(&poolAnchor.lock);
}
