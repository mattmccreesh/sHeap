#include <stdio.h>
#include <stdlib.h>

int main(){
    void* b = malloc(24);

    char* x = "Worked\0";
    char* xx = "Failed\0";

    if(b == 0xaabbccdd){
        write(0,x,8);
    }else{
        write(0,xx,8);
    }
}
