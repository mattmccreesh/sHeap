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
void* allocate_blocks ( int n_blocks ) {
    void* malloc_addr =  sbrk(BLOCK_SIZE * n_blocks );
    if(__SHEAP_LAST_VALID_LOC != NULL && malloc_addr != __SHEAP_LAST_VALID_LOC + 1){
	    char c[] = "This program has changed the program break. sHeap does not support programs\n\tthat directly change the program break\n\0";
	    write(2, &c, 118);
	    exit(1);
    }
    __SHEAP_LAST_MALLOCD = malloc_addr;
    __SHEAP_LAST_VALID_LOC = malloc_addr + BLOCK_SIZE * n_blocks -1;
    return malloc_addr;
}

void write_char(char c){
  write(1, &c, 1);
}

char hex_digit(int v) {
  if (v >= 0 && v < 10)
    return '0' + v;
  else
    return 'a' + v - 10; // <-- Here
}

void print_address_hex(void* p0) {
  int i;
  uintptr_t p = (uintptr_t)p0;

  write_char('0'); write_char('x');
  for(i = (sizeof(p) << 3) - 4; i>=0; i -= 4) {
    write_char(hex_digit((p >> i) & 0xf));
  }
  write_char('\n');
}
