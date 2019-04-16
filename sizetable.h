#ifndef __SHEAP_SIZETABLE_
#define __SHEAP_SIZETABLE_
#include "sheap.h"
#include "flist.h"
#include <stddef.h>

#define NUM_LARGE_SIZE_CLASSES 50
#define __SHEAP_ST_BLOCKS 2

//50 size classes for exponential size class growth
//supports max size_t size allocation request on 64 bit system
struct st_elem {
    struct flist_node* freeptr[NUM_LARGE_SIZE_CLASSES];
};

// Externs
extern void* __SHEAP_ST_START;//probably not important at all
extern void* __SHEAP_ST_END;
extern struct st_elem* __SHEAP_ST_NEXT;

//update global constants
void* __init_st(void* start_block);
//return pointer to struct
struct st_elem* create_st_elem();
struct flist_node** st_get_freeptr(struct st_elem*, size_t alloc_size);
void* st_allocate_block(struct st_elem** pool_ptr, size_t alloc_size, void* call_site);
//todo
//void allocate_block_from_sizetable(struct st_elem*, size_t);
int get_sizeclass_index(size_t);
#endif
