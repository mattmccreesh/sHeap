#include <stdlib.h>
#include "sheap.h"
#include "pool_hash_table.h"

// Load in the pool ht pointer
struct pht_entry* PHT_BASE = NULL;

// Initializes the pht ptr
void* __init_pht(void* block_start){
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
    // Get the supposed offset into the hash table
    int offset = pht_hash(call_site) * sizeof(struct pht_entry);
    
    // Check to see if a pht_entry exists here
    if(PHT_BASE+offset == 0x0000000000000000){
        // If not, lets create one
        (PHT_BASE+offset)->call_site = call_site;
        // This may not be needed, but idk if the memory will be zeroed out
        (PHT_BASE+offset)->pool_ptr = 0x0000000000000000;
    }

    // Return the pht_entry ptr
    return (struct pht_entry*) PHT_BASE+offset;
}
