#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "sheap.h"
#include "pool_hash_table.h"

// Load & define global ptrs
void* METAHEAP_BASE = NULL;
void* METAHEAP_END = NULL;
void* POOL_HASH_TABLE_BASE = NULL;
void* NEXT_FREE_METAHEAP_CHUNK = NULL;

// Kick-off the initialization process of the metaheap construction
void __init_meta_heap(){
    // Map out the metaheap
    METAHEAP_BASE = mmap(0, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    // Set the next free pointer
    NEXT_FREE_METAHEAP_CHUNK = METAHEAP_BASE;
    // Set the pool HT base
    POOL_HASH_TABLE_BASE = NEXT_FREE_METAHEAP_CHUNK;
    // Update the end pointer
    METAHEAP_END = METAHEAP_BASE + BLOCK_SIZE;
    // 
    __init_pht();
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
