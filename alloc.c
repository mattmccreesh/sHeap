#include "alloc.h"
#include <sys/mman.h>

/**
 *  Allocates a number of blocks and returns a pointer to the start of them
 *  @param nBlocks The number of blocks to allocate
 *  @returns A pointer to the start of the allocated blocks
 */
void* allocate_blocks ( int nBlocks ) {
  return mmap(0, BLOCK_SIZE * nBlocks, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
}