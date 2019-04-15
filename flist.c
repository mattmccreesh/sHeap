#include "flist.h"
#include "util.h"
#include <stdlib.h>
#include <stdint.h>

void* __SHEAP_FLIST_START;
void* __SHEAP_BLOCK_START;
void* __SHEAP_FLIST_UNUSED;

void* __init_flist(void* start_addr){
    __SHEAP_FLIST_START = start_addr;
    __SHEAP_FLIST_UNUSED = start_addr;
    __SHEAP_BLOCK_START = start_addr + (BLOCK_SIZE+__SHEAP_FLIST_BLOCKS);
    return __SHEAP_BLOCK_START;
}

void* get_location_from_node ( struct flist_node* node ) {
  // Ensure the node is valid
  if ( (long)node & 0x1111 ) {
    exit ( 1 );
  }
  return ((void*)node - __SHEAP_FLIST_START) * BLOCK_SIZE + __SHEAP_BLOCK_START;
}

struct flist_node* get_node_from_location ( void* loc ) {
  // Compute the offset (the number of blocks this location is from the start of the blocks
  long offset = (loc - __SHEAP_BLOCK_START) / BLOCK_SIZE;
  // Compute the actual node (basically the corresponding node_
  struct flist_node* node = (struct flist_node*) (__SHEAP_FLIST_START + ( offset * sizeof ( struct flist_node ) ));
  // Is the pointer aligned as a multiple of 32? It should be for a struct node.
  if ( (long)node & 0x1111 != 0 ) {
    exit ( 1 );
  }
  return node;
}

void* flist_alloc_space ( int n_blocks, void* type, struct flist_node** head ) {
  int size;
  size = n_blocks * BLOCK_SIZE + 1;

  struct flist_node* _head = *head;
  if ( _head == NULL ) {
    void* loc = allocate_blocks ( n_blocks );
    struct flist_node* node = (struct flist_node*) __SHEAP_FLIST_UNUSED;
    node->next = NULL;
    node->prev = NULL;
    node->n_blocks = n_blocks;
    node->type = type;
    node->size = size;
    return loc;
  } else {
    struct flist_node* nhead = _head->next;
    void* loc = get_location_from_node(_head);
    _head->next = NULL;
    if ( nhead != NULL ) {
      nhead->prev = NULL;
    }
    *head = nhead;
    return loc;
  }
}

void flist_dealloc_space ( void* loc, struct flist_node** head ) {
  struct flist_node* node = get_node_from_location ( loc );
  struct flist_node* _head = *head;
  // Now we need to free the location
  if ( _head == NULL ) {
    _head = node;
    _head->next = _head->prev = NULL;
  } else {
    node->next = _head;
    _head->prev = node;
    node->prev = NULL;
    *head = node;
  }
}
