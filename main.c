#include "globals.h"
#include "circuit.h"
#include "full_adder.h"
#include "adder.h"
#include "twos_complement.h"
#include "subtractor.h"
#include "multiplier.h"
#include "bitwise_nand.h"
#include "decoder.h"

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

  int n1[] = {1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0};
  int n2[] = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0};
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

  /*
  // int n1[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0}; // overflow test
  // int n2[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
  Circuit *sub = subtractor(n1, n2, n3, &overflow);

  simulateCircuit(sub);

  printf(" ");
  printWord(n1);
  printf("-");
  printWord(n2);
  printf("=\n ");
  printWord(n3);
  printf("overflow=%d\n", overflow);
  */

  /*
  int dec_in[4] = {1, 0, 1, 1};
  int dec_out[16];
  Circuit *dec = decoder(dec_in, dec_out);

  simulateCircuit(dec);

  for (int i = 3; i >= 0; i--)
  {
    printf("%d", dec_in[i]);
  }
  printf("\ndec->");
  for (int i = 15; i >= 0; i--)
  {
    printf("%d", dec_out[i]);
  }
  printf("\n");
  */

  /*
  // int mul_in1[16] = {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // negative mutiplication test
  // int mul_in2[16] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  int mul_out[32];
  int mul_overflow;
  Circuit *mul = multiplier(n1, n2, mul_out, &mul_overflow);
  // Circuit *mul = multiplier(mul_in1, mul_in2, mul_out, &mul_overflow);

  simulateCircuit(mul);

  printf("                ");
  // printWord(n1);
  printWord(mul_in1);
  printf("               *");
  // printWord(n2);
  printWord(mul_in2);
  printf("=\n");
  for (int i = 31; i >= 0; i--)
  {
    printf("%d", mul_out[i]);
  }
  printf("\n");
  */

  Circuit *bit_nand = bitwise_nand(n1, n2, n3);
  simulateCircuit(bit_nand);
  printWord(n1);
  printWord(n2);
  printf("NAND->\n");
  printWord(n3);

  return 0;
}
