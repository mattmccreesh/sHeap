#ifndef __SHEAP_POOL_HT_
#define __SHEAP_POOL_HT_

#include <stdlib.h>
#include "sheap.h"
#include "sizetable.h"

// Represents the number of entries in the HT
#define  POOL_HASH_TABLE_SIZE 250;

// Points to the frist phtEntry in the HT
struct phtEntry* POOL_HASH_TABLE;

// Struct to represent a HT entry
struct phtEntry {
    // Stores the call site of the call to malloc
    void*                       callSite;
    // Stores the pointer to the appropriate pool table entry
    struct size_table_elem*     poolPtr;
};

// Hashes the call site to an offset into the hash table
int phtHash(void* callSite);
// Searches for a hash table entry
struct phtEntry* phtSearch(void* callSite, size_t allocSize);
void __init_pht();

#endif
