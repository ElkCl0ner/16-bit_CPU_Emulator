#ifndef CPU_H
#define CPU_H

#include "circuit.h"

typedef struct Cpu
{
  char registers[256];          // ptr to register file
  char memory[65536];           // ptr to main memory (64kB)
  char z_flag[1];               // Z flag
  char instruction[16];         // fetched instruction
  char Rdst[16];                // Rdst
  char RA[4];                   // RA
  char RB[4];                   // RB
  char RA_val[16];              // actual value of register RA
  char RB_val[16];              // actual value of register RB
  char Imm[16];                 // Imm
  char zeroRA[1];               // set ALU input1 to 0
  char LSL[16];                 // LSL ALU input1 before going into the ALU // TODO: implement the LSL circuit
  char useImm[1];               // use Imm instead of RB for ALU input2
  char alu_add[1];              // ALU control: add
  char alu_sub[1];              // ALU control: sub
  char alu_mul[1];              // ALU control: mul
  char alu_nand[1];             // ALU control: nand
  char setZ[1];                 // set Z flag
  char setLink[1];              // set LR to PC (before storing the output of the ALU into Rdst)
  char store_output_to_Rdst[1]; // store ALU output to Rdst
  char halt[1];                 // halt execution
  char mem_inter_load[1];       // memory interface control: load
  char mem_inter_store[1];      // memory interface control: store
  char mem_inter_addr[16];      // memory interface address
  char mem_inter_data_in[16];   // memory interface data in
  char mem_inter_data_out[16];  // memory interface data out
  char alu_input1[16];          // ALU input 1
  char alu_input2[16];          // ALU input 2
  char alu_output[16];          // ALU output
  char alu_out_Z[1];            // ALU out: Z flag
  // Circuit *PC_incrementor; // TODO: consider how to implement this ciruit
  Circuit *cu;
  Circuit *register_reader;
  Circuit *lsl_module; // TODO: imeplement, add and simulate lsl_module
  Circuit *mux_alu_input1;
  Circuit *mux_alu_input2;
  Circuit *alu;
  Circuit *register_writer;
  Circuit *z_flag_writer;
} Cpu;

Cpu *createCpu();
void cpuStart(Cpu *cpu);
void printRegister(Cpu *cpu, int index);
void printAllRegisters(Cpu *cpu);

#endif
