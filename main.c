#include "globals.h"
#include "circuit.h"
#include "full_adder.h"
#include "adder.h"
#include "twos_complement.h"
#include "subtractor.h"
#include "multiplier.h"
#include "bitwise_nand.h"
#include "alu.h"
#include "decoder.h"
#include "control_unit.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void printArr(char *arr_ptr, int arr_len)
{
  for (int i = arr_len - 1; i >= 0; i--)
  {
    printf("%d", arr_ptr[i]);
  }
  printf("\n");
}

void printWord(char *word)
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
  char n1[] = {1,0};
  char n2[] = {1,1};
  char n3[3];

  Circuit *add1 = full_adder(&n1[0], &n2[0], &zero, &n3[0], &n3[1]);
  Circuit *add2 = full_adder(&n1[1], &n2[1], &n3[1], &n3[1], &n3[2]);

  simulateCircuit(add1);
  simulateCircuit(add2);

  printf("%d%d%d\n", n3[2], n3[1], n3[0]);
  */

  char n1[16] = {1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0};
  char n2[16] = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0};
  // char n1[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // to test ALU zero flag
  // char n2[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  char n3[16], overflow;
  clock_t start, end;
  start = clock();

  printf("adder\n");
  Circuit *add = adder(n1, n2, n3, &overflow);
  simulateCircuit(add);
  printf(" ");
  printWord(n1);
  printf("+");
  printWord(n2);
  printf("=\n%d", overflow);
  printWord(n3);

  printf("twos complement\n");
  Circuit *twos = twos_complement(n1, n3);
  simulateCircuit(twos);
  printWord(n1);
  printf("2s->\n");
  printWord(n3);

  printf("subtractor\n");
  // char n1[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0}; // overflow test
  // char n2[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
  Circuit *sub = subtractor(n1, n2, n3, &overflow);
  simulateCircuit(sub);
  printf(" ");
  printWord(n1);
  printf("-");
  printWord(n2);
  printf("=\n ");
  printWord(n3);
  printf("overflow=%d\n", overflow);

  printf("decoder\n");
  char dec_in[4] = {1, 0, 1, 1};
  char dec_out[16];
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

  printf("multiplier\n");
  // char mul_in1[16] = {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // negative mutiplication test
  // char mul_in2[16] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}; // negative mutiplication test
  char mul_out[32];
  Circuit *mul = multiplier(n1, n2, mul_out); // default
  // Circuit *mul = multiplier(mul_in1, mul_in2, mul_out); // negative mutiplication test
  simulateCircuit(mul);
  printf("                ");
  printWord(n1); // default
  // printWord(mul_in1); // negative mutiplication test
  printf("               *");
  printWord(n2); // default
  // printWord(mul_in2); // negative mutiplication test
  printf("=\n");
  for (int i = 31; i >= 0; i--)
  {
    printf("%d", mul_out[i]);
  }
  printf("\n");

  printf("bitwise NAND\n");
  Circuit *bit_nand = bitwise_nand(n1, n2, n3);
  simulateCircuit(bit_nand);
  printWord(n1);
  printWord(n2);
  printf("NAND->\n");
  printWord(n3);

  printf("alu\n");
  char z;
  char control_add = 1;
  char control_sub = 0;
  char control_mul = 0;
  char control_nand = 0;
  Circuit *al = alu(n1, n2, n3, &z, &control_add, &control_sub, &control_mul, &control_nand);
  simulateCircuit(al);
  printWord(n1);
  printWord(n2);
  if (control_add)
    printf("add\n");
  if (control_sub)
    printf("sub\n");
  if (control_mul)
    printf("mul\n");
  if (control_nand)
    printf("nand\n");
  printWord(n3);
  printf("zero=%d\n", z);

  printf("control unit\n");
  // char cu_instruction[16] = {1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0}; // MOVI R5 #13
  char cu_instruction[16] = {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1}; // BLZ #5
  char cu_Zin = 0;
  char cu_Rdst[4];
  char cu_RA[4];
  char cu_RB[4];
  char cu_Addr[4];
  char cu_Imm[16];
  char cu_zeroRA;
  char cu_LSL[16];
  char cu_useImm;
  char cu_add;
  char cu_sub;
  char cu_mul;
  char cu_nand;
  char cu_memLoad;
  char cu_memStore;
  char cu_setZ;
  char cu_link;
  char cu_storeOutputToRdst;
  char cu_halt;
  Circuit *cu = control_unit(
      cu_instruction,
      &cu_Zin,
      cu_Rdst,
      cu_RA,
      cu_RB,
      cu_Addr,
      cu_Imm,
      &cu_zeroRA,
      cu_LSL,
      &cu_useImm,
      &cu_add,
      &cu_sub,
      &cu_mul,
      &cu_nand,
      &cu_memLoad,
      &cu_memStore,
      &cu_setZ,
      &cu_link,
      &cu_storeOutputToRdst,
      &cu_halt);
  simulateCircuit(cu);
  printf("Rdst=");
  printArr(cu_Rdst, 4);
  printf("RA=");
  printArr(cu_RA, 4);
  printf("RB=");
  printArr(cu_RB, 4);
  printf("Addr=");
  printArr(cu_Addr, 4);
  printf("Imm=");
  printWord(cu_Imm);
  printf("zeroRA=%d\n", cu_zeroRA);
  printf("LSL=");
  printWord(cu_LSL);
  printf("useImm=%d\n", cu_useImm);
  printf("add=%d\n", cu_add);
  printf("sub=%d\n", cu_sub);
  printf("mul=%d\n", cu_mul);
  printf("nand=%d\n", cu_nand);
  printf("memLoad=%d\n", cu_memLoad);
  printf("memStore=%d\n", cu_memStore);
  printf("setZ=%d\n", cu_setZ);
  printf("link=%d\n", cu_link);
  printf("storeOutputToRdst=%d\n", cu_storeOutputToRdst);
  printf("halt=%d\n", cu_halt);

  end = clock();
  printf("total time=%f\n", ((double)(end - start)) / CLOCKS_PER_SEC);

  return 0;
}
