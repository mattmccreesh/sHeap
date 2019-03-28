#include <stdio.h>
#include <stdlib.h>

int main(){
    void* t = malloc(24);
    void* b = realloc(t, 25);

    char* x = "Worked\0";
    char* xx = "Failed\0";

    if(b == 0xaabbccdd){
        write(0,x,8);
    }else{
        write(0,xx,8);
    }
}
