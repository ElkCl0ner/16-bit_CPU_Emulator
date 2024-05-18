#include "full_adder.h"

#include <stdlib.h>

Circuit full_adder(int *A, int *B, int *C_in, int *sum, int *C_out)
{
  Circuit circuit = createCircuit(5, 0);

  circuit.values = (int*) malloc(3*sizeof(int));

  setGate(circuit, 0, XOR, A, B, &circuit.values[0]);
  setGate(circuit, 1, AND, A, B, &circuit.values[2]);
  setGate(circuit, 2, AND, &circuit.values[0], C_in, &circuit.values[1]);
  setGate(circuit, 3, XOR, &circuit.values[0], C_in, sum);
  setGate(circuit, 4, OR, &circuit.values[1], &circuit.values[2], C_out);

  return circuit;
}
