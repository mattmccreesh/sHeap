#include "sheap.h"
#include "pool_hash_table.h"

// Load in the pool size constant
extern const int POOL_HASH_TABLE_SIZE;
// Load in the pool ht pointer
extern pool_hash_table_entry* POOL_HASH_TABLE;

// Initializes the HT with empty entries
void initialize_pool_hash_table(){
    // Create the blank hash table entries
    for(int i=0; i<POOL_HASH_TABLE_SIZE; i++){
        // Calculate the offset from POOL_HASH_TABLE
        int offset = i * sizeof(pool_hash_table_entry);
        // Create the new HT entry struct
        struct pool_hash_table_entry new_ht_entry;
        // Point the HT to it
        *(POOL_HASH_TABLE+offset) = &new_ht_entry;
    }
}
