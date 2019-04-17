#include "sizetable.h"
#include "sheap.h"
#include "flist.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>

void* __SHEAP_ST_START;
void* __SHEAP_ST_END;
struct st_elem* __SHEAP_ST_NEXT;

void* __init_st(void* start_addr){
    __SHEAP_ST_START = start_addr;
    __SHEAP_ST_NEXT = __SHEAP_ST_START;
    __SHEAP_ST_END = __SHEAP_ST_START + (BLOCK_SIZE * __SHEAP_ST_BLOCKS);
    return __SHEAP_ST_END;
}

void* st_allocate_block(struct st_elem** pool_ptr, size_t alloc_size, void* call_site){
    if(*pool_ptr == 0){
        *pool_ptr = create_st_elem(alloc_size);
    }
    size_t size_class = get_sizeclass_index(alloc_size);
    int num_blocks = 1<<size_class;//should allocate all blocks for size class even if some unused for specific allocation
    return flist_alloc_space(num_blocks, call_site, &((*pool_ptr)->freeptr[size_class])); 
}


//TODO: ensure I don't overflow into next part of heap data
struct st_elem* create_st_elem(){
    struct st_elem* ret = __SHEAP_ST_NEXT++;
    if((void*)__SHEAP_ST_NEXT > __SHEAP_ST_END){
        exit(1);//we ran out of space for size table
    }
    return ret;
}

struct flist_node** st_get_freeptr(struct st_elem* table_elem, size_t alloc_size){
    size_t size_class = get_sizeclass_index(alloc_size);
    return &table_elem->freeptr[size_class];
}


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
