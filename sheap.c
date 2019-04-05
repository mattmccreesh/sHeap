#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

/*
    I think these signatures are right. This is just
    something to keep in mind. Any alternative signatures
    may need to be added later on.
*/
void write_char(char c){
  write(1, &c, 1);
}

char hex_digit(int v) {
  if (v >= 0 && v < 10)
    return '0' + v;
  else
    return 'a' + v - 10; // <-- Here
}

void print_address_hex(void* p0) {
  int i;
  uintptr_t p = (uintptr_t)p0;

  write_char('0'); write_char('x');
  for(i = (sizeof(p) << 3) - 4; i>=0; i -= 4) {
    write_char(hex_digit((p >> i) & 0xf));
  }
  write_char('\n');
}

// Global Meta-Heap Pointers
void* METAHEAP_BASE = NULL; // First value of our metaheap
void* METAHEAP_END = NULL; // First non-valid value
void* NEXT_FREE_METAHEAP_CHUNK = NULL; // Next allocated but unused metaheap location
void* POOL_HASHTABLE_BASE = NULL; // Points to start of the pool hashtable

// 
void __init_meta_heap(){
    // Map out the metaheap
    METAHEAP_BASE = mmap(0, 16<<10, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);

    // Update the end pointer
    METAHEAP_END = METAHEAP_BASE + 16 << 10;
}

// Allocates the memory and returns a pointer to it
void* malloc(size_t size){
    // Initialize meta heap
    __init_meta_heap();
    // Get the call site address to malloc
    void* p = __builtin_return_address(0);
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
