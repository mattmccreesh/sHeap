#include "util.h"
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

void* __SHEAP_LAST_MALLOCD = NULL;
void* __SHEAP_LAST_VALID_LOC = NULL;


/**
 *  Allocates a number of blocks and returns a pointer to the start of them
 *  @param nBlocks The number of blocks to allocate
 *  @returns A pointer to the start of the allocated blocks
 */
void* allocate_blocks ( int n_blocks ) 
//@requires n_blocks > 0 && n_blocks * BLOCK_SIZE < INT_MAX;
//@ensures true;
{
    // Verifast seems to be unable to find or use the global variables if this isn't here, but since its modular,
    // it isn't strictly relevant, and other functions that use them can make their own assumptions about them
    void* __SHEAP_LAST_MALLOCD = NULL;
    void* __SHEAP_LAST_VALID_LOC = NULL;
    void* malloc_addr = sbrk (BLOCK_SIZE * n_blocks );
    if ( malloc_addr == NULL ) {
    	return NULL;
    }
    __SHEAP_LAST_MALLOCD = malloc_addr;
    __SHEAP_LAST_VALID_LOC = malloc_addr + BLOCK_SIZE * n_blocks -1;
    // Verifast attempts to ensure safety, here you need to explicitly mention that you are leaking heap chunks
    //@leak malloc_block(malloc_addr, n_blocks * BLOCK_SIZE);
    //@leak chars(malloc_addr, n_blocks * BLOCK_SIZE, _);
    return malloc_addr;
}

// This function is effectively just a wrapper for "write" which is side-effectless,
//  so I don't bother verifying it. (it would just be requires & ensures true
void write_char(char c)
//@requires true;
//@ensures true;
{
  write(1, &c, 1);
}

char hex_digit(int v) 
//@requires v >= 0 && v < 16;
//@ensures (result >= '0' && result <= '9') || (result >= 'a' && result <= 'f');
{
  if (v >= 0 && v < 10)
    return (char)('0' + v);
  else
    return (char)('a' + v - 10);
}

// This function is just for printing things, so it doesn't need any pre/post conditions
void print_address_hex(void* p0) 
//@requires (unsigned long)p0 < LONG_MAX; // Necessary in order to ensure that the cast of p to a 
//          long doesn't cause arithmentic overflows, and preserves the >> operation's requirements
//@ensures true;
{
  int i;
  uintptr_t p = (uintptr_t)p0;

  write_char('0'); write_char('x');
  for(i = (8 << 3) - 4; i>=0; i -= 4) 
  //@invariant true;
  {
    // This was previously one line, but verifast seems to have some problems with that
    int v = ((long)p >> i) & 0xf;
    write_char(hex_digit(v));
  }
  write_char('\n');
}
