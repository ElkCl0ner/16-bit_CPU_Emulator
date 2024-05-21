#include "alu.h"
#include "globals.h"
#include "adder.h"
#include "subtractor.h"
#include "multiplier.h"
#include "bitwise_nand.h"

#include <stdlib.h>
#include <stdio.h>

/**
 * Creates a 16-bit ALU
 * @param input1 (char[16] *)
 * @param input2 (char[16] *)
 * @param output (char[16] *)
 * @param zero (char *) ouput flag
 * @param add (char *) control flag
 * @param sub (char *) control flag
 * @param mul (char *) control flag
 * @param nand (char *) control flag
 * @warning Expects exactly 1 control flag to be set (but does not crash if not)
 */
Circuit *alu(char *input1, char *input2, char *output, char *zero, char *add, char *sub, char *mul, char *nand)
{
  Circuit *c = createCircuit(112, 4); // 4*16 AND + 3*16 OR

  c->values = (char *)malloc(80 * sizeof(char)); // 5*16 = 80 we don't care about the last 16 bits but need the memory
  if (c->values == NULL)
  {
    perror("Malloc failed. Terminating.");
    exit(1);
  }

  c->subCircuits[0] = adder(input1, input2, c->values, &trash);
  c->subCircuits[1] = subtractor(input1, input2, c->values + 16, &trash);
  c->subCircuits[3] = bitwise_nand(input1, input2, c->values + 32);
  c->subCircuits[2] = multiplier(input1, input2, c->values + 48);

  for (int i = 0; i < 16; i++)
  {
    setGate(c, i, AND, c->values + i, add, c->values + i);
  }
  for (int i = 16; i < 32; i++)
  {
    setGate(c, i, AND, c->values + i, sub, c->values + i);
  }
  for (int i = 32; i < 48; i++)
  {
    setGate(c, i, AND, c->values + i, nand, c->values + i);
  }
  for (int i = 48; i < 64; i++) // don't care about 16 MSB of mul result
  {
    setGate(c, i, AND, c->values + i, mul, c->values + i);
  }

  for (int i = 0; i < 16; i++)
  {
    setGate(c, 64 + i, OR, c->values + i, c->values + 16 + i, output + i);
  }
  for (int i = 1; i < 3; i++)
  {
    for (int j = 0; j < 16; j++)
    {
      setGate(c, 64 + i * 16 + j, OR, output + j, c->values + 16 * (i + 1) + j, output + j);
    }
  }

  return c;
}
