#ifndef _SHEAP_H
#define _SHEAP_H

#define BLOCK_SIZE 16384
void* METAHEAP_BASE; // First value of our metaheap
void* METAHEAP_END; // First non-valid value
void* NEXT_FREE_METAHEAP_CHUNK; // Next allocated but unused metaheap location
void* POOL_HASHTABLE_BASE; // Points to start of the pool hashtable

#endif
