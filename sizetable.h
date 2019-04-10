#ifndef __SHEAP_SIZETABLE_
#define __SHEAP_SIZETABLE_
#include "sheap.h"
#include <stddef.h>

// Externs
extern void* __SHEAP_SIZETABLE_START;
extern void* __SHEAP_SIZETABLE_END;

//can probably do this with better space efficiency
//this does have good time efficiency for lookups though
//500 to suppport allocations up to 8 MB which covered all in Cling paper
struct size_table_elem {
    void* freeptr[500];
};

//update global constants
void initialize_sizetable(int nElems);
void expand_sizetable(int nElems);

#endif
