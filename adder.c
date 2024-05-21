#include "adder.h"
#include "half_adder.h"
#include "full_adder.h"

/**
 * Creates a 16-bit adder
 * @param input1 (char[16] *) first number
 * @param input2 (char[16] *) second number
 * @param output (char[16] *) result
 * @param overflow (char *) overflow
 */
Circuit *adder(char *input1, char *input2, char *output, char *overflow)
{
  Circuit *c = createCircuit(0, 16);

  c->subCircuits[0] = half_adder(input1, input2, output, output + 1);
  for (int i = 1; i < 15; i++)
  {
    c->subCircuits[i] = full_adder(input1 + i, input2 + i, output + i, output + i, output + i + 1);
  }
  c->subCircuits[15] = full_adder(input1 + 15, input2 + 15, output + 15, output + 15, overflow);

  return c;
}
