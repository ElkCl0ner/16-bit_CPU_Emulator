#include "control_unit.h"
#include "globals.h"
#include "decoder.h"

#include <stdlib.h>
#include <stdio.h>

/**
 * Creates a control unit
 * @param instruction (char[16] *) (input)
 * @param Zin (char *) (input) Z flag
 * @param Rdst (char[4] *)
 * @param RA (char[4] *)
 * @param RB (char[4] *)
 * @param Addr (char[4] *)
 * @param Imm (char[16] *)
 * @param zeroRA (char *) signal to set input1 of the ALU to 0
 * @param LSL (char[16] *) (decoded control bits) specifies by how many bits to left shift the value of RegA entering the ALU
 * @param useImm (char *) signal to specify whether input2 of the ALU should be RB or Imm
 * @param add (char *) ALU control signal
 * @param sub (char *) ALU control signal
 * @param mul (char *) ALU control signal
 * @param nand (char *) ALU control signal
 * @param memLoad (char *) signal that the current operation loads a word from main memory
 * @param memStore (char *) signal that the current operation stores a word to main memory
 * @param setZ (char *) signal to set the Z flag
 * @param link (char *) signal to set LR to PC (before storing the output of the ALU into Rdst)
 * @param storeOutputToRdst (char *) signal to specify whether the output of the ALU should be stored in Rdst
 * @param halt (char *) signal to halt execution
 */
Circuit *control_unit(
    char *instruction,
    char *Zin,
    char *Rdst,
    char *RA,
    char *RB,
    char *Addr,
    char *Imm,
    char *zeroRA,
    char *LSL,
    char *useImm,
    char *add,
    char *sub,
    char *mul,
    char *nand,
    char *memLoad,
    char *memStore,
    char *setZ,
    char *link,
    char *storeOutputToRdst,
    char *halt)
{
  Circuit *c = createCircuit(203, 2);

  c->values = (char *)malloc(30 * sizeof(char));
  if (c->values == NULL)
  {
    perror("Malloc failed. Terminating.");
    exit(1);
  }

  // Decode instruction opcode, subCircuit 0, no gates, values [0,15]
  c->subCircuits[0] = decoder(instruction + 12, c->values);

  // Rdst, gates [0,3] U [58,77], values [18,23]
  setGate(c, 0, OR, c->values + 8, c->values + 9, c->values + 18); // Rdst = SP
  setGate(c, 1, OR, c->values + 10, c->values + 11, c->values + 19);
  setGate(c, 2, OR, c->values + 19, c->values + 12, c->values + 19);  // Rdst = PC
  setGate(c, 3, NOR, c->values + 18, c->values + 19, c->values + 20); // Rdst = Rdst
  for (int i = 0; i < 4; i++)
  {
    setGate(c, 58 + i * 5, AND, const4_SP + i, c->values + 18, c->values + 21);       // Rdst = SP
    setGate(c, 59 + i * 5, AND, const4_PC + i, c->values + 19, c->values + 22);       // Rdst = PC
    setGate(c, 60 + i * 5, AND, instruction + 8 + i, c->values + 20, c->values + 23); // Rdst = Rdst
    setGate(c, 61 + i * 5, OR, c->values + 21, c->values + 22, Rdst + i);
    setGate(c, 62 + i * 5, OR, Rdst + i, c->values + 23, Rdst + i);
  }

  // RA, gates [4,7] U [183,202], values [26,29]
  setGate(c, 4, OR, c->values + 8, c->values + 9, c->values + 26); // RA = SP
  setGate(c, 5, OR, c->values + 10, c->values + 11, c->values + 27);
  setGate(c, 6, OR, c->values + 27, c->values + 12, c->values + 27);  // RA = PC
  setGate(c, 7, NOR, c->values + 26, c->values + 27, c->values + 28); // RA = RA (not SP nor PC)

  for (int i = 0; i < 4; i++)
  {
    setGate(c, 183 + i * 5, AND, c->values + 26, const4_SP + i, RA + i);               // SP
    setGate(c, 184 + i * 5, AND, c->values + 27, const4_PC + i, c->values + 29);       // PC
    setGate(c, 185 + i * 5, OR, RA + i, c->values + 29, RA + i);                       // SP OR PC
    setGate(c, 186 + i * 5, AND, c->values + 28, instruction + 4 + i, c->values + 29); // RA
    setGate(c, 187 + i * 5, OR, RA + i, c->values + 29, RA + i);                       // SP OR PC OR RA
  }

  // RB, gates [8,11]
  for (int i = 0; i < 4; i++)
  {
    setGate(c, 8 + i, AND, instruction + i, instruction + i, RB + i);
  }

  // Addr, gates [12,15]
  for (int i = 0; i < 4; i++)
  {
    setGate(c, 12 + i, AND, instruction + i, instruction + i, Addr + i);
  }

  // Imm, gates [16,34] U {36} U [78,163]\{151}, values [16,17] U {24}
  setGate(c, 16, OR, c->values + 5, c->values + 12, c->values + 16);
  setGate(c, 17, OR, c->values + 16, c->values + 13, c->values + 16); // Imm = 0
  setGate(c, 18, OR, c->values + 10, c->values + 11, c->values + 17); // Imm = Imm12

  setGate(c, 19, AND, c->values + 8, &one, Imm);              // -1
  setGate(c, 20, AND, c->values + 16, &zero, c->values + 24); // 0
  setGate(c, 21, OR, Imm, c->values + 24, Imm);
  setGate(c, 22, AND, c->values + 9, &one, c->values + 24); // 1
  setGate(c, 23, OR, Imm, c->values + 24, Imm);
  setGate(c, 24, AND, c->values + 15, instruction, c->values + 24); // Imm4
  setGate(c, 25, OR, Imm, c->values + 24, Imm);
  setGate(c, 26, AND, c->values + 4, instruction, c->values + 24); // Imm8
  setGate(c, 27, OR, Imm, c->values + 24, Imm);
  setGate(c, 28, AND, c->values + 17, instruction, c->values + 24); // Imm12
  setGate(c, 29, OR, Imm, c->values + 24, Imm);

  setGate(c, 30, OR, c->values + 16, c->values + 9, c->values + 16); // Imm = 0 or 1 (fill all subsequent bits with 0)
  setGate(c, 31, AND, c->values + 8, &one, Imm + 1);                 // -1
  setGate(c, 32, AND, c->values + 16, &zero, c->values + 24);        // 0 or 1
  setGate(c, 33, OR, Imm + 1, c->values + 24, Imm + 1);
  setGate(c, 34, AND, c->values + 15, instruction + 1, c->values + 24); // Imm4
  setGate(c, 78, OR, Imm + 1, c->values + 24, Imm + 1);
  setGate(c, 79, AND, c->values + 4, instruction + 1, c->values + 24); // Imm8
  setGate(c, 80, OR, Imm + 1, c->values + 24, Imm + 1);
  setGate(c, 81, AND, c->values + 17, instruction + 1, c->values + 24); // Imm12
  setGate(c, 82, OR, Imm + 1, c->values + 24, Imm + 1);
  for (int i = 2; i < 4; i++)
  {
    setGate(c, 83 + (i - 2) * 9, AND, c->values + 8, &one, Imm + i);          // -1
    setGate(c, 84 + (i - 2) * 9, AND, c->values + 16, &zero, c->values + 24); // 0 or 1
    setGate(c, 85 + (i - 2) * 9, OR, Imm + i, c->values + 24, Imm + i);
    setGate(c, 86 + (i - 2) * 9, AND, c->values + 15, instruction + i, c->values + 24); // Imm4
    setGate(c, 87 + (i - 2) * 9, OR, Imm + i, c->values + 24, Imm + i);
    setGate(c, 88 + (i - 2) * 9, AND, c->values + 4, instruction + i, c->values + 24); // Imm8
    setGate(c, 89 + (i - 2) * 9, OR, Imm + i, c->values + 24, Imm + i);
    setGate(c, 90 + (i - 2) * 9, AND, c->values + 17, instruction + i, c->values + 24); // Imm12
    setGate(c, 91 + (i - 2) * 9, OR, Imm + i, c->values + 24, Imm + i);
  }

  setGate(c, 101, OR, c->values + 16, c->values + 15, c->values + 16); // Imm = 0 or 1 or Imm4
  for (int i = 0; i < 4; i++)
  {
    setGate(c, 102 + i * 7, AND, c->values + 8, &one, Imm + 4 + i);      // -1
    setGate(c, 103 + i * 7, AND, c->values + 16, &zero, c->values + 24); // 0 or 1 or Imm4
    setGate(c, 104 + i * 7, OR, Imm + 4 + i, c->values + 24, Imm + 4 + i);
    setGate(c, 105 + i * 7, AND, c->values + 4, instruction + 4 + i, c->values + 24); // Imm8
    setGate(c, 106 + i * 7, OR, Imm + 4 + i, c->values + 24, Imm + 4 + i);
    setGate(c, 107 + i * 7, AND, c->values + 17, instruction + 4 + i, c->values + 24); // Imm12
    setGate(c, 108 + i * 7, OR, Imm + 4 + i, c->values + 24, Imm + 4 + i);
  }

  setGate(c, 130, OR, c->values + 16, c->values + 4, c->values + 16); // Imm = 0 or 1 or Imm4 or Imm8
  for (int i = 0; i < 4; i++)
  {
    setGate(c, 131 + i * 5, AND, c->values + 8, &one, Imm + 8 + i);      // -1
    setGate(c, 132 + i * 5, AND, c->values + 16, &zero, c->values + 24); // 0 or 1 or Imm4 or Imm8
    setGate(c, 133 + i * 5, OR, Imm + 8 + i, c->values + 24, Imm + 8 + i);
    setGate(c, 134 + i * 5, AND, c->values + 17, instruction + 8 + i, c->values + 24); // Imm12
    setGate(c, 135 + i * 5, OR, Imm + 8 + i, c->values + 24, Imm + 8 + i);
  }

  setGate(c, 36, OR, c->values + 16, c->values + 17, c->values + 16); // Imm = 0 or 1 or Imm4 or Imm8 or Imm12
  for (int i = 0; i < 4; i++)
  {
    setGate(c, 152 + i * 3, AND, c->values + 8, &one, Imm + 12 + i);     // -1
    setGate(c, 153 + i * 3, AND, c->values + 16, &zero, c->values + 24); // 0 or 1 or Imm4 or Imm8 or Imm12
    setGate(c, 154 + i * 3, OR, Imm + 12 + i, c->values + 24, Imm + 12 + i);
  }

  // zeroRA, gate 35
  setGate(c, 35, AND, c->values + 4, c->values + 4, zeroRA);

  // LSL, gates [164,179]
  c->subCircuits[1] = decoder(instruction, LSL);
  for (int i = 0; i < 16; i++)
  {
    setGate(c, 164 + i, AND, LSL + i, c->values + 13, LSL + i);
  }

  // useImm, gate 151
  setGate(c, 151, OR, c->values + 16, c->values + 8, useImm); // uses values computed in the Imm extraction step

  // add, gates [37,43]
  setGate(c, 37, OR, c->values + 1, c->values + 4, add);
  setGate(c, 38, OR, add, c->values + 5, add);
  setGate(c, 39, OR, add, c->values + 9, add);
  setGate(c, 40, OR, add, c->values + 10, add);
  setGate(c, 41, OR, add, c->values + 11, add);
  setGate(c, 42, OR, add, c->values + 12, add);
  setGate(c, 43, OR, add, c->values + 13, add);

  // sub, gates [44.45]
  setGate(c, 44, OR, c->values + 2, c->values + 8, sub);
  setGate(c, 45, OR, sub, c->values + 15, sub);

  // mul, gate 46
  setGate(c, 46, AND, c->values + 3, c->values + 3, mul);

  // nand, gate 47
  setGate(c, 47, AND, c->values + 14, c->values + 14, nand);

  // memLoad, gate 48
  setGate(c, 48, AND, c->values + 6, c->values + 6, memLoad);

  // memStore, gate 49
  setGate(c, 49, AND, c->values + 7, c->values + 7, memStore);

  // setZ, gates [50,54]
  setGate(c, 50, OR, c->values + 1, c->values + 2, setZ);
  setGate(c, 51, OR, setZ, c->values + 3, setZ);
  setGate(c, 52, OR, setZ, c->values + 13, setZ);
  setGate(c, 53, OR, setZ, c->values + 14, setZ);
  setGate(c, 54, OR, setZ, c->values + 15, setZ);

  // link, gate 55
  setGate(c, 55, OR, c->values + 12, c->values + 13, link);

  // storeOutputToRdst, gates {56} U [180,182], value 25
  setGate(c, 56, NOR, c->values + 6, c->values + 7, storeOutputToRdst);
  setGate(c, 180, NOT, c->values + 11, c->values + 11, c->values + 25);
  setGate(c, 181, OR, c->values + 25, Zin, c->values + 25);
  setGate(c, 182, AND, storeOutputToRdst, c->values + 25, storeOutputToRdst);

  // halt, gate 57
  setGate(c, 57, AND, c->values, c->values, halt);

  return c;
}
