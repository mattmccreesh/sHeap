#include <stdlib.h>
#include "sheap.h"
#include "pool_hash_table.h"

// Load in the pool size constant
extern const int POOL_HASH_TABLE_SIZE;
// Load in the pool ht pointer
extern struct phtEntry* POOL_HASH_TABLE;

// Initializes the HT with empty entries
int phtHash(void* callSite){
    return 0;    
}

// Searches for a hash table entry
struct phtEntry* phtSearch(void* callSite, size_t allocSize){
    // Get the supposed offset into the hash table
    int offset = phtHash(callSite) * sizeof(struct phtEntry);
    // Check to see if a phtEntry exists here
    if(POOL_HASH_TABLE+offset == 0x00000000){
        // If not, lets create one
        (POOL_HASH_TABLE+offset)->callSite = callSite;
        (POOL_HASH_TABLE+offset)->poolPtr = create_sizetable_elem(allocSize);
    }

    // Return it
    return (struct phtEntry*) POOL_HASH_TABLE+offset;
}
