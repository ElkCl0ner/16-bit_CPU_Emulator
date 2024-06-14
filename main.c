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
#include "linker.h"
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
  char cu_instruction[16] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0}; // ADD R2 R0 R1
  // char cu_instruction[16] = {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1}; // BLZ #5
  char cu_Zin = 0;
  char cu_Rdst[4];
  char cu_RA[4];
  char cu_RB[4];
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
      &cu_halt,
      &cu_add,
      &cu_sub,
      &cu_mul,
      &cu_nand,
      &cu_memLoad,
      &cu_memStore,
      &cu_zeroRA,
      cu_LSL,
      &cu_useImm,
      &cu_setZ,
      &cu_link,
      &cu_storeOutputToRdst,
      cu_Rdst,
      cu_RA,
      cu_RB,
      cu_Imm);
  simulateCircuit(cu);
  printf("Rdst=");
  printArr(cu_Rdst, 4);
  printf("RA=");
  printArr(cu_RA, 4);
  printf("RB=");
  printArr(cu_RB, 4);
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
  for (int i = 0; i < 256; i++)
  {
    writer_registers[i] = 0;
  }
  char writer_alu_output[16] = {1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  char writer_mem_inter_data_out[16] = {0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  char writer_Rdst[4] = {0, 0, 0, 0};
  char writer_storeOutput = 0;
  char writer_mem_load = 1;
  Circuit *writer = register_writer(writer_registers, writer_alu_output, writer_mem_inter_data_out, writer_Rdst, &writer_storeOutput, &writer_mem_load);
  simulateCircuit(writer);
  for (int i = 15; i >= 0; i--)
  {
    printf("%d", writer_registers[1 + i * 16]);
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

  /*
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
  */

  /*
  printf("linker\n");
  char registers[256];
  for (int i = 0; i < 256; i++)
  {
    registers[i] = 0;
  }
  for (int i = 0; i < 16; i++)
  {
    registers[14 + i * 16] = 1; // LR
    registers[15 + i * 16] = 1; // PC
  }
  registers[14] = 0;
  char link = 1;
  Circuit *tst_linker = linker(registers, &link);
  simulateCircuit(tst_linker);
  printf("LR=");
  for (int i = 0; i < 16; i++)
  {
    printf("%d", registers[14 + i * 16]);
  }
  printf("\n");
  */

  Cpu *cpu = createCpu();
  for (int i = 0; i < 256; i++)
  {
    cpu->registers[i] = 0;
  }
  cpu->memory[0] = 0b00001101; // MOVI R0 #13
  cpu->memory[1] = 0b01000000;
  cpu->memory[2] = 0b00000111; // MOVI R1 #7
  cpu->memory[3] = 0b01000001;
  cpu->memory[4] = 0b00000001; // ADD R2 R0 R1
  cpu->memory[5] = 0b00010010;
  cpu->memory[6] = 0b00000001; // SUB R3 R0 R1
  cpu->memory[7] = 0b00100011;
  cpu->memory[8] = 0b00000001; // MUL R4 R0 R1
  cpu->memory[9] = 0b00110100;
  cpu->memory[10] = 0b11111110; // MOVI R5 0b11111110
  cpu->memory[11] = 0b01000101;
  cpu->memory[12] = 0b00100101; // STR R2 R5
  cpu->memory[13] = 0b01110000;
  cpu->memory[14] = 0b00000101; // LDR R6 R5
  cpu->memory[15] = 0b01100110;
  cpu->memory[16] = 0b01010000; // PUSH R5
  cpu->memory[17] = 0b10000000;
  cpu->memory[18] = 0b00000000; // POP R7
  cpu->memory[19] = 0b10010111;
  cpu->memory[20] = 0b00000100; // BL #4
  cpu->memory[21] = 0b10100000;

  cpu->memory[30] = 0b00000001; // SUB R8 R0 R1
  cpu->memory[31] = 0b00101000;
  cpu->memory[32] = 0b00001000; // BLZ #8
  cpu->memory[33] = 0b10110000;
  cpu->memory[34] = 0b00001101; // MOVI R8 #13
  cpu->memory[35] = 0b01001000;
  cpu->memory[36] = 0b00001000; // SUB R8 R0 R8
  cpu->memory[37] = 0b00101000;
  cpu->memory[38] = 0b00000101; // BLZ #5
  cpu->memory[39] = 0b10110000;

  cpu->memory[50] = 0b01000000; // MOVI R9 0b01000000
  cpu->memory[51] = 0b01001001;
  cpu->memory[52] = 0b10010000; // BX R9
  cpu->memory[53] = 0b11000000;

  cpu->memory[64] = 0b11111111; // MOVI R10 0b01111111
  cpu->memory[65] = 0b01001010;
  cpu->memory[66] = 0b10100001; // LSL R10 R10 #1
  cpu->memory[67] = 0b11011010;
  cpu->memory[68] = 0b10101010; // MOVI R11 0b10101010
  cpu->memory[69] = 0b01001011;
  cpu->memory[70] = 0b10101011; // NAND R11 R10 R11
  cpu->memory[71] = 0b11101011;
  cpu->memory[72] = 0b00000010; // MOVI R12 #2
  cpu->memory[73] = 0b01001100;
  cpu->memory[74] = 0b11000011; // DEC R12 R12 #3
  cpu->memory[75] = 0b11111100;
  cpu->memory[76] = 0b00000001; // HLT
  cpu->memory[77] = 0b00000000;

  cpuStart(cpu);
  printAllRegisters(cpu);
  printf("cu_add=%d\n", cpu->alu_add[0]);
  printf("cu_sub=%d\n", cpu->alu_sub[0]);
  printf("cu_mul=%d\n", cpu->alu_mul[0]);
  printf("cu_nand=%d\n", cpu->alu_nand[0]);
  printf("cu_zeroRA=%d\n", cpu->zeroRA[0]);
  printf("cu_useImm=%d\n", cpu->useImm[0]);
  printf("cu_storeALUOutput=%d\n", cpu->store_output_to_Rdst[0]);
  printf("cu_mem_store=%d\n", cpu->mem_inter_store[0]);
  printf("cu_mem_load=%d\n", cpu->mem_inter_load[0]);
  printArr(cpu->RA, 4);
  printArr(cpu->RB, 4);
  printArr(cpu->Rdst, 4);
  printArr(cpu->alu_input1, 16);
  printArr(cpu->alu_input2, 16);
  printArr(cpu->alu_output, 16);
  printArr(cpu->mem_inter_data_out, 16);
  printf("%d\n", cpu->memory[65534]);
  printf("%d\n", cpu->memory[65535]);
  printArr(cpu->register_writer->values + 0, 16);

  end = clock();
  printf("total time=%f\n", ((double)(end - start)) / CLOCKS_PER_SEC);

  return 0;
}
