#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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


int main(){
    void* t = malloc(24);
    void* b = realloc(t, 25);

    char* x = "Worked\0";
    char* xx = "Failed\0";

    print_address_hex(main);
    if(b == 0xaabbccdd){
        write(0,x,8);
    }else{
        write(0,xx,8);
    }
}
