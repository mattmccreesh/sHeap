#ifndef __SHEAP_POOL_HT_
#define __SHEAP_POOL_HT_

#include <stdlib.h>
#include "sheap.h"
#include "sizetable.h"

// Struct to represent a HT entry
struct phtEntry {
    // Stores the call site of the call to malloc
    void*                       callSite;
    // Stores the pointer to the appropriate pool table entry
    struct size_table_elem*     poolPtr;
};

// Represents the number of entries in the HT
const int POOL_HASH_TABLE_SIZE = 250;
// Points to the start of the hash table
struct phtEntry* POOL_HASH_TABLE = NULL;

// Hashes the call site to an offset into the hash table
int phtHash(void* callSite);
// Searches for a hash table entry
struct phtEntry* phtSearch(void* callSite, size_t allocSize);

#endif
