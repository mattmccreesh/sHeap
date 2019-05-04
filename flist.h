#ifndef __SHEAP_FLIST_
#define __SHEAP_FLIST_
#include "sheap.h"
#include <stddef.h>

// Externs
/*
extern void* __SHEAP_FLIST_START;
extern void* __SHEAP_BLOCK_START;
extern void* __SHEAP_FLIST_UNUSED;
*/

#define __SHEAP_FLIST_BLOCKS 100

struct flist_node {
  struct flist_node* next;
  struct flist_node* prev;
  void* type;
  int n_blocks;
  int size;
};


/*
*
*/
void* __init_flist(void* start_addr);

/**
 * Allocates a new block, or reuses an old free one
 * @param nBlocks the number of blocks to allocate (should be positive)
 * @param type the the type to add (the location of the allocation site)
 * @param head the address of the address of the head of this size-class
 * @returns The address allocated (The head pointer is potentially updated as well)
 */
void* flist_alloc_space ( size_t size, int n_blocks, void* type, struct flist_node** head );

/**
 * Frees a block, adding the allocated block to the freelist
 * @param loc The location to free (should be the start of the allocation)
 * @param head The start of the linked list for this size class
 */
void flist_dealloc_space ( void* loc, struct flist_node** head );

struct flist_node* get_node_from_location ( void* loc );

#endif
