#include "bitwise_nand.h"

/**
 * Creates a 16-bit bitwise NAND gate array
 * @param input1 (int[16] *)
 * @param input2 (int[16] *)
 * @param output (int[16] *)
 */
Circuit *bitwise_nand(int *input1, int *input2, int *output)
{
  Circuit *c = createCircuit(16, 0);

  for (int i = 0; i < 16; i++)
  {
    setGate(c, i, NAND, input1 + i, input2 + i, output + i);
  }

  return c;
}
