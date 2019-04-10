#include "sizetable.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>

void* __SHEAP_SIZETABLE_START;
void* __SHEAP_SIZETABLE_END;
struct size_table_elem* __SHEAP_SIZETABLE_NEXT;

void initialize_sizetable(int nElems)
{
    __SHEAP_SIZETABLE_START = mmap(0, nElems*sizeof(struct size_table_elem), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    __SHEAP_SIZETABLE_NEXT = __SHEAP_SIZETABLE_START;
    __SHEAP_SIZETABLE_END = __SHEAP_SIZETABLE_START + nElems*sizeof(struct size_table_elem);
}

void expand_sizetable(int nElems)
{
    mmap(__SHEAP_SIZETABLE_END, nElems*sizeof(struct size_table_elem), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    __SHEAP_SIZETABLE_END += nElems*sizeof(struct size_table_elem);
}

struct size_table_elem* create_sizetable_elem(){
    if((void*)__SHEAP_SIZETABLE_NEXT >= __SHEAP_SIZETABLE_END){
        expand_sizetable(10);//should use intelligently decided constant
    }
    struct size_table_elem* ret = __SHEAP_SIZETABLE_NEXT++;
    return ret;
}
