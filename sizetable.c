#include "sizetable.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>

void* __SHEAP_SIZETABLE_START;
void* __SHEAP_SIZETABLE_END;

void initialize_sizetable(int nElems)
{
    __SHEAP_SIZETABLE_START = mmap(0, nElems*sizeof(struct size_table_elem), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    __SHEAP_SIZETABLE_END = __SHEAP_SIZETABLE_START + nElems*sizeof(struct size_table_elem);
}
void expand_sizetable(int nElems)
{
    mmap(__SHEAP_SIZETABLE_END, nElems*sizeof(struct size_table_elem), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    __SHEAP_SIZETABLE_END += nElems*sizeof(struct size_table_elem);
}
/*
int main()
{
    initialize_sizetable(20);
    printf("%p\n", __SHEAP_SIZETABLE_START);
}*/
