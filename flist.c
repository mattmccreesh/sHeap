#include "flist.h"

void* gib_space ( int nBlocks, void* type, struct node** head ) {
  int size;
  size = nBlocks * __SHEAP_BLOCK_SIZE + 1;

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
    void* loc = (_head - __SHEAP_FLIST_START) * __SHEAP_BLOCK_SIZE + __SHEAP_BLOCK_START;
    _head->next = NULL;
    *head = nhead;
    return loc;
  }
}

int kill_space ( void* loc ) {
  // This is the number of blocks this is away from the start of the block section
  int offset = (loc - __SHEAP_BLOCK_START) / __SHEAP_BLOCK_SIZE;
  struct node* node = (struct node*) (__SHEAP_FLIST_START + ( offset * sizeof ( struct node ) ));
  if ( node & 0x1111 != 0 ) {
    // This means that the pointer is not aligned correctly.
    exit ( 1 );
  }
  // Now we need to free the location
  
}
