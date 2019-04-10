#ifndef __SHEAP_SIZETABLE_
#define __SHEAP_SIZETABLE_
#include "sheap.h"
#include <stddef.h>

#define NUM_LARGE_SIZE_CLASSES 500

//can probably do this with better space efficiency
//this does have good time efficiency for lookups though
//500 to suppport allocations up to 8 MB which covered all in Cling paper
struct size_table_elem {
    void* freeptr[NUM_LARGE_SIZE_CLASSES];
};

// Externs
extern void* __SHEAP_SIZETABLE_START;//probably not important at all
extern void* __SHEAP_SIZETABLE_END;
extern struct size_table_elem* __SHEAP_SIZETABLE_NEXT;

//update global constants
void initialize_sizetable(int nElems);
void expand_sizetable(int nElems);
//return pointer to struct
struct size_table_elem* create_sizetable_elem(size_t allocSize);
void* get_sizetable_freeptr(struct size_table_elem*, size_t allocSize);

#endif
