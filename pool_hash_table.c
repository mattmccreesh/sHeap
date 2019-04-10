#include <stdlib.h>
#include "sheap.h"
#include "pool_hash_table.h"

// Load in the pool ht pointer
// TODO: vvv This won't compile vvv
struct phtEntry* POOL_HASH_TABLE = POOL_HASH_TABLE_BASE;

// Load in the pool ht pointer
struct phtEntry* POOL_HASH_TABLE;

// Generates a hash value for the callSite
int phtHash(void* callSite){
    return 0;    
}

// Searches for a hash table entry
struct phtEntry* phtSearch(void* callSite, size_t allocSize){
    /*
    // Get the supposed offset into the hash table
    int offset = phtHash(callSite) * sizeof(struct phtEntry);
    
    // Check to see if a phtEntry exists here
    if(POOL_HASH_TABLE+offset == 0x00000000){
        // If not, lets create one
        (POOL_HASH_TABLE+offset)->callSite = callSite;
        (POOL_HASH_TABLE+offset)->poolPtr = create_sizetable_elem(allocSize);
    }

    // Return the phtEntry ptr
    return (struct phtEntry*) POOL_HASH_TABLE+offset;
    */
    return NULL;
}
