#include "twos_complement.h"
#include "globals.h"
#include "adder.h"

#include <stdlib.h>
#include <stdio.h>

/**
 * Creates a 16-bit two's complement circuit
 * @param input (char[16] *) input
 * @param output (char[16] *) output
 */
Circuit *twos_complement(char *input, char *output)
{
  Circuit *c = createCircuit(0, 2);

  c->values = (char *)malloc(16 * sizeof(char));
  if (c->values == NULL)
  {
    perror("Malloc failed. Terminating.");
    exit(1);
  }

  // One's complement circuit
  c->subCircuits[0] = createCircuit(16, 0);
  for (int i = 0; i < 16; i++)
  {
    setGate(c->subCircuits[0], i, NOT, input + i, input + 1, c->values + i);
  }

  // Add 1
  c->subCircuits[1] = adder(c->values, const_1, output, &trash);

  return c;
}
