/* 

*/


#include <string.h>
#include "ghpringbuf.h"


ghpringbuf* ghpringbuf_create(size_t capacity, size_t item_size, int is_overwriting, void (*item_cleaner)(void*))
{
  ghpringbuf* b = calloc(1, sizeof(ghpringbuf));
  if(!b)
    return NULL;
  b->capacity = capacity;
  b->item_sz = item_size;
  b->is_overwriting = is_overwriting;
  b->clean_item = item_cleaner;
  b->items = calloc(capacity, item_size);
  if(!b->items)
    {
      free(b);
      return NULL;
    }
  return b;
}


void ghpringbuf_destroy(ghpringbuf* b)
{
  if(b->clean_item)
    {
      size_t i, count = ghpringbuf_count(b); /* pop decrements count */
      for(i=0; i < count; ++i)
	ghpringbuf_pop(b);
    }
  free(b->items);
  free(b);
}


int ghpringbuf_put(ghpringbuf* b, void* item)
{
  b->lock = 1;
  if (b->count < b->capacity)
    {
      char* it = b->items;
      it += b->item_sz * b->iput;
      memcpy(it, item, b->item_sz);
      b->iput = (b->iput+1) == b->capacity ? 0 : b->iput+1; /* advance or wrap around */
      b->count++;
    }
  else
    {
      if(b->is_overwriting)
	{
	  char* it = b->items;
	  it += b->item_sz * b->iput;

	  if(b->clean_item) /* clean out item that we will overwrite */
	    b->clean_item(it);

	  memcpy(it, item, b->item_sz);
	  b->iget = b->iput = (b->iput+1) == b->capacity ? 0 : b->iput+1; /* advance or wrap around */
	}
      else
	{
	  b->lock = 0;
	  return 0; /* buffer full */
	}
    }
  b->lock = 0;
  return 1;
}


int ghpringbuf_at(ghpringbuf* b, size_t index, void* dst)
{
  b->lock = 1;
  if (b->count > 0 && index < b->count) 
    {
      size_t pos = b->iget + index;
      if(pos >= b->capacity)
	pos -= b->capacity;

      char* it = b->items;
      it += b->item_sz * pos;
      memcpy(dst, it, b->item_sz);
      b->lock = 0;
      return 1;
    }
  b->lock = 0;
  return 0;
}


int ghpringbuf_pop(ghpringbuf* b)
{
  b->lock = 1;
  if (b->count > 0) 
    {
      if(b->clean_item)
	{
	  char* it = b->items;
	  it += b->item_sz * b->iget; /* go to iget index */
	  if(b->clean_item) /* clean out item that we will abandon */
	    b->clean_item(it);
	}

      b->iget = (b->iget+1) == b->capacity ? 0 : b->iget+1; /* advance or wrap around */
      b->count--;
      b->lock = 0;
      return 1;
    }
  b->lock = 0;
  return 0;
}


size_t ghpringbuf_count(ghpringbuf* b)
{
  return b->count;
}












#include <stdio.h>


#define NMAX 4

int iput = 0;  /* Position fuer naechstes abzulegende Zeichen */
int iget = 0;  /* Position fuer naechstes zu lesendes Zeichen */
int n = 0;     /* Anzahl der Zeichen im Buffer */
int isOverwriting = 0;

double buffer[NMAX];


/* addring schreitet im Ring um eins weiter und sorgt dafuer dass
   (NMAX - 1) + 1 nicht NMAX sondern wieder 0 ist */

int advance(int i)
{
  return (i+1) == NMAX ? 0 : i+1;
}

/* pop front */
double pop(void)
{
  int pos;

  if (n > 0) {
    pos = iget;
    iget = advance(iget);
    n--;
    return buffer[pos];
    }
  else {
    printf("Buffer ist leer\n");
    return 0.0;
  }
}

void put(double z)
{
  if (n < NMAX) {
    buffer[iput] = z;
    iput = advance(iput);
    n++;
    }
  else
    if(isOverwriting)
    {
      buffer[iput] = z;
      iget = iput = advance(iput);
    }
    else
      printf("Buffer ist voll\n");
}


double at(int idx)
{
  if (n > 0 && idx < n) {
    int pos = iget + idx;
    if(pos >= NMAX)
      pos -= NMAX;
    return buffer[pos];
    }
  else {
    printf("Buffer leer oder index ungültig\n");
    return 0.0;
  }
}


void printbuf()
{
  int i;
  printf("\nbufsize: %d\n", n);
  printf("iget: %d\n", iget);
  printf("iput: %d\n", iput);
  for(i=0; i < NMAX; ++i)
    printf("buf[%d]:%.2f ", i , buffer[i]);
  printf("\n\n");
} 


