#include "globals.h"
#include "circuit.h"
#include "full_adder.h"
#include "eight_bit_adder.h"
#include "twos_complement.h"

#include <stdio.h>
#include <stdlib.h>

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

  /*
  int n1[] = {1,0,0,1,1,0,0,1};
  int n2[] = {1,1,0,0,1,0,0,1};
  int n3[8], overflow;
  Circuit *add = eight_bit_adder(n1, n2, n3, &overflow);

  simulateCircuit(add);

  printf(" ");
  for (int i = 7; i >= 0; i--)
  {
    printf("%d", n1[i]);
  }
  printf(" +\n ");
  for (int i = 7; i >= 0; i--)
  {
    printf("%d", n2[i]);
  }
  printf(" =\n%d", overflow);
  for (int i = 7; i >= 0; i--)
  {
    printf("%d", n3[i]);
  }
  printf("\n");
  */

  int n1[] = {1,0,0,1,1,0,0,1};
  int n2[8];

  Circuit *twos = twos_complement(n1, n2);

  simulateCircuit(twos);

  for (int i = 7; i >= 0; i--)
  {
    printf("%d", n1[i]);
  }
  printf(" ->\n");
  for (int i = 7; i >= 0; i--)
  {
    printf("%d", n2[i]);
  }
  printf("\n");

  return 0;
}
