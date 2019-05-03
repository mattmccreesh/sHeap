#define UNW_LOCAL_ONLY
#include <stdlib.h>
#include <stdio.h>
#include "sheap.h"
#include "util.h"
#include "pool_hash_table.h"
#include "sizetable.h"
#include "flist.h"
#include <libunwind.h>
#include <pthread.h>

pthread_mutex_t alloc_mutex;//to protect alignment of metadata to data

// Load & define global ptr
void* __SHEAP_BASE = NULL;

extern void* __SHEAP_LAST_MALLOCD;
extern void* __SHEAP_LAST_VALID_RET;

pthread_key_t key_last_ret;
pthread_key_t key_ret_addr_overwritten;
pthread_key_t key_overwritten_stack_location;
pthread_key_t key_wrapper_entry;

void markAsWrapper(){
  ((struct st_elem*) pthread_getspecific(key_wrapper_entry))->wrapper_or_alloc_size = 0;
  pthread_setspecific(key_overwritten_stack_location, NULL);
}


void markAsNonWrapper(){
  ((struct st_elem*) pthread_getspecific(key_wrapper_entry))->wrapper_or_alloc_size = (size_t)-1;
  pthread_setspecific(key_overwritten_stack_location, NULL);
}

//overwrite with this addr + 4 (4 to avoid prologue of function)
//will interept return of susupected wrapper, determine if it really is a wrapper
//must get state back to caller of wrapper as if returned directly to it
void wrapperDetector(){
  //archive value returned but intercepted
  asm("mov %rax, %r11");
  asm("push %r11");//push as register may change in function call
  //asm("mov %0, %%rax" : : "r"(last_ret));
  pthread_getspecific(key_last_ret);//get this in rax
  //if return value (rax) of suspected warpper is same as last malloc ret, return
  asm("cmp %rax, %r11");
  asm goto("jne %l0\n" : : : : notWrapper);
  //if here it is a wrapper
  /*
  write_char('W');
  write_char('\n');*/
  markAsWrapper();
  asm goto("jmp %l0\n" : : : : jmpBack);
notWrapper:
  //mark as non-wrapper
  /*
  write_char('N');
  write_char('W');
  write_char('\n');*/
  markAsNonWrapper();
jmpBack:
  pthread_getspecific(key_ret_addr_overwritten);//put this in rax
  asm("mov %rax, %r11");
  asm("pop %rax");//restore stack, get rax back to propogate return value properly
  asm("jmp *%r11");
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
    pthread_key_create(&key_ret_addr_overwritten, NULL);   
    pthread_key_create(&key_overwritten_stack_location, NULL);   
    pthread_key_create(&key_wrapper_entry, NULL);   
    pthread_key_create(&key_last_ret, NULL);   
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
  
  pthread_mutex_lock(&alloc_mutex);
  
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
    //unwind to get real call site
    /*write_char('W');
    write_char('\n');*/
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
  else if(pht_e->pool_ptr != NULL && pht_e->pool_ptr->wrapper_or_alloc_size != size && pht_e->pool_ptr->wrapper_or_alloc_size != (size_t)-1){
    //do stack unwinding
    /* 
    write_char('S');
    write_char('W');
    write_char('\n');*/
    //first step is to see if it is nested suspected wrapper
    //if we are already probing caller, we should stop probing outer [it is highly unikely to be a malloc wrapper] but still probe inner
    if(pthread_getspecific(key_overwritten_stack_location) != NULL){
      *((void**)pthread_getspecific(key_overwritten_stack_location)) = pthread_getspecific(key_ret_addr_overwritten);//abort probing for outer 
    }

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
    void* ret_addr_on_stack = (void*)ip;
    void** ret_addr_to_overwrite = (void**) sp;
    //must ensre ALL wrapper detection globals correspond to the current suspected wrapper
    pthread_setspecific(key_overwritten_stack_location, ret_addr_to_overwrite);
    //overwrite return address to assembler for detection
    //to prevent infinite loop of detector, probably unnecessary now that we check if for nested suspected wrappers
    //nested suspected wrappper check should handle 2 mallocs in suspected wrapper to prevent infinite loop there
    if(ret_addr_on_stack != &wrapperDetector +4){
      pthread_setspecific(key_ret_addr_overwritten, ret_addr_on_stack);
      *ret_addr_to_overwrite = &wrapperDetector + 4;//+4 to skip prologue of function
      //save address as global to compare to in wrapper detection routine 
      pthread_setspecific(key_last_ret, st_allocate_block(&(pht_e->pool_ptr), size, pht_e->call_site));
      pthread_setspecific(key_wrapper_entry, pht_e->pool_ptr);
      pthread_mutex_unlock(&alloc_mutex);
      return pthread_getspecific(key_last_ret);
    }//in else case we could abort detection of previous guy too but not necessary
  }
  void* ret = st_allocate_block(&(pht_e->pool_ptr), size, pht_e->call_site);
  pthread_mutex_unlock(&alloc_mutex);
  return ret;
}

void* _malloc(void* call_site, size_t size) {
  pthread_mutex_lock(&alloc_mutex);
  struct pht_entry* pht_e = pht_search(call_site);
  // Return the memory address from ST
  void* ret = st_allocate_block(&(pht_e->pool_ptr), size, pht_e->call_site);
  pthread_mutex_unlock(&alloc_mutex);
  return ret;
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
      if(size > target_node->size){//in order to correctly track number of blocks, only update size if it grows as still all blocks will be there
        target_node->size = size;
      }
    return ptr;
  }
}

// Frees a memory allocation pointed to by ptr
void free(void* ptr){
    if(ptr && (ptr >=  __SHEAP_BLOCK_START && ptr <= __SHEAP_LAST_MALLOCD)){
        if ( PRINT ) {
            write_char('F');
        }
        pthread_mutex_lock(&alloc_mutex);
        // Get the flist node  
        struct flist_node* target_node = get_node_from_location(ptr);
        // Get the pht_entry
        struct pht_entry* pht_e = pht_search(target_node->type);
        // Free the space
        flist_dealloc_space(ptr, st_get_freeptr(pht_e->pool_ptr, target_node->size));
	    pthread_mutex_unlock(&alloc_mutex);
    }
}
