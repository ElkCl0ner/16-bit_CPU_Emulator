#include "circuit.h"

#include <stdio.h>
#include <stdlib.h>

// AND Gate
int AND(int a, int b)
{
  return a && b;
}

// OR Gate
int OR(int a, int b)
{
  return a || b;
}

// NOT Gate
int NOT(int a, int b)
{
  return !a;
}

// NAND Gate
int NAND(int a, int b)
{
  return !(a && b);
}

// NOR Gate
int NOR(int a, int b)
{
  return !(a || b);
}

// XOR Gate
int XOR(int a, int b)
{
  return a ^ b;
}

// XNOR Gate
int XNOR(int a, int b)
{
  return !(a ^ b);
}

// Allocate circuit
Circuit createCircuit(int gateCount)
{
  Circuit circuit;
  circuit.gateCount = gateCount;

  circuit.gates = (Gate *)malloc(gateCount * sizeof(Gate));
  if (circuit.gates == NULL)
  {
    perror("Malloc failed. Terminating.");
    exit(1);
  }

  return circuit;
}

// Set a gate in the circuit
void setGate(Circuit circuit, int index, int (*gateFunction)(int, int), int *input1, int *input2, int *output)
{
  circuit.gates[index].gateFunction = gateFunction;
  circuit.gates[index].input1 = input1;
  circuit.gates[index].input2 = input2;
  circuit.gates[index].output = output;
}

// Simulate the circuit
void simulateCircuit(Circuit *circuit)
{
  for (int i = 0; i < circuit->gateCount; i++)
  {
    Gate *gate = &circuit->gates[i];
    *gate->output = gate->gateFunction(*gate->input1, *gate->input2);
  }
}
