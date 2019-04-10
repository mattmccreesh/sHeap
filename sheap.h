#ifndef _SHEAP_H
#define _SHEAP_H

#define BLOCK_SIZE 16384

// Global Meta-Heap Pointers
void* METAHEAP_BASE = NULL; // First value of our metaheap
void* METAHEAP_END = NULL; // First non-valid value
void* NEXT_FREE_METAHEAP_CHUNK = NULL; // Next allocated but unused metaheap location
void* POOL_HASHTABLE_BASE = NULL; // Points to start of the pool hashtable

#endif
