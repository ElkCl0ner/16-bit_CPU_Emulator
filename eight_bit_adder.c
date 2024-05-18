#include "globals.h"
#include "eight_bit_adder.h"
#include "full_adder.h"

#include <stdlib.h>

Circuit eight_bit_adder(int *n1, int *n2, int *n3, int *overflow)
{
  Circuit circuit = createCircuit(0, 8);

  Circuit fa1 = full_adder(&n1[0], &n2[0], &zero, &n3[0], &n3[1]);
  circuit.subCircuits[0] = &fa1;

  for (int i = 1; i < 7; i++)
  {
    Circuit fa = full_adder(&n1[i], &n2[i], &n3[i], &n3[i], &n3[i+1]);
    circuit.subCircuits[i] = &fa;
  }

  Circuit fa8 = full_adder(&n1[7], &n2[7], &n3[7], &n3[7], overflow);
  circuit.subCircuits[7] = &fa8;

  return circuit;
}
