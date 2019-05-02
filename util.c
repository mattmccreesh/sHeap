#include "util.h"
#include <unistd.h>
#include <stdint.h>

void* __SHEAP_LAST_MALLOCD = NULL;

/**
 *  Allocates a number of blocks and returns a pointer to the start of them
 *  @param nBlocks The number of blocks to allocate
 *  @returns A pointer to the start of the allocated blocks
 */
void* allocate_blocks ( int n_blocks ) {
    void* malloc_addr =  sbrk(BLOCK_SIZE * n_blocks );
    __SHEAP_LAST_MALLOCD = malloc_addr;
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
