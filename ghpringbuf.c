/* ringbuffer.c
die Verwendung von n, um sich zu merken, wieviele Zahlen momentan
im Buffer sind, vereinfacht die Loesung stark
*/

#include <stdio.h>
#include <ctype.h>

#include "ghpringbuf.h"

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


