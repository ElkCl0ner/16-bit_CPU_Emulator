#include "full_adder.h"

#include <stdlib.h>

Circuit full_adder(int *A, int *B, int *C_in, int *sum, int *C_out)
{
  Circuit circuit = createCircuit(5);

  int *values = (int*) malloc(3*sizeof(int));

  setGate(circuit, 0, XOR, A, B, &values[0]);
  setGate(circuit, 1, AND, A, B, &values[2]);
  setGate(circuit, 2, AND, &values[0], C_in, &values[1]);
  setGate(circuit, 3, XOR, &values[0], C_in, sum);
  setGate(circuit, 4, OR, &values[1], &values[2], C_out);

  return circuit;
}
