#include <stdlib.h>

/*
    I think these signatures are right. This is just
    something to keep in mind. Any alternative signatures
    may need to be added later on.
*/

// Allocates the memory and returns a pointer to it
void* malloc(size_t size){
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
