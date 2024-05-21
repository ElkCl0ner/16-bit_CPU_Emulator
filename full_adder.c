#include "full_adder.h"

#include <stdlib.h>

/**
 * Creates a 1-bit full adder
 * @param input1 (char *) first bit
 * @param input2 (char *) second bit
 * @param C_in (char *) carry in
 * @param sum (char *) sum
 * @param C_out (char *) carry out
 */
Circuit *full_adder(char *input1, char *input2, char *C_in, char *sum, char *C_out)
{
  Circuit *c = createCircuit(5, 0);

  c->values = (char *)malloc(3 * sizeof(char));

  setGate(c, 0, XOR, input1, input2, &c->values[0]);
  setGate(c, 1, AND, input1, input2, &c->values[2]);
  setGate(c, 2, AND, &c->values[0], C_in, &c->values[1]);
  setGate(c, 3, XOR, &c->values[0], C_in, sum);
  setGate(c, 4, OR, &c->values[1], &c->values[2], C_out);

  return c;
}
