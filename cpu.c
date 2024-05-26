#include "cpu.h"
#include "globals.h"
#include "adder.h"
#include "control_unit.h"
#include "register_reader.h"
#include "mux_alu_input2.h"
#include "alu.h"
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
    index |= (register_bits[i] << i) & 0b1;
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
    printf(stderr, "Invalid register index provided.");
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

  // cpu->PC_incrementor = createCircuit(16, 1); // TODO: consider how to implement this circuit
  // cpu->PC_incrementor->values = (char *)malloc(32 * sizeof(char));
  // if (cpu->PC_incrementor->values == NULL)
  // {
  //   perror("Malloc failed. Terminating.");
  //   exit(1);
  // }
  // cpu->PC_incrementor->values[0] = 0; // constant 2
  // cpu->PC_incrementor->values[1] = 1;
  // for (int i = 2; i < 16; i++)
  // {
  //   cpu->PC_incrementor->values[i] = 0;
  // }
  // for (int i = 0; i < 16; i++) // get PC into 1 continuous array
  // {
  //   cpu->PC_incrementor->values[16+i] = 0;
  // }
  // cpu->PC_incrementor->subCircuits[0] = adder();

  cpu->cu = control_unit(
      cpu->instruction,
      cpu->z_flag,
      cpu->Rdst,
      cpu->RA,
      cpu->RB,
      cpu->mem_inter_addr,
      cpu->Imm,
      cpu->zeroRA,
      cpu->LSL,
      cpu->useImm,
      cpu->alu_add,
      cpu->alu_sub,
      cpu->alu_mul,
      cpu->alu_nand,
      cpu->mem_inter_load,
      cpu->mem_inter_store,
      cpu->setZ,
      cpu->setLink,
      cpu->store_output_to_Rdst,
      cpu->halt);

  cpu->register_reader = register_reader(
      cpu->registers,
      cpu->RA,
      cpu->RB,
      cpu->RA_val,
      cpu->RB_val);

  // TODO: implement and replace this with the real lsl_module
  // currently just sends RA_val to alu_input1 without shifting
  cpu->lsl_module = createCircuit(16, 0);
  for (int i = 0; i < 16; i++)
  {
    setGate(cpu->lsl_module, i, AND, cpu->RA_val + i, cpu->RA_val + i, cpu->alu_input1 + i);
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

  cpu->register_writer = register_writer(
      cpu->registers,
      cpu->alu_output,
      cpu->Rdst,
      cpu->store_output_to_Rdst);

  cpu->z_flag_writer = z_flag_writer(
      cpu->z_flag,
      cpu->alu_out_Z,
      cpu->setZ);

  return cpu;
}

void cpuStart(Cpu *cpu)
{
  int max_iterations = 1;
  while (max_iterations--)
  {
    // Get the value of PC
    char PC[16];
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
    simulateCircuit(cpu->lsl_module);

    // MUX RB_val
    simulateCircuit(cpu->mux_alu_input2);

    // ALU compute output
    simulateCircuit(cpu->alu);

    // Memory Interface
    if (cpu->mem_inter_load) // load
    {
      memRead(cpu->memory, cpu->mem_inter_addr, cpu->mem_inter_data_out);
      writeRegister(cpu->registers, convertRegisterIndex(cpu->Rdst), cpu->mem_inter_data_out);
    }
    else if (cpu->mem_inter_store) // store
    {
      memWrite(cpu->memory, cpu->mem_inter_addr, cpu->mem_inter_data_in);
    }

    // Stack operations // TODO: Currently magic, emulate it
    if (cpu->cu->values + 8) // push
    {
      // char SP_data[16];
      // readRegister(cpu->registers, 13, SP_data);
      // int SP_addr = convertBitArrayToInt(SP_data);
      // memWrite(cpu->memory, SP_data, );
      printf("WIP: PUSH STACK\n");
    }
    else if (cpu->cu->values + 9) // pop
    {
      printf("WIP: POP STACK\n");
    }

    // Write to Rdst
    simulateCircuit(cpu->register_writer); // BUG: needs to consider storing data_out of the memory interface if it was a load instruction

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
