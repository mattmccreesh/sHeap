#ifndef __SHEAP_ALLOC_H
#define __SHEAP_ALLOC_H
#include <stddef.h>
#include "sheap.h"

/*
void* __SHEAP_LAST_MALLOCD;
void* __SHEAP_LAST_VALID_LOC;
*/
// Allocation wrapper
void* allocate_blocks ( int n_blocks );
// Print function for ptrs
void print_address_hex(void* ptr);
// Print a single char
void write_char(char c);

#endif
