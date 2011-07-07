/* ringbuffer.c
die Verwendung von n, um sich zu merken, wieviele Zahlen momentan
im Buffer sind, vereinfacht die Loesung stark
*/

#include <stdio.h>
#include <ctype.h>

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




/* ringbuffertest.c */

int main(void)
{
  char wastun[5];
  double z;

  do {
    printbuf();
    printf("Was tun ? ");
    scanf("%s", (char*)&wastun);

    switch (tolower(wastun[0])) {
      case 'a': /* put */
        printf("Zahl ? ");
        scanf("%lf", &z);
        put(z);
        break;
      case 'g': /* pop */
        z = pop();
        printf("%8.2f vom Buffer geholt\n", z);
        break;
      case 'e':
        printf("Ende\n");
        break;
    case 'i':
      printf("index ? ");
      scanf("%lf", &z);
      z = at(z);
        printf("%8.2f vom Buffer geholt\n", z);
        break;
      default:
        printf("%s - ungueltiger Befehl!\n", wastun);
    } /* end switch */

  } while (wastun[0] != 'e');

  return 0;

}
