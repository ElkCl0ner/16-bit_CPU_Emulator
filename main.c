#include "circuit.h"
#include "full_adder.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int zero = 0;
  int n1[] = {1,0};
  int n2[] = {1,1};
  int n3[3];

  Circuit add1 = full_adder(&n1[0], &n2[0], &zero, &n3[0], &n3[1]);
  Circuit add2 = full_adder(&n1[1], &n2[1], &n3[1], &n3[1], &n3[2]);

  simulateCircuit(&add1);
  // printf("%d%d%d\n", n3[2], n3[1], n3[0]);

  simulateCircuit(&add2);

  // Result
  printf("%d%d%d\n", n3[2], n3[1], n3[0]);

  // int A = 1;
  // int B = 1;
  // int C_in = 1;
  // int sum, C_out;

  // Circuit add = full_adder(&A, &B, &C_in, &sum, &C_out);

  // simulateCircuit(&add);

  // printf("A = %d\n", A);
  // printf("B = %d\n", B);
  // printf("C_in = %d\n", C_in);
  // printf("sum = %d\n", sum);
  // printf("C_out = %d\n", C_out);

  return 0;
}
