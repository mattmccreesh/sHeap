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

//wrapper
int* make_ptr(int num){
  int* n = (int*) malloc(sizeof(int)*num);
  *n = num;
  return n;
}

//not a malloc warpper
int non_wrapper(int size){
  void * v = malloc(size);
  return 5;
}

int main(){
  int* a = make_ptr(2);
  int* b = make_ptr(3);
  int x = non_wrapper(2);
  int y = non_wrapper(5);
}

