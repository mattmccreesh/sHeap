#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
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


int main(){
    int fd = open("/tmp/sheap", O_RDWR);
    print_address_hex(fd);
    short* p = (short*) mmap(0, 16<<10, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    print_address_hex(p);

    p[1000] = (short) 5;

    printf("%d", p[1000]);
}
