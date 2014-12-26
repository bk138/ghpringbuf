/*
  Simple test program for ghpringbuf.
*/


#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "ghpringbuf.h"


typedef struct _B
{
  double d;
  double e;
} B;


typedef struct _A
{
  int x;
  int y;
  int z;
  B* ptr_B;
} A;


/* 
   Since A's can be overwritten or deleted from the buffer,
   we need to supply a callback that cleans any deep references
   contained in an A.
 */
void A_cleaner(void * p)
{
  free(((A*)p)->ptr_B);
  printf("cleaned out %p\n", p);
}


void test_printbuf(ghpringbuf *b)
{
  int i;
  A *it = b->items;

  printf("\ncount: %zu\n", ghpringbuf_count(b));
  printf("capacity: %zu\n", b->capacity);
  printf("iget: %zu\n", b->iget);
  printf("iput: %zu\n", b->iput);

  for(i = 0; i < b->capacity; ++i)
    {
      if(it->ptr_B)
	printf("buf[%d]:%.2f ", i , it->ptr_B->d);
      else
	printf("buf[%d]: - ", i);
      ++it;
    }
  printf("\n\n");
} 



int main(void)
{
  double z;
  A a;
  A *a_ptr;
  ghpringbuf *buf = ghpringbuf_create(4, sizeof(A), 1, A_cleaner);
  
  
  char what[5];
  do {
    test_printbuf(buf);

    printf("What now: (q)uit, (p)ut, (g)et&pop, (i)ndex? ");
    scanf("%s", (char*)&what);

    switch (tolower(what[0])) {

    case 'a': /* put */
      printf("number ? ");
      scanf("%lf", &z);

      a.ptr_B = malloc(sizeof(B));
      a.ptr_B->d = z;

      if(!ghpringbuf_put(buf, &a))
	printf("buffer full!\n");
      break;

    case 'g': /* pop */
      
      if(a_ptr = ghpringbuf_at(buf, 0))
	{
	  memcpy(&a, a_ptr, sizeof(A));  
	  ghpringbuf_pop(buf);
	  printf("got %8.2f from buffer\n", a.ptr_B->d);
	}
      else
	printf("buffer empty!\n");
      break;

    case 'i':

      printf("what index? ");
      scanf("%lf", &z);
      if(a_ptr = ghpringbuf_at(buf, z)) {
	memcpy(&a, a_ptr, sizeof(A));
	printf("got %8.2f from buffer\n", a.ptr_B->d);
      }
      else
	printf("invalid index!\n");
      break;

    case 'q':
      printf("bye\n");
      ghpringbuf_destroy(buf);
      exit(EXIT_SUCCESS);
      break;

    default:
      printf("%s - unknown command!\n", what);
    }

  } while (1);

  return 0;
}



