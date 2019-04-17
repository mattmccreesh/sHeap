#include <stdlib.h>
#include "sheap.h"
#include "util.h"
#include "pool_hash_table.h"

// Load in the pool ht pointer
struct pht_entry* PHT_BASE = NULL;

// Initializes the pht ptr
void* __init_pht(void* block_start){
    // Populate the pointer with papa pointer
    PHT_BASE = block_start;
    // Return the end of PHT section
    return block_start + (BLOCK_SIZE * __SHEAP_PHT_BLOCKS);
}

// Generates a hash value for the call_site
int pht_hash(void* call_site){
    long int first_four = ((long int) call_site) & ((long int) 0xffffffff);
    long int last_four = (((long int) call_site) >> (8*4)) & 0xffffffff;
    return (int) ((first_four ^ last_four) % PHT_SIZE);
}

// Searches for a hash table entry
struct pht_entry* pht_search(void* call_site){
    // Get the supposed offset into the hash table
    int offset = pht_hash(call_site);

    // Check to see if no pht_entry is occupying the slot
    if((PHT_BASE+offset)->call_site == NULL){
        // If not, lets create one
        (PHT_BASE+offset)->call_site = call_site;
        // This may not be needed, but idk if the memory will be zeroed out
        (PHT_BASE+offset)->pool_ptr = NULL;
    // Checks to see if a collision is occurring
    }else if((PHT_BASE+offset)->call_site != call_site){
        for(int i=0; i<PHT_SIZE; i++){
            // Bump the offset
            offset = (offset + 1) % PHT_SIZE;
            // Check for collision
            if((PHT_BASE+offset)->call_site == NULL){
                // If not, lets create one
                (PHT_BASE+offset)->call_site = call_site;
                // This may not be needed, but idk if the memory will be zeroed out
                (PHT_BASE+offset)->pool_ptr = NULL;
                break;
            }else if((PHT_BASE+offset)->call_site == call_site){
                break;
            }
        }
        // IF WE GET HERE THERE IS NO SPACE
        exit(1);
    }

    // Return the pht_entry ptr
    return (struct pht_entry*) PHT_BASE+offset;
}
