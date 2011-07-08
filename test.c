

#include "ghpringbuf.h"

/* test.c */

int main(void)
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
