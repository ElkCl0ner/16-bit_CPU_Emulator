#include "adder.h"
#include "globals.h"
#include "full_adder.h"

/**
 * Creates a 16-bit adder
 * @param input1 (int[16] *) first number
 * @param input2 (int[16] *) second number
 * @param output (int[16] *) result
 * @param overflow (int *) overflow
*/
Circuit *adder(int *input1, int *input2, int *output, int *overflow)
{
  Circuit *c = createCircuit(0, 16);

  c->subCircuits[0] = full_adder(input1, input2, &zero, output, output+1);
  for (int i = 1; i < 15; i++)
  {
    c->subCircuits[i] = full_adder(input1+i, input2+i, output+i, output+i, output+i+1);
  }
  c->subCircuits[15] = full_adder(input1+15, input2+15, output+15, output+15, overflow);

  return c;
}
