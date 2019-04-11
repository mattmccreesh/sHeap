#include <stdlib.h>
#include "sheap.h"
#include "pool_hash_table.h"

// Load in the pool ht pointer
struct pht_entry* PHT_BASE = NULL;

// Initializes the pht ptr
void __init_pht(void* block_start){
    // Populate the pointer with papa pointer
    PHT_BASE = block_start;
    // Return the end of PHT section
    return PHT_BASE + (BLOCK_SIZE * __SHEAP_PHT_BLOCKS);
}

// Generates a hash value for the call_site
int pht_hash(void* call_site){
    // TODO: This needs to be less garbage
    return 0;    
}

// Searches for a hash table entry
struct pht_entry* pht_search(void* call_site){
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
