#include "flist.h"
#include "alloc.h"
#include <stdlib.h>

void* getLocationFromNode ( struct node* node ) {
  // Ensure the node is valid
  if ( (long)node & 0x1111 ) {
    exit ( 1 );
  }
  return ((void*)node - __SHEAP_FLIST_START) * BLOCK_SIZE + __SHEAP_BLOCK_START;
}

struct node* getNodeFromLocation ( void* loc ) {
  // Compute the offset (the number of blocks this location is from the start of the blocks
  long offset = (loc - __SHEAP_BLOCK_START) / BLOCK_SIZE;
  // Compute the actual node (basically the corresponding node_
  struct node* node = (struct node*) (__SHEAP_FLIST_START + ( offset * sizeof ( struct node ) ));
  // Is the pointer aligned as a multiple of 32? It should be for a struct node.
  if ( (long)node & 0x1111 != 0 ) {
    exit ( 1 );
  }
  return node;
}

void* gib_space ( int nBlocks, void* type, struct node** head ) {
  int size;
  size = nBlocks * BLOCK_SIZE + 1;

  struct node* _head = *head;
  if ( _head == NULL ) {
    void* loc = allocate_blocks ( nBlocks );
    struct node* node = (struct node*) __SHEAP_FLIST_UNUSED;
    node->next = NULL;
    node->prev = NULL;
    node->nBlocks = nBlocks;
    node->type = type;
    node->size = size;
    return loc;
  } else {
    struct node* nhead = _head->next;
    void* loc = getLocationFromNode(_head);
    _head->next = NULL;
    if ( nhead != NULL ) {
      nhead->prev = NULL;
    }
    *head = nhead;
    return loc;
  }
}

void kill_space ( void* loc, struct node** head ) {
  struct node* node = getNodeFromLocation ( loc );
  struct node* _head = *head;
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
