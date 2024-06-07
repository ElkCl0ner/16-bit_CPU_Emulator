#include "new_control_unit.h"
#include "globals.h"
#include "decoder.h"

#include <stdlib.h>
#include <stdio.h>

/**
 * Creates a control unit
 * @param instruction (char[16] *) (input)
 * @param Zin (char *) (input) current Z Flag
 * @param halt (char *) signal to halt execution
 * @param add (char *) ALU control signal
 * @param sub (char *) ALU control signal
 * @param mul (char *) ALU control signal
 * @param nand (char *) ALU control signal
 * @param memLoad (char *) Memory Interface & Register Writer signal
 * @param memStore (char *) Memory Interface signal
 * @param zeroRA (char *) signal to set input1 to 0
 * @param LSL (char[16] *) (one-hot encoded) LSL Module control signals
 * @param useImm (char *) signal to use Imm as input2
 * @param setZ (char *) Z Flag Writer control signal
 * @param link (char *) Linker Module control signal
 * @param storeALUOutputToRdst (char *) Register Writer control signal
 * @param Rdst (char[4] *) distination register
 * @param RA (char[4] *) ALU input1 | Memory Interface data in
 * @param RB (char[4] *) ALU input2 | Memory Interface address
 * @param Imm (char[16] *) ALU input2
 */
Circuit *new_control_unit(
    char *instruction,
    char *Zin,
    char *halt,
    char *add,
    char *sub,
    char *mul,
    char *nand,
    char *memLoad,
    char *memStore,
    char *zeroRA,
    char *LSL,
    char *useImm,
    char *setZ,
    char *link,
    char *storeALUOutputToRdst,
    char *Rdst,
    char *RA,
    char *RB,
    char *Imm)
{
  Circuit *c = createCircuit(137, 2);

  c->values = (char *)malloc(29 * sizeof(char));
  if (c->values == NULL)
  {
    perror("Malloc failed. Terminating.");
    exit(1);
  }

  // Decode instruction opcode, subCircuit {0}, no gates, values [0,15]
  c->subCircuits[0] = decoder(instruction + 12, c->values);

  // halt, gate {0}, no values
  setGate(c, 0, AND, c->values, c->values, halt);

  // add, gates [1,7], no values
  setGate(c, 1, OR, c->values + 1, c->values + 4, add);
  setGate(c, 2, OR, add, c->values + 5, add);
  setGate(c, 3, OR, add, c->values + 9, add);
  setGate(c, 4, OR, add, c->values + 10, add);
  setGate(c, 5, OR, add, c->values + 11, add);
  setGate(c, 6, OR, add, c->values + 12, add);
  setGate(c, 7, OR, add, c->values + 13, add);

  // sub, gates [8,9], no values
  setGate(c, 8, OR, c->values + 2, c->values + 8, sub);
  setGate(c, 9, OR, sub, c->values + 15, sub);

  // mul, gate {10}, no values
  setGate(c, 10, AND, c->values + 3, c->values + 3, mul);

  // nand, gate {11}, no values
  setGate(c, 11, AND, c->values + 4, c->values + 4, nand);

  // memLoad, gate {12}, no values
  setGate(c, 12, AND, c->values + 6, c->values + 6, memLoad);

  // memStore, gate {13}, no values
  setGate(c, 13, AND, c->values + 7, c->values + 7, memLoad);

  // zeroRA, gate {14}, no values
  setGate(c, 14, AND, c->values + 4, c->values + 4, zeroRA);

  // LSL, subCircuit {1}, gates [15,30], no values
  c->subCircuits[1] = decoder(instruction, LSL);
  for (int i = 0; i < 16; i++)
  {
    setGate(c, 15 + i, AND, c->values + 13, LSL + i, LSL + i);
  }

  // useImm, gates [31,32], no values
  // (1 AND 2) NOR c->val+14
  setGate(c, 31, AND, instruction + 15, instruction + 14, useImm);
  setGate(c, 32, NOR, useImm, c->values + 14, useImm);

  // setZ, gates [33,35], value {16}
  // (1 XNOR 2) AND (3 OR 4)
  setGate(c, 33, XNOR, instruction + 15, instruction + 14, setZ);
  setGate(c, 34, OR, instruction + 13, instruction + 14, c->values + 16);
  setGate(c, 35, AND, setZ, c->values + 16, setZ);

  // link, gate {36,37}, no values
  setGate(c, 36, AND, c->values + 11, Zin, link);
  setGate(c, 37, OR, link, c->values + 10, link);

  // storeALUOutputToRdst, gate [38,41], no values
  setGate(c, 38, NOT, Zin, Zin, storeALUOutputToRdst);
  setGate(c, 39, AND, c->values + 11, storeALUOutputToRdst, storeALUOutputToRdst);
  setGate(c, 40, OR, storeALUOutputToRdst, c->values + 6, storeALUOutputToRdst);
  setGate(c, 41, NOR, storeALUOutputToRdst, c->values + 7, storeALUOutputToRdst);

  // Rdst, gates [42,65], values [17,20]
  setGate(c, 42, OR, c->values + 8, c->values + 9, c->values + 17); // Rdst = SP

  setGate(c, 43, OR, c->values + 10, c->values + 11, c->values + 18);
  setGate(c, 44, OR, c->values + 18, c->values + 12, c->values + 18); // Rdst = PC

  setGate(c, 45, NOR, c->values + 17, c->values + 18, c->values + 19); // Rdst = Rdst

  for (int i = 0; i < 4; i++)
  {
    setGate(c, 46 + i * 5, AND, c->values + 17, const4_SP + i, Rdst + i);             // SP
    setGate(c, 47 + i * 5, AND, c->values + 18, const4_PC + i, c->values + 20);       // PC
    setGate(c, 48 + i * 5, OR, Rdst + i, c->values + 20, Rdst + i);                   // SP OR PC
    setGate(c, 49 + i * 5, AND, c->values + 19, instruction + 8 + i, c->values + 20); // Rdst
    setGate(c, 50 + i * 5, OR, Rdst + i, c->values + 20, Rdst + i);                   // SP OR PC OR Rdst
  }

  // RA, gates [66,88], values [21,24]
  setGate(c, 66, OR, c->values + 8, c->values + 9, c->values + 21);    // SP
  setGate(c, 67, OR, c->values + 10, c->values + 11, c->values + 22);  // PC
  setGate(c, 68, NOR, c->values + 21, c->values + 22, c->values + 23); // RA

  for (int i = 0; i < 4; i++)
  {
    setGate(c, 69 + i * 5, AND, c->values + 21, const4_SP + i, RA + i);           // SP
    setGate(c, 70 + i * 5, AND, c->values + 22, const4_PC, c->values + 24);       // PC
    setGate(c, 71 + i * 5, OR, RA + i, c->values + 24, RA + i);                   // SP OR PC
    setGate(c, 72 + i * 5, AND, c->values + 23, instruction + 4, c->values + 24); // RA
    setGate(c, 73 + i * 5, OR, RA + i, c->values + 24, RA + i);                   // SP OR PC OR RA
  }

  // RB, gates [89,92], no values
  for (int i = 0; i < 4; i++)
  {
    setGate(c, 89 + i, AND, instruction + i, instruction + i, RB + i);
  }

  // Imm, gates [93,136], vaues [25,28]
  setGate(c, 93, OR, c->values + 5, c->values + 12, c->values + 25);
  setGate(c, 94, OR, c->values + 25, c->values + 13, c->values + 25); // const0

  setGate(c, 95, OR, c->values + 8, c->values + 9, c->values + 26); // const1

  // c->values+15 = imm4

  // c->values+4 = imm8

  setGate(c, 96, OR, c->values + 10, c->values + 11, c->values + 27); // imm12

  for (int i = 0; i < 3; i++)
  {
    setGate(c, 97 + i, AND, &zero, &zero, Imm + 13 + i);
  }
  setGate(c, 100, AND, c->values + 27, instruction + 11, Imm + 12);

  for (int i = 0; i < 4; i++)
  {
    setGate(c, 101 + i, AND, c->values + 27, instruction + 7 + i, Imm + 8 + i);
  }

  for (int i = 0; i < 4; i++)
  {
    setGate(c, 105 + i * 3, AND, c->values + 27, instruction + 3 + i, Imm + 4 + i);
    setGate(c, 106 + i * 3, AND, c->values + 4, instruction + 4 + i, c->values + 28);
    setGate(c, 107 + i * 3, OR, Imm + 4 + i, c->values + 28, Imm + 4 + i);
  }

  for (int i = 0; i < 3; i++)
  {
    setGate(c, 117 + i * 5, AND, c->values + 27, instruction + i, Imm + 1 + i);
    setGate(c, 118 + i * 5, AND, c->values + 4, instruction + 1 + i, c->values + 28);
    setGate(c, 119 + i * 5, OR, Imm + 1 + i, c->values + 28, Imm + 1 + i);
    setGate(c, 120 + i * 5, AND, c->values + 15, instruction + 1 + i, c->values + 28);
    setGate(c, 121 + i * 5, OR, Imm + 1 + i, c->values + 28, Imm + 1 + i);
  }
  setGate(c, 132, AND, c->values + 4, instruction, Imm);
  setGate(c, 133, AND, c->values + 15, instruction, c->values + 28);
  setGate(c, 134, OR, Imm, c->values + 28, Imm);
  setGate(c, 135, AND, c->values + 26, &one, c->values + 28);
  setGate(c, 136, OR, Imm, c->values + 28, Imm);

  return c;
}
