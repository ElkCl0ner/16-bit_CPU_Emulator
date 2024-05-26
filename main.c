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
#include "register_writer.h"
#include "register_reader.h"
#include "z_flag_writer.h"
#include "memory.h"
#include "cpu.h"

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

  char n1[16] = {1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}; // 153
  char n2[16] = {1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}; // 147
  // char n1[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // to test ALU zero flag
  // char n2[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  char n3[16], overflow;
  clock_t start, end;
  start = clock();
  /*
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

  printf("register writer\n");
  char writer_registers[256];
  for (int i = 0; i < 16; i++)
  {
    writer_registers[i * 16] = 0;
  }
  char writer_alu_output[16] = {1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  char writer_Rdst[4] = {0, 0, 0, 0};
  char writer_storeOutput = 1;
  Circuit *writer = register_writer(writer_registers, writer_alu_output, writer_Rdst, &writer_storeOutput);
  simulateCircuit(writer);
  for (int i = 15; i >= 0; i--)
  {
    printf("%d", writer_registers[i * 16]);
  }
  printf("\n");

  printf("register reader\n");
  char reader_registers[256];
  for (int i = 0; i < 256; i++)
  {
    reader_registers[i] = 0;
  }
  reader_registers[0] = 1;
  reader_registers[1] = 1;
  reader_registers[17] = 1;
  char reader_RAin[4] = {0, 0, 0, 0};
  char reader_RBin[4] = {1, 0, 0, 0};
  char reader_RAout[16];
  char reader_RBout[16];
  Circuit *reader = register_reader(reader_registers, reader_RAin, reader_RBin, reader_RAout, reader_RBout);
  simulateCircuit(reader);
  printf("RA_out=");
  printWord(reader_RAout);
  printf("RB_out=");
  printWord(reader_RBout);

  printf("z flag writer\n");
  char z_z = 1;
  char z_alu_zero = 0;
  char z_cu_setZ = 0;
  Circuit *z_writer = z_flag_writer(&z_z, &z_alu_zero, &z_cu_setZ);
  printf("z_before=%d\n", z_z);
  simulateCircuit(z_writer);
  printf("z_after=%d\n", z_z);
  printf("alu_zero=%d\n", z_alu_zero);
  printf("cu_setZ=%d\n", z_cu_setZ);
  */

  /*
  printf("memory\n");
  char mem[16];
  for (int i = 0; i < 16; i++)
  {
    mem[i] = i;
  }
  char addr[16] = {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  char d_out[16];
  char d_in[16] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  memRead(mem, addr, d_out);
  printWord(d_out);
  memWrite(mem, addr, d_in);
  memRead(mem, addr, d_out);
  printWord(d_out);
  */

  Cpu *cpu = createCpu();
  for (int i = 0; i < 256; i++)
  {
    cpu->registers[i] = 0;
  }
  cpu->memory[1] = 0b01000001; // MOV1 R1 0b1011
  cpu->memory[0] = 0b00001011;
  cpu->memory[2] = 0b00100000; // BL #32 (PC should end up at #32+#4 when halted (assuming instruction at addr #36 is HLT))
  cpu->memory[3] = 0b10100000;
  printAllRegisters(cpu);
  cpuStart(cpu);
  printAllRegisters(cpu);
  cpuStart(cpu);
  printAllRegisters(cpu);
  printf("Rdst=");
  printArr(cpu->Rdst, 4);
  printf("RA=");
  printArr(cpu->RA, 4);
  printf("decoded opcode=");
  printWord(cpu->cu->values);
  cpuStart(cpu);
  printAllRegisters(cpu);

  end = clock();
  printf("total time=%f\n", ((double)(end - start)) / CLOCKS_PER_SEC);

  return 0;
}
