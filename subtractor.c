#include "subtractor.h"
#include "globals.h"
#include "twos_complement.h"
#include "adder.h"

#include <stdlib.h>
#include <stdio.h>

/**
 * Creates a 16-bit subtraction circuit
 * @param input1 (int[16] *)
 * @param input2 (int[16] *)
 * @param output (int[16] *)
 * @param overflow (int *)
*/
Circuit *subtractor(int *input1, int *input2, int *output, int *overflow)
{
  Circuit *circuit = createCircuit(0, 2);

  circuit->values = (int *)malloc(16 * sizeof(int));
  if (circuit->values == NULL)
  {
    perror("Malloc failed. Terminating.");
    exit(1);
  }

  circuit->subCircuits[0] = twos_complement(input2, circuit->values);
  circuit->subCircuits[1] = adder(input1, circuit->values, output, overflow);

  return circuit;
}
