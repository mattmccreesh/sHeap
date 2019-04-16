#include <stdlib.h>
#include <stdio.h>
#include "sheap.h"
#include "util.h"
#include "pool_hash_table.h"
#include "sizetable.h"
#include "flist.h"

// Load & define global ptr
void* __SHEAP_BASE = NULL;

// Kick-off the initialization process of the metaheap construction
void __init_sheap(){
    // Allocate space
    __SHEAP_BASE = allocate_blocks(__SHEAP_PHT_BLOCKS +
                                   __SHEAP_FLIST_BLOCKS +
                                   __SHEAP_ST_BLOCKS);
    // Initialize the system components
    void* pht_end = __init_pht(__SHEAP_BASE);
    void* st_end = __init_st(pht_end);
    void* flist_end = __init_flist(st_end);
}

// Allocates the memory and returns a pointer to it
void* malloc(size_t size){
    // Check for sheap init
    if(!__SHEAP_BASE){
        __init_sheap();
    }
    // Get the call site address to malloc
    void* call_site = __builtin_return_address(0);
    // Search for pool ptr
    struct pht_entry* pht_e = pht_search(call_site);
    // Return the memory address from ST
    return st_allocate_block(&(pht_e->pool_ptr), size, pht_e->call_site);
    //return 0xaabbccdd;
}

// Does the same thing as malloc, but zeroes out the memory
void* calloc(size_t nitems, size_t size){
    return (void*) 0xaabbccdd;
}

// Given a pointer to memory and a size, it will attempt to resize the memory chunk
void* realloc(void* ptr, size_t size){
    return (void*) 0xaabbccdd;
}

// Frees a memory allocation pointed to by ptr
void free(void* ptr){
    return;
}
