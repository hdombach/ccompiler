#pragma once

#include "hash.h"

typedef void (*FreeFunc)(void*);
typedef int (*CmpFunc)(void const *, void const *);
typedef void (*CpFunc)(void *, void const*);
typedef int (*PrintFunc)(void const *);

typedef hash_t (*HashFunc)(void const *);
