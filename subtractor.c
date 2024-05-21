#include "subtractor.h"
#include "globals.h"
#include "twos_complement.h"
#include "adder.h"

#include <stdlib.h>
#include <stdio.h>

/**
 * Creates a 16-bit subtraction circuit
 * @param input1 (char[16] *)
 * @param input2 (char[16] *)
 * @param output (char[16] *)
 * @param overflow (char *)
 */
Circuit *subtractor(char *input1, char *input2, char *output, char *overflow)
{
  Circuit *c = createCircuit(3, 2);

  // Subtractor circuit
  c->values = (char *)malloc(18 * sizeof(char));
  if (c->values == NULL)
  {
    perror("Malloc failed. Terminating.");
    exit(1);
  }

  c->subCircuits[0] = twos_complement(input2, c->values);
  c->subCircuits[1] = adder(input1, c->values, output, &trash);

  // Overflow detection circuit
  setGate(c, 0, XOR, input1 + 15, input2 + 15, c->values + 16); // can this operation overflow?
  setGate(c, 1, AND, input2 + 15, output + 15, c->values + 17); // has it overflown?
  setGate(c, 2, AND, c->values + 16, c->values + 17, overflow); // combine to get overflow

  return c;
}
