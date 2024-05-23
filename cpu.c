#include "cpu.h"
#include "control_unit.h"
#include "register_reader.h"
#include "mux_alu_input2.h"
#include "alu.h"
#include "register_writer.h"
#include "z_flag_writer.h"
#include "memory.h"

#include <stdlib.h>
#include <stdio.h>

Cpu *createCpu()
{
  Cpu *cpu = (Cpu *)malloc(sizeof(Cpu));
  if (cpu == NULL)
  {
    perror("Malloc failed. Terminating.");
    exit(1);
  }

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
  // Get the value of PC
  char PC[16];
  for (int i = 0; i < 16; i++)
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

  // Decode instruction
  simulateCircuit(cpu->cu);
}
