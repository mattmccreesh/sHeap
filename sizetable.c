#include "sizetable.h"
#include "sheap.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>

void* __SHEAP_SIZETABLE_START;
void* __SHEAP_SIZETABLE_END;
struct size_table_elem* __SHEAP_SIZETABLE_NEXT;

//stub function to replace with real alloction function
void* getFreeBlockAddress()
{
    return (void*) 0xaabbccdd;
}

//could pull out nElems to be a constant
void initialize_sizetable(int nElems)
{
    __SHEAP_SIZETABLE_START = mmap(0, nElems*sizeof(struct size_table_elem), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    __SHEAP_SIZETABLE_NEXT = __SHEAP_SIZETABLE_START;
    __SHEAP_SIZETABLE_END = __SHEAP_SIZETABLE_START + nElems*sizeof(struct size_table_elem);
}

//expand size table to support more allocation sites. table will likely end up fragmented in address space
void expand_sizetable(int nElems)
{
    __SHEAP_SIZETABLE_NEXT = mmap(__SHEAP_SIZETABLE_END, nElems*sizeof(struct size_table_elem), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    __SHEAP_SIZETABLE_END = __SHEAP_SIZETABLE_NEXT + nElems*sizeof(struct size_table_elem);
}

struct size_table_elem* create_sizetable_elem(size_t allocSize){
    if(allocSize > BLOCK_SIZE*NUM_LARGE_SIZE_CLASSES){
        exit(1);//invalid allocation size
    }
    if((void*)__SHEAP_SIZETABLE_NEXT >= __SHEAP_SIZETABLE_END){
        //printf("expanding\n");
        expand_sizetable(10);//should use intelligently decided constant
    }
    size_t size_class = allocSize/BLOCK_SIZE;
    struct size_table_elem* ret = __SHEAP_SIZETABLE_NEXT++;
    ret->freeptr[size_class] = getFreeBlockAddress();//stub function to be replaced
    return ret;
}

void* get_sizetable_freeptr(struct size_table_elem* tableElem, size_t allocSize)
{
    size_t size_class = allocSize/BLOCK_SIZE;
    if(size_class >= NUM_LARGE_SIZE_CLASSES){
        exit(1);//allocation size too large
    }
    return tableElem->freeptr[size_class];
}

/*
void allocate_block_from_sizetable(struct size_table_elem* tableElem, size_t allocSize)
{
    void* block = get_sizetable_freeptr(tableElem, allocSize);i
    size_t size_class = allocSize/BLOCK_SIZE;
    if(block == NULL){
        tableElem->freeptr[size_class] = getFreeBlockAddress();
    }
    else{
        //call function from freelist to remove from freelist
    } 
}*/

int get_sizeclass_index(size_t allocSize){
    int index = 0;
    allocSize--;//16KB can fit on 1 block, so account for that here
    allocSize = allocSize>>14;
    while(allocSize != 0){
        allocSize = allocSize>>1;
        index++;
    }
    return index;
}
