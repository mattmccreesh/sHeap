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
//@requires n_blocks > 0 && n_blocks * BLOCK_SIZE < INT_MAX;
//@ensures true;

// Print function for ptrs
void print_address_hex(void* ptr);
//@requires (unsigned long)ptr < LONG_MAX; // Necessary in order to ensure that the cast of p to a 
//          long doesn't cause arithmentic overflows, and preserves the >> operation's requirements
//@ensures true;

// Print a single char
void write_char(char c);
//@requires true;
//@ensures true;

#endif
