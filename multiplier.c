#include "multiplier.h"
#include "half_adder.h"
#include "full_adder.h"
#include "adder.h"

#include <stdlib.h>
#include <stdio.h>

/**
 * Creates a 16x16-bit multiplier circuit with a 32-bit output
 * @param input1 (int[16] *)
 * @param input2 (int[16] *)
 * @param output (int[32] *)
 */
Circuit *multiplier(int *input1, int *input2, int *output, int *overflow)
{
  Circuit *c = createCircuit(16, 16);

  // AND matrix
  c->subCircuits[0] = createCircuit(256, 0); // 16*16 = 256

  c->values = (int *)malloc(256 * sizeof(int));
  if (c->values == NULL)
  {
    perror("Malloc failed. Terminating.");
    exit(1);
  }

  for (int i2 = 0; i2 < 16; i2++)
  {
    for (int i1 = 0; i1 < 16; i1++)
    {
      setGate(c->subCircuits[0], i2 * 16 + i1, AND, input1 + i1, input2 + i2, c->values + i2 * 16 + i1);
    }
  }
  setGate(c->subCircuits[0], 0, AND, input1, input2, output); // output LSB

  // 15 x 16-bit adders
  c->subCircuits[1] = createCircuit(0, 16);
  c->subCircuits[1]->subCircuits[0] = half_adder(c->values + 1, c->values + 16, output + 1, c->values + 16);
  for (int i = 1; i < 15; i++) // input1 = 1 level up, input2 = same level, C_in = prev c_out, sum -> same level (aka idem input2), C-out -> next sum (aka same level or sum+1)
  {
    c->subCircuits[1]->subCircuits[i] = full_adder(c->values + 1 + i, c->values + 16 + i, c->values + 16 + i - 1, c->values + 16 + i - 1, c->values + 16 + i);
  }
  c->subCircuits[1]->subCircuits[15] = half_adder(c->values + 16 + 14, c->values + 16 + 15, c->values + 16 + 14, c->values + 16 + 15);

  for (int i = 1; i < 15; i++)
  {
    c->subCircuits[i + 1] = createCircuit(0, 16);
    c->subCircuits[i + 1]->subCircuits[0] = half_adder(c->values + i * 16, c->values + (i + 1) * 16, output + 1 + i, c->values + (i + 1) * 16);
    for (int j = 1; j < 16; j++)
    {
      c->subCircuits[i + 1]->subCircuits[j] = full_adder(c->values + i * 16 + j, c->values + (i + 1) * 16 + j, c->values + (i + 1) * 16 + j - 1, c->values + (i + 1) * 16 + j - 1, c->values + (i + 1) * 16 + j);
    }
  }

  // Fake gates to copy 16 MSB result to output
  for (int i = 0; i < 16; i++)
  {
    setGate(c, i, AND, c->values + 240 + i, c->values + 240 + i, output + 16 + i);
  }

  return c;
}
