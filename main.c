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
    char* ptr = (char*) malloc(sizeof(char));
    char* ptr1 = (char*) malloc(sizeof(char));
    char* ptr2 = (char*) malloc(sizeof(char));
    int* mp = make_ptr(4);
    char* ptr3 = (char*) malloc(sizeof(char));
    char* ptr4 = (char*) malloc(sizeof(char));
    char* ptr5 = (char*) malloc(sizeof(char));
    int* mp1 = make_ptr(6);
    print_address_hex(ptr);
    print_address_hex(ptr1);
    print_address_hex(ptr2);
    print_address_hex(ptr3);
    print_address_hex(ptr4);
    print_address_hex(ptr5);
    print_address_hex(mp);
    print_address_hex(mp1);
    //free(ptr);
}
