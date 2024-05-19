#include "half_adder.h"

/**
 * Creates a 1-bit half adder
 * @param input1 (int *)
 * @param input2 (int *)
 * @param sum (int *)
 * @param C_out (int *)
*/
Circuit *half_adder(int *input1, int *input2, int *sum, int *C_out)
{
  Circuit *c = createCircuit(2, 0);

  setGate(c, 0, XOR, input1, input2, sum);
  setGate(c, 1, AND, input1, input2, C_out);

  return c;
}
