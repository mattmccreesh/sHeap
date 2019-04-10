#ifndef _SHEAP_H
#define _SHEAP_H
#include <string.h>

#define BLOCK_SIZE 16384

// Global Meta-Heap Pointers
extern void* METAHEAP_BASE; // First value of our metaheap
extern void* METAHEAP_END; // First non-valid value
extern void* NEXT_FREE_METAHEAP_CHUNK; // Next allocated but unused metaheap location
extern void* POOL_HASH_TABLE_BASE; // Points to start of the pool hashtable

#endif
