#include "cpu.h"
#include "globals.h"
#include "adder.h"
#include "control_unit.h"
#include "register_reader.h"
#include "mux_alu_input2.h"
#include "alu.h"
#include "linker.h"
#include "register_writer.h"
#include "z_flag_writer.h"
#include "memory.h"

#include <stdlib.h>
#include <stdio.h>

/**
 * Converts a 4-bit register index to a real int
 * @param register_bits (char[4] *)
 */
int convertRegisterIndex(char *register_bits)
{
  int index = 0;
  for (int i = 0; i < 4; i++)
  {
    index |= register_bits[i] << i;
  }
  return index;
}

/**
 * Write data to a register
 * @param registers (char[256] *)
 * @param index (int)
 * @param data (char[16] *)
 */
void writeRegister(char *registers, int index, char *data)
{
  if (index < 0 || index >= 16)
  {
    fprintf(stderr, "Invalid register index provided.");
    return;
  }
  for (int i = 0; i < 16; i++)
  {
    registers[index + i * 16] = data[i];
  }
}

/**
 * Reads the data in a register
 * @param registers (char[256] *)
 * @param index (int)
 * @param data_out (char[16] *)
 */
void readRegister(char *registers, int index, char *data_out)
{
  for (int i = 0; i < 16; i++)
  {
    data_out[i] = registers[index + i * 16];
  }
}

Cpu *createCpu()
{
  Cpu *cpu = (Cpu *)malloc(sizeof(Cpu));
  if (cpu == NULL)
  {
    perror("Malloc failed. Terminating.");
    exit(1);
  }

  cpu->PC_incrementor = createCircuit(16, 2);
  cpu->PC_incrementor->values = (char *)malloc(32 * sizeof(char));
  if (cpu->PC_incrementor->values == NULL)
  {
    perror("Malloc failed. Terminating.");
    exit(1);
  }
  for (int i = 0; i < 16; i++)
  {
    setGate(cpu->PC_incrementor, i, AND, cpu->registers + 15 + i * 16, cpu->registers + 15 + i * 16, cpu->PC_incrementor->values + i);
  }
  cpu->PC_incrementor->subCircuits[0] = adder(cpu->PC_incrementor->values, const16_0, cpu->PC_incrementor->values + 16, &trash);
  cpu->PC_incrementor->subCircuits[1] = createCircuit(16, 0);
  for (int i = 0; i < 16; i++)
  {
    setGate(cpu->PC_incrementor->subCircuits[1], i, AND, cpu->PC_incrementor->values + 16 + i, cpu->PC_incrementor->values + 16 + i, cpu->registers + 15 + i * 16);
  }

  cpu->cu = control_unit(
      cpu->instruction,
      cpu->z_flag,
      cpu->halt,
      cpu->alu_add,
      cpu->alu_sub,
      cpu->alu_mul,
      cpu->alu_nand,
      cpu->mem_inter_load,
      cpu->mem_inter_store,
      cpu->zeroRA,
      cpu->LSL,
      cpu->useImm,
      cpu->setZ,
      cpu->setLink,
      cpu->store_output_to_Rdst,
      cpu->Rdst,
      cpu->RA,
      cpu->RB,
      cpu->Imm);

  cpu->register_reader = register_reader(
      cpu->registers,
      cpu->RA,
      cpu->RB,
      cpu->RA_val,
      cpu->RB_val);

  // TODO: implement and replace this with the real lsl_module
  // currently just sends RA_val to alu_input1 without shifting
  // cpu->lsl_module = createCircuit(16, 0);
  // for (int i = 0; i < 16; i++)
  // {
  //   setGate(cpu->lsl_module, i, AND, cpu->RA_val + i, cpu->RA_val + i, cpu->alu_input1 + i);
  // }

  cpu->mux_alu_input1 = createCircuit(17, 0);
  cpu->mux_alu_input1->values = (char *)malloc(1 * sizeof(char));
  if (cpu->mux_alu_input1->values == NULL)
  {
    perror("Malloc failed. Terminating.");
    exit(1);
  }
  setGate(cpu->mux_alu_input1, 0, NOT, cpu->zeroRA, cpu->zeroRA, cpu->mux_alu_input1->values);
  for (int i = 0; i < 16; i++)
  {
    setGate(cpu->mux_alu_input1, 1 + i, AND, cpu->mux_alu_input1->values, cpu->RA_val + i, cpu->alu_input1 + i);
  }

  cpu->mux_alu_input2 = mux_alu_input2(
      cpu->RB_val,
      cpu->Imm,
      cpu->useImm,
      cpu->alu_input2);

  cpu->alu = alu(
      cpu->alu_input1,
      cpu->alu_input2,
      cpu->alu_output,
      cpu->alu_out_Z,
      cpu->alu_add,
      cpu->alu_sub,
      cpu->alu_mul,
      cpu->alu_nand);

  cpu->linker = linker(
      cpu->registers,
      cpu->setLink);

  cpu->register_writer = register_writer(
      cpu->registers,
      cpu->alu_output,
      cpu->mem_inter_data_out,
      cpu->Rdst,
      cpu->store_output_to_Rdst,
      cpu->mem_inter_load);

  cpu->z_flag_writer = z_flag_writer(
      cpu->z_flag,
      cpu->alu_out_Z,
      cpu->setZ);

  return cpu;
}

void cpuStart(Cpu *cpu)
{
  int max_iterations = 25;
  while (max_iterations--)
  {
    // Get the value of PC
    char PC[16];
    printf("current address=");
    for (int i = 15; i >= 0; i--)
    {
      PC[i] = cpu->registers[15 + i * 16];
      printf("%d", PC[i]);
    }
    printf("\n");

    // Fetch instruction
    memRead(
        cpu->memory,
        PC,
        cpu->instruction);
    printf("instruction=");
    for (int i = 15; i >= 0; i--)
    {
      printf("%d", cpu->instruction[i]);
    }
    printf("\n");

    // Increment PC by 2 // TODO: use real circuit once it is designed
    int fake_PC = 0;
    for (int i = 0; i < 16; i++)
    {
      fake_PC |= cpu->registers[15 + i * 16] << i;
    }
    fake_PC += 2;
    for (int i = 0; i < 16; i++)
    {
      cpu->registers[15 + i * 16] = (fake_PC >> i) & 0b1;
    }
    // simulateCircuit(cpu->PC_incrementor);

    // Decode instruction
    simulateCircuit(cpu->cu);

    // Check halt
    if (cpu->halt[0])
    {
      printf("Halt\n");
      return;
    }

    // Read RA and RB
    simulateCircuit(cpu->register_reader);

    // LSL RA_val
    // simulateCircuit(cpu->lsl_module);
    int RA_val_real = 0;
    for (int i = 0; i < 16; i++)
    {
      RA_val_real |= cpu->RA_val[i] << i;
    }
    int LSL_val_real = 0;
    for (int i = 0; i < 16; i++)
    {
      if (cpu->LSL[i] == 1)
      {
        LSL_val_real = i;
        break;
      }
    }
    RA_val_real = RA_val_real << LSL_val_real;
    for (int i = 0; i < 16; i++)
    {
      cpu->RA_val[i] = (RA_val_real >> i) & 1;
    }

    // MUX RA_val
    simulateCircuit(cpu->mux_alu_input1);

    // MUX RB_val
    simulateCircuit(cpu->mux_alu_input2);

    // ALU compute output
    simulateCircuit(cpu->alu);

    // Memory Interface
    if (cpu->mem_inter_load[0]) // load
    {
      printf("load\n");
      memRead(cpu->memory, cpu->RB_val, cpu->mem_inter_data_out);
      // writeRegister(cpu->registers, convertRegisterIndex(cpu->Rdst), cpu->mem_inter_data_out);
    }
    else if (cpu->mem_inter_store[0]) // store
    {
      printf("store\n");
      memWrite(cpu->memory, cpu->RB_val, cpu->RA_val);
    }

    // Stack operations
    if (cpu->cu->values[8]) // push
    {
      // Extract SP value
      char SP_addr_fake[16];
      readRegister(cpu->registers, 13, SP_addr_fake);
      int SP_addr_int = convertBitArrayToInt(SP_addr_fake) - 2;
      char SP_addr_real[16];
      convertIntToBitArray(SP_addr_int, SP_addr_real);

      // Extract src register
      int Rsrc = 0;
      for (int i = 0; i < 4; i++)
      {
        Rsrc |= (cpu->instruction[4 + i] & 1) << i;
      }

      // Extract src register value
      char Rsrc_value[16];
      readRegister(cpu->registers, Rsrc, Rsrc_value);

      // Write to memory
      memWrite(cpu->memory, SP_addr_real, Rsrc_value);
    }
    else if (cpu->cu->values[9]) // pop
    {
      // Extract SP value
      char SP_addr[16];
      readRegister(cpu->registers, 13, SP_addr);

      // Read from memory
      char data_out[16];
      memRead(cpu->memory, SP_addr, data_out);

      // Extract Rdst
      int Rdst = 0;
      for (int i = 0; i < 4; i++)
      {
        Rdst |= (cpu->instruction[8 + i] & 1) << i;
      }

      // Write to Rdst
      writeRegister(cpu->registers, Rdst, data_out);
    }

    // Write to Rdst
    simulateCircuit(cpu->register_writer);

    // Write Z flag
    simulateCircuit(cpu->z_flag_writer);
  }
}

void printRegister(Cpu *cpu, int index)
{
  if (0 <= index && index < 13) // R0-R12
    printf("R%d =\t", index);
  else if (index == 13) // SP
    printf("SP =\t");
  else if (index == 14) // LR
    printf("LR =\t");
  else if (index == 15) // PC
    printf("PC =\t");
  else
  {
    printf("Invalid register index provided: %d\n", index);
    return;
  }
  for (int i = 15; i >= 0; i--)
  {
    printf("%d", cpu->registers[index + i * 16]);
  }
  printf("\n");
}

void printAllRegisters(Cpu *cpu)
{
  for (int i = 0; i < 16; i++)
  {
    printRegister(cpu, i);
  }
}
