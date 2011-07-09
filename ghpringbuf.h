/*


*/

#ifndef GHPRINGBUF_H
#define GHPRINGBUF_H

#include <stdlib.h>


/**
  the ringbuffer struct
*/
typedef struct _ghpringbuf {
  void* items;
  size_t iput;      /* index for next item to be put */
  size_t iget;      /* index for next item to be got */
  size_t item_sz;      /* size of one item */
  void (*clean_item)(void *item); /* item cleaner callback. set in case item contains pointers to other stuff. */
  size_t count;        /* number of items in buffer */
  size_t capacity;     /* max item count */
  int lock;            /* internal lock */
  int is_overwriting;  /* if this is an overwriting buffer or not */
  int flags;           /*< general purpose flags to mark buffer */
} ghpringbuf;



/** creates a new empty ringbuffer of capacity capacity, item size item_size,
    if this is an overwriting buffer or not, and a item cleaner callback or NULL */
ghpringbuf* ghpringbuf_create(size_t capacity, size_t item_size, int is_overwriting, void (*item_cleaner)(void*));

/** destroys a ghpringbuf, deallocating all internal data */
void ghpringbuf_destroy(ghpringbuf* b);

/** put item into ringbuffer. returns 1 on success, 0 if buffer full */
int ghpringbuf_put(ghpringbuf* b, void* item);

/** access item at index, will be copied to dst. returns 1 on success, 0 if index out of bounds */
int ghpringbuf_at(ghpringbuf* b, size_t index, void* dst);

/** remove first item. returns 1 on success, 0 if buffer empty */
int ghpringbuf_pop(ghpringbuf* b);

/** get number of buffered items */
size_t ghpringbuf_count(ghpringbuf* b);


#endif
