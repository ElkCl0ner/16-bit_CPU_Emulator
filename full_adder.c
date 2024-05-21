#include "full_adder.h"

#include <stdlib.h>

/**
 * Creates a 1-bit full adder
 * @param input1 (int *) first bit
 * @param input2 (int *) second bit
 * @param C_in (int *) carry in
 * @param sum (int *) sum
 * @param C_out (int *) carry out
 */
Circuit *full_adder(int *input1, int *input2, int *C_in, int *sum, int *C_out)
{
  Circuit *c = createCircuit(5, 0);

  c->values = (int *)malloc(3 * sizeof(int));

  setGate(c, 0, XOR, input1, input2, &c->values[0]);
  setGate(c, 1, AND, input1, input2, &c->values[2]);
  setGate(c, 2, AND, &c->values[0], C_in, &c->values[1]);
  setGate(c, 3, XOR, &c->values[0], C_in, sum);
  setGate(c, 4, OR, &c->values[1], &c->values[2], C_out);

  return c;
}
