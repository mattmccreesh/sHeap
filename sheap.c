#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "sheap.h"
#include "pool_hash_table.h"

// Load & define global ptrs
void* PHT_BASE = NULL;

// Kick-off the initialization process of the metaheap construction
void __init_meta_heap(){
}

// Allocates the memory and returns a pointer to it
void* malloc(size_t size){
    // Initialize meta heap
    __init_meta_heap();
    // Get the call site address to malloc
    void* p = __builtin_return_address(0);
    // Get a pool hash table entry
    //struct phtEntry* poolTablePtr = phtSearch(p, size);
    return (void*) 0xaabbccdd;
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
