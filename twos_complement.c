#include "twos_complement.h"
#include "globals.h"
#include "eight_bit_adder.h"

#include <stdlib.h>
#include <stdio.h>

/**
 * Creates an 8-bit two's complement circuit
 * @param n1 pointer to an array of 8 ints
 * @param n2 pointer to an array of 8 ints
*/
Circuit *twos_complement(int *n1, int *n2)
{
  Circuit *circuit = createCircuit(0, 2);

  circuit->values = (int *)malloc(16 * sizeof(int));
  if (circuit->values == NULL)
  {
    perror("Malloc failed. Terminating.");
    exit(1);
  }

  Circuit *ones_complement = createCircuit(8, 0);
  for(int i = 0; i < 8; i++)
  {
    setGate(ones_complement, i, NOT, &n1[i], &n1[i], &circuit->values[i]);
  }
  circuit->subCircuits[0] = ones_complement;

  circuit->values[8] = 1;     // create the constant
  for(int i = 9; i < 16; i++) // 00000001 to populate
  {                           // the adder
    circuit->values[i] = 0;
  }
  Circuit *adder = eight_bit_adder(circuit->values, circuit->values+8, n2, &trash);
  circuit->subCircuits[1] = adder;

  return circuit;
}
