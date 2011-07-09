
#include <stdio.h>
#include <ctype.h>
#include "ghpringbuf.h"




void test_printbuf(ghpringbuf *b)
{
  int i;
  double *it = b->items;

  printf("\ncount: %d\n", ghpringbuf_count(b));
  printf("capacity: %d\n", b->capacity);
  printf("iget: %d\n", b->iget);
  printf("iput: %d\n", b->iput);

  for(i = 0; i < b->capacity; ++i)
    {
      printf("buf[%d]:%.2f ", i , *it);
      ++it;
    }
  printf("\n\n");
} 



int main(void)
{
  double z;
  ghpringbuf *buf = ghpringbuf_create(4, sizeof(double), 1, NULL);
  
  if(!buf)
    printf("HUHU");

  char what[5];
  do {
    test_printbuf(buf);

    printf("What now: (q)uit, (p)ut, (g)et&pop, (i)ndex? ");
    scanf("%s", (char*)&what);

    switch (tolower(what[0])) {

    case 'a': /* put */
      printf("number ? ");
      scanf("%lf", &z);
      if(!ghpringbuf_put(buf, &z))
	printf("buffer full!\n");
      break;

    case 'g': /* pop */
      if(ghpringbuf_at(buf, 0, &z))
	{
	  ghpringbuf_pop(buf);
	  printf("got %8.2f from buffer\n", z);
	}
      else
	printf("buffer empty!\n");
      break;

    case 'i':
      printf("what index? ");
      scanf("%lf", &z);
      if(ghpringbuf_at(buf, z, &z))
	printf("got %8.2f from buffer\n", z);
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








int main_old(void)
{
  char what[5];
  double z;

  do {
    printbuf();
    printf("What now? ");
    scanf("%s", (char*)&what);

    switch (tolower(what[0])) {
    case 'a': /* put */
      printf("Zahl ? ");
      scanf("%lf", &z);
      put(z);
      break;
    case 'g': /* pop */
      z = pop();
      printf("got %8.2f from buffert\n", z);
      break;
    case 'i':
      printf("what index? ");
      scanf("%lf", &z);
      z = at(z);
      printf("got %8.2f from buffer\n", z);
      break;
    default:
      printf("%s - unknown command! (look at test.c)\n", what);
    }

  } while (what[0] != 'e');

  return 0;
}
