#ifndef CAT_H
#define CAT_H

#include <stdint.h>

typedef void * CATData;

CATData CAT_new (int64_t value);

int64_t CAT_get (CATData v);

void CAT_set (CATData v, int64_t value);

void CAT_add (CATData result, CATData v1, CATData v2);

void CAT_sub (CATData result, CATData v1, CATData v2);

int64_t CAT_invocations (void);

#endif
