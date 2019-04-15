#include "alloc.h"
#include <unistd.h>

/**
 *  Allocates a number of blocks and returns a pointer to the start of them
 *  @param nBlocks The number of blocks to allocate
 *  @returns A pointer to the start of the allocated blocks
 */
void* allocate_blocks ( int n_blocks ) {
  return sbrk(BLOCK_SIZE * n_blocks );
}
