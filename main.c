#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

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

int* make_ptr(int num){
  int* n = (int*) malloc(sizeof(int));
  *n = num;
  return n;
}

int main(){
  void* p =  realloc ( NULL, 5 );
  realloc ( p, 0 );
  p = realloc ( NULL, 100 );
  printf ( "%p\n", p );
  p = realloc ( p, 16384+1 );
  printf ( "%p\n", p );
  p = realloc ( p, 16384*2+1 );
  printf ( "%p\n", p );
  p = realloc ( p, 16384*4+1 );
  printf ( "%p\n", p );
  //    c = realloc ( c, 10100001001);
}
