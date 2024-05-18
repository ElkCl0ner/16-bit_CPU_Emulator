#include "subtractor.h"
#include "globals.h"
#include "twos_complement.h"
#include "eight_bit_adder.h"

#include <stdlib.h>
#include <stdio.h>

Circuit *subtractor(int *n1, int *n2, int *n3)
{
  Circuit *circuit = createCircuit(0, 2);

  circuit->values = (int *)malloc(8 * sizeof(int));
  if (circuit->values == NULL)
  {
    perror("Malloc failed. Terminating.");
    exit(1);
  }

  circuit->subCircuits[0] = twos_complement(n2, circuit->values);
  circuit->subCircuits[1] = eight_bit_adder(n1, circuit->values, n3, &trash);

  return circuit;
}
