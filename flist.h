#ifndef __SHEAP_FLIST_
#define __SHEAP_FLIST_
#include "sheap.h"
#include <stddef.h>

// Externs
extern void* __SHEAP_FLIST_START;
extern void* __SHEAP_BLOCK_START;
extern void* __SHEAP_FLIST_UNUSED;

struct node {
  struct node* next;
  struct node* prev;
  int nBlocks;
  void* type;
  int size;
};

/**
 * Allocates a new block, or reuses an old free one
 * @param nBlocks the number of blocks to allocate (should be positive)
 * @param type the the type to add (the location of the allocation site)
 * @param head the address of the address of the head of this size-class
 * @returns The address allocated (The head pointer is potentially updated as well)
 */
void* gib_space ( int nBlocks, void* type, struct node** head );

/**
 * Frees a block, adding the allocated block to the freelist
 * @param loc The location to free (should be the start of the allocation)
 * @param head The start of the linked list for this size class
 */
void kill_space ( void* loc, struct node** head );

#endif
