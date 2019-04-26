#define UNW_LOCAL_ONLY
#include <stdlib.h>
#include <stdio.h>
#include "sheap.h"
#include "util.h"
#include "pool_hash_table.h"
#include "sizetable.h"
#include "flist.h"
#include <libunwind.h>

// Load & define global ptr
void* __SHEAP_BASE = NULL;

int count_valid = 0;
int count_nptr = 0;
void* last_ret = (void*) 100;
void* ret_addr_overwritten;

//unwind stuff
void* fake_call(){
    return (void*) 9;
}

//this should actually be an assembly routine
void detector(){
    void* v = fake_call();
    if(v==last_ret){
       write_char('W');
       write_char('\n');
    }
    else{
        write_char('N');
	write_char('W');
	write_char('\n');
    }
    //asm volatile("mov %0, %r15" : : "r"(ret_addr_overwritten));
    //asm volatile("mov %0, %rax" : : "r"(v));
    asm("mov %0, %%r11" : : "r"(ret_addr_overwritten));
    asm("mov %0, %%rax" : : "r"(v));
    asm("jmp *%r11");
}

void* unwinder()
{
    unw_cursor_t cursor;
    unw_context_t uc;
    unw_word_t ip;
    unw_word_t sp;
    unw_getcontext(&uc);
    unw_init_local(&cursor, &uc);
    unw_step(&cursor);
    unw_step(&cursor);
    unw_get_reg(&cursor, UNW_REG_IP, &ip);
    printf("instruction pointer at %lx\n", (long)ip);
    unw_get_reg(&cursor, UNW_REG_SP, &sp);
    sp -= 8;//minus 8 bytes for return address
    printf("return address at %lx\n", (long)sp);
    last_ret = 0;//value returned by us
    ret_addr_overwritten = (void*) ip;
    void** ret_addr_to_overwrite = (void**) sp;
    *ret_addr_to_overwrite = &detector + 18;
    return (void*) 0;
}


// Kick-off the initialization process of the metaheap construction
void __init_sheap(){
    // Allocate space
    __SHEAP_BASE = allocate_blocks(__SHEAP_PHT_BLOCKS +
                                   __SHEAP_FLIST_BLOCKS +
                                   __SHEAP_ST_BLOCKS);
    // Initialize the system components
    void* pht_end = __init_pht(__SHEAP_BASE);
    void* st_end = __init_st(pht_end);
    void* flist_end = __init_flist(st_end);
}

// Allocates the memory and returns a pointer to it
void* malloc(size_t size){
  if ( PRINT ) {
    write_char('M');
  }
  // Handle 0-case
  if(size == 0){
    return NULL;
  }
  // Check for sheap init
  if(!__SHEAP_BASE){
    __init_sheap();
  }
  // Get the call site address to malloc
  void* call_site = __builtin_return_address(0);
  // Search for pool ptr
  struct pht_entry* pht_e = pht_search(call_site);
  
  //if it is a wrapper
  if(pht_e->pool_ptr != NULL && pht_e->pool_ptr->wrapper_or_alloc_size == 0){
    //unwidn to get real call site
    unw_cursor_t cursor;
    unw_context_t uc;
    unw_word_t ip;
    unw_getcontext(&uc);
    unw_init_local(&cursor, &uc);
    unw_step(&cursor);
    unw_step(&cursor);
    unw_get_reg(&cursor, UNW_REG_IP, &ip);
    call_site = (void*) ip; 
    //redo pht_search for the real call site
    pht_e = pht_search(call_site);
  } 
  else if(pht_e->pool_ptr != NULL && pht_e->pool_ptr->wrapper_or_alloc_size != size){
    //do stack unwinding       
    unw_cursor_t cursor;
    unw_context_t uc;
    unw_word_t ip;
    unw_word_t sp;
    unw_getcontext(&uc);
    unw_init_local(&cursor, &uc);
    unw_step(&cursor);
    unw_step(&cursor);
    unw_get_reg(&cursor, UNW_REG_IP, &ip);
    unw_get_reg(&cursor, UNW_REG_SP, &sp);
    sp -= 8;//minus 8 bytes for return address
    ret_addr_overwritten = (void*) ip;
    void** ret_addr_to_overwrite = (void**) sp;
    //overwrite return address to assembler for detection
    *ret_addr_to_overwrite = &detector + 18;
    //save address as global to compare to in wrapper detection routine 
    last_ret = st_allocate_block(&(pht_e->pool_ptr), size, pht_e->call_site);
    return last_ret;
  }
  return st_allocate_block(&(pht_e->pool_ptr), size, pht_e->call_site);
}

void* _malloc(void* call_site, size_t size) {
  struct pht_entry* pht_e = pht_search(call_site);
  // Return the memory address from ST
  return st_allocate_block(&(pht_e->pool_ptr), size, pht_e->call_site);
}

// Does the same thing as malloc, but zeroes out the memory
void* calloc(size_t nitems, size_t size){
  if ( PRINT ) {
    write_char('C');
  }
    // Make a standard malloc call
    char* l = (char*) malloc(nitems * size);
    // Zero out the memory
    for(int i=0; i<nitems*size; i++){
        l[i] = 0;
    }
    // Return the zeroed memory to the user
    return (void*) l;
}

// Given a pointer to memory and a size, it will attempt to resize the memory chunk
void* realloc(void* ptr, size_t size){
  if ( PRINT ) {
    write_char('R');
  }
  if ( NULL == ptr ) {
    return malloc ( size );
  } else if ( 0 == size ) {
    free ( ptr );
    return NULL;
  }

  void* call_site = __builtin_return_address(0);
  
  struct flist_node* target_node = get_node_from_location(ptr);
  struct pht_entry* pht_e = pht_search(target_node->type);
  struct flist_node** head = st_get_freeptr(pht_e->pool_ptr, target_node->size);

  int old_sc = get_sizeclass_index(target_node->size);
  int new_sc = get_sizeclass_index(size);
  // Do we need to give them more/less space?
  if ( old_sc < new_sc ) {
    // Malloc a new poitner
    char* temp = (char*) _malloc ( call_site, size );
    char* old_ptr = (char*) ptr;
    // Fill it with the old data
    for ( int i = 0; i < target_node->size && i < size; i++ ) {
      temp[i] = old_ptr[i];
    }

    // Free the old pointer
    flist_dealloc_space(ptr, st_get_freeptr(pht_e->pool_ptr, target_node->size));
    // Return the new one
    return (void*) temp;
  } else {
    return ptr;
  }
}

// Frees a memory allocation pointed to by ptr
void free(void* ptr){
    if(ptr){
      if ( PRINT ) {
        write_char('F');
      }
        // Get the flist node  
        struct flist_node* target_node = get_node_from_location(ptr);
        // Get the 
        struct pht_entry* pht_e = pht_search(target_node->type);
        // Free the space
        flist_dealloc_space(ptr, st_get_freeptr(pht_e->pool_ptr, target_node->size));
    }
}

