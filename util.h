#ifndef __SHEAP_ALLOC_H
#define __SHEAP_ALLOC_H
#include "sheap.h"

// Allocation wrapper
void* allocate_blocks ( int n_blocks );
// Print function for ptrs
void print_address_hex(void* ptr);

#endif
