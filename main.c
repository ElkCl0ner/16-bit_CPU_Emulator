#include "globals.h"
#include "circuit.h"
#include "full_adder.h"
#include "adder.h"
#include "twos_complement.h"
#include "subtractor.h"

#include <stdio.h>
#include <stdlib.h>

void printWord(int *word)
{
  for (int i = 15; i >= 0; i--)
  {
    printf("%d", word[i]);
  }
  printf("\n");
}

int main(int argc, char *argv[])
{
  /*
  int n1[] = {1,0};
  int n2[] = {1,1};
  int n3[3];

  Circuit *add1 = full_adder(&n1[0], &n2[0], &zero, &n3[0], &n3[1]);
  Circuit *add2 = full_adder(&n1[1], &n2[1], &n3[1], &n3[1], &n3[2]);

  simulateCircuit(add1);
  simulateCircuit(add2);

  printf("%d%d%d\n", n3[2], n3[1], n3[0]);
  */

  int n1[] = {1,0,0,1,1,0,0,1,0,0,0,0,0,0,0,0};
  int n2[] = {1,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0};
  int n3[16], overflow;

  /*
  Circuit *add = adder(n1, n2, n3, &overflow);

  simulateCircuit(add);

  printf(" ");
  printWord(n1);
  printf("+");
  printWord(n2);
  printf("=\n%d", overflow);
  printWord(n3);
  */

  /*
  Circuit *twos = twos_complement(n1, n3);

  simulateCircuit(twos);

  printWord(n1);
  printf("2s->\n");
  printWord(n3);
  */


  Circuit *sub = subtractor(n1, n2, n3, &overflow);

  simulateCircuit(sub);

  printf(" ");
  printWord(n1);
  printf("-");
  printWord(n2);
  printf("=\n");
  printWord(n3);
  printf("overflow=%d\n", overflow);

  return 0;
}
