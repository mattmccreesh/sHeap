#ifndef __SHEAP_POOL_HT_
#define __SHEAP_POOL_HT_

#include "sheap.h"
#include "sizetable.h"

// Represents the number of entries in the HT
const int POOL_HASH_TABLE_SIZE = 250;
// Points to the start of the hash table
pool_ht_entry* POOL_HASH_TABLE = NULL;

// Struct to represent a HT entry
struct pool_hash_table_entry {
    // Stores the call site of the call to malloc
    char                call_site[8];
    // Stores the pointer to the appropriate pool table entry
    size_table_elem*    pool_ptr;
};

// Function to initialize the pool HT
void initialize_pool_hash_table();

#endif
