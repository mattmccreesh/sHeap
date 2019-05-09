#ifndef __SHEAP_SIZETABLE_
#define __SHEAP_SIZETABLE_
#include "sheap.h"
#include "flist.h"
#include <stddef.h>

#define NUM_LARGE_SIZE_CLASSES 50
#define __SHEAP_ST_BLOCKS 25

// 50 size classes for exponential size class growth
// Supports max size_t size allocation request on 64 bit system
struct st_elem {
    // 0 indicates wrapper, -1 indicates non-wrapper
    size_t wrapper_or_alloc_size;
    // Pointer to freelist head
    struct flist_node* freeptr[NUM_LARGE_SIZE_CLASSES];
};

// Externs
extern void* __SHEAP_ST_START;//probably not important at all
extern void* __SHEAP_ST_END;
extern struct st_elem* __SHEAP_ST_NEXT;

//update global constants
void* __init_st(void* start_block);
//allocate and return pointer to size_table element in table
struct st_elem* create_st_elem(size_t alloc_size);
//Reutrn address of free list pointer for given size and element in table
struct flist_node** st_get_freeptr(struct st_elem*, size_t alloc_size);
//allocate block for malloc/calloc/realloc
void* st_allocate_block(struct st_elem** pool_ptr, size_t alloc_size, void* call_site);
//convert size to sizeclass index. Number of blocks is 2^sizeclassindex
int get_sizeclass_index(size_t);
#endif
