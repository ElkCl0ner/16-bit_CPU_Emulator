#include "half_adder.h"

/**
 * Creates a 1-bit half adder
 * @param input1 (char *)
 * @param input2 (char *)
 * @param sum (char *)
 * @param C_out (char *)
 */
Circuit *half_adder(char *input1, char *input2, char *sum, char *C_out)
{
  Circuit *c = createCircuit(2, 0);

  setGate(c, 0, XOR, input1, input2, sum);
  setGate(c, 1, AND, input1, input2, C_out);

  return c;
}
