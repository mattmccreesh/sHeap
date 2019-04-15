#include "sizetable.h"
#include "sheap.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>

void* __SHEAP_ST_START;
void* __SHEAP_ST_END;
struct st_elem* __SHEAP_ST_NEXT;

//stub function to replace with real alloction function
void* get_free_block_address()
{
    return (void*) 0xaabbccdd;
}

//could pull out nElems to be a constant
void* __init_st(void* start_addr)
{
    __SHEAP_ST_START = start_addr;
    __SHEAP_ST_NEXT = __SHEAP_ST_START;
    __SHEAP_ST_END = __SHEAP_ST_START + (BLOCK_SIZE * __SHEAP_ST_BLOCKS);
    return __SHEAP_ST_END;
}

struct st_elem* create_st_elem(size_t alloc_size){
    size_t size_class = get_sizeclass_index(alloc_size);
    struct st_elem* ret = __SHEAP_ST_NEXT++;
    ret->freeptr[size_class] = get_free_block_address();//stub function to be replaced
    return ret;
}

void* st_get_freeptr(struct st_elem* table_elem, size_t alloc_size)
{
    size_t size_class = get_sizeclass_index(alloc_size);
    return table_elem->freeptr[size_class];
}

/*
void allocate_block_from_sizetable(struct st_elem* table_elem, size_t allocSize)
{
    void* block = st_get_freeptr(tableElem, allocSize);i
    size_t size_class = allocSize/BLOCK_SIZE;
    if(block == NULL){
        tableElem->freeptr[size_class] = getFreeBlockAddress();
    }
    else{
        //call function from freelist to remove from freelist
    } 
}*/

int get_sizeclass_index(size_t alloc_size){
    int index = 0;
    alloc_size--;//16KB can fit on 1 block, so account for that here
    alloc_size = alloc_size>>14;
    while(alloc_size != 0){
        alloc_size = alloc_size>>1;
        index++;
    }
    return index;
}
