#ifndef __SHEAP_POOL_HT_
#define __SHEAP_POOL_HT_

#include <stdlib.h>
#include "sheap.h"
#include "sizetable.h"

// Represents the number of entries in the HT
#define PHT_SIZE 250
// Represents the number of alloc blocks for the HT
#define __SHEAP_PHT_BLOCKS 3

// Points to the frist phtEntry in the HT
struct pht_entry* PHT_BASE;

// Struct to represent a HT entry
struct pht_entry {
    // Stores the call site of the call to malloc
    void*               call_site;
    // Stores the pointer to the appropriate pool table entry
    struct st_elem*     pool_ptr;
};

// Hashes the call site to an offset into the hash table
int pht_hash(void* call_site);
// Searches for a hash table entry
struct pht_entry* pht_search(void* call_site);
// Initializes the pool hash table
void* __init_pht(void* block_start);

#endif
